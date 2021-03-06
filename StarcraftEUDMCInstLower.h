//===-- StarcraftEUDMCInstLower.h - Lower MachineInstr to MCInst ---------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_STARCRAFTEUD_STARCRAFTEUDMCINSTLOWER_H
#define LLVM_LIB_TARGET_STARCRAFTEUD_STARCRAFTEUDMCINSTLOWER_H

#include "llvm/Support/Compiler.h"

namespace llvm {
class AsmPrinter;
class MCContext;
class MCInst;
class MCOperand;
class MCSymbol;
class MachineInstr;
class MachineModuleInfoMachO;
class MachineOperand;
class Mangler;

// StarcraftEUDMCInstLower - This class is used to lower an MachineInstr into an MCInst.
class LLVM_LIBRARY_VISIBILITY StarcraftEUDMCInstLower {
  MCContext &Ctx;

  AsmPrinter &Printer;

public:
  StarcraftEUDMCInstLower(MCContext &ctx, AsmPrinter &printer)
      : Ctx(ctx), Printer(printer) {}
  void Lower(const MachineInstr *MI, MCInst &OutMI) const;

  MCOperand LowerSymbolOperand(const MachineOperand &MO, MCSymbol *Sym) const;

  MCSymbol *GetGlobalAddressSymbol(const MachineOperand &MO) const;
  MCSymbol *GetExternalSymbolSymbol(const MachineOperand &MO) const;
};
}

#endif
