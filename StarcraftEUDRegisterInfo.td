//===-- StarcraftEUDRegisterInfo.td - StarcraftEUD Register defs -------------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Declarations that describe the StarcraftEUD register file
//===----------------------------------------------------------------------===//

let Namespace = "StarcraftEUD" in {
  def sub_32 : SubRegIndex<32>;
}

class Wi<bits<16> Enc, string n> : Register<n> {
  let HWEncoding = Enc;
  let Namespace = "StarcraftEUD";
}

// Registers are identified with 4-bit ID numbers.
// Ri - 64-bit integer registers
class Ri<bits<16> Enc, string n, list<Register> subregs>
  : RegisterWithSubRegs<n, subregs> {
  let HWEncoding = Enc;
  let Namespace = "StarcraftEUD";
  let SubRegIndices = [sub_32];
}

foreach I = 0-11 in {
  // 32-bit Integer (alias to low part of 64-bit register).
  def W#I  : Wi<I,  "w"#I>,  DwarfRegNum<[I]>;
  // 64-bit Integer registers
  def R#I  : Ri<I,  "r"#I,  [!cast<Wi>("W"#I)]>,  DwarfRegNum<[I]>;
}

// Register classes.
def GPR32 : RegisterClass<"StarcraftEUD", [i32], 32, (add
  (sequence "W%u", 1, 9),
  W0, // Return value
  W11, // Stack Ptr
  W10  // Frame Ptr
)>;

def GPR : RegisterClass<"StarcraftEUD", [i64], 64, (add
  (sequence "R%u", 1, 9),
  R0, // Return value
  R11, // Stack Ptr
  R10  // Frame Ptr
)>;
