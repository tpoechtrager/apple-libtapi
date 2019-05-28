//==--- DiagnosticTAPIKinds.td - TAPI diagnostics -------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// Define the diagnostic severities.
class Severity<string N> {
  string Name = N;
}
def SEV_Ignored : Severity<"Ignored">;
def SEV_Remark  : Severity<"Remark">;
def SEV_Warning : Severity<"Warning">;
def SEV_Error   : Severity<"Error">;
def SEV_Fatal   : Severity<"Fatal">;

// Define the diagnostic classes.
class DiagClass;
def CLASS_NOTE      : DiagClass;
def CLASS_REMARK    : DiagClass;
def CLASS_WARNING   : DiagClass;
def CLASS_EXTENSION : DiagClass;
def CLASS_ERROR     : DiagClass;

// Responses to a diagnostic in a SFINAE context.
class SFINAEResponse;
def SFINAE_Suppress            : SFINAEResponse;

// Diagnostic Categories.  These can be applied to groups or individual
// diagnostics to specify a category.
class DiagCategory<string Name> {
  string CategoryName = Name;
}

// Diagnostic Groups.
class DiagGroup<string Name, list<DiagGroup> subgroups = []> {
  string GroupName = Name;
  list<DiagGroup> SubGroups = subgroups;
  string CategoryName = "";
  code Documentation = [{}];
}
class InGroup<DiagGroup G> { DiagGroup Group = G; }

// All diagnostics emitted by the compiler are an indirect subclass of this.
class Diagnostic<string text, DiagClass DC, Severity defaultmapping> {
  string         Component = ?;
  string         Text = text;
  DiagClass      Class = DC;
  SFINAEResponse SFINAE = SFINAE_Suppress;
  bit            AccessControl = 0;
  bit            WarningNoWerror = 0;
  bit            ShowInSystemHeader = 0;
  Severity       DefaultSeverity = defaultmapping;
  DiagGroup      Group;
  string         CategoryName = "";
}

class Error<string str>     : Diagnostic<str, CLASS_ERROR, SEV_Error>;
class Warning<string str>   : Diagnostic<str, CLASS_WARNING, SEV_Warning>;
class Note<string str>      : Diagnostic<str, CLASS_NOTE, SEV_Fatal/*ignored*/>;

let Component = "TAPI" in {
let CategoryName = "Command Line Issue" in {

def err : Error<"%0: '%1'">;
def err_no_architecture : Error<"no architecture specified: add -arch <architecture>">;
def err_no_target : Error<"no target specified: add --target=<triple>">;
def err_one_architecture : Error<"only one architecture can be specified '%0'">;
def err_one_target : Error<"only one target can be specified">;
def err_no_matching_target : Error<"no matching target found for target variant '%0'">;
def err_unsupported_vendor : Error<"vendor '%0' is not supported: '%1'">;
def err_unsupported_os : Error<"os '%0' is not supported: '%1'">;
def err_unsupported_environment : Error<"environment '%0' is not supported: '%1'">;
def err_no_install_name : Error<"no install name specified: add -install_name <path>">;
def err_invalid_current_version : Error<"invalid current version '%0'">;
def err_invalid_compatibility_version : Error<"invalid compatibility version '%0'">;
def err_no_deployment_target : Error<"no deployment target specified">;
def err_missing_sysroot : Error<"no such sysroot directory: '%0'">;
def err_cannot_open_file : Error<"cannot open file: '%0'">;
def err_no_such_public_header_file : Error<"no such public header file: '%0'">;
def err_no_such_private_header_file : Error<"no such private header file: '%0'">;
def err_no_such_public_pre_header_file : Error<"no such public pre-include header file: '%0'">;
def err_no_such_private_pre_header_file : Error<"no such private pre-include header file: '%0'">;
def err_no_such_excluded_public_header_file : Error<"no such excluded public header file: '%0'">;
def err_no_such_excluded_private_header_file : Error<"no such excluded private header file: '%0'">;
def err_no_such_symbols_file : Error<"no such symbols file: '%0'">;
def err_invalid_input_file : Error<"invalid input file: '%0'">;
def err_no_framework : Error<"no framework/dynamic library found">;
def err_more_than_one_framework : Error<" more than one framework/dynamic library found">;
def err_no_such_public_umbrella_header_file : Error<"no such public umbrella header file: '%0'">;
def err_no_such_private_umbrella_header_file : Error<"no such private umbrella header file: '%0'">;
def err_invalid_public_umbrella_header_file : Error<"no such public umbrella header '%0' in the public header directory">;
def err_invalid_private_umbrella_header_file : Error<"no such private umbrella header '%0' in the public header directory">;
def err_cannot_read_file : Error<"cannot read file '%0': %1">;
def err_cannot_write_file : Error<"cannot write file '%0': %1">;
def err_cannot_create_directory : Error<"cannot create directory '%0': %1">;
def err_cannot_find : Error<"cannot find %0: %1">;
def err_platform_mismatch : Error<"platform does not match: '%0' (provided) vs '%1' (found)">;
def err_architecture_mismatch : Error<"architectures do not match: '%0' (provided) vs '%1' (found)">;
def err_install_name_mismatch : Error<"install_name does not match: '%0' (provided) vs '%1' (found)">;
def err_current_version_mismatch : Error<"current_version does not match: '%0' (provided) vs '%1' (found)">;
def err_compatibility_version_mismatch : Error<"compatibility_version does not match: '%0' (provided) vs '%1' (found)">;
def err_appextension_safe_mismatch : Error<"ApplicationExtensionSafe flag does not match: '%0' (provided) vs '%1' (found)">;
def err_reexported_libraries_missing : Error<"re-exported library missing from %0: '%1'">;
def err_reexported_libraries_mismatch : Error<"re-exported libraries do not match: '%0' (provided) vs '%1' (found)">;
def err_allowable_clients_missing : Error<"allowable client missing from %0: '%1'">;
def err_allowable_clients_mismatch : Error<"allowable clients do not match: '%0' (provided) vs '%1' (found)">;
def warn_parent_umbrella_mismatch : Warning<"parent umbrella does not match: '%0' (provided) vs '%1' (found)">;
def err_no_twolevel_namespace : Error<"flat namespace libraries are not supported">;
def err_library_missing_symbol : Error<"library doesn't have symbol '%0'">;
def err_header_symbol_tlv_mismatch : Error<"header symbol '%0' is thread local, but dynamic library symbol '%1' is not">;
def err_dylib_symbol_tlv_mismatch : Error<"dynamic library symbol '%0' is thread local, but header symbol '%1' is not">;
def err_header_symbol_weak_mismatch : Error<"header symbol '%0' is weak defined, but dynamic library symbol '%1' is not">;
def err_dylib_symbol_weak_mismatch : Error<"dynamic library symbol '%0' is weak defined, but header symbol '%1' is not">;
def err_availability_mismatch : Error<"symbol '%0' doesn't have the same availability information: '%1' (header) vs '%2' (found)">;
def err_parsing_disabled : Error<"parsing of all headers disabled">;
def err_no_directory : Error<"input '%0' is not a directory">;
def err_not_a_dylib : Error<"input '%0' is not a dynamic library">;
def err_cannot_convert_dylib : Error<"cannot convert '%0' to a text-based stub file">;
def err_cannot_generate_stub : Error<"cannot generate text-based stub file">;
def err_cannot_find_reexport : Error<"cannot find re-exported library '%0'">;
def err_property_mismatch : Error<"%0 in '%1' does not match '%2'">;
def err_not_all_architectures: Error<"dynamic library '%0' doesn't have all the required architectures">;
def err_expected_one_input_file: Error<"expected only one input file">;
def err_unsupported_file_type: Error<"unsupported file type">;
def err_no_output_file: Error<"no output file specified">;
def err_header_symbol_missing : Error<"headers don't have symbol '%0'">;

def warn : Warning<"%0">;
def warn_truncating_current_version : Warning<
    "truncating current version '%0' to fit in 32-bit space used by old mach-o format">;
def warn_symbol_unavailable : Warning<"header marked symbol '%0' as unavailable">;
def warn_broken_symlink : Warning<"ignoring broken symlink '%0'">;
def warn_header_symbol_missing : Warning<"headers don't have symbol '%0'">;
def warn_glob_did_not_match: Warning<"glob '%0' did not match any header file">;

} // end of command line category.
} // end of TAPI component.
