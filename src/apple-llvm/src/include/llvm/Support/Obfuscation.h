//===-- Obfuscation.h - String obfuscation helper ---------------*- C++ -*-===//
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

#ifndef LLVM_SUPPORT_OBFUSCATION_H
#define LLVM_SUPPORT_OBFUSCATION_H

#include "llvm/ADT/DenseSet.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/MemoryBuffer.h"

namespace llvm {

/// \brief Base class providing basic obfuscation capability
///
/// Derived classes are to implement obfuscateImpl
class Obfuscator {
  using MapTy = StringMap<StringRef, BumpPtrAllocator &>;

public:
  Obfuscator()
      : Alloc(), ForwardMap(64, Alloc), IrreveribleForwardMap(32, Alloc) {}

  Obfuscator(const Obfuscator &) = delete;
  Obfuscator &operator = (const Obfuscator &) = delete;

  /// \brief Obfuscate a string, remember in the reverseMap if reverse is true.
  StringRef obfuscate(StringRef Src, bool reverse = false);

  /// \brief Revese lookup a string in the map.
  ErrorOr<StringRef> lookup(StringRef ObfStr) const {
    return lookupImpl(ObfStr);
  }

  /// \brief Write out the reverse map in textural form.
  ///
  /// Derived classes may wish to use a different format
  void writeReverseMap(raw_ostream &OS) const {
    writeReverseMapImpl(OS);
  }

  /// \brief Read back the reverse map from textural form.
  ///
  /// Derived classes may wish to use a different format
  void readReverseMap(MemoryBuffer *buf) {
    readReverseMapImpl(buf);
  }

  // Access the allocator
  BumpPtrAllocator &getAllocator() { return Alloc; }

  // As StringRef::copy, but will terminate with null
  char *copyString(StringRef S) {
    auto Ret = allocateString(S.size());
    std::copy(S.begin(), S.end(), Ret);
    return Ret;
  }

  // look up symbol in the forward map
  ErrorOr<StringRef> lookupForwardMap(StringRef Symbol) const;

protected:
  // Derived classes will implement this method to return the newly obfuscated
  // string. Derived classe are free to track internal state, but are not
  // required to (e.g. the base class tracks the reverse mapping, forward
  // mapping, and provides an allocator).
  //
  // The base class will only ever call this once for any unique input,
  // enforcing idempotence under sequential composition.
  virtual StringRef obfuscateImpl(StringRef, bool) = 0;

  // Other virtual implemationation need to provided by subclass
  virtual ErrorOr<StringRef> lookupImpl(StringRef) const = 0;
  virtual void writeReverseMapImpl(raw_ostream &OS) const = 0;
  virtual void readReverseMapImpl(MemoryBuffer *buf) = 0;

  // For sub classes, new up some new memory for a string. Will terminate
  // The memory with null.
  char *allocateString(unsigned Length) {
    assert(Length > 0 && "no length?");
    char *P = (char *)Alloc.Allocate(Length + 1, 1);
    P[Length] = '\0';
    return P;
  }

  virtual ~Obfuscator() {}

private:
  BumpPtrAllocator Alloc;
  // Track the mapping, to enforce idempotence under sequential composition
  MapTy ForwardMap;

  MapTy IrreveribleForwardMap;

  virtual void anchor() = 0;
};

/// \brief Simple ROT13 obfuscator
///
/// involutory and stateless
class Rot13Obfuscator : public Obfuscator {
  using ReverseMapTy = StringMap<StringRef, BumpPtrAllocator &>;
  using Obfuscator::Obfuscator;
public:
  Rot13Obfuscator() : Obfuscator(), ReverseMap(16, getAllocator()) {}

protected:
  virtual StringRef obfuscateImpl(StringRef, bool) override;
  virtual ErrorOr<StringRef> lookupImpl(StringRef) const override;
  virtual void writeReverseMapImpl(raw_ostream &OS) const override;
  virtual void readReverseMapImpl(MemoryBuffer* buf) override;

private:
  // For when we want to track a reverse mapping
  ReverseMapTy ReverseMap;

  static char rot13(char C) {
    if (C >= 'A' && C <= 'Z')
      return ((C - 'A' + 13) % 26) + 'A';
    if (C >= 'a' && C <= 'z')
      return ((C - 'a' + 13) % 26) + 'a';
    return C;
  }

  virtual void anchor() override;

};

/// \brief Stateful incrementor obfuscator
///
/// Minimal stateful obfuscator, which track the number of obfuscated symbols
/// and appends that number to the end of a prefix. Thus, to get the same
/// resulting obfuscation, this must be fed the inputs in the same order
class IncrementObfuscator : public Obfuscator {
  using ReverseMapTy = std::vector<StringRef>;
  using Obfuscator::Obfuscator;
public:
  IncrementObfuscator(StringRef Prefix = "__hidden#", StringRef Suffix = "_",
                      StringRef IrPrefix = "__ir_hidden#")
      : Obfuscator(), Pre(copyString(Prefix)),
        Suf(copyString(Suffix)), IrPre(IrPrefix)
  {
    assert(!Prefix.equals(IrPrefix) &&
        "Prefix and IrPrefix cannot be the same");
  }

protected:
  virtual StringRef obfuscateImpl(StringRef, bool) override;
  virtual ErrorOr<StringRef> lookupImpl(StringRef) const override;
  virtual void writeReverseMapImpl(raw_ostream &OS) const override;
  virtual void readReverseMapImpl(MemoryBuffer* buf) override;

private:
  StringRef Pre;
  StringRef Suf;
  StringRef IrPre;
  unsigned Num = 0;
  unsigned IrNum = 0;
  ReverseMapTy ReverseMap;

  // Extract the number from a key
  unsigned findIndex(StringRef Key) const;

  virtual void anchor() override;

};

} // End llvm namespace

#endif // LLVM_SUPPORT_OBFUSCATION_H
