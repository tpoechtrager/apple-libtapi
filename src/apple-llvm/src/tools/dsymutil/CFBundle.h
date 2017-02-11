
#include "llvm/ADT/StringRef.h"
#include <string>

void getBundleInfo(llvm::StringRef ExePath, std::string &bundleVersionStr,
                   std::string &bundleShortVersionStr, std::string &bundleIDStr,
                   bool &OmitShortVersion);
