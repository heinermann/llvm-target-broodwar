//===-- StarcraftEUDFrameLowering.cpp - StarcraftEUD Frame Information ------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the StarcraftEUD implementation of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#include "StarcraftEUDFrameLowering.h"

#include "StarcraftEUDSubtarget.h"
#include "llvm/CodeGen/MachineFunction.h"

using namespace llvm;

bool StarcraftEUDFrameLowering::hasFP(const MachineFunction &MF) const { return true; }

void StarcraftEUDFrameLowering::emitPrologue(MachineFunction &MF,
                                      MachineBasicBlock &MBB) const {}

void StarcraftEUDFrameLowering::emitEpilogue(MachineFunction &MF,
                                      MachineBasicBlock &MBB) const {}
