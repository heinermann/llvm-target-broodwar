//===-- StarcraftEUDTargetInfo.cpp - StarcraftEUD Target Implementation -----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "StarcraftEUD.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

Target &llvm::getTheStarcraftEUDTarget() {
  static Target TheStarcraftEUDTarget;
  return TheStarcraftEUDTarget;
}

extern "C" void LLVMInitializeStarcraftEUDTargetInfo() {
  TargetRegistry::RegisterTarget(getTheStarcraftEUDTarget(), "broodwar", "StarcraftEUD",
                               "StarcraftEUD", [](Triple::ArchType) { return false; },
                               true);
}
