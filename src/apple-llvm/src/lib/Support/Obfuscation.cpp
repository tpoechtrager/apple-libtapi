//===-- Obfuscation.cpp - String obfuscation helper -------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides helper classes to perform string obfuscation
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Obfuscation.h"

using namespace llvm;

// Obfuscate value name.
StringRef Obfuscator::obfuscate(StringRef Src, bool reverse) {
  assert(Src.size() && "passed in empty string");

  // See if we've already encountered this value
  if (reverse) {
    if (ForwardMap.count(Src))
      return ForwardMap.lookup(Src);

    auto Res = obfuscateImpl(Src, reverse);
    ForwardMap[Src] = Res;
    return Res;
  } else {
    if (IrreveribleForwardMap.count(Src))
      return IrreveribleForwardMap.lookup(Src);
    auto Res = obfuscateImpl(Src, reverse);
    IrreveribleForwardMap[Src] = Res;
    return Res;
  }
}

// look up symbol in the forward map
ErrorOr<StringRef> Obfuscator::lookupForwardMap(StringRef Symbol) const {
  if (ForwardMap.count(Symbol))
    return ForwardMap.lookup(Symbol);

  return std::error_code(1, std::generic_category());
}

// Rot13Obfuscator implementation.
void Rot13Obfuscator::anchor() {}

StringRef Rot13Obfuscator::obfuscateImpl(StringRef S, bool reverse = false) {
  char *NewStr = allocateString(S.size());
  assert(NewStr[S.size()] == '\0' && "not null terminated");
  for (unsigned i = 0; i < S.size(); ++i)
    NewStr[i] = rot13(S[i]);
  if (reverse)
    ReverseMap.insert({S, NewStr});
  return StringRef{NewStr};
}

ErrorOr<StringRef> Rot13Obfuscator::lookupImpl(StringRef ObfStr) const {
  if (ReverseMap.count(ObfStr))
    return ReverseMap.lookup(ObfStr);

  return std::error_code(1, std::generic_category());
}

void Rot13Obfuscator::writeReverseMapImpl(raw_ostream &OS) const {
  for (auto &STE : ReverseMap)
    OS << STE.getKey() << ":" << STE.getValue() << "\n";
}

void Rot13Obfuscator::readReverseMapImpl(MemoryBuffer* buf) {
  llvm::StringRef Data(buf->getBufferStart(),
                       buf->getBufferSize());
  llvm::StringRef LHS;
  while (!Data.empty()) {
    std::tie(LHS, Data) = Data.split('\n');
    StringRef key, value;
    std::tie(key, value) = LHS.split(':');
    ReverseMap.insert({copyString(value), copyString(key)});
  }
}

// IncrementObfuscator implementations.
void IncrementObfuscator::anchor() {}

// Implementation for IncrementObfuscator.
StringRef IncrementObfuscator::obfuscateImpl(StringRef S, bool reverse) {
  SmallString<128> NextVal;
  raw_svector_ostream OS(NextVal);

  if (reverse) {
    // If we begin with \01l_OBJC or \01L_OBJC, keep that prefix
    if (S.startswith("\01L") || S.startswith("\01l"))
      OS << S.substr(0, StringRef("\01l").size());

    OS << Pre << Num++ << Suf;
  } else {
    OS << IrPre << IrNum++ << Suf;
  }

  if (reverse) {
    ReverseMap.push_back(copyString(S));
    assert(Num == ReverseMap.size() && "ReverseMap has wrong size");
  }

  return copyString(OS.str());
}


ErrorOr<StringRef> IncrementObfuscator::lookupImpl(StringRef ObfStr) const {
  unsigned i = findIndex(ObfStr);
  if (i < ReverseMap.size())
    return ReverseMap[i];

  return std::error_code(1, std::generic_category());
}

// Write reverse mapping.
void IncrementObfuscator::writeReverseMapImpl(raw_ostream &OS) const {
  OS << "BCSymbolMap Version: 2.0\n";
  for (auto S : ReverseMap) {
    assert(S != "" && "failed to initialize a member");
    OS << S << "\n";
  }
}

void IncrementObfuscator::readReverseMapImpl(llvm::MemoryBuffer* buf) {
  llvm::StringRef Data(buf->getBufferStart(),
                       buf->getBufferSize());
  llvm::StringRef LHS;
  // Check version string first.
  std::tie(LHS, Data) = Data.split('\n');
  if (!LHS.startswith("BCSymbolMap Version:")) {
    // Version string not present, warns but try to parse it.
    llvm::errs() << "warning: missing version string. Assuming 1.0.\n";
    ReverseMap.emplace_back(copyString(LHS));
    ++ Num;
  } else if (!LHS.equals("BCSymbolMap Version: 1.0")) {
    llvm::StringRef VersionNum;
    std::tie(LHS, VersionNum) = LHS.split(':');
    llvm::errs() << "warning: symbol map version "
                 << VersionNum << "is not supported. Not unobfuscating.\n";
    return;
  }
  while (!Data.empty()) {
    std::tie(LHS, Data) = Data.split('\n');
    ReverseMap.emplace_back(copyString(LHS));
    ++ Num;
  }
}

// Extract the number from a key.
unsigned IncrementObfuscator::findIndex(StringRef Key) const {
  assert(Key.size() > Pre.size() + Suf.size() && "invalid key");

  size_t Offset = Pre.size();
  if (Key.startswith("\01L") || Key.startswith("\01l"))
    Offset += 2;
  const char *NumStart = Key.data() + Offset;

  size_t Sz = Key.rfind(Suf);
  assert(Sz != StringRef::npos && "out of bounds");
  unsigned long long Ret;
  bool Error = getAsUnsignedInteger({NumStart, Sz - Offset}, 10, Ret);
  (void) Error;
  assert(!Error && "failed to convert to integer");

  return Ret;
};
