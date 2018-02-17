//===-- StarcraftEUDRegisterInfo.h - StarcraftEUD Register Information Impl ---*- C++ -*-===//
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

#ifndef LLVM_LIB_TARGET_NIOS2_NIOS2REGISTERINFO_H
#define LLVM_LIB_TARGET_NIOS2_NIOS2REGISTERINFO_H

#include "StarcraftEUD.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "StarcraftEUDGenRegisterInfo.inc"

namespace llvm {
class StarcraftEUDSubtarget;
class TargetInstrInfo;
class Type;

class StarcraftEUDRegisterInfo : public StarcraftEUDGenRegisterInfo {
protected:
  const StarcraftEUDSubtarget &Subtarget;

public:
  StarcraftEUDRegisterInfo(const StarcraftEUDSubtarget &Subtarget);

  const MCPhysReg *getCalleeSavedRegs(const MachineFunction *MF) const override;

  BitVector getReservedRegs(const MachineFunction &MF) const override;

  /// Stack Frame Processing Methods
  void eliminateFrameIndex(MachineBasicBlock::iterator II, int SPAdj,
                           unsigned FIOperandNum,
                           RegScavenger *RS = nullptr) const override;

  /// Debug information queries.
  unsigned getFrameRegister(const MachineFunction &MF) const override;

  /// Return GPR register class.
  const TargetRegisterClass *intRegClass(unsigned Size) const;
};

} // end namespace llvm
#endif
