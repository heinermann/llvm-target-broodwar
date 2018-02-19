//===-- StarcraftEUD.h - Top-level interface for StarcraftEUD representation ------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_STARCRAFTEUD_STARCRAFTEUD_H
#define LLVM_LIB_TARGET_STARCRAFTEUD_STARCRAFTEUD_H

#include "MCTargetDesc/StarcraftEUDMCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class StarcraftEUDTargetMachine;

FunctionPass *createStarcraftEUDISelDag(StarcraftEUDTargetMachine &TM);
}

#endif
