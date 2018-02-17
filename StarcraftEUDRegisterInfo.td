//===-- StarcraftEUDRegisterInfo.td - StarcraftEUD Register defs ---------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// We have bank of 32 registers.
class StarcraftEUDReg<string n> : Register<n> {
  field bits<5> Num;
  let Namespace = "StarcraftEUD";
}

// StarcraftEUD CPU Registers
class StarcraftEUDGPRReg<bits<5> num, string n> : StarcraftEUDReg<n> {
  let Num = num;
}

//===----------------------------------------------------------------------===//
//  Registers
//===----------------------------------------------------------------------===//

let Namespace = "StarcraftEUD" in {
  // General Purpose Registers
  def ZERO : StarcraftEUDGPRReg<0, "zero">, DwarfRegNum<[ 0 ]>;
  def AT : StarcraftEUDGPRReg<1, "at">, DwarfRegNum<[ 1 ]>;
  foreach RegNum = 2 - 23 in {
    def R #RegNum : StarcraftEUDGPRReg<RegNum, "r" #RegNum>, DwarfRegNum<[ RegNum ]>;
  }
  def ET : StarcraftEUDGPRReg<24, "et">, DwarfRegNum<[ 24 ]>;
  def BT : StarcraftEUDGPRReg<25, "bt">, DwarfRegNum<[ 25 ]>;
  def GP : StarcraftEUDGPRReg<26, "gp">, DwarfRegNum<[ 26 ]>;
  def SP : StarcraftEUDGPRReg<27, "sp">, DwarfRegNum<[ 27 ]>;
  def FP : StarcraftEUDGPRReg<28, "fp">, DwarfRegNum<[ 28 ]>;
  def EA : StarcraftEUDGPRReg<29, "ea">, DwarfRegNum<[ 29 ]>;
  def BA : StarcraftEUDGPRReg<30, "ba">, DwarfRegNum<[ 30 ]>;
  def RA : StarcraftEUDGPRReg<31, "ra">, DwarfRegNum<[ 31 ]>;
  def PC : StarcraftEUDReg<"pc">, DwarfRegNum<[ 32 ]>;
}

//===----------------------------------------------------------------------===//
// Register Classes
//===----------------------------------------------------------------------===//

def CPURegs : RegisterClass<"StarcraftEUD", [ i32 ], 32,
                            (add
                            // Reserved
                            ZERO,
                            AT,
                            // Return Values and Arguments
                            (sequence "R%u", 2, 7),
                            // Not preserved across procedure calls
                            // Caller saved
                            (sequence "R%u", 8, 15),
                            // Callee saved
                            (sequence "R%u", 16, 23),
                            // Reserved
                            ET, BT, GP, SP, FP, EA, BA, RA, PC)>;
