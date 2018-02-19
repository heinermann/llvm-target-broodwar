//===-- StarcraftEUDInstrInfo.td - Target Description for StarcraftEUD Target ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file describes the StarcraftEUD instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

include "StarcraftEUDInstrFormats.td"

// Instruction Operands and Patterns

// These are target-independent nodes, but have target-specific formats.
def SDT_StarcraftEUDCallSeqStart : SDCallSeqStart<[SDTCisVT<0, iPTR>,
                                          SDTCisVT<1, iPTR>]>;
def SDT_StarcraftEUDCallSeqEnd   : SDCallSeqEnd<[SDTCisVT<0, iPTR>, SDTCisVT<1, iPTR>]>;
def SDT_StarcraftEUDCall         : SDTypeProfile<0, -1, [SDTCisVT<0, iPTR>]>;
def SDT_StarcraftEUDSetFlag      : SDTypeProfile<0, 3, [SDTCisSameAs<0, 1>]>;
def SDT_StarcraftEUDSelectCC     : SDTypeProfile<1, 5, [SDTCisSameAs<1, 2>,
                                               SDTCisSameAs<0, 4>,
                                               SDTCisSameAs<4, 5>]>;
def SDT_StarcraftEUDBrCC         : SDTypeProfile<0, 4, [SDTCisSameAs<0, 1>,
                                               SDTCisVT<3, OtherVT>]>;
def SDT_StarcraftEUDWrapper      : SDTypeProfile<1, 1, [SDTCisSameAs<0, 1>,
                                               SDTCisPtrTy<0>]>;

def StarcraftEUDcall         : SDNode<"StarcraftEUDISD::CALL", SDT_StarcraftEUDCall,
                             [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue,
                              SDNPVariadic]>;
def StarcraftEUDretflag      : SDNode<"StarcraftEUDISD::RET_FLAG", SDTNone,
                             [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;
def StarcraftEUDcallseq_start: SDNode<"ISD::CALLSEQ_START", SDT_StarcraftEUDCallSeqStart,
                             [SDNPHasChain, SDNPOutGlue]>;
def StarcraftEUDcallseq_end  : SDNode<"ISD::CALLSEQ_END",   SDT_StarcraftEUDCallSeqEnd,
                             [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;
def StarcraftEUDbrcc         : SDNode<"StarcraftEUDISD::BR_CC", SDT_StarcraftEUDBrCC,
                             [SDNPHasChain, SDNPOutGlue, SDNPInGlue]>;

def StarcraftEUDselectcc     : SDNode<"StarcraftEUDISD::SELECT_CC", SDT_StarcraftEUDSelectCC, [SDNPInGlue]>;
def StarcraftEUDWrapper      : SDNode<"StarcraftEUDISD::Wrapper", SDT_StarcraftEUDWrapper>;
def StarcraftEUDIsLittleEndian : Predicate<"CurDAG->getDataLayout().isLittleEndian()">;
def StarcraftEUDIsBigEndian    : Predicate<"!CurDAG->getDataLayout().isLittleEndian()">;

def brtarget : Operand<OtherVT> {
  let PrintMethod = "printBrTargetOperand";
}
def calltarget : Operand<i64>;

def u64imm   : Operand<i64> {
  let PrintMethod = "printImm64Operand";
}

def i64immSExt32 : PatLeaf<(i64 imm),
                [{return isInt<32>(N->getSExtValue()); }]>;

// Addressing modes.
def ADDRri : ComplexPattern<i64, 2, "SelectAddr", [], []>;
def FIri : ComplexPattern<i64, 2, "SelectFIAddr", [add, or], []>;

// Address operands
def MEMri : Operand<i64> {
  let PrintMethod = "printMemOperand";
  let EncoderMethod = "getMemoryOpValue";
  let DecoderMethod = "decodeMemoryOpValue";
  let MIOperandInfo = (ops GPR, i16imm);
}

// Conditional code predicates - used for pattern matching for jump instructions
def StarcraftEUD_CC_EQ  : PatLeaf<(i64 imm),
                         [{return (N->getZExtValue() == ISD::SETEQ);}]>;
def StarcraftEUD_CC_NE  : PatLeaf<(i64 imm),
                         [{return (N->getZExtValue() == ISD::SETNE);}]>;
def StarcraftEUD_CC_GE  : PatLeaf<(i64 imm),
                         [{return (N->getZExtValue() == ISD::SETGE);}]>;
def StarcraftEUD_CC_GT  : PatLeaf<(i64 imm),
                         [{return (N->getZExtValue() == ISD::SETGT);}]>;
def StarcraftEUD_CC_GTU : PatLeaf<(i64 imm),
                         [{return (N->getZExtValue() == ISD::SETUGT);}]>;
def StarcraftEUD_CC_GEU : PatLeaf<(i64 imm),
                         [{return (N->getZExtValue() == ISD::SETUGE);}]>;
def StarcraftEUD_CC_LE  : PatLeaf<(i64 imm),
                         [{return (N->getZExtValue() == ISD::SETLE);}]>;
def StarcraftEUD_CC_LT  : PatLeaf<(i64 imm),
                         [{return (N->getZExtValue() == ISD::SETLT);}]>;
def StarcraftEUD_CC_LTU : PatLeaf<(i64 imm),
                         [{return (N->getZExtValue() == ISD::SETULT);}]>;
def StarcraftEUD_CC_LEU : PatLeaf<(i64 imm),
                         [{return (N->getZExtValue() == ISD::SETULE);}]>;

// For arithmetic and jump instructions the 8-bit 'code'
// field is divided into three parts:
//
//  +----------------+--------+--------------------+
//  |   4 bits       |  1 bit |   3 bits           |
//  | operation code | source | instruction class  |
//  +----------------+--------+--------------------+
//  (MSB)                                      (LSB)
class TYPE_ALU_JMP<bits<4> op, bits<1> srctype,
                   dag outs, dag ins, string asmstr, list<dag> pattern>
  : InstStarcraftEUD<outs, ins, asmstr, pattern> {

  let Inst{63-60} = op;
  let Inst{59} = srctype;
}

//For load and store instructions the 8-bit 'code' field is divided as:
//
//  +--------+--------+-------------------+
//  | 3 bits | 2 bits |   3 bits          |
//  |  mode  |  size  | instruction class |
//  +--------+--------+-------------------+
//  (MSB)                             (LSB)
class TYPE_LD_ST<bits<3> mode, bits<2> size,
                 dag outs, dag ins, string asmstr, list<dag> pattern>
  : InstStarcraftEUD<outs, ins, asmstr, pattern> {

  let Inst{63-61} = mode;
  let Inst{60-59} = size;
}

// jump instructions
class JMP_RR<StarcraftEUDJumpOp Opc, string OpcodeStr, PatLeaf Cond>
    : TYPE_ALU_JMP<Opc.Value, StarcraftEUD_X.Value,
                   (outs),
                   (ins GPR:$dst, GPR:$src, brtarget:$BrDst),
                   "if $dst "#OpcodeStr#" $src goto $BrDst",
                   [(StarcraftEUDbrcc i64:$dst, i64:$src, Cond, bb:$BrDst)]> {
  bits<4> dst;
  bits<4> src;
  bits<16> BrDst;

  let Inst{55-52} = src;
  let Inst{51-48} = dst;
  let Inst{47-32} = BrDst;
  let StarcraftEUDClass = StarcraftEUD_JMP;
}

class JMP_RI<StarcraftEUDJumpOp Opc, string OpcodeStr, PatLeaf Cond>
    : TYPE_ALU_JMP<Opc.Value, StarcraftEUD_K.Value,
                   (outs),
                   (ins GPR:$dst, i64imm:$imm, brtarget:$BrDst),
                   "if $dst "#OpcodeStr#" $imm goto $BrDst",
                   [(StarcraftEUDbrcc i64:$dst, i64immSExt32:$imm, Cond, bb:$BrDst)]> {
  bits<4> dst;
  bits<16> BrDst;
  bits<32> imm;

  let Inst{51-48} = dst;
  let Inst{47-32} = BrDst;
  let Inst{31-0} = imm;
  let StarcraftEUDClass = StarcraftEUD_JMP;
}

multiclass J<StarcraftEUDJumpOp Opc, string OpcodeStr, PatLeaf Cond> {
  def _rr : JMP_RR<Opc, OpcodeStr, Cond>;
  def _ri : JMP_RI<Opc, OpcodeStr, Cond>;
}

let isBranch = 1, isTerminator = 1, hasDelaySlot=0 in {
// cmp+goto instructions
defm JEQ  : J<StarcraftEUD_JEQ, "==",  StarcraftEUD_CC_EQ>;
defm JUGT : J<StarcraftEUD_JGT, ">", StarcraftEUD_CC_GTU>;
defm JUGE : J<StarcraftEUD_JGE, ">=", StarcraftEUD_CC_GEU>;
defm JNE  : J<StarcraftEUD_JNE, "!=",  StarcraftEUD_CC_NE>;
defm JSGT : J<StarcraftEUD_JSGT, "s>", StarcraftEUD_CC_GT>;
defm JSGE : J<StarcraftEUD_JSGE, "s>=", StarcraftEUD_CC_GE>;
defm JULT : J<StarcraftEUD_JLT, "<", StarcraftEUD_CC_LTU>;
defm JULE : J<StarcraftEUD_JLE, "<=", StarcraftEUD_CC_LEU>;
defm JSLT : J<StarcraftEUD_JSLT, "s<", StarcraftEUD_CC_LT>;
defm JSLE : J<StarcraftEUD_JSLE, "s<=", StarcraftEUD_CC_LE>;
}

// ALU instructions
class ALU_RI<StarcraftEUDOpClass Class, StarcraftEUDArithOp Opc,
             dag outs, dag ins, string asmstr, list<dag> pattern>
    : TYPE_ALU_JMP<Opc.Value, StarcraftEUD_K.Value, outs, ins, asmstr, pattern> {
  bits<4> dst;
  bits<32> imm;

  let Inst{51-48} = dst;
  let Inst{31-0} = imm;
  let StarcraftEUDClass = Class;
}

class ALU_RR<StarcraftEUDOpClass Class, StarcraftEUDArithOp Opc,
             dag outs, dag ins, string asmstr, list<dag> pattern>
    : TYPE_ALU_JMP<Opc.Value, StarcraftEUD_X.Value, outs, ins, asmstr, pattern> {
  bits<4> dst;
  bits<4> src;

  let Inst{55-52} = src;
  let Inst{51-48} = dst;
  let StarcraftEUDClass = Class;
}

multiclass ALU<StarcraftEUDArithOp Opc, string OpcodeStr, SDNode OpNode> {
  def _rr : ALU_RR<StarcraftEUD_ALU64, Opc,
                   (outs GPR:$dst),
                   (ins GPR:$src2, GPR:$src),
                   "$dst "#OpcodeStr#" $src",
                   [(set GPR:$dst, (OpNode i64:$src2, i64:$src))]>;
  def _ri : ALU_RI<StarcraftEUD_ALU64, Opc,
                   (outs GPR:$dst),
                   (ins GPR:$src2, i64imm:$imm),
                   "$dst "#OpcodeStr#" $imm",
                   [(set GPR:$dst, (OpNode GPR:$src2, i64immSExt32:$imm))]>;
  def _rr_32 : ALU_RR<StarcraftEUD_ALU, Opc,
                   (outs GPR32:$dst),
                   (ins GPR32:$src2, GPR32:$src),
                   "$dst "#OpcodeStr#" $src",
                   [(set GPR32:$dst, (OpNode i32:$src2, i32:$src))]>;
  def _ri_32 : ALU_RI<StarcraftEUD_ALU, Opc,
                   (outs GPR32:$dst),
                   (ins GPR32:$src2, i32imm:$imm),
                   "$dst "#OpcodeStr#" $imm",
                   [(set GPR32:$dst, (OpNode GPR32:$src2, i32:$imm))]>;
}

let Constraints = "$dst = $src2" in {
let isAsCheapAsAMove = 1 in {
  defm ADD : ALU<StarcraftEUD_ADD, "+=", add>;
  defm SUB : ALU<StarcraftEUD_SUB, "-=", sub>;
  defm OR  : ALU<StarcraftEUD_OR, "|=", or>;
  defm AND : ALU<StarcraftEUD_AND, "&=", and>;
  defm SLL : ALU<StarcraftEUD_LSH, "<<=", shl>;
  defm SRL : ALU<StarcraftEUD_RSH, ">>=", srl>;
  defm XOR : ALU<StarcraftEUD_XOR, "^=", xor>;
  defm SRA : ALU<StarcraftEUD_ARSH, "s>>=", sra>;
}
  defm MUL : ALU<StarcraftEUD_MUL, "*=", mul>;
  defm DIV : ALU<StarcraftEUD_DIV, "/=", udiv>;
}

class NEG_RR<StarcraftEUDOpClass Class, StarcraftEUDArithOp Opc,
             dag outs, dag ins, string asmstr, list<dag> pattern>
    : TYPE_ALU_JMP<Opc.Value, 0, outs, ins, asmstr, pattern> {
  bits<4> dst;

  let Inst{51-48} = dst;
  let StarcraftEUDClass = Class;
}

let Constraints = "$dst = $src", isAsCheapAsAMove = 1 in {
  def NEG_64: NEG_RR<StarcraftEUD_ALU64, StarcraftEUD_NEG, (outs GPR:$dst), (ins GPR:$src),
                     "$dst = -$src",
                     [(set GPR:$dst, (ineg i64:$src))]>;
  def NEG_32: NEG_RR<StarcraftEUD_ALU, StarcraftEUD_NEG, (outs GPR32:$dst), (ins GPR32:$src),
                     "$dst = -$src",
                     [(set GPR32:$dst, (ineg i32:$src))]>;
}

class LD_IMM64<bits<4> Pseudo, string OpcodeStr>
    : TYPE_LD_ST<StarcraftEUD_IMM.Value, StarcraftEUD_DW.Value,
                 (outs GPR:$dst),
                 (ins u64imm:$imm),
                 "$dst "#OpcodeStr#" ${imm} ll",
                 [(set GPR:$dst, (i64 imm:$imm))]> {

  bits<4> dst;
  bits<64> imm;

  let Inst{51-48} = dst;
  let Inst{55-52} = Pseudo;
  let Inst{47-32} = 0;
  let Inst{31-0} = imm{31-0};
  let StarcraftEUDClass = StarcraftEUD_LD;
}

let isReMaterializable = 1, isAsCheapAsAMove = 1 in {
def LD_imm64 : LD_IMM64<0, "=">;
def MOV_rr : ALU_RR<StarcraftEUD_ALU64, StarcraftEUD_MOV,
                    (outs GPR:$dst),
                    (ins GPR:$src),
                    "$dst = $src",
                    []>;
def MOV_ri : ALU_RI<StarcraftEUD_ALU64, StarcraftEUD_MOV,
                    (outs GPR:$dst),
                    (ins i64imm:$imm),
                    "$dst = $imm",
                    [(set GPR:$dst, (i64 i64immSExt32:$imm))]>;
def MOV_rr_32 : ALU_RR<StarcraftEUD_ALU, StarcraftEUD_MOV,
                    (outs GPR32:$dst),
                    (ins GPR32:$src),
                    "$dst = $src",
                    []>;
def MOV_ri_32 : ALU_RI<StarcraftEUD_ALU, StarcraftEUD_MOV,
                    (outs GPR32:$dst),
                    (ins i32imm:$imm),
                    "$dst = $imm",
                    [(set GPR32:$dst, (i32 i32:$imm))]>;
}

def FI_ri
    : TYPE_LD_ST<StarcraftEUD_IMM.Value, StarcraftEUD_DW.Value,
                 (outs GPR:$dst),
                 (ins MEMri:$addr),
                 "lea\t$dst, $addr",
                 [(set i64:$dst, FIri:$addr)]> {
  // This is a tentative instruction, and will be replaced
  // with MOV_rr and ADD_ri in PEI phase
  let Inst{51-48} = 0;
  let Inst{55-52} = 2;
  let Inst{47-32} = 0;
  let Inst{31-0} = 0;
  let StarcraftEUDClass = StarcraftEUD_LD;
}

def LD_pseudo
    : TYPE_LD_ST<StarcraftEUD_IMM.Value, StarcraftEUD_DW.Value,
                 (outs GPR:$dst),
                 (ins i64imm:$pseudo, u64imm:$imm),
                 "ld_pseudo\t$dst, $pseudo, $imm",
                 [(set GPR:$dst, (int_bpf_pseudo imm:$pseudo, imm:$imm))]> {

  bits<4> dst;
  bits<64> imm;
  bits<4> pseudo;

  let Inst{51-48} = dst;
  let Inst{55-52} = pseudo;
  let Inst{47-32} = 0;
  let Inst{31-0} = imm{31-0};
  let StarcraftEUDClass = StarcraftEUD_LD;
}

// STORE instructions
class STORE<StarcraftEUDWidthModifer SizeOp, string OpcodeStr, list<dag> Pattern>
    : TYPE_LD_ST<StarcraftEUD_MEM.Value, SizeOp.Value,
                 (outs),
                 (ins GPR:$src, MEMri:$addr),
                 "*("#OpcodeStr#" *)($addr) = $src",
                 Pattern> {
  bits<4> src;
  bits<20> addr;

  let Inst{51-48} = addr{19-16}; // base reg
  let Inst{55-52} = src;
  let Inst{47-32} = addr{15-0}; // offset
  let StarcraftEUDClass = StarcraftEUD_STX;
}

class STOREi64<StarcraftEUDWidthModifer Opc, string OpcodeStr, PatFrag OpNode>
    : STORE<Opc, OpcodeStr, [(OpNode i64:$src, ADDRri:$addr)]>;

def STW : STOREi64<StarcraftEUD_W, "u32", truncstorei32>;
def STH : STOREi64<StarcraftEUD_H, "u16", truncstorei16>;
def STB : STOREi64<StarcraftEUD_B, "u8", truncstorei8>;
def STD : STOREi64<StarcraftEUD_DW, "u64", store>;

// LOAD instructions
class LOAD<StarcraftEUDWidthModifer SizeOp, string OpcodeStr, list<dag> Pattern>
    : TYPE_LD_ST<StarcraftEUD_MEM.Value, SizeOp.Value,
                 (outs GPR:$dst),
                 (ins MEMri:$addr),
                 "$dst = *("#OpcodeStr#" *)($addr)",
                 Pattern> {
  bits<4> dst;
  bits<20> addr;

  let Inst{51-48} = dst;
  let Inst{55-52} = addr{19-16};
  let Inst{47-32} = addr{15-0};
  let StarcraftEUDClass = StarcraftEUD_LDX;
}

class LOADi64<StarcraftEUDWidthModifer SizeOp, string OpcodeStr, PatFrag OpNode>
    : LOAD<SizeOp, OpcodeStr, [(set i64:$dst, (OpNode ADDRri:$addr))]>;

def LDW : LOADi64<StarcraftEUD_W, "u32", zextloadi32>;
def LDH : LOADi64<StarcraftEUD_H, "u16", zextloadi16>;
def LDB : LOADi64<StarcraftEUD_B, "u8", zextloadi8>;
def LDD : LOADi64<StarcraftEUD_DW, "u64", load>;

class BRANCH<StarcraftEUDJumpOp Opc, string OpcodeStr, list<dag> Pattern>
    : TYPE_ALU_JMP<Opc.Value, StarcraftEUD_K.Value,
                   (outs),
                   (ins brtarget:$BrDst),
                   !strconcat(OpcodeStr, " $BrDst"),
                   Pattern> {
  bits<16> BrDst;

  let Inst{47-32} = BrDst;
  let StarcraftEUDClass = StarcraftEUD_JMP;
}

class CALL<string OpcodeStr>
    : TYPE_ALU_JMP<StarcraftEUD_CALL.Value, StarcraftEUD_K.Value,
                   (outs),
                   (ins calltarget:$BrDst),
                   !strconcat(OpcodeStr, " $BrDst"),
                   []> {
  bits<32> BrDst;

  let Inst{31-0} = BrDst;
  let StarcraftEUDClass = StarcraftEUD_JMP;
}

class CALLX<string OpcodeStr>
    : TYPE_ALU_JMP<StarcraftEUD_CALL.Value, StarcraftEUD_X.Value,
                   (outs),
                   (ins calltarget:$BrDst),
                   !strconcat(OpcodeStr, " $BrDst"),
                   []> {
  bits<32> BrDst;

  let Inst{31-0} = BrDst;
  let StarcraftEUDClass = StarcraftEUD_JMP;
}

// Jump always
let isBranch = 1, isTerminator = 1, hasDelaySlot=0, isBarrier = 1 in {
  def JMP : BRANCH<StarcraftEUD_JA, "goto", [(br bb:$BrDst)]>;
}

// Jump and link
let isCall=1, hasDelaySlot=0, Uses = [R11],
    // Potentially clobbered registers
    Defs = [R0, R1, R2, R3, R4, R5] in {
  def JAL  : CALL<"call">;
  def JALX  : CALLX<"callx">;
}

class NOP_I<string OpcodeStr>
    : TYPE_ALU_JMP<StarcraftEUD_MOV.Value, StarcraftEUD_X.Value,
                   (outs),
                   (ins i32imm:$imm),
                   !strconcat(OpcodeStr, "\t$imm"),
                   []> {
  // mov r0, r0 == nop
  let Inst{55-52} = 0;
  let Inst{51-48} = 0;
  let StarcraftEUDClass = StarcraftEUD_ALU64;
}

let hasSideEffects = 0 in
  def NOP : NOP_I<"nop">;

class RET<string OpcodeStr>
    : TYPE_ALU_JMP<StarcraftEUD_EXIT.Value, StarcraftEUD_K.Value,
                   (outs),
                   (ins),
                   !strconcat(OpcodeStr, ""),
                   [(StarcraftEUDretflag)]> {
  let Inst{31-0} = 0;
  let StarcraftEUDClass = StarcraftEUD_JMP;
}

let isReturn = 1, isTerminator = 1, hasDelaySlot=0, isBarrier = 1,
    isNotDuplicable = 1 in {
  def RET : RET<"exit">;
}

// ADJCALLSTACKDOWN/UP pseudo insns
let Defs = [R11], Uses = [R11], isCodeGenOnly = 1 in {
def ADJCALLSTACKDOWN : Pseudo<(outs), (ins i64imm:$amt1, i64imm:$amt2),
                              "#ADJCALLSTACKDOWN $amt1 $amt2",
                              [(StarcraftEUDcallseq_start timm:$amt1, timm:$amt2)]>;
def ADJCALLSTACKUP   : Pseudo<(outs), (ins i64imm:$amt1, i64imm:$amt2),
                              "#ADJCALLSTACKUP $amt1 $amt2",
                              [(StarcraftEUDcallseq_end timm:$amt1, timm:$amt2)]>;
}

let usesCustomInserter = 1, isCodeGenOnly = 1 in {
  def Select : Pseudo<(outs GPR:$dst),
                      (ins GPR:$lhs, GPR:$rhs, i64imm:$imm, GPR:$src, GPR:$src2),
                      "# Select PSEUDO $dst = $lhs $imm $rhs ? $src : $src2",
                      [(set i64:$dst,
                       (StarcraftEUDselectcc i64:$lhs, i64:$rhs, (i64 imm:$imm), i64:$src, i64:$src2))]>;
  def Select_Ri : Pseudo<(outs GPR:$dst),
                      (ins GPR:$lhs, i64imm:$rhs, i64imm:$imm, GPR:$src, GPR:$src2),
                      "# Select PSEUDO $dst = $lhs $imm $rhs ? $src : $src2",
                      [(set i64:$dst,
                       (StarcraftEUDselectcc i64:$lhs, (i64immSExt32:$rhs), (i64 imm:$imm), i64:$src, i64:$src2))]>;
}

// load 64-bit global addr into register
def : Pat<(StarcraftEUDWrapper tglobaladdr:$in), (LD_imm64 tglobaladdr:$in)>;

// 0xffffFFFF doesn't fit into simm32, optimize common case
def : Pat<(i64 (and (i64 GPR:$src), 0xffffFFFF)),
          (SRL_ri (SLL_ri (i64 GPR:$src), 32), 32)>;

// Calls
def : Pat<(StarcraftEUDcall tglobaladdr:$dst), (JAL tglobaladdr:$dst)>;
def : Pat<(StarcraftEUDcall texternalsym:$dst), (JAL texternalsym:$dst)>;
def : Pat<(StarcraftEUDcall imm:$dst), (JAL imm:$dst)>;
def : Pat<(StarcraftEUDcall GPR:$dst), (JALX GPR:$dst)>;

// Loads
def : Pat<(extloadi8  ADDRri:$src), (i64 (LDB ADDRri:$src))>;
def : Pat<(extloadi16 ADDRri:$src), (i64 (LDH ADDRri:$src))>;
def : Pat<(extloadi32 ADDRri:$src), (i64 (LDW ADDRri:$src))>;

// Atomics
class XADD<StarcraftEUDWidthModifer SizeOp, string OpcodeStr, PatFrag OpNode>
    : TYPE_LD_ST<StarcraftEUD_XADD.Value, SizeOp.Value,
                 (outs GPR:$dst),
                 (ins MEMri:$addr, GPR:$val),
                 "lock *("#OpcodeStr#" *)($addr) += $val",
                 [(set GPR:$dst, (OpNode ADDRri:$addr, GPR:$val))]> {
  bits<4> dst;
  bits<20> addr;

  let Inst{51-48} = addr{19-16}; // base reg
  let Inst{55-52} = dst;
  let Inst{47-32} = addr{15-0}; // offset
  let StarcraftEUDClass = StarcraftEUD_STX;
}

let Constraints = "$dst = $val" in {
def XADD32 : XADD<StarcraftEUD_W, "u32", atomic_load_add_32>;
def XADD64 : XADD<StarcraftEUD_DW, "u64", atomic_load_add_64>;
// undefined def XADD16 : XADD<1, "xadd16", atomic_load_add_16>;
// undefined def XADD8  : XADD<2, "xadd8", atomic_load_add_8>;
}

// bswap16, bswap32, bswap64
class BSWAP<bits<32> SizeOp, string OpcodeStr, StarcraftEUDSrcType SrcType, list<dag> Pattern>
    : TYPE_ALU_JMP<StarcraftEUD_END.Value, SrcType.Value,
                   (outs GPR:$dst),
                   (ins GPR:$src),
                   "$dst = "#OpcodeStr#" $src",
                   Pattern> {
  bits<4> dst;

  let Inst{51-48} = dst;
  let Inst{31-0} = SizeOp;
  let StarcraftEUDClass = StarcraftEUD_ALU;
}


let Constraints = "$dst = $src" in {
    let Predicates = [StarcraftEUDIsLittleEndian] in {
        def BE16 : BSWAP<16, "be16", StarcraftEUD_TO_BE, [(set GPR:$dst, (srl (bswap GPR:$src), (i64 48)))]>;
        def BE32 : BSWAP<32, "be32", StarcraftEUD_TO_BE, [(set GPR:$dst, (srl (bswap GPR:$src), (i64 32)))]>;
        def BE64 : BSWAP<64, "be64", StarcraftEUD_TO_BE, [(set GPR:$dst, (bswap GPR:$src))]>;
    }
    let Predicates = [StarcraftEUDIsBigEndian] in {
        def LE16 : BSWAP<16, "le16", StarcraftEUD_TO_LE, [(set GPR:$dst, (srl (bswap GPR:$src), (i64 48)))]>;
        def LE32 : BSWAP<32, "le32", StarcraftEUD_TO_LE, [(set GPR:$dst, (srl (bswap GPR:$src), (i64 32)))]>;
        def LE64 : BSWAP<64, "le64", StarcraftEUD_TO_LE, [(set GPR:$dst, (bswap GPR:$src))]>;
    }
}

let Defs = [R0, R1, R2, R3, R4, R5], Uses = [R6], hasSideEffects = 1,
    hasExtraDefRegAllocReq = 1, hasExtraSrcRegAllocReq = 1, mayLoad = 1 in {
class LOAD_ABS<StarcraftEUDWidthModifer SizeOp, string OpcodeStr, Intrinsic OpNode>
    : TYPE_LD_ST<StarcraftEUD_ABS.Value, SizeOp.Value,
                 (outs),
                 (ins GPR:$skb, i64imm:$imm),
                 "r0 = *("#OpcodeStr#" *)skb[$imm]",
                 [(set R0, (OpNode GPR:$skb, i64immSExt32:$imm))]> {
  bits<32> imm;

  let Inst{31-0} = imm;
  let StarcraftEUDClass = StarcraftEUD_LD;
}

class LOAD_IND<StarcraftEUDWidthModifer SizeOp, string OpcodeStr, Intrinsic OpNode>
    : TYPE_LD_ST<StarcraftEUD_IND.Value, SizeOp.Value,
                 (outs),
                 (ins GPR:$skb, GPR:$val),
                 "r0 = *("#OpcodeStr#" *)skb[$val]",
                 [(set R0, (OpNode GPR:$skb, GPR:$val))]> {
  bits<4> val;

  let Inst{55-52} = val;
  let StarcraftEUDClass = StarcraftEUD_LD;
}
}

def LD_ABS_B : LOAD_ABS<StarcraftEUD_B, "u8", int_bpf_load_byte>;
def LD_ABS_H : LOAD_ABS<StarcraftEUD_H, "u16", int_bpf_load_half>;
def LD_ABS_W : LOAD_ABS<StarcraftEUD_W, "u32", int_bpf_load_word>;

def LD_IND_B : LOAD_IND<StarcraftEUD_B, "u8", int_bpf_load_byte>;
def LD_IND_H : LOAD_IND<StarcraftEUD_H, "u16", int_bpf_load_half>;
def LD_IND_W : LOAD_IND<StarcraftEUD_W, "u32", int_bpf_load_word>;
