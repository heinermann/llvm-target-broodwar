//===-- StarcraftEUDCallingConv.td - Calling Conventions StarcraftEUD --------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This describes the calling conventions for the StarcraftEUD architecture.
//
//===----------------------------------------------------------------------===//

// StarcraftEUD 64-bit C return-value convention.
def RetCC_StarcraftEUD64 : CallingConv<[CCIfType<[i64], CCAssignToReg<[R0]>>]>;

// StarcraftEUD 64-bit C Calling convention.
def CC_StarcraftEUD64 : CallingConv<[
  // Promote i8/i16/i32 args to i64
  CCIfType<[ i8, i16, i32 ], CCPromoteToType<i64>>,

  // All arguments get passed in integer registers if there is space.
  CCIfType<[i64], CCAssignToReg<[ R1, R2, R3, R4, R5 ]>>,

  // Could be assigned to the stack in 8-byte aligned units, but unsupported
  CCAssignToStack<8, 8>
]>;

def CSR : CalleeSavedRegs<(add R6, R7, R8, R9, R10)>;
