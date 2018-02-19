//===-- StarcraftEUDMCTargetDesc.h - StarcraftEUD Target Descriptions -------------*- C++ -*-===//
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

#ifndef LLVM_LIB_TARGET_STARCRAFTEUD_MCTARGETDESC_STARCRAFTEUDMCTARGETDESC_H
#define LLVM_LIB_TARGET_STARCRAFTEUD_MCTARGETDESC_STARCRAFTEUDMCTARGETDESC_H

#include "llvm/Config/config.h"
#include "llvm/Support/DataTypes.h"

#include <memory>

namespace llvm {
class MCAsmBackend;
class MCCodeEmitter;
class MCContext;
class MCInstrInfo;
class MCObjectWriter;
class MCRegisterInfo;
class MCSubtargetInfo;
class MCTargetOptions;
class StringRef;
class Target;
class Triple;
class raw_ostream;
class raw_pwrite_stream;

Target &getTheStarcraftEUDleTarget();
Target &getTheStarcraftEUDbeTarget();
Target &getTheStarcraftEUDTarget();

MCCodeEmitter *createStarcraftEUDMCCodeEmitter(const MCInstrInfo &MCII,
                                      const MCRegisterInfo &MRI,
                                      MCContext &Ctx);
MCCodeEmitter *createStarcraftEUDbeMCCodeEmitter(const MCInstrInfo &MCII,
                                        const MCRegisterInfo &MRI,
                                        MCContext &Ctx);

MCAsmBackend *createStarcraftEUDAsmBackend(const Target &T, const MCSubtargetInfo &STI,
                                  const MCRegisterInfo &MRI,
                                  const MCTargetOptions &Options);
MCAsmBackend *createStarcraftEUDbeAsmBackend(const Target &T, const MCSubtargetInfo &STI,
                                    const MCRegisterInfo &MRI,
                                    const MCTargetOptions &Options);

std::unique_ptr<MCObjectWriter> createStarcraftEUDELFObjectWriter(raw_pwrite_stream &OS,
                                                         uint8_t OSABI,
                                                         bool IsLittleEndian);
}

// Defines symbolic names for StarcraftEUD registers.  This defines a mapping from
// register name to register number.
//
#define GET_REGINFO_ENUM
#include "StarcraftEUDGenRegisterInfo.inc"

// Defines symbolic names for the StarcraftEUD instructions.
//
#define GET_INSTRINFO_ENUM
#include "StarcraftEUDGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "StarcraftEUDGenSubtargetInfo.inc"

#endif
