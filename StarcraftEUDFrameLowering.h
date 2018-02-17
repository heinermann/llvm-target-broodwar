//===-- StarcraftEUDFrameLowering.h - Define frame lowering for StarcraftEUD --*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//
//
//===----------------------------------------------------------------------===//
#ifndef LLVM_LIB_TARGET_NIOS2_NIOS2FRAMELOWERING_H
#define LLVM_LIB_TARGET_NIOS2_NIOS2FRAMELOWERING_H

#include "StarcraftEUD.h"
#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {
class StarcraftEUDSubtarget;

class StarcraftEUDFrameLowering : public TargetFrameLowering {
protected:
  const StarcraftEUDSubtarget &STI;

public:
  explicit StarcraftEUDFrameLowering(const StarcraftEUDSubtarget &sti)
      : TargetFrameLowering(TargetFrameLowering::StackGrowsDown, 4, 0, 4),
        STI(sti) {}

  bool hasFP(const MachineFunction &MF) const override;
  /// emitProlog/emitEpilog - These methods insert prolog and epilog code into
  /// the function.
  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
};
} // namespace llvm

#endif
