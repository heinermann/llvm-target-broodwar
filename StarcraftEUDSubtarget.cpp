//===-- StarcraftEUDSubtarget.cpp - StarcraftEUD Subtarget Information ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the StarcraftEUD specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#include "StarcraftEUDSubtarget.h"
#include "StarcraftEUD.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "broodwar-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "StarcraftEUDGenSubtargetInfo.inc"

void StarcraftEUDSubtarget::anchor() {}

StarcraftEUDSubtarget &StarcraftEUDSubtarget::initializeSubtargetDependencies(StringRef CPU,
                                                            StringRef FS) {
  initializeEnvironment();
  initSubtargetFeatures(CPU, FS);
  return *this;
}

void StarcraftEUDSubtarget::initializeEnvironment() {
  HasJmpExt = false;
}

void StarcraftEUDSubtarget::initSubtargetFeatures(StringRef CPU, StringRef FS) {
  /*if (CPU == "probe")
    CPU = sys::detail::getHostCPUNameForBPF();
  if (CPU == "generic" || CPU == "v1")
    return;
  if (CPU == "v2") {
    HasJmpExt = true;
    return;
  }*/
}

StarcraftEUDSubtarget::StarcraftEUDSubtarget(const Triple &TT, const std::string &CPU,
                           const std::string &FS, const TargetMachine &TM)
    : StarcraftEUDGenSubtargetInfo(TT, CPU, FS), InstrInfo(),
      FrameLowering(initializeSubtargetDependencies(CPU, FS)),
      TLInfo(TM, *this) {}
