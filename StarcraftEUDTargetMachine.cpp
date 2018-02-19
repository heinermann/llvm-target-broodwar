//===-- StarcraftEUDTargetMachine.cpp - Define TargetMachine for StarcraftEUD ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Implements the info about StarcraftEUD target spec.
//
//===----------------------------------------------------------------------===//

#include "StarcraftEUDTargetMachine.h"
#include "StarcraftEUD.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Target/TargetOptions.h"
using namespace llvm;

extern "C" void LLVMInitializeStarcraftEUDTarget() {
  // Register the target.
  RegisterTargetMachine<StarcraftEUDTargetMachine> X(getTheStarcraftEUDleTarget());
  RegisterTargetMachine<StarcraftEUDTargetMachine> Y(getTheStarcraftEUDbeTarget());
  RegisterTargetMachine<StarcraftEUDTargetMachine> Z(getTheStarcraftEUDTarget());
}

// DataLayout: little or big endian
static std::string computeDataLayout(const Triple &TT) {
  return "e-m:e-p:64:64-i64:64-n32:64-S128";
}

static Reloc::Model getEffectiveRelocModel(Optional<Reloc::Model> RM) {
  if (!RM.hasValue())
    return Reloc::PIC_;
  return *RM;
}

static CodeModel::Model getEffectiveCodeModel(Optional<CodeModel::Model> CM) {
  if (CM)
    return *CM;
  return CodeModel::Small;
}

StarcraftEUDTargetMachine::StarcraftEUDTargetMachine(const Target &T, const Triple &TT,
                                   StringRef CPU, StringRef FS,
                                   const TargetOptions &Options,
                                   Optional<Reloc::Model> RM,
                                   Optional<CodeModel::Model> CM,
                                   CodeGenOpt::Level OL, bool JIT)
    : LLVMTargetMachine(T, computeDataLayout(TT), TT, CPU, FS, Options,
                        getEffectiveRelocModel(RM), getEffectiveCodeModel(CM),
                        OL),
      TLOF(make_unique<TargetLoweringObjectFileELF>()),
      Subtarget(TT, CPU, FS, *this) {
  initAsmInfo();
}
namespace {
// StarcraftEUD Code Generator Pass Configuration Options.
class StarcraftEUDPassConfig : public TargetPassConfig {
public:
  StarcraftEUDPassConfig(StarcraftEUDTargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  StarcraftEUDTargetMachine &getStarcraftEUDTargetMachine() const {
    return getTM<StarcraftEUDTargetMachine>();
  }

  bool addInstSelector() override;
};
}

TargetPassConfig *StarcraftEUDTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new StarcraftEUDPassConfig(*this, PM);
}

// Install an instruction selector pass using
// the ISelDag to gen StarcraftEUD code.
bool StarcraftEUDPassConfig::addInstSelector() {
  addPass(createStarcraftEUDISelDag(getStarcraftEUDTargetMachine()));

  return false;
}
