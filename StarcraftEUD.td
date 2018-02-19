//===-- StarcraftEUD.td - Describe the StarcraftEUD Target Machine -----------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

include "StarcraftEUDRegisterInfo.td"
include "StarcraftEUDCallingConv.td"
include "StarcraftEUDInstrInfo.td"

def StarcraftEUDInstrInfo : InstrInfo;

class Proc<string Name, list<SubtargetFeature> Features>
 : Processor<Name, NoItineraries, Features>;

def : Proc<"generic", []>;
def : Proc<"v1", []>;
def : Proc<"v2", []>;
def : Proc<"probe", []>;

def DummyFeature : SubtargetFeature<"dummy", "isDummyMode",
                                    "true", "unused feature">;

def StarcraftEUDInstPrinter : AsmWriter {
  string AsmWriterClassName  = "InstPrinter";
  bit isMCAsmWriter = 1;
}

def StarcraftEUDAsmParser : AsmParser {
  bit HasMnemonicFirst = 0;
}

def StarcraftEUDAsmParserVariant : AsmParserVariant {
  int Variant = 0;
  string Name = "StarcraftEUD";
  string BreakCharacters = ".";
  string TokenizingCharacters = "#()[]=:.<>!+*";
}

def StarcraftEUD : Target {
  let InstructionSet = StarcraftEUDInstrInfo;
  let AssemblyWriters = [StarcraftEUDInstPrinter];
  let AssemblyParsers = [StarcraftEUDAsmParser];
  let AssemblyParserVariants = [StarcraftEUDAsmParserVariant];
}
