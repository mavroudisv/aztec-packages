#pragma once

#include <cstdint>
#include <span>
#include <sys/types.h>
#include <unordered_map>
#include <utility>
#include <vector>

#include "barretenberg/vm2/common/aztec_types.hpp"
#include "barretenberg/vm2/simulation/serialization.hpp"

namespace bb::avm::simulation {

using BytecodeId = uint32_t;

// Manages the bytecode operations of all calls in a transaction.
// In particular, it will not duplicate hashing and decomposition.
class TxBytecodeManagerInterface {
  public:
    virtual ~TxBytecodeManagerInterface() = default;

    // Retrieves the bytecode and
    // (1) sets up the address-class id connection,
    // (2) hashes it if needed.
    virtual BytecodeId get_bytecode(const AztecAddress& address) = 0;
    // Retrieves an instruction and decomposes it if needed.
    virtual std::pair<Instruction, /*read_bytes*/ uint32_t> read_instruction(BytecodeId bytecode_id, size_t pc) = 0;
    // Retrieves the class id of a bytecode, in case you need it.
    virtual ContractClassId get_class_id(BytecodeId bytecode_id) const = 0;
};

class TxBytecodeManager : public TxBytecodeManagerInterface {
  public:
    BytecodeId get_bytecode(const AztecAddress& address) override;
    std::pair<Instruction, /*read_bytes*/ uint32_t> read_instruction(BytecodeId bytecode_id, size_t pc) override;
    ContractClassId get_class_id(BytecodeId bytecode_id) const override;

  private:
    struct BytecodeInfo {
        std::vector<uint8_t> bytecode;
        ContractClassId class_id;
    };

    std::unordered_map<BytecodeId, const BytecodeInfo> bytecodes;
    std::unordered_map<AztecAddress, BytecodeId> resolved_addresses;
    BytecodeId next_bytecode_id = 0;
};

// Manages the bytecode of a single nested call.
// Mostly a wrapper around a TxBytecodeManager.
class BytecodeManagerInterface {
  public:
    virtual ~BytecodeManagerInterface() = default;

    virtual std::pair<Instruction, /*read_bytes*/ uint32_t> read_instruction(size_t pc) const = 0;
    virtual ContractClassId get_class_id() const = 0;
};

class BytecodeManager : public BytecodeManagerInterface {
  public:
    BytecodeManager(BytecodeId bytecode_id, TxBytecodeManagerInterface& tx_bytecode_manager)
        : bytecode_id(bytecode_id)
        , tx_bytecode_manager(tx_bytecode_manager)
    {}

    std::pair<Instruction, /*read_bytes*/ uint32_t> read_instruction(size_t pc) const override
    {
        return tx_bytecode_manager.read_instruction(bytecode_id, pc);
    }
    ContractClassId get_class_id() const override { return tx_bytecode_manager.get_class_id(bytecode_id); }

  private:
    BytecodeId bytecode_id;
    TxBytecodeManagerInterface& tx_bytecode_manager;
};

} // namespace bb::avm::simulation