//===-- StarcraftEUDISelDAGToDAG.cpp - A Dag to Dag Inst Selector for StarcraftEUD ------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines an instruction selector for the NIOS2 target.
//
//===----------------------------------------------------------------------===//

#include "StarcraftEUD.h"
#include "StarcraftEUDTargetMachine.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/Support/Debug.h"
using namespace llvm;

#define DEBUG_TYPE "broodwar-isel"

//===----------------------------------------------------------------------===//
// Instruction Selector Implementation
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// StarcraftEUDDAGToDAGISel - NIOS2 specific code to select NIOS2 machine
// instructions for SelectionDAG operations.
//===----------------------------------------------------------------------===//

namespace {

class StarcraftEUDDAGToDAGISel : public SelectionDAGISel {
  /// Subtarget - Keep a pointer to the StarcraftEUD Subtarget around so that we can
  /// make the right decision when generating code for different targets.
  const StarcraftEUDSubtarget *Subtarget;

public:
  explicit StarcraftEUDDAGToDAGISel(StarcraftEUDTargetMachine &TM, CodeGenOpt::Level OL)
      : SelectionDAGISel(TM, OL) {}

  bool runOnMachineFunction(MachineFunction &MF) override {
    Subtarget = &MF.getSubtarget<StarcraftEUDSubtarget>();
    return SelectionDAGISel::runOnMachineFunction(MF);
  }

  void Select(SDNode *N) override;

  // Pass Name
  StringRef getPassName() const override {
    return "NIOS2 DAG->DAG Pattern Instruction Selection";
  }

#include "StarcraftEUDGenDAGISel.inc"
};
} // namespace

// Select instructions not customized! Used for
// expanded, promoted and normal instructions
void StarcraftEUDDAGToDAGISel::Select(SDNode *Node) {

  // If we have a custom node, we already have selected!
  if (Node->isMachineOpcode()) {
    DEBUG(errs() << "== "; Node->dump(CurDAG); errs() << "\n");
    Node->setNodeId(-1);
    return;
  }

  // Select the default instruction
  SelectCode(Node);
}

FunctionPass *llvm::createStarcraftEUDISelDag(StarcraftEUDTargetMachine &TM,
                                       CodeGenOpt::Level OptLevel) {
  return new StarcraftEUDDAGToDAGISel(TM, OptLevel);
}
