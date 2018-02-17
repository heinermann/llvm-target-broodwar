//===-- StarcraftEUD.td - Describe the StarcraftEUD Target Machine -------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Calling Conv, Instruction Descriptions
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"
include "StarcraftEUDRegisterInfo.td"
include "StarcraftEUDSchedule.td"
include "StarcraftEUDInstrInfo.td"
include "StarcraftEUDCallingConv.td"

//===----------------------------------------------------------------------===//
// StarcraftEUD processors supported.
//===----------------------------------------------------------------------===//

class Proc<string Name, list<SubtargetFeature> Features>
 : Processor<Name, StarcraftEUDGenericItineraries, Features>;

def : Proc<"generic", []>;

def StarcraftEUDInstrInfo : InstrInfo;

def StarcraftEUDAsmParser : AsmParser {
  let ShouldEmitMatchRegisterName = 0;
}

//===----------------------------------------------------------------------===//
// Declare the target which we are implementing
//===----------------------------------------------------------------------===//

def StarcraftEUDAsmWriter : AsmWriter {
  string AsmWriterClassName  = "InstPrinter";
  int PassSubtarget = 1;
  int Variant = 0;
}

def StarcraftEUD : Target {
// def StarcraftEUDInstrInfo : InstrInfo as before.
  let InstructionSet = StarcraftEUDInstrInfo;
  let AssemblyParsers = [StarcraftEUDAsmParser];
  let AssemblyWriters = [StarcraftEUDAsmWriter];
}
