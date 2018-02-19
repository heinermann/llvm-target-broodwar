//===-- StarcraftEUDInstrFormats.td - StarcraftEUD Instruction Formats -------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

class StarcraftEUDOpClass<bits<3> val> {
  bits<3> Value = val;
}

def StarcraftEUD_LD    : StarcraftEUDOpClass<0x0>;
def StarcraftEUD_LDX   : StarcraftEUDOpClass<0x1>;
def StarcraftEUD_ST    : StarcraftEUDOpClass<0x2>;
def StarcraftEUD_STX   : StarcraftEUDOpClass<0x3>;
def StarcraftEUD_ALU   : StarcraftEUDOpClass<0x4>;
def StarcraftEUD_JMP   : StarcraftEUDOpClass<0x5>;
def StarcraftEUD_ALU64 : StarcraftEUDOpClass<0x7>;

class StarcraftEUDSrcType<bits<1> val> {
  bits<1> Value = val;
}

def StarcraftEUD_K : StarcraftEUDSrcType<0x0>;
def StarcraftEUD_X : StarcraftEUDSrcType<0x1>;

class StarcraftEUDArithOp<bits<4> val> {
  bits<4> Value = val;
}

def StarcraftEUD_ADD  : StarcraftEUDArithOp<0x0>;
def StarcraftEUD_SUB  : StarcraftEUDArithOp<0x1>;
def StarcraftEUD_MUL  : StarcraftEUDArithOp<0x2>;
def StarcraftEUD_DIV  : StarcraftEUDArithOp<0x3>;
def StarcraftEUD_OR   : StarcraftEUDArithOp<0x4>;
def StarcraftEUD_AND  : StarcraftEUDArithOp<0x5>;
def StarcraftEUD_LSH  : StarcraftEUDArithOp<0x6>;
def StarcraftEUD_RSH  : StarcraftEUDArithOp<0x7>;
def StarcraftEUD_NEG  : StarcraftEUDArithOp<0x8>;
def StarcraftEUD_XOR  : StarcraftEUDArithOp<0xa>;
def StarcraftEUD_MOV  : StarcraftEUDArithOp<0xb>;
def StarcraftEUD_ARSH : StarcraftEUDArithOp<0xc>;
def StarcraftEUD_END  : StarcraftEUDArithOp<0xd>;

class StarcraftEUDEndDir<bits<1> val> {
  bits<1> Value = val;
}

def StarcraftEUD_TO_LE : StarcraftEUDSrcType<0x0>;
def StarcraftEUD_TO_BE : StarcraftEUDSrcType<0x1>;

class StarcraftEUDJumpOp<bits<4> val> {
  bits<4> Value = val;
}

def StarcraftEUD_JA   : StarcraftEUDJumpOp<0x0>;
def StarcraftEUD_JEQ  : StarcraftEUDJumpOp<0x1>;
def StarcraftEUD_JGT  : StarcraftEUDJumpOp<0x2>;
def StarcraftEUD_JGE  : StarcraftEUDJumpOp<0x3>;
def StarcraftEUD_JNE  : StarcraftEUDJumpOp<0x5>;
def StarcraftEUD_JSGT : StarcraftEUDJumpOp<0x6>;
def StarcraftEUD_JSGE : StarcraftEUDJumpOp<0x7>;
def StarcraftEUD_CALL : StarcraftEUDJumpOp<0x8>;
def StarcraftEUD_EXIT : StarcraftEUDJumpOp<0x9>;
def StarcraftEUD_JLT  : StarcraftEUDJumpOp<0xa>;
def StarcraftEUD_JLE  : StarcraftEUDJumpOp<0xb>;
def StarcraftEUD_JSLT : StarcraftEUDJumpOp<0xc>;
def StarcraftEUD_JSLE : StarcraftEUDJumpOp<0xd>;

class StarcraftEUDWidthModifer<bits<2> val> {
  bits<2> Value = val;
}

def StarcraftEUD_W  : StarcraftEUDWidthModifer<0x0>;
def StarcraftEUD_H  : StarcraftEUDWidthModifer<0x1>;
def StarcraftEUD_B  : StarcraftEUDWidthModifer<0x2>;
def StarcraftEUD_DW : StarcraftEUDWidthModifer<0x3>;

class StarcraftEUDModeModifer<bits<3> val> {
  bits<3> Value = val;
}

def StarcraftEUD_IMM  : StarcraftEUDModeModifer<0x0>;
def StarcraftEUD_ABS  : StarcraftEUDModeModifer<0x1>;
def StarcraftEUD_IND  : StarcraftEUDModeModifer<0x2>;
def StarcraftEUD_MEM  : StarcraftEUDModeModifer<0x3>;
def StarcraftEUD_XADD : StarcraftEUDModeModifer<0x6>;

class InstStarcraftEUD<dag outs, dag ins, string asmstr, list<dag> pattern>
  : Instruction {
  field bits<64> Inst;
  field bits<64> SoftFail = 0;
  let Size = 8;

  let Namespace = "StarcraftEUD";
  let DecoderNamespace = "StarcraftEUD";

  StarcraftEUDOpClass StarcraftEUDClass;
  let Inst{58-56} = StarcraftEUDClass.Value;

  dag OutOperandList = outs;
  dag InOperandList = ins;
  let AsmString = asmstr;
  let Pattern = pattern;
}

// Pseudo instructions
class Pseudo<dag outs, dag ins, string asmstr, list<dag> pattern>
  : InstStarcraftEUD<outs, ins, asmstr, pattern> {
  let Inst{63-0} = 0;
  let isPseudo = 1;
}
