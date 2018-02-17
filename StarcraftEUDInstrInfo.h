//===-- StarcraftEUDInstrInfo.h - StarcraftEUD Instruction Information --------*- C++ -*-===//
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

#ifndef LLVM_LIB_TARGET_NIOS2_NIOS2INSTRINFO_H
#define LLVM_LIB_TARGET_NIOS2_NIOS2INSTRINFO_H

#include "StarcraftEUDRegisterInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "StarcraftEUDGenInstrInfo.inc"

namespace llvm {

class StarcraftEUDSubtarget;

class StarcraftEUDInstrInfo : public StarcraftEUDGenInstrInfo {
  const StarcraftEUDRegisterInfo RI;
  const StarcraftEUDSubtarget &Subtarget;
  virtual void anchor();

public:
  explicit StarcraftEUDInstrInfo(StarcraftEUDSubtarget &ST);

  /// getRegisterInfo - TargetInstrInfo is a superset of MRegister info.  As
  /// such, whenever a client has an instance of instruction info, it should
  /// always be able to get register info as well (through this method).
  ///
  const StarcraftEUDRegisterInfo &getRegisterInfo() const { return RI; };

  bool expandPostRAPseudo(MachineInstr &MI) const override;

  void copyPhysReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator MI,
                   const DebugLoc &DL, unsigned DestReg, unsigned SrcReg,
                   bool KillSrc) const override;
};
} // namespace llvm

#endif
