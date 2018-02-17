//===-- StarcraftEUDTargetStreamer.h - StarcraftEUD Target Streamer ----------*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_NIOS2_NIOS2TARGETSTREAMER_H
#define LLVM_LIB_TARGET_NIOS2_NIOS2TARGETSTREAMER_H

#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCStreamer.h"

namespace llvm {

class StarcraftEUDTargetStreamer : public MCTargetStreamer {
public:
  StarcraftEUDTargetStreamer(MCStreamer &S);
};

// This part is for ascii assembly output
class StarcraftEUDTargetAsmStreamer : public StarcraftEUDTargetStreamer {
  formatted_raw_ostream &OS;

public:
  StarcraftEUDTargetAsmStreamer(MCStreamer &S, formatted_raw_ostream &OS);
};

} // namespace llvm
#endif
