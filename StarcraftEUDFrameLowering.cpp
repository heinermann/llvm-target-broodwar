//===-- StarcraftEUDFrameLowering.cpp - StarcraftEUD Frame Information ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the StarcraftEUD implementation of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#include "StarcraftEUDFrameLowering.h"
#include "StarcraftEUDInstrInfo.h"
#include "StarcraftEUDSubtarget.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"

using namespace llvm;

bool StarcraftEUDFrameLowering::hasFP(const MachineFunction &MF) const { return true; }

void StarcraftEUDFrameLowering::emitPrologue(MachineFunction &MF,
                                    MachineBasicBlock &MBB) const {}

void StarcraftEUDFrameLowering::emitEpilogue(MachineFunction &MF,
                                    MachineBasicBlock &MBB) const {}

void StarcraftEUDFrameLowering::determineCalleeSaves(MachineFunction &MF,
                                            BitVector &SavedRegs,
                                            RegScavenger *RS) const {
  TargetFrameLowering::determineCalleeSaves(MF, SavedRegs, RS);
  SavedRegs.reset(StarcraftEUD::R6);
  SavedRegs.reset(StarcraftEUD::R7);
  SavedRegs.reset(StarcraftEUD::R8);
  SavedRegs.reset(StarcraftEUD::R9);
}
