//===-- StarcraftEUDInstrInfo.cpp - StarcraftEUD Instruction Information ----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the StarcraftEUD implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#include "StarcraftEUDInstrInfo.h"
#include "StarcraftEUDTargetMachine.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include "StarcraftEUDGenInstrInfo.inc"

// Pin the vtable to this file.
void StarcraftEUDInstrInfo::anchor() {}

StarcraftEUDInstrInfo::StarcraftEUDInstrInfo(StarcraftEUDSubtarget &ST)
    : StarcraftEUDGenInstrInfo(), RI(ST), Subtarget(ST) {}

/// Expand Pseudo instructions into real backend instructions
bool StarcraftEUDInstrInfo::expandPostRAPseudo(MachineInstr &MI) const {
  MachineBasicBlock &MBB = *MI.getParent();

  switch (MI.getDesc().getOpcode()) {
  default:
    return false;
  case StarcraftEUD::RetRA:
    BuildMI(MBB, MI, MI.getDebugLoc(), get(StarcraftEUD::RET)).addReg(StarcraftEUD::RA);
    break;
  }

  MBB.erase(MI);
  return true;
}

void StarcraftEUDInstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                                 MachineBasicBlock::iterator I,
                                 const DebugLoc &DL, unsigned DestReg,
                                 unsigned SrcReg, bool KillSrc) const {
  unsigned opc = StarcraftEUD::ADD;
  BuildMI(MBB, I, DL, get(opc))
    .addReg(DestReg, RegState::Define)
    .addReg(StarcraftEUD::ZERO)
    .addReg(SrcReg, getKillRegState(KillSrc));
}
