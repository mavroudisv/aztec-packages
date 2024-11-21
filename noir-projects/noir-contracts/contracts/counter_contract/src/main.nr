// docs:start:setup
use dep::aztec::macros::aztec;

#[aztec]
contract Counter {
    // docs:end:setup
    // docs:start:imports
    use aztec::macros::{functions::{initializer, private, public}, storage::storage};
    use aztec::prelude::{AztecAddress, Map};
    use easy_private_state::EasyPrivateUint;
    use value_note::{balance_utils, value_note::ValueNote};
    // docs:end:imports

    // docs:start:storage_struct
    #[storage]
    struct Storage<Context> {
        counters: Map<AztecAddress, EasyPrivateUint<Context>, Context>,
    }
    // docs:end:storage_struct

    // docs:start:constructor
    #[initializer]
    #[private]
    // We can name our initializer anything we want as long as it's marked as aztec(initializer)
    fn initialize(headstart: u64, owner: AztecAddress, outgoing_viewer: AztecAddress) {
        let counters = storage.counters;
        counters.at(owner).add(headstart, owner, outgoing_viewer, context.msg_sender());
    }
    // docs:end:constructor

    // docs:start:increment
    #[private]
    fn increment(owner: AztecAddress, outgoing_viewer: AztecAddress) {
        unsafe {
            dep::aztec::oracle::debug_log::debug_log_format(
                "Incrementing counter for owner {0}",
                [owner.to_field()],
            );
        }
        let counters = storage.counters;
        counters.at(owner).add(1, owner, outgoing_viewer, outgoing_viewer);
    }

    #[private]
    fn increment_two(owner: AztecAddress, outgoing_viewer: AztecAddress) {
        unsafe {
            dep::aztec::oracle::debug_log::debug_log_format(
                "Incrementing counter for owner {0}",
                [owner.to_field()],
            );
        }
        let counters = storage.counters;
        counters.at(owner).add(1, owner, outgoing_viewer, outgoing_viewer);
        counters.at(owner).add(1, owner, outgoing_viewer, outgoing_viewer);
    }

    #[private]
    fn increment_three(owner: AztecAddress, outgoing_viewer: AztecAddress) {
        unsafe {
            dep::aztec::oracle::debug_log::debug_log_format(
                "Incrementing counter for owner {0}",
                [owner.to_field()],
            );
        }
        let counters = storage.counters;
        counters.at(owner).add(1, owner, outgoing_viewer, outgoing_viewer);
        counters.at(owner).sub(1, owner, outgoing_viewer, outgoing_viewer);
    }

    #[private]
    fn decrement_three(owner: AztecAddress, outgoing_viewer: AztecAddress) {
        unsafe {
            dep::aztec::oracle::debug_log::debug_log_format(
                "Incrementing counter for owner {0}",
                [owner.to_field()],
            );
        }
        let counters = storage.counters;
        counters.at(owner).sub(1, owner, outgoing_viewer, outgoing_viewer);
    }

    #[public]
    fn public_emission(owner: AztecAddress, outgoing_viewer: AztecAddress) {
        unsafe {
            dep::aztec::oracle::debug_log::debug_log_format(
                "Incrementing counter for owner {0}",
                [owner.to_field()],
            );
        }
        context.emit_unencrypted_log(1);
    }

    // docs:end:increment
    // docs:start:get_counter
    unconstrained fn get_counter(owner: AztecAddress) -> pub Field {
        let counters = storage.counters;
        balance_utils::get_balance(counters.at(owner).set)
    }

    // docs:end:get_counter
    // docs:start:test_imports
    use dep::aztec::note::note_getter::{MAX_NOTES_PER_PAGE, view_notes};
    use dep::aztec::note::note_viewer_options::NoteViewerOptions;
    use dep::aztec::note::lifecycle::destroy_note;

    use dep::aztec::protocol_types::storage::map::derive_storage_slot_in_map;
    use dep::aztec::test::helpers::{cheatcodes, test_environment::TestEnvironment};
    use dep::aztec::protocol_types::debug_log::debug_log_format;

    // docs:end:test_imports
    // docs:start:txe_test_increment
    // #[test]
    // unconstrained fn test_increment() {
    //     // Setup env, generate keys
    //     let mut env = TestEnvironment::new();
    //     let owner = env.create_account();
    //     let outgoing_viewer = env.create_account();
    //     let initial_value: Field = 5;
    //     env.impersonate(owner);
    //     // Deploy contract and initialize
    //     let initializer =
    //         Counter::interface().initialize(initial_value as u64, owner, outgoing_viewer);
    //     let counter_contract = env.deploy_self("Counter").with_private_initializer(initializer);
    //     let contract_address = counter_contract.to_address();
    //     // docs:start:txe_test_read_notes
    //     // Read the stored value in the note
    //     env.impersonate(contract_address);
    //     let counter_slot = Counter::storage_layout().counters.slot;
    //     let owner_slot = derive_storage_slot_in_map(counter_slot, owner);
    //     let mut options = NoteViewerOptions::new();
    //     let notes: BoundedVec<ValueNote, MAX_NOTES_PER_PAGE> = view_notes(owner_slot, options);
    //     let initial_note_value = notes.get(0).value;
    //     assert(
    //         initial_note_value == initial_value,
    //         f"Expected {initial_value} but got {initial_note_value}",
    //     );
    //     // docs:end:txe_test_read_notes
    //     // Increment the counter
    //     Counter::at(contract_address).increment(owner, outgoing_viewer).call(&mut env.private());
    //     // get_counter is an unconstrained function, so we call it directly (we're in the same module)
    //     let current_value_for_owner = get_counter(owner);
    //     let expected_current_value = initial_value + 1;
    //     assert(
    //         expected_current_value == current_value_for_owner,
    //         f"Expected {expected_current_value} but got {current_value_for_owner}",
    //     );
    // }
    // // docs:end:txe_test_increment


    // #[test]
    // unconstrained fn increment_test() {
    //     // Setup env, generate keys
    //     let mut env = TestEnvironment::new();
    //     let owner = env.create_account();
    //     let outgoing_viewer = env.create_account();
    //     let initial_value: Field = 5;
    //     env.impersonate(owner);
    //     // Deploy contract and initialize
    //     let initializer =
    //         Counter::interface().initialize(initial_value as u64, owner, outgoing_viewer);
    //     let counter_contract = env.deploy_self("Counter").with_private_initializer(initializer);
    //     let contract_address = counter_contract.to_address();
    //     env.advance_block_by(1);
    //     std::println("COUNTER CONTRACT ADDRESS:");
    //     std::println(contract_address.to_field());
    //     std::println("OWNER CONTRACT ADDRESS:");
    //     std::println(owner.to_field());

    //     // docs:start:txe_test_read_notes
    //     // Read the stored value in the note
    //     env.impersonate(contract_address);
    //     let counter_slot = Counter::storage_layout().counters.slot;
    //     let owner_slot = derive_storage_slot_in_map(counter_slot, owner);
    //     let mut options = NoteViewerOptions::new();
    //     let notes: BoundedVec<ValueNote, MAX_NOTES_PER_PAGE> = view_notes(owner_slot, options);
    //     let initial_note_value = notes.get(0).value;
    //     assert(
    //         initial_note_value == initial_value,
    //         f"Expected {initial_value} but got {initial_note_value}",
    //     );

    //     // env.advance_block_by(1);

    //     let old_note = notes.get(0);

    //     std::println("OLD NOTE NONCE:");
    //     std::println(old_note.header.nonce);

    //     env.private().get_header().prove_note_inclusion(old_note);
    // }

    #[test]
    unconstrained fn inclusion_proofs() {
        // Setup env, generate keys
        let mut env = TestEnvironment::new();
        let owner = env.create_account();
        let outgoing_viewer = env.create_account();
        let initial_value: Field = 5;
        env.impersonate(owner);
        // Deploy contract and initialize
        let initializer =
            Counter::interface().initialize(initial_value as u64, owner, outgoing_viewer);
        let counter_contract = env.deploy_self("Counter").with_private_initializer(initializer);
        let contract_address = counter_contract.to_address();
        std::println("COUNTER CONTRACT ADDRESS:");
        std::println(contract_address.to_field());
        std::println("OWNER CONTRACT ADDRESS:");
        std::println(owner.to_field());

        env.advance_block_by(1);
        env.advance_block_by(1);

        env.impersonate(contract_address);
        let counter_slot = Counter::storage_layout().counters.slot;
        let owner_slot = derive_storage_slot_in_map(counter_slot, owner);
        let mut options = NoteViewerOptions::new();
        let notes: BoundedVec<ValueNote, MAX_NOTES_PER_PAGE> = view_notes(owner_slot, options);
        let initial_note_value = notes.get(0).value;
        assert(
            initial_note_value == initial_value,
            f"Expected {initial_value} but got {initial_note_value}",
        );

        let old_note = notes.get(0);

        env.private().get_header().prove_note_validity(old_note, &mut env.private());

        // destroy_note(&mut env.private(), old_note);
        // env.advance_block_by(1);

        // env.private().get_header().prove_note_is_nullified(old_note, &mut env.private());
        // env.private().get_header().prove_note_inclusion(old_note);

        // let notes: BoundedVec<ValueNote, MAX_NOTES_PER_PAGE> = view_notes(owner_slot, options);

        // assert(notes.len() == 0);

        // let notes: BoundedVec<ValueNote, MAX_NOTES_PER_PAGE> = view_notes(owner_slot, options);
    }

    //  #[test]
    // unconstrained fn increment_test() {
    //     // Setup env, generate keys
    //     let mut env = TestEnvironment::new();
    //     let owner = env.create_account();
    //     let outgoing_viewer = env.create_account();
    //     let initial_value: Field = 5;
    //     env.impersonate(owner);
    //     // Deploy contract and initialize
    //     let initializer =
    //         Counter::interface().initialize(initial_value as u64, owner, outgoing_viewer);
    //     let counter_contract = env.deploy_self("Counter").with_private_initializer(initializer);
    //     let contract_address = counter_contract.to_address();
    //     std::println("COUNTER CONTRACT ADDRESS:");
    //     std::println(contract_address.to_field());
    //     std::println("OWNER CONTRACT ADDRESS:");
    //     std::println(owner.to_field());

    //     env.impersonate(contract_address);
    //     let counter_slot = Counter::storage_layout().counters.slot;
    //     let owner_slot = derive_storage_slot_in_map(counter_slot, owner);
    //     let mut options = NoteViewerOptions::new();
    //     let notes: BoundedVec<ValueNote, MAX_NOTES_PER_PAGE> = view_notes(owner_slot, options);
    //     let initial_note_value = notes.get(0).value;
    //     assert(
    //         initial_note_value == initial_value,
    //         f"Expected {initial_value} but got {initial_note_value}",
    //     );

    //     env.advance_block_by(1);
    //     env.impersonate(contract_address);
    //     let counter_slot = Counter::storage_layout().counters.slot;
    //     let owner_slot = derive_storage_slot_in_map(counter_slot, owner);
    //     let mut options = NoteViewerOptions::new();
    //     let notes: BoundedVec<ValueNote, MAX_NOTES_PER_PAGE> = view_notes(owner_slot, options);
    //     let initial_note_value = notes.get(0).value;
    //     assert(
    //         initial_note_value == initial_value,
    //         f"Expected {initial_value} but got {initial_note_value}",
    //     );

    //     Counter::at(contract_address).increment_two(owner, outgoing_viewer).call(&mut env.private());

    //     let notes: BoundedVec<ValueNote, MAX_NOTES_PER_PAGE> = view_notes(owner_slot, options);
    //     assert(
    //         notes.len() == 3
    //     );
    //     assert(get_counter(owner) == 7);

    //     env.advance_block_by(1);
    //     let notes: BoundedVec<ValueNote, MAX_NOTES_PER_PAGE> = view_notes(owner_slot, options);
    //     assert(get_counter(owner) == 7);
    //     assert(
    //         notes.len() == 3
    //     );

    //     Counter::at(contract_address).increment_three(owner, outgoing_viewer).call(&mut env.private());
    //     let notes: BoundedVec<ValueNote, MAX_NOTES_PER_PAGE> = view_notes(owner_slot, options);
    //     assert(get_counter(owner) == 7);
    //     assert(
    //         notes.len() == 4
    //     );

    //     env.advance_block_by(1);

    //     let notes: BoundedVec<ValueNote, MAX_NOTES_PER_PAGE> = view_notes(owner_slot, options);
    //     std::println("COUNT");
    //     std::println((get_counter(owner)));

    //     assert(get_counter(owner) == 7);
    //     assert(
    //         notes.len() == 4
    //     );

    //     Counter::at(contract_address).decrement_three(owner, outgoing_viewer).call(&mut env.private());
    //     assert(
    //         notes.len() == 4
    //     );
    //     std::println("COUNT");
    //     std::println((get_counter(owner)));

    //     // assert(get_counter(owner) == 8);

    //     env.advance_block_by(1);
    //     std::println("COUNT");
    //     std::println((get_counter(owner)));
    //     // assert(get_counter(owner) == 6);
    // }

    // #[test]
    // unconstrained fn public_emission_test() {
    //     // Setup env, generate keys
    //     let mut env = TestEnvironment::new();
    //     let owner = env.create_account();
    //     let outgoing_viewer = env.create_account();
    //     let initial_value: Field = 5;
    //     env.impersonate(owner);
    //     // Deploy contract and initialize
    //     let initializer =
    //         Counter::interface().initialize(initial_value as u64, owner, outgoing_viewer);
    //     let counter_contract = env.deploy_self("Counter").with_private_initializer(initializer);
    //     let contract_address = counter_contract.to_address();

    //     env.advance_block_by(1);
    //     // env.impersonate(contract_address);
    //     // let counter_slot = Counter::storage_layout().counters.slot;
    //     // let owner_slot = derive_storage_slot_in_map(counter_slot, owner);
    //     // let mut options = NoteViewerOptions::new();
    //     // let notes: BoundedVec<ValueNote, MAX_NOTES_PER_PAGE> = view_notes(owner_slot, options);
    //     // let initial_note_value = notes.get(0).value;
    //     // assert(
    //     //     initial_note_value == initial_value,
    //     //     f"Expected {initial_value} but got {initial_note_value}",
    //     // );

    //     Counter::at(contract_address).public_emission(owner, outgoing_viewer).call(&mut env.public());
    // }
}
