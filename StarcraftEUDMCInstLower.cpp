//===-- StarcraftEUDMCInstLower.cpp - Convert StarcraftEUD MachineInstr to MCInst -------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains code to lower StarcraftEUD MachineInstrs to their corresponding
// MCInst records.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/StarcraftEUDMCExpr.h"
#include "StarcraftEUD.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineOperand.h"

using namespace llvm;

static MCOperand LowerSymbolOperand(const MachineOperand &MO, AsmPrinter &AP) {
  MCSymbolRefExpr::VariantKind Kind = MCSymbolRefExpr::VK_None;
  StarcraftEUDMCExpr::StarcraftEUDExprKind TargetKind = StarcraftEUDMCExpr::CEK_None;
  const MCSymbol *Symbol;

  switch (MO.getType()) {
  case MachineOperand::MO_GlobalAddress:
    Symbol = AP.getSymbol(MO.getGlobal());
    break;

  case MachineOperand::MO_MachineBasicBlock:
    Symbol = MO.getMBB()->getSymbol();
    break;

  case MachineOperand::MO_BlockAddress:
    Symbol = AP.GetBlockAddressSymbol(MO.getBlockAddress());
    break;

  case MachineOperand::MO_ExternalSymbol:
    Symbol = AP.GetExternalSymbolSymbol(MO.getSymbolName());
    break;

  case MachineOperand::MO_JumpTableIndex:
    Symbol = AP.GetJTISymbol(MO.getIndex());
    break;

  case MachineOperand::MO_ConstantPoolIndex:
    Symbol = AP.GetCPISymbol(MO.getIndex());
    break;

  default:
    llvm_unreachable("<unknown operand type>");
  }

  const MCExpr *Expr = MCSymbolRefExpr::create(Symbol, Kind, AP.OutContext);

  if (TargetKind != StarcraftEUDMCExpr::CEK_None)
    Expr = StarcraftEUDMCExpr::create(TargetKind, Expr, AP.OutContext);

  return MCOperand::createExpr(Expr);
}

static MCOperand LowerOperand(const MachineOperand &MO, AsmPrinter &AP) {

  switch (MO.getType()) {
  default:
    llvm_unreachable("unknown operand type");
  case MachineOperand::MO_Register:
    // Ignore all implicit register operands.
    if (MO.isImplicit())
      break;
    return MCOperand::createReg(MO.getReg());
  case MachineOperand::MO_Immediate:
    return MCOperand::createImm(MO.getImm());
  case MachineOperand::MO_MachineBasicBlock:
  case MachineOperand::MO_ExternalSymbol:
  case MachineOperand::MO_JumpTableIndex:
  case MachineOperand::MO_BlockAddress:
  case MachineOperand::MO_GlobalAddress:
  case MachineOperand::MO_ConstantPoolIndex:
    return LowerSymbolOperand(MO, AP);
  case MachineOperand::MO_RegisterMask:
    break;
  }

  return MCOperand();
}

void llvm::LowerStarcraftEUDMachineInstToMCInst(const MachineInstr *MI, MCInst &OutMI,
                                         AsmPrinter &AP) {

  OutMI.setOpcode(MI->getOpcode());

  for (unsigned i = 0, e = MI->getNumOperands(); i != e; ++i) {
    const MachineOperand &MO = MI->getOperand(i);
    MCOperand MCOp = LowerOperand(MO, AP);

    if (MCOp.isValid())
      OutMI.addOperand(MCOp);
  }
}
