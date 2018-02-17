//===-- StarcraftEUDMCTargetDesc.cpp - StarcraftEUD Target Descriptions -----------------===//
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

#include "StarcraftEUDMCTargetDesc.h"
#include "InstPrinter/StarcraftEUDInstPrinter.h"
#include "StarcraftEUDMCAsmInfo.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "StarcraftEUDGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "StarcraftEUDGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "StarcraftEUDGenRegisterInfo.inc"

static MCInstrInfo *createStarcraftEUDMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitStarcraftEUDMCInstrInfo(X); // defined in StarcraftEUDGenInstrInfo.inc
  return X;
}

static MCRegisterInfo *createStarcraftEUDMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitStarcraftEUDMCRegisterInfo(X, StarcraftEUD::R15); // defined in StarcraftEUDGenRegisterInfo.inc
  return X;
}

static MCSubtargetInfo *createStarcraftEUDMCSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
  if (CPU.empty()) CPU = "generic";
  return createStarcraftEUDMCSubtargetInfoImpl(TT, CPU, FS);
  // createStarcraftEUDMCSubtargetInfoImpl defined in StarcraftEUDGenSubtargetInfo.inc
}

static MCAsmInfo *createStarcraftEUDMCAsmInfo(const MCRegisterInfo &MRI,
                                       const Triple &TT) {
  MCAsmInfo *MAI = new StarcraftEUDMCAsmInfo(TT);

  unsigned SP = MRI.getDwarfRegNum(StarcraftEUD::SP, true);
  MCCFIInstruction Inst = MCCFIInstruction::createDefCfa(nullptr, SP, 0);
  MAI->addInitialFrameState(Inst);

  return MAI;
}

static MCInstPrinter *createStarcraftEUDMCInstPrinter(const Triple &T,
                                               unsigned SyntaxVariant,
                                               const MCAsmInfo &MAI,
                                               const MCInstrInfo &MII,
                                               const MCRegisterInfo &MRI) {
  return new StarcraftEUDInstPrinter(MAI, MII, MRI);
}

extern "C" void LLVMInitializeStarcraftEUDTargetMC() {
  Target *T = &getTheStarcraftEUDTarget();

  // Register the MC asm info.
  RegisterMCAsmInfoFn X(*T, createStarcraftEUDMCAsmInfo);

  // Register the MC instruction info.
  TargetRegistry::RegisterMCInstrInfo(*T, createStarcraftEUDMCInstrInfo);

  // Register the MC register info.
  TargetRegistry::RegisterMCRegInfo(*T, createStarcraftEUDMCRegisterInfo);

  // Register the MC subtarget info.
  TargetRegistry::RegisterMCSubtargetInfo(*T, createStarcraftEUDMCSubtargetInfo);
  // Register the MCInstPrinter.
  TargetRegistry::RegisterMCInstPrinter(*T, createStarcraftEUDMCInstPrinter);

  // Register the asm backend.
  //TargetRegistry::RegisterMCAsmBackend(*T, createStarcraftEUDAsmBackend);
}
