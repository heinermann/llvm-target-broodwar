//===-- StarcraftEUDMCExpr.cpp - StarcraftEUD specific MC expression classes ------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "StarcraftEUD.h"

#include "StarcraftEUDMCExpr.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCObjectStreamer.h"
#include "llvm/MC/MCSymbolELF.h"

using namespace llvm;

#define DEBUG_TYPE "broodwarmcexpr"

const StarcraftEUDMCExpr *StarcraftEUDMCExpr::create(StarcraftEUDMCExpr::StarcraftEUDExprKind Kind,
                                       const MCExpr *Expr, MCContext &Ctx) {
  return new (Ctx) StarcraftEUDMCExpr(Kind, Expr);
}

const StarcraftEUDMCExpr *StarcraftEUDMCExpr::create(const MCSymbol *Symbol,
                                       StarcraftEUDMCExpr::StarcraftEUDExprKind Kind,
                                       MCContext &Ctx) {
  const MCSymbolRefExpr *MCSym =
      MCSymbolRefExpr::create(Symbol, MCSymbolRefExpr::VK_None, Ctx);
  return new (Ctx) StarcraftEUDMCExpr(Kind, MCSym);
}

void StarcraftEUDMCExpr::printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const {

  switch (Kind) {
  case CEK_None:
  case CEK_Special:
    llvm_unreachable("CEK_None and CEK_Special are invalid");
    break;
  case CEK_ABS_HI:
    OS << "%hiadj";
    break;
  case CEK_ABS_LO:
    OS << "%lo";
    break;
  }

  OS << '(';
  Expr->print(OS, MAI, true);
  OS << ')';
}

bool StarcraftEUDMCExpr::evaluateAsRelocatableImpl(MCValue &Res,
                                            const MCAsmLayout *Layout,
                                            const MCFixup *Fixup) const {
  return getSubExpr()->evaluateAsRelocatable(Res, Layout, Fixup);
}

void StarcraftEUDMCExpr::visitUsedExpr(MCStreamer &Streamer) const {
  Streamer.visitUsedExpr(*getSubExpr());
}

void StarcraftEUDMCExpr::fixELFSymbolsInTLSFixups(MCAssembler &Asm) const {
  switch (getKind()) {
  case CEK_None:
  case CEK_Special:
    llvm_unreachable("CEK_None and CEK_Special are invalid");
    break;
  case CEK_ABS_HI:
  case CEK_ABS_LO:
    break;
  }
}
