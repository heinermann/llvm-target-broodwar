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
// StarcraftEUD Subtarget features
//===----------------------------------------------------------------------===//
def FeatureStarcraftEUDr1     : SubtargetFeature<"broodwarr1", "StarcraftEUDArchVersion", 
                                "StarcraftEUDr1", "StarcraftEUD R1 ISA Support">;
def FeatureStarcraftEUDr2     : SubtargetFeature<"broodwarr2", "StarcraftEUDArchVersion",                      
                               "StarcraftEUDr2", "StarcraftEUD R2 ISA Support">;

//===----------------------------------------------------------------------===//
// StarcraftEUD processors supported.
//===----------------------------------------------------------------------===//

class Proc<string Name, list<SubtargetFeature> Features>
 : Processor<Name, StarcraftEUDGenericItineraries, Features>;

def : Proc<"broodwarr1", [FeatureStarcraftEUDr1]>;
def : Proc<"broodwarr2", [FeatureStarcraftEUDr2]>;

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
