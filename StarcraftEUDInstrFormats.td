//===-- StarcraftEUDInstrFormats.td - StarcraftEUD Instruction Formats ---*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Describe NIOS2 instructions format
//
//
//===----------------------------------------------------------------------===//

// Format specifies the encoding used by the instruction.  This is part of the
// ad-hoc solution used to emit machine instruction encodings by our machine
// code emitter.
class Format<bits<6> val> {
  bits<6> Value = val;
}

def Pseudo     : Format<0>;
// StarcraftEUD R1 instr formats:
def FrmI       : Format<1>;
def FrmR       : Format<2>;
def FrmJ       : Format<3>;
def FrmOther   : Format<4>;  // Instruction w/ a custom format
// StarcraftEUD R2 instr 32-bit formats:
def FrmL26     : Format<5>;  // corresponds to J format in R1
def FrmF2I16   : Format<6>;  // corresponds to I format in R1
def FrmF2X4I12 : Format<7>;
def FrmF1X4I12 : Format<8>;
def FrmF1X4L17 : Format<9>;
def FrmF3X6L5  : Format<10>; // corresponds to R format in R1
def FrmF2X6L10 : Format<11>;
def FrmF3X6    : Format<12>; // corresponds to R format in R1
def FrmF3X8    : Format<13>; // corresponds to custom format in R1
// StarcraftEUD R2 instr 16-bit formats:
def FrmI10     : Format<14>;
def FrmT1I7    : Format<15>; 
def FrmT2I4    : Format<16>;
def FrmT1X1I6  : Format<17>;
def FrmX1I7    : Format<18>;
def FrmL5I4X1  : Format<19>;
def FrmT2X1L3  : Format<20>;
def FrmT2X1I3  : Format<21>;
def FrmT3X1    : Format<22>;
def FrmT2X3    : Format<23>;
def FrmF1X1    : Format<24>;
def FrmX2L5    : Format<25>;
def FrmF1I5    : Format<26>;
def FrmF2      : Format<27>;

//===----------------------------------------------------------------------===//
// Instruction Predicates:
//===----------------------------------------------------------------------===//

class PredicateControl {
  // Predicates related to specific target CPU features
  list<Predicate> FeaturePredicates = [];
  // Predicates for the instruction group membership in given ISA
  list<Predicate> InstrPredicates = [];
  
  list<Predicate> Predicates = !listconcat(FeaturePredicates, InstrPredicates);
}

//===----------------------------------------------------------------------===//
// Base classes for 32-bit, 16-bit and pseudo instructions
//===----------------------------------------------------------------------===//

class StarcraftEUDInst32<dag outs, dag ins, string asmstr, list<dag> pattern,
                  InstrItinClass itin, Format f>: Instruction,
                                      PredicateControl {
  field bits<32> Inst;
  Format Form = f;

  let Namespace = "StarcraftEUD";
  let Size = 4;

  bits<6> Opcode = 0;

  // Bottom 6 bits are the 'opcode' field
  let Inst{5-0} = Opcode;

  let OutOperandList = outs;
  let InOperandList  = ins;

  let AsmString = asmstr;
  let Pattern   = pattern;
  let Itinerary = itin;

  // Attributes specific to StarcraftEUD instructions:

  // TSFlags layout should be kept in sync with StarcraftEUDInstrInfo.h.
  let TSFlags{5-0} = Form.Value;
  let DecoderNamespace = "StarcraftEUD";
  field bits<32> SoftFail = 0;
}

class StarcraftEUDPseudo<dag outs, dag ins, string asmstr, list<dag> pattern,
      InstrItinClass Itin = IIPseudo>:
  StarcraftEUDInst32<outs, ins, asmstr, pattern, Itin, Pseudo> {

  let isCodeGenOnly = 1;
  let isPseudo = 1;
}

//===----------------------------------------------------------------------===//
// Format I instruction class in StarcraftEUD : <|A|B|immediate|opcode|>
//===----------------------------------------------------------------------===//

class FI<bits<6> op, dag outs, dag ins, string asmstr, list<dag> pattern,
         InstrItinClass itin>: StarcraftEUDInst32<outs, ins, asmstr,
                                       pattern, itin, FrmI> {

  bits<5>  rA;
  bits<5>  rB;
  bits<16> imm;

  let Opcode = op;

  let Inst{31-27} = rA;
  let Inst{26-22} = rB;
  let Inst{21-6} = imm;
}


//===----------------------------------------------------------------------===//
// Format R instruction : <|A|B|C|opx|imm|opcode|>
//===----------------------------------------------------------------------===//

class FR<bits<6> opx, dag outs, dag ins, string asmstr, list<dag> pattern,
         InstrItinClass itin>: StarcraftEUDInst32<outs, ins, asmstr,
                                       pattern, itin, FrmR> {
  bits<5> rA;
  bits<5> rB;
  bits<5> rC;
  bits<5> imm = 0;

  let Opcode = 0x3a; /* opcode is always 0x3a for R instr. */

  let Inst{31-27} = rA;
  let Inst{26-22} = rB;
  let Inst{21-17} = rC;
  let Inst{16-11} = opx; /* opx stands for opcode extension */
  let Inst{10-6}  = imm; /* optional 5-bit immediate value */
}

//===----------------------------------------------------------------------===//
// Format J instruction class in StarcraftEUD : <|address|opcode|>
//===----------------------------------------------------------------------===//

class FJ<bits<6> op, dag outs, dag ins, string asmstr, list<dag> pattern,
         InstrItinClass itin>:
      StarcraftEUDInst32<outs, ins, asmstr, pattern, itin, FrmJ> {
  bits<26> addr;
  let Opcode = op;
  let Inst{31-6} = addr;
}

//===----------------------------------------------------------------------===//
// Format F3X6 (R2) instruction : <|opx|RSV|C|B|A|opcode|>
//===----------------------------------------------------------------------===//

class F3X6<bits<6> opx, dag outs, dag ins, string asmstr, list<dag> pattern,
           InstrItinClass itin>:
      StarcraftEUDInst32<outs, ins, asmstr, pattern, itin, FrmF3X6> {
  bits<5> rC;
  bits<5> rB;
  bits<5> rA;
  bits<5> rsv = 0;

  let Opcode = 0x20; /* opcode is always 0x20 (OPX group) for F3X6 instr. */

  let Inst{31-26} = opx; /* opx stands for opcode extension */
  let Inst{25-21} = rsv;
  let Inst{20-16} = rC;
  let Inst{15-11} = rB;
  let Inst{10-6}  = rA;
}

//===----------------------------------------------------------------------===//
// Multiclasses for common instructions of both R1 and R2:
//===----------------------------------------------------------------------===//

// Multiclass for instructions that have R format in R1 and F3X6 format in R2
// and their opx values are the same in R1 and R2
multiclass CommonInstr_R_F3X6<bits<6> opx, dag outs, dag ins, string asmstr,
                              list<dag> pattern, InstrItinClass itin> {
  def NAME# : FR<opx, outs, ins, asmstr, pattern, itin>;
}

// Multiclass for instructions that have I format in R1 and F2I16 format in R2
// and their op code values are the same in R1 and R2
multiclass CommonInstr_I_F2I16<bits<6> op, dag outs, dag ins, string asmstr,
                               list<dag> pattern, InstrItinClass itin> {
  def NAME# : FI<op, outs, ins, asmstr, pattern, itin>;
}
