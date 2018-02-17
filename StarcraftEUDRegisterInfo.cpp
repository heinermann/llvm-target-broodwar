//===-- StarcraftEUDRegisterInfo.cpp - StarcraftEUD Register Information -== ------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the StarcraftEUD implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "broodwar-reg-info"

#include "StarcraftEUDRegisterInfo.h"

#include "StarcraftEUD.h"
#include "StarcraftEUDSubtarget.h"

#define GET_REGINFO_TARGET_DESC
#include "StarcraftEUDGenRegisterInfo.inc"

using namespace llvm;

StarcraftEUDRegisterInfo::StarcraftEUDRegisterInfo(const StarcraftEUDSubtarget &ST)
    : StarcraftEUDGenRegisterInfo(StarcraftEUD::RA), Subtarget(ST) {}

const TargetRegisterClass *StarcraftEUDRegisterInfo::intRegClass(unsigned Size) const {
  return &StarcraftEUD::CPURegsRegClass;
}

const MCPhysReg *
StarcraftEUDRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  return CSR_SaveList;
}

BitVector StarcraftEUDRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  static const MCPhysReg ReservedCPURegs[] = {StarcraftEUD::ZERO, StarcraftEUD::AT, StarcraftEUD::SP,
                                             StarcraftEUD::RA,   StarcraftEUD::PC, StarcraftEUD::GP};
  BitVector Reserved(getNumRegs());

  for (unsigned I = 0; I < array_lengthof(ReservedCPURegs); ++I)
    Reserved.set(ReservedCPURegs[I]);

  return Reserved;
}

void StarcraftEUDRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                            int SPAdj, unsigned FIOperandNum,
                                            RegScavenger *RS) const {}

unsigned StarcraftEUDRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  return StarcraftEUD::SP;
}
