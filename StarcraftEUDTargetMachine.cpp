//===-- StarcraftEUDTargetMachine.cpp - Define TargetMachine for StarcraftEUD -----------===//
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
#include "StarcraftEUDTargetObjectFile.h"

#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "broodwar"

extern "C" void LLVMInitializeStarcraftEUDTarget() {
  // Register the target.
  RegisterTargetMachine<StarcraftEUDTargetMachine> X(getTheStarcraftEUDTarget());
}

static std::string computeDataLayout() {
  return "e-p:32:32:32-i8:8:32-i16:16:32-n32";
}

static Reloc::Model getEffectiveRelocModel(Optional<Reloc::Model> RM) {
  if (!RM.hasValue())
    return Reloc::Static;
  return *RM;
}

static CodeModel::Model getEffectiveCodeModel(Optional<CodeModel::Model> CM,
                                              Reloc::Model RM, bool JIT) {
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
    : LLVMTargetMachine(
          T, computeDataLayout(), TT, CPU, FS, Options,
          getEffectiveRelocModel(RM),
          getEffectiveCodeModel(CM, getEffectiveRelocModel(RM), JIT), OL),
      TLOF(make_unique<StarcraftEUDTargetObjectFile>()),
      Subtarget(TT, CPU, FS, *this) {
  initAsmInfo();
}

StarcraftEUDTargetMachine::~StarcraftEUDTargetMachine() {}

const StarcraftEUDSubtarget *
StarcraftEUDTargetMachine::getSubtargetImpl(const Function &F) const {
  Attribute CPUAttr = F.getFnAttribute("target-cpu");
  Attribute FSAttr = F.getFnAttribute("target-features");

  std::string CPU = !CPUAttr.hasAttribute(Attribute::None)
                        ? CPUAttr.getValueAsString().str()
                        : TargetCPU;
  std::string FS = !FSAttr.hasAttribute(Attribute::None)
                       ? FSAttr.getValueAsString().str()
                       : TargetFS;

  auto &I = SubtargetMap[CPU + FS];
  if (!I) {
    // This needs to be done before we create a new subtarget since any
    // creation will depend on the TM and the code generation flags on the
    // function that reside in TargetOptions.
    resetTargetOptions(F);
    I = llvm::make_unique<StarcraftEUDSubtarget>(TargetTriple, CPU, FS, *this);
  }
  return I.get();
}

namespace {
/// StarcraftEUD Code Generator Pass Configuration Options.
class StarcraftEUDPassConfig : public TargetPassConfig {
public:
  StarcraftEUDPassConfig(StarcraftEUDTargetMachine &TM, PassManagerBase *PM)
      : TargetPassConfig(TM, *PM) {}

  StarcraftEUDTargetMachine &getStarcraftEUDTargetMachine() const {
    return getTM<StarcraftEUDTargetMachine>();
  }

  void addCodeGenPrepare() override;
  bool addInstSelector() override;
  void addIRPasses() override;
};
} // namespace

TargetPassConfig *StarcraftEUDTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new StarcraftEUDPassConfig(*this, &PM);
}

void StarcraftEUDPassConfig::addCodeGenPrepare() {
  TargetPassConfig::addCodeGenPrepare();
}

void StarcraftEUDPassConfig::addIRPasses() { TargetPassConfig::addIRPasses(); }

// Install an instruction selector pass using
// the ISelDag to gen StarcraftEUD code.
bool StarcraftEUDPassConfig::addInstSelector() {
  addPass(createStarcraftEUDISelDag(getStarcraftEUDTargetMachine(), getOptLevel()));
  return false;
}
