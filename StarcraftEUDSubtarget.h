//===-- StarcraftEUDSubtarget.h - Define Subtarget for the StarcraftEUD -----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the StarcraftEUD specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_STARCRAFTEUD_STARCRAFTEUDSUBTARGET_H
#define LLVM_LIB_TARGET_STARCRAFTEUD_STARCRAFTEUDSUBTARGET_H

#include "StarcraftEUDFrameLowering.h"
#include "StarcraftEUDISelLowering.h"
#include "StarcraftEUDInstrInfo.h"
#include "llvm/CodeGen/SelectionDAGTargetInfo.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/Target/TargetMachine.h"

#define GET_SUBTARGETINFO_HEADER
#include "StarcraftEUDGenSubtargetInfo.inc"

namespace llvm {
class StringRef;

class StarcraftEUDSubtarget : public StarcraftEUDGenSubtargetInfo {
  virtual void anchor();
  StarcraftEUDInstrInfo InstrInfo;
  StarcraftEUDFrameLowering FrameLowering;
  StarcraftEUDTargetLowering TLInfo;
  SelectionDAGTargetInfo TSInfo;

private:
  void initializeEnvironment();
  void initSubtargetFeatures(StringRef CPU, StringRef FS);
  bool probeJmpExt();

protected:
  // unused
  bool isDummyMode;

  // whether the cpu supports jmp ext
  bool HasJmpExt;

public:
  // This constructor initializes the data members to match that
  // of the specified triple.
  StarcraftEUDSubtarget(const Triple &TT, const std::string &CPU, const std::string &FS,
               const TargetMachine &TM);

  StarcraftEUDSubtarget &initializeSubtargetDependencies(StringRef CPU, StringRef FS);

  // ParseSubtargetFeatures - Parses features string setting specified
  // subtarget options.  Definition of function is auto generated by tblgen.
  void ParseSubtargetFeatures(StringRef CPU, StringRef FS);
  bool getHasJmpExt() const { return HasJmpExt; }

  const StarcraftEUDInstrInfo *getInstrInfo() const override { return &InstrInfo; }
  const StarcraftEUDFrameLowering *getFrameLowering() const override {
    return &FrameLowering;
  }
  const StarcraftEUDTargetLowering *getTargetLowering() const override {
    return &TLInfo;
  }
  const SelectionDAGTargetInfo *getSelectionDAGInfo() const override {
    return &TSInfo;
  }
  const TargetRegisterInfo *getRegisterInfo() const override {
    return &InstrInfo.getRegisterInfo();
  }
};
} // End llvm namespace

#endif
