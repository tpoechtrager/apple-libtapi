#include "tapi/Core/ClangDiagnostics.h"

using clang::DiagnosticBuilder;

namespace llvm {
namespace MachO {
const DiagnosticBuilder &operator<<(const DiagnosticBuilder &db,
                                    Architecture Arch) {
  db.AddString(getArchitectureName(Arch));
  return db;
}

const DiagnosticBuilder &operator<<(const DiagnosticBuilder &db,
                                    ArchitectureSet ArchSet) {
  db.AddString(std::string(ArchSet));
  return db;
}

const DiagnosticBuilder &operator<<(const DiagnosticBuilder &db,
                                    PlatformKind platform) {
  #if 0
  db.AddString(getPlatformName(platform));
  #endif
  abort();
  return db;
}

const DiagnosticBuilder &operator<<(const DiagnosticBuilder &db,
                                    PlatformSet platforms) {
  std::string diagString;
  diagString.append("[ ");
  unsigned index = 0;
  for (auto platform : platforms) {
    if (index > 0)
      diagString.append(", ");
    diagString.append(std::string(getPlatformName(platform)));
    ++index;
  }
  diagString.append(" ]");
  db.AddString(diagString);
  return db;
}
} // namespace MachO
} // namespace llvm
