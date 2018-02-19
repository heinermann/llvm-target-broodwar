//===-- StarcraftEUDELFObjectWriter.cpp - StarcraftEUD ELF Writer ---------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/StarcraftEUDMCTargetDesc.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/Support/ErrorHandling.h"
#include <cstdint>

using namespace llvm;

namespace {

class StarcraftEUDELFObjectWriter : public MCELFObjectTargetWriter {
public:
  StarcraftEUDELFObjectWriter(uint8_t OSABI);
  ~StarcraftEUDELFObjectWriter() override = default;

protected:
  unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsPCRel) const override;
};

} // end anonymous namespace

StarcraftEUDELFObjectWriter::StarcraftEUDELFObjectWriter(uint8_t OSABI)
    : MCELFObjectTargetWriter(/*Is64Bit*/ true, OSABI, ELF::EM_M32,
                              /*HasRelocationAddend*/ false) {}

unsigned StarcraftEUDELFObjectWriter::getRelocType(MCContext &Ctx, const MCValue &Target,
                                          const MCFixup &Fixup,
                                          bool IsPCRel) const {
  return ELF::R_BPF_NONE;
  // determine the type of the relocation
  /*
  switch ((unsigned)Fixup.getKind()) {
  default:
    llvm_unreachable("invalid fixup kind!");
  case FK_SecRel_8:
    return ELF::R_BPF_64_64;
  case FK_PCRel_4:
  case FK_SecRel_4:
    return ELF::R_BPF_64_32;
  case FK_Data_8:
    return ELF::R_BPF_64_64;
  case FK_Data_4:
    return ELF::R_BPF_64_32;
  }
  */
}

std::unique_ptr<MCObjectWriter>
llvm::createStarcraftEUDELFObjectWriter(raw_pwrite_stream &OS, uint8_t OSABI,
                               bool IsLittleEndian) {
  return createELFObjectWriter(llvm::make_unique<StarcraftEUDELFObjectWriter>(OSABI), OS,
                               IsLittleEndian);
}
