//===-- StarcraftEUDMCTargetDesc.cpp - StarcraftEUD Target Descriptions ---------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides StarcraftEUD specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/StarcraftEUDMCTargetDesc.h"
#include "StarcraftEUD.h"
#include "InstPrinter/StarcraftEUDInstPrinter.h"
#include "MCTargetDesc/StarcraftEUDMCAsmInfo.h"
#include "llvm/MC/MCInstrAnalysis.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/TargetRegistry.h"

#define GET_INSTRINFO_MC_DESC
#include "StarcraftEUDGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "StarcraftEUDGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "StarcraftEUDGenRegisterInfo.inc"

using namespace llvm;

static MCInstrInfo *createStarcraftEUDMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitStarcraftEUDMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createStarcraftEUDMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitStarcraftEUDMCRegisterInfo(X, StarcraftEUD::R11 /* RAReg doesn't exist */);
  return X;
}

static MCSubtargetInfo *createStarcraftEUDMCSubtargetInfo(const Triple &TT,
                                                 StringRef CPU, StringRef FS) {
  return createStarcraftEUDMCSubtargetInfoImpl(TT, CPU, FS);
}

static MCStreamer *createStarcraftEUDMCStreamer(const Triple &T, MCContext &Ctx,
                                       std::unique_ptr<MCAsmBackend> &&MAB,
                                       raw_pwrite_stream &OS,
                                       std::unique_ptr<MCCodeEmitter> &&Emitter,
                                       bool RelaxAll) {
  return createELFStreamer(Ctx, std::move(MAB), OS, std::move(Emitter),
                           RelaxAll);
}

static MCInstPrinter *createStarcraftEUDMCInstPrinter(const Triple &T,
                                             unsigned SyntaxVariant,
                                             const MCAsmInfo &MAI,
                                             const MCInstrInfo &MII,
                                             const MCRegisterInfo &MRI) {
  if (SyntaxVariant == 0)
    return new StarcraftEUDInstPrinter(MAI, MII, MRI);
  return nullptr;
}

namespace {

class StarcraftEUDMCInstrAnalysis : public MCInstrAnalysis {
public:
  explicit StarcraftEUDMCInstrAnalysis(const MCInstrInfo *Info)
      : MCInstrAnalysis(Info) {}

  bool evaluateBranch(const MCInst &Inst, uint64_t Addr, uint64_t Size,
                      uint64_t &Target) const override {
    // The target is the 3rd operand of cond inst and the 1st of uncond inst.
    int16_t Imm;
    if (isConditionalBranch(Inst)) {
      Imm = Inst.getOperand(2).getImm();
    } else if (isUnconditionalBranch(Inst))
      Imm = Inst.getOperand(0).getImm();
    else
      return false;

    Target = Addr + Size + Imm * Size;
    return true;
  }
};

} // end anonymous namespace

static MCInstrAnalysis *createStarcraftEUDInstrAnalysis(const MCInstrInfo *Info) {
  return new StarcraftEUDMCInstrAnalysis(Info);
}

extern "C" void LLVMInitializeStarcraftEUDTargetMC() {
  for (Target *T :
       {&getTheStarcraftEUDleTarget(), &getTheStarcraftEUDbeTarget(), &getTheStarcraftEUDTarget()}) {
    // Register the MC asm info.
    RegisterMCAsmInfo<StarcraftEUDMCAsmInfo> X(*T);

    // Register the MC instruction info.
    TargetRegistry::RegisterMCInstrInfo(*T, createStarcraftEUDMCInstrInfo);

    // Register the MC register info.
    TargetRegistry::RegisterMCRegInfo(*T, createStarcraftEUDMCRegisterInfo);

    // Register the MC subtarget info.
    TargetRegistry::RegisterMCSubtargetInfo(*T,
                                            createStarcraftEUDMCSubtargetInfo);

    // Register the object streamer
    TargetRegistry::RegisterELFStreamer(*T, createStarcraftEUDMCStreamer);

    // Register the MCInstPrinter.
    TargetRegistry::RegisterMCInstPrinter(*T, createStarcraftEUDMCInstPrinter);

    // Register the MC instruction analyzer.
    TargetRegistry::RegisterMCInstrAnalysis(*T, createStarcraftEUDInstrAnalysis);
  }

  // Register the MC code emitter
  TargetRegistry::RegisterMCCodeEmitter(getTheStarcraftEUDleTarget(),
                                        createStarcraftEUDMCCodeEmitter);
  TargetRegistry::RegisterMCCodeEmitter(getTheStarcraftEUDbeTarget(),
                                        createStarcraftEUDbeMCCodeEmitter);

  // Register the ASM Backend
  TargetRegistry::RegisterMCAsmBackend(getTheStarcraftEUDleTarget(),
                                       createStarcraftEUDAsmBackend);
  TargetRegistry::RegisterMCAsmBackend(getTheStarcraftEUDbeTarget(),
                                       createStarcraftEUDbeAsmBackend);

  if (sys::IsLittleEndianHost) {
    TargetRegistry::RegisterMCCodeEmitter(getTheStarcraftEUDTarget(),
                                          createStarcraftEUDMCCodeEmitter);
    TargetRegistry::RegisterMCAsmBackend(getTheStarcraftEUDTarget(),
                                         createStarcraftEUDAsmBackend);
  } else {
    TargetRegistry::RegisterMCCodeEmitter(getTheStarcraftEUDTarget(),
                                          createStarcraftEUDbeMCCodeEmitter);
    TargetRegistry::RegisterMCAsmBackend(getTheStarcraftEUDTarget(),
                                         createStarcraftEUDbeAsmBackend);
  }

}
