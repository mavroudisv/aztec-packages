import { PublicKernelType, type PublicProvingRequest, type Tx } from '@aztec/circuit-types';
import { type PublicKernelCircuitPublicInputs } from '@aztec/circuits.js';
import { type ProtocolArtifact } from '@aztec/noir-protocol-circuits-types';

import { AbstractPhaseManager, type PhaseConfig, makeAvmProvingRequest } from './abstract_phase_manager.js';

/**
 * The phase manager responsible for performing the fee preparation phase.
 */
export class AppLogicPhaseManager extends AbstractPhaseManager {
  constructor(config: PhaseConfig, public override phase: PublicKernelType = PublicKernelType.APP_LOGIC) {
    super(config);
  }

  override async handle(
    tx: Tx,
    previousPublicKernelOutput: PublicKernelCircuitPublicInputs,
    previousCircuit: ProtocolArtifact,
  ) {
    this.log.verbose(`Processing tx ${tx.getTxHash()}`);
    // add new contracts to the contracts db so that their functions may be found and called
    // TODO(#4073): This is catching only private deployments, when we add public ones, we'll
    // have to capture contracts emitted in that phase as well.
    // TODO(@spalladino): Should we allow emitting contracts in the fee preparation phase?
    // TODO(#6464): Should we allow emitting contracts in the private setup phase?
    // if so, this should only add contracts that were deployed during private app logic.
    await this.worldStateDB.addNewContracts(tx);
    const {
      publicProvingInformation,
      kernelOutput,
      lastKernelArtifact,
      newUnencryptedLogs,
      revertReason,
      returnValues,
      gasUsed,
    } = await this.processEnqueuedPublicCalls(tx, previousPublicKernelOutput, previousCircuit).catch(
      // if we throw for any reason other than simulation, we need to rollback and drop the TX
      async err => {
        await this.worldStateDB.rollbackToCommit();
        throw err;
      },
    );

    if (revertReason) {
      // TODO(#6464): Should we allow emitting contracts in the private setup phase?
      // if so, this is removing contracts deployed in private setup
      await this.worldStateDB.removeNewContracts(tx);
      await this.worldStateDB.rollbackToCheckpoint();
      tx.filterRevertedLogs(kernelOutput);
    } else {
      tx.unencryptedLogs.addFunctionLogs(newUnencryptedLogs);
      // TODO(#6470): we should be adding contracts deployed in those logs to the publicContractsDB
    }

    // Return a list of app logic proving requests
    const publicProvingRequests: PublicProvingRequest[] = publicProvingInformation.map(info => {
      return makeAvmProvingRequest(info, PublicKernelType.APP_LOGIC);
    });
    return {
      publicProvingRequests,
      publicKernelOutput: kernelOutput,
      lastKernelArtifact,
      revertReason,
      returnValues,
      gasUsed,
    };
  }
}