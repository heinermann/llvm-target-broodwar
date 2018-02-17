//===-- llvm/Target/StarcraftEUDTargetObjectFile.h - StarcraftEUD Object Info -*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_NIOS2_NIOS2TARGETOBJECTFILE_H
#define LLVM_LIB_TARGET_NIOS2_NIOS2TARGETOBJECTFILE_H

#include "StarcraftEUDTargetMachine.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"

namespace llvm {

class StarcraftEUDTargetObjectFile : public TargetLoweringObjectFileELF {
  const StarcraftEUDTargetMachine *TM;

public:
  StarcraftEUDTargetObjectFile() : TargetLoweringObjectFileELF() {}

  void Initialize(MCContext &Ctx, const TargetMachine &TM) override;
};
} // end namespace llvm

#endif
