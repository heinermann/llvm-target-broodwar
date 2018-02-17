//===-- StarcraftEUD.h - Top-level interface for StarcraftEUD representation --*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in
// the LLVM StarcraftEUD back-end.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_NIOS2_NIOS2_H
#define LLVM_LIB_TARGET_NIOS2_NIOS2_H

#include "MCTargetDesc/StarcraftEUDMCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class FunctionPass;
class formatted_raw_ostream;
class StarcraftEUDTargetMachine;
class AsmPrinter;
class MachineInstr;
class MCInst;

FunctionPass *createStarcraftEUDISelDag(StarcraftEUDTargetMachine &TM,
                                 CodeGenOpt::Level OptLevel);
void LowerStarcraftEUDMachineInstToMCInst(const MachineInstr *MI, MCInst &OutMI,
                                   AsmPrinter &AP);
} // namespace llvm

#endif
