//===-- StarcraftEUDSubtarget.cpp - StarcraftEUD Subtarget Information ------------------===//
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

using namespace llvm;

#define DEBUG_TYPE "broodwar-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "StarcraftEUDGenSubtargetInfo.inc"

void StarcraftEUDSubtarget::anchor() {}

StarcraftEUDSubtarget::StarcraftEUDSubtarget(const Triple &TT, const std::string &CPU,
                               const std::string &FS, const TargetMachine &TM)
    :

      // StarcraftEUDGenSubtargetInfo will display features by llc -march=broodwar
      // -mcpu=help
      StarcraftEUDGenSubtargetInfo(TT, CPU, FS), TargetTriple(TT),
      InstrInfo(initializeSubtargetDependencies(CPU, FS)), TLInfo(TM, *this),
      FrameLowering(*this) {}

StarcraftEUDSubtarget &StarcraftEUDSubtarget::initializeSubtargetDependencies(StringRef CPU,
                                                                StringRef FS) {
  // Parse features string.
  ParseSubtargetFeatures(CPU, FS);

  return *this;
}
