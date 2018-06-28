set(LLVM_TARGET_DEFINITIONS TAPIOptions.td)
tablegen(LLVM TAPIOptions.inc -gen-opt-parser-defs)
add_public_tablegen_target(TapiDriverOptions)

tapi_clang_tablegen(DiagnosticTAPIKinds.inc -gen-clang-diags-defs
  SOURCE DiagnosticTAPIKinds.td
  TARGET TAPIDiagnosticTAPI)
