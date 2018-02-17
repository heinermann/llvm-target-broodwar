//===-- StarcraftEUDTargetMachine.h - Define TargetMachine for StarcraftEUD ---*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the StarcraftEUD specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_NIOS2_NIOS2TARGETMACHINE_H
#define LLVM_LIB_TARGET_NIOS2_NIOS2TARGETMACHINE_H

#include "StarcraftEUDSubtarget.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class StarcraftEUDTargetMachine : public LLVMTargetMachine {
  mutable StringMap<std::unique_ptr<StarcraftEUDSubtarget>> SubtargetMap;
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  StarcraftEUDSubtarget Subtarget;

public:
  StarcraftEUDTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                     StringRef FS, const TargetOptions &Options,
                     Optional<Reloc::Model> RM, Optional<CodeModel::Model> CM,
                     CodeGenOpt::Level OL, bool JIT);
  ~StarcraftEUDTargetMachine() override;

  const StarcraftEUDSubtarget *getSubtargetImpl() const { return &Subtarget; }
  const StarcraftEUDSubtarget *getSubtargetImpl(const Function &F) const override;

  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }

  // Pass Pipeline Configuration
  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;
};
} // namespace llvm

#endif
