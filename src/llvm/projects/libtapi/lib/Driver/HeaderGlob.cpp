//===- lib/Core/HeaderGlob.cpp - Header Glob --------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements a simple header file glob matcher.
///
//===----------------------------------------------------------------------===//

#include "tapi/Driver/HeaderGlob.h"
#include "llvm/ADT/SmallString.h"

using namespace llvm;

TAPI_NAMESPACE_INTERNAL_BEGIN

static StringLiteral regexMetachars = "()^$|+.[]\\{}";

HeaderGlob::HeaderGlob(StringRef globString, Regex &&regex, HeaderType type)
    : globString(globString), regex(std::move(regex)), headerType(type) {}

bool HeaderGlob::match(const HeaderFile &header) {
  if (header.type != headerType)
    return false;

  bool result = regex.match(header.fullPath);
  if (result)
    foundMatch = true;
  return result;
}

Expected<std::unique_ptr<HeaderGlob>> HeaderGlob::create(StringRef globString,
                                                         HeaderType type) {
  SmallString<128> regexString("^");
  unsigned numWildcards = 0;
  for (unsigned i = 0; i < globString.size(); ++i) {
    char c = globString[i];
    switch (c) {
    case '?':
      regexString += '.';
      break;
    case '*': {
      const char *previousChar = i > 0 ? globString.data() + i - 1 : nullptr;
      numWildcards = 1;
      ++i;
      while (i < globString.size() && globString[i] == '*') {
        ++numWildcards;
        ++i;
      }
      const char *nextChar =
          i < globString.size() ? globString.data() + i : nullptr;

      if ((numWildcards > 1) &&
          (previousChar == nullptr || *previousChar == '/') &&
          (nextChar == nullptr || *nextChar == '/')) {
        regexString += "(([^/]*(/|$))*)";
      } else
        regexString += "([^/]*)";
      break;
    }
    default:
      if (regexMetachars.find(c) != StringRef::npos)
        regexString.push_back('\\');
      regexString.push_back(c);
    }
  }
  regexString.push_back('$');

  if (numWildcards == 0)
    return make_error<StringError>("not a glob", inconvertibleErrorCode());

  auto regex = Regex(regexString);
  std::string error;
  if (regex.isValid(error))
    return make_unique<HeaderGlob>(globString, std::move(regex), type);

  return make_error<StringError>(error, inconvertibleErrorCode());
}

TAPI_NAMESPACE_INTERNAL_END
