//===-- StarcraftEUDMCTargetDesc.h - StarcraftEUD Target Descriptions ---------*- C++ -*-===//
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

#ifndef LLVM_LIB_TARGET_NIOS2_MCTARGETDESC_NIOS2MCTARGETDESC_H
#define LLVM_LIB_TARGET_NIOS2_MCTARGETDESC_NIOS2MCTARGETDESC_H

#include <memory>

namespace llvm {
class MCAsmBackend;
class MCObjectWriter;
class MCRegisterInfo;
class MCTargetOptions;
class Target;
class Triple;
class StringRef;
class raw_pwrite_stream;

Target &getTheStarcraftEUDTarget();

} // namespace llvm

// Defines symbolic names for StarcraftEUD registers.  This defines a mapping from
// register name to register number.
#define GET_REGINFO_ENUM
#include "StarcraftEUDGenRegisterInfo.inc"

// Defines symbolic names for the StarcraftEUD instructions.
#define GET_INSTRINFO_ENUM
#include "StarcraftEUDGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "StarcraftEUDGenSubtargetInfo.inc"

#endif
