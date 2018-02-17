//===-- StarcraftEUDTargetStreamer.cpp - StarcraftEUD Target Streamer Methods -----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides StarcraftEUD specific target streamer methods.
//
//===----------------------------------------------------------------------===//

#include "StarcraftEUDTargetStreamer.h"

using namespace llvm;

StarcraftEUDTargetStreamer::StarcraftEUDTargetStreamer(MCStreamer &S) : MCTargetStreamer(S) {}

StarcraftEUDTargetAsmStreamer::StarcraftEUDTargetAsmStreamer(MCStreamer &S,
                                               formatted_raw_ostream &OS)
    : StarcraftEUDTargetStreamer(S), OS(OS) {}
