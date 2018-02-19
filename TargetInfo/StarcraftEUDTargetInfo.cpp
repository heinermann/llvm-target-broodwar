//===-- StarcraftEUDTargetInfo.cpp - StarcraftEUD Target Implementation ---------------------===//
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

namespace llvm {
Target &getTheStarcraftEUDleTarget() {
  static Target TheStarcraftEUDleTarget;
  return TheStarcraftEUDleTarget;
}
Target &getTheStarcraftEUDbeTarget() {
  static Target TheStarcraftEUDbeTarget;
  return TheStarcraftEUDbeTarget;
}
Target &getTheStarcraftEUDTarget() {
  static Target TheStarcraftEUDTarget;
  return TheStarcraftEUDTarget;
}
} // namespace llvm

extern "C" void LLVMInitializeStarcraftEUDTargetInfo() {
  TargetRegistry::RegisterTarget(getTheStarcraftEUDTarget(), "broodwar", "StarcraftEUD (host endian)",
                                 "StarcraftEUD", [](Triple::ArchType) { return true; },
                                 false);
  RegisterTarget<Triple::bpfel, /*HasJIT=*/false> X(
      getTheStarcraftEUDleTarget(), "broodwarel", "StarcraftEUD (little endian)", "StarcraftEUD");
  RegisterTarget<Triple::bpfeb, /*HasJIT=*/false> Y(getTheStarcraftEUDbeTarget(), "broodwareb",
                                                   "StarcraftEUD (big endian)", "StarcraftEUD");
}
