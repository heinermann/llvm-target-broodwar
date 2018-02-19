//===-- StarcraftEUDTargetMachine.h - Define TargetMachine for StarcraftEUD --- C++ ---===//
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

#ifndef LLVM_LIB_TARGET_StarcraftEUD_StarcraftEUDTARGETMACHINE_H
#define LLVM_LIB_TARGET_StarcraftEUD_StarcraftEUDTARGETMACHINE_H

#include "StarcraftEUDSubtarget.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class StarcraftEUDTargetMachine : public LLVMTargetMachine {
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  StarcraftEUDSubtarget Subtarget;

public:
  StarcraftEUDTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                   StringRef FS, const TargetOptions &Options,
                   Optional<Reloc::Model> RM, Optional<CodeModel::Model> CM,
                   CodeGenOpt::Level OL, bool JIT);

  const StarcraftEUDSubtarget *getSubtargetImpl() const { return &Subtarget; }
  const StarcraftEUDSubtarget *getSubtargetImpl(const Function &) const override {
    return &Subtarget;
  }

  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;

  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }
};
}

#endif
