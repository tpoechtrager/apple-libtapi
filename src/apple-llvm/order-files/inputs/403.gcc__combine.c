// RUN: %clang_cc1 -triple x86_64-apple-macosx10.11.0 -Wdeprecated-objc-isa-usage -Werror=deprecated-objc-isa-usage -emit-obj -mrelax-all -disable-free -disable-llvm-verifier -main-file-name %s -mrelocation-model pic -pic-level 2 -mthread-model posix -mdisable-fp-elim -masm-verbose -munwind-tables -target-cpu core2 -target-linker-version 253.9 -gdwarf-2 -dwarf-column-info -O0 -Wnon-modular-include-in-framework-module -Wno-trigraphs -Wno-missing-field-initializers -Wno-missing-prototypes -Wreturn-type -Wunreachable-code -Wdeprecated-objc-isa-usage -Wobjc-root-class -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wconditional-uninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wshorten-64-to-32 -Wno-newline-eof -Wdeprecated-declarations -Wno-sign-conversion -Wpointer-sign -Wno-unneeded-internal-declaration -Wno-enum-conversion -Wno-conditional-uninitialized -Wno-shorten-64-to-32 -ferror-limit 19 -fmessage-length 0 -stack-protector 1 -mstackrealign -fblocks -fobjc-runtime=macosx-10.11.0 -fencode-extended-block-signature -fmax-type-align=16 -fdiagnostics-show-option -x c %s
// RUN: %clang_cc1 -triple i386-apple-macosx10.11.0 -emit-obj -mrelax-all -disable-free -disable-llvm-verifier -main-file-name %s -mrelocation-model pic -pic-level 2 -mthread-model posix -mdisable-fp-elim -masm-verbose -target-cpu yonah -target-linker-version 253.9 -gdwarf-2 -dwarf-column-info -O0 -Wnon-modular-include-in-framework-module -Wno-trigraphs -Wno-missing-field-initializers -Wno-missing-prototypes -Wreturn-type -Wunreachable-code -Wdeprecated-objc-isa-usage -Wobjc-root-class -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wconditional-uninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wshorten-64-to-32 -Wno-newline-eof -Wdeprecated-declarations -Wno-sign-conversion -Wpointer-sign -Wno-unneeded-internal-declaration -Wno-enum-conversion -Wno-conditional-uninitialized -Wno-shorten-64-to-32 -ferror-limit 19 -fmessage-length 0 -stack-protector 1 -mstackrealign -fblocks -fobjc-runtime=macosx-fragile-10.11.0 -fobjc-subscripting-legacy-runtime -fencode-extended-block-signature -fmax-type-align=16 -fdiagnostics-show-option -x c %s
// RUN: %clang_cc1 -triple arm64-apple-macosx10.11.0 -Wdeprecated-objc-isa-usage -Werror=deprecated-objc-isa-usage -emit-obj -mrelax-all -disable-free -disable-llvm-verifier -main-file-name %s -mrelocation-model pic -pic-level 2 -mthread-model posix -mdisable-fp-elim -masm-verbose -target-cpu cyclone -target-feature +neon -target-feature +crc -target-feature +crypto -target-feature +zcm -target-feature +zcz -target-abi darwinpcs -target-linker-version 253.9 -gdwarf-2 -dwarf-column-info -O0 -Wnon-modular-include-in-framework-module -Wno-trigraphs -Wno-missing-field-initializers -Wno-missing-prototypes -Wreturn-type -Wunreachable-code -Wdeprecated-objc-isa-usage -Wobjc-root-class -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wconditional-uninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wshorten-64-to-32 -Wno-newline-eof -Wdeprecated-declarations -Wno-sign-conversion -Wpointer-sign -Wno-unneeded-internal-declaration -Wno-enum-conversion -Wno-conditional-uninitialized -Wno-shorten-64-to-32 -ferror-limit 19 -fmessage-length 0 -stack-protector 1 -mstackrealign -fallow-half-arguments-and-returns -fblocks -fobjc-runtime=macosx-10.11.0 -fencode-extended-block-signature -fmax-type-align=16 -fdiagnostics-show-option -x c %s
// RUN: %clang_cc1 -triple thumbv7-apple-macosx10.11.0 -emit-obj -mrelax-all -disable-free -disable-llvm-verifier -main-file-name %s -mrelocation-model pic -pic-level 2 -mthread-model posix -mdisable-fp-elim -masm-verbose -target-cpu cortex-a8 -target-feature +soft-float-abi -target-abi apcs-gnu -mfloat-abi soft -target-linker-version 253.9 -gdwarf-2 -dwarf-column-info -O0 -Wnon-modular-include-in-framework-module -Wno-trigraphs -Wno-missing-field-initializers -Wno-missing-prototypes -Wreturn-type -Wunreachable-code -Wdeprecated-objc-isa-usage -Wobjc-root-class -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wconditional-uninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wshorten-64-to-32 -Wno-newline-eof -Wdeprecated-declarations -Wno-sign-conversion -Wpointer-sign -Wno-unneeded-internal-declaration -Wno-enum-conversion -Wno-conditional-uninitialized -Wno-shorten-64-to-32 -ferror-limit 19 -fmessage-length 0 -stack-protector 1 -mstackrealign -fblocks -fobjc-runtime=macosx-10.11.0 -fencode-extended-block-signature -fsjlj-exceptions -fmax-type-align=16 -fdiagnostics-show-option -x c %s
// RUN: %clang_cc1 -triple x86_64-apple-macosx10.11.0 -Wdeprecated-objc-isa-usage -Werror=deprecated-objc-isa-usage -emit-obj -disable-free -disable-llvm-verifier -main-file-name %s -mrelocation-model pic -pic-level 2 -mthread-model posix -mdisable-fp-elim -masm-verbose -munwind-tables -target-cpu core2 -target-linker-version 253.9 -gdwarf-2 -dwarf-column-info -Os -Wnon-modular-include-in-framework-module -Wno-trigraphs -Wno-missing-field-initializers -Wno-missing-prototypes -Wreturn-type -Wunreachable-code -Wdeprecated-objc-isa-usage -Wobjc-root-class -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wconditional-uninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wshorten-64-to-32 -Wno-newline-eof -Wdeprecated-declarations -Wno-sign-conversion -Wpointer-sign -Wno-unneeded-internal-declaration -Wno-enum-conversion -Wno-conditional-uninitialized -Wno-shorten-64-to-32 -ferror-limit 19 -fmessage-length 0 -stack-protector 1 -mstackrealign -fblocks -fobjc-runtime=macosx-10.11.0 -fencode-extended-block-signature -fmax-type-align=16 -fdiagnostics-show-option -vectorize-loops -vectorize-slp -x c %s
// RUN: %clang_cc1 -triple i386-apple-macosx10.11.0 -emit-obj -disable-free -disable-llvm-verifier -main-file-name %s -mrelocation-model pic -pic-level 2 -mthread-model posix -mdisable-fp-elim -masm-verbose -target-cpu yonah -target-linker-version 253.9 -gdwarf-2 -dwarf-column-info -Os -Wnon-modular-include-in-framework-module -Wno-trigraphs -Wno-missing-field-initializers -Wno-missing-prototypes -Wreturn-type -Wunreachable-code -Wdeprecated-objc-isa-usage -Wobjc-root-class -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wconditional-uninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wshorten-64-to-32 -Wno-newline-eof -Wdeprecated-declarations -Wno-sign-conversion -Wpointer-sign -Wno-unneeded-internal-declaration -Wno-enum-conversion -Wno-conditional-uninitialized -Wno-shorten-64-to-32 -ferror-limit 19 -fmessage-length 0 -stack-protector 1 -mstackrealign -fblocks -fobjc-runtime=macosx-fragile-10.11.0 -fobjc-subscripting-legacy-runtime -fencode-extended-block-signature -fmax-type-align=16 -fdiagnostics-show-option -vectorize-loops -vectorize-slp -x c %s
// RUN: %clang_cc1 -triple arm64-apple-macosx10.11.0 -Wdeprecated-objc-isa-usage -Werror=deprecated-objc-isa-usage -emit-obj -disable-free -disable-llvm-verifier -main-file-name %s -mrelocation-model pic -pic-level 2 -mthread-model posix -mdisable-fp-elim -masm-verbose -target-cpu cyclone -target-feature +neon -target-feature +crc -target-feature +crypto -target-feature +zcm -target-feature +zcz -target-abi darwinpcs -target-linker-version 253.9 -gdwarf-2 -dwarf-column-info -Os -Wnon-modular-include-in-framework-module -Wno-trigraphs -Wno-missing-field-initializers -Wno-missing-prototypes -Wreturn-type -Wunreachable-code -Wdeprecated-objc-isa-usage -Wobjc-root-class -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wconditional-uninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wshorten-64-to-32 -Wno-newline-eof -Wdeprecated-declarations -Wno-sign-conversion -Wpointer-sign -Wno-unneeded-internal-declaration -Wno-enum-conversion -Wno-conditional-uninitialized -Wno-shorten-64-to-32 -ferror-limit 19 -fmessage-length 0 -stack-protector 1 -mstackrealign -fallow-half-arguments-and-returns -fblocks -fobjc-runtime=macosx-10.11.0 -fencode-extended-block-signature -fmax-type-align=16 -fdiagnostics-show-option -vectorize-loops -vectorize-slp -x c %s
// RUN: %clang_cc1 -triple thumbv7-apple-macosx10.11.0 -emit-obj -disable-free -disable-llvm-verifier -main-file-name %s -mrelocation-model pic -pic-level 2 -mthread-model posix -mdisable-fp-elim -masm-verbose -target-cpu cortex-a8 -target-feature +soft-float-abi -target-abi apcs-gnu -mfloat-abi soft -target-linker-version 253.9 -gdwarf-2 -dwarf-column-info -Os -Wnon-modular-include-in-framework-module -Wno-trigraphs -Wno-missing-field-initializers -Wno-missing-prototypes -Wreturn-type -Wunreachable-code -Wdeprecated-objc-isa-usage -Wobjc-root-class -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wconditional-uninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wshorten-64-to-32 -Wno-newline-eof -Wdeprecated-declarations -Wno-sign-conversion -Wpointer-sign -Wno-unneeded-internal-declaration -Wno-enum-conversion -Wno-conditional-uninitialized -Wno-shorten-64-to-32 -ferror-limit 19 -fmessage-length 0 -stack-protector 1 -mstackrealign -fblocks -fobjc-runtime=macosx-10.11.0 -fencode-extended-block-signature -fsjlj-exceptions -fmax-type-align=16 -fdiagnostics-show-option -vectorize-loops -vectorize-slp -x c %s
// RUN: %clang_cc1 -triple x86_64-apple-macosx10.11.0 -Wdeprecated-objc-isa-usage -Werror=deprecated-objc-isa-usage -emit-obj -disable-free -disable-llvm-verifier -main-file-name %s -mrelocation-model pic -pic-level 2 -mthread-model posix -mdisable-fp-elim -masm-verbose -munwind-tables -target-cpu core2 -target-linker-version 253.9 -dwarf-column-info -O2 -Wnon-modular-include-in-framework-module -Wno-trigraphs -Wno-missing-field-initializers -Wno-missing-prototypes -Wreturn-type -Wunreachable-code -Wdeprecated-objc-isa-usage -Wobjc-root-class -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wconditional-uninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wshorten-64-to-32 -Wno-newline-eof -Wdeprecated-declarations -Wno-sign-conversion -Wpointer-sign -Wno-unneeded-internal-declaration -Wno-enum-conversion -Wno-conditional-uninitialized -Wno-shorten-64-to-32 -ferror-limit 19 -fmessage-length 0 -stack-protector 1 -mstackrealign -fblocks -fobjc-runtime=macosx-10.11.0 -fencode-extended-block-signature -fmax-type-align=16 -fdiagnostics-show-option -vectorize-loops -vectorize-slp -x c %s
// RUN: %clang_cc1 -triple i386-apple-macosx10.11.0 -emit-obj -disable-free -disable-llvm-verifier -main-file-name %s -mrelocation-model pic -pic-level 2 -mthread-model posix -mdisable-fp-elim -masm-verbose -target-cpu yonah -target-linker-version 253.9 -dwarf-column-info -O2 -Wnon-modular-include-in-framework-module -Wno-trigraphs -Wno-missing-field-initializers -Wno-missing-prototypes -Wreturn-type -Wunreachable-code -Wdeprecated-objc-isa-usage -Wobjc-root-class -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wconditional-uninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wshorten-64-to-32 -Wno-newline-eof -Wdeprecated-declarations -Wno-sign-conversion -Wpointer-sign -Wno-unneeded-internal-declaration -Wno-enum-conversion -Wno-conditional-uninitialized -Wno-shorten-64-to-32 -ferror-limit 19 -fmessage-length 0 -stack-protector 1 -mstackrealign -fblocks -fobjc-runtime=macosx-fragile-10.11.0 -fobjc-subscripting-legacy-runtime -fencode-extended-block-signature -fmax-type-align=16 -fdiagnostics-show-option -vectorize-loops -vectorize-slp -x c %s
// RUN: %clang_cc1 -triple arm64-apple-macosx10.11.0 -Wdeprecated-objc-isa-usage -Werror=deprecated-objc-isa-usage -emit-obj -disable-free -disable-llvm-verifier -main-file-name %s -mrelocation-model pic -pic-level 2 -mthread-model posix -mdisable-fp-elim -masm-verbose -target-cpu cyclone -target-feature +neon -target-feature +crc -target-feature +crypto -target-feature +zcm -target-feature +zcz -target-abi darwinpcs -target-linker-version 253.9 -dwarf-column-info -O2 -Wnon-modular-include-in-framework-module -Wno-trigraphs -Wno-missing-field-initializers -Wno-missing-prototypes -Wreturn-type -Wunreachable-code -Wdeprecated-objc-isa-usage -Wobjc-root-class -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wconditional-uninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wshorten-64-to-32 -Wno-newline-eof -Wdeprecated-declarations -Wno-sign-conversion -Wpointer-sign -Wno-unneeded-internal-declaration -Wno-enum-conversion -Wno-conditional-uninitialized -Wno-shorten-64-to-32 -ferror-limit 19 -fmessage-length 0 -stack-protector 1 -mstackrealign -fallow-half-arguments-and-returns -fblocks -fobjc-runtime=macosx-10.11.0 -fencode-extended-block-signature -fmax-type-align=16 -fdiagnostics-show-option -vectorize-loops -vectorize-slp -x c %s
// RUN: %clang_cc1 -triple thumbv7-apple-macosx10.11.0 -emit-obj -disable-free -disable-llvm-verifier -main-file-name %s -mrelocation-model pic -pic-level 2 -mthread-model posix -mdisable-fp-elim -masm-verbose -target-cpu cortex-a8 -target-feature +soft-float-abi -target-abi apcs-gnu -mfloat-abi soft -target-linker-version 253.9 -dwarf-column-info -O2 -Wnon-modular-include-in-framework-module -Wno-trigraphs -Wno-missing-field-initializers -Wno-missing-prototypes -Wreturn-type -Wunreachable-code -Wdeprecated-objc-isa-usage -Wobjc-root-class -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wconditional-uninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wshorten-64-to-32 -Wno-newline-eof -Wdeprecated-declarations -Wno-sign-conversion -Wpointer-sign -Wno-unneeded-internal-declaration -Wno-enum-conversion -Wno-conditional-uninitialized -Wno-shorten-64-to-32 -ferror-limit 19 -fmessage-length 0 -stack-protector 1 -mstackrealign -fblocks -fobjc-runtime=macosx-10.11.0 -fencode-extended-block-signature -fsjlj-exceptions -fmax-type-align=16 -fdiagnostics-show-option -vectorize-loops -vectorize-slp -x c %s
// RUN: %clang_cc1 -triple x86_64-apple-macosx10.11.0 -Wdeprecated-objc-isa-usage -Werror=deprecated-objc-isa-usage -emit-obj -disable-free -disable-llvm-verifier -main-file-name %s -mrelocation-model pic -pic-level 2 -mthread-model posix -mdisable-fp-elim -masm-verbose -munwind-tables -target-cpu core2 -target-linker-version 253.9 -dwarf-column-info -O3 -Wnon-modular-include-in-framework-module -Wno-trigraphs -Wno-missing-field-initializers -Wno-missing-prototypes -Wreturn-type -Wunreachable-code -Wdeprecated-objc-isa-usage -Wobjc-root-class -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wconditional-uninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wshorten-64-to-32 -Wno-newline-eof -Wdeprecated-declarations -Wno-sign-conversion -Wpointer-sign -Wno-unneeded-internal-declaration -Wno-enum-conversion -Wno-conditional-uninitialized -Wno-shorten-64-to-32 -ferror-limit 19 -fmessage-length 0 -stack-protector 1 -mstackrealign -fblocks -fobjc-runtime=macosx-10.11.0 -fencode-extended-block-signature -fmax-type-align=16 -fdiagnostics-show-option -vectorize-loops -vectorize-slp -x c %s
// RUN: %clang_cc1 -triple i386-apple-macosx10.11.0 -emit-obj -disable-free -disable-llvm-verifier -main-file-name %s -mrelocation-model pic -pic-level 2 -mthread-model posix -mdisable-fp-elim -masm-verbose -target-cpu yonah -target-linker-version 253.9 -dwarf-column-info -O3 -Wnon-modular-include-in-framework-module -Wno-trigraphs -Wno-missing-field-initializers -Wno-missing-prototypes -Wreturn-type -Wunreachable-code -Wdeprecated-objc-isa-usage -Wobjc-root-class -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wconditional-uninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wshorten-64-to-32 -Wno-newline-eof -Wdeprecated-declarations -Wno-sign-conversion -Wpointer-sign -Wno-unneeded-internal-declaration -Wno-enum-conversion -Wno-conditional-uninitialized -Wno-shorten-64-to-32 -ferror-limit 19 -fmessage-length 0 -stack-protector 1 -mstackrealign -fblocks -fobjc-runtime=macosx-fragile-10.11.0 -fobjc-subscripting-legacy-runtime -fencode-extended-block-signature -fmax-type-align=16 -fdiagnostics-show-option -vectorize-loops -vectorize-slp -x c %s
// RUN: %clang_cc1 -triple arm64-apple-macosx10.11.0 -Wdeprecated-objc-isa-usage -Werror=deprecated-objc-isa-usage -emit-obj -disable-free -disable-llvm-verifier -main-file-name %s -mrelocation-model pic -pic-level 2 -mthread-model posix -mdisable-fp-elim -masm-verbose -target-cpu cyclone -target-feature +neon -target-feature +crc -target-feature +crypto -target-feature +zcm -target-feature +zcz -target-abi darwinpcs -target-linker-version 253.9 -dwarf-column-info -O3 -Wnon-modular-include-in-framework-module -Wno-trigraphs -Wno-missing-field-initializers -Wno-missing-prototypes -Wreturn-type -Wunreachable-code -Wdeprecated-objc-isa-usage -Wobjc-root-class -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wconditional-uninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wshorten-64-to-32 -Wno-newline-eof -Wdeprecated-declarations -Wno-sign-conversion -Wpointer-sign -Wno-unneeded-internal-declaration -Wno-enum-conversion -Wno-conditional-uninitialized -Wno-shorten-64-to-32 -ferror-limit 19 -fmessage-length 0 -stack-protector 1 -mstackrealign -fallow-half-arguments-and-returns -fblocks -fobjc-runtime=macosx-10.11.0 -fencode-extended-block-signature -fmax-type-align=16 -fdiagnostics-show-option -vectorize-loops -vectorize-slp -x c %s
// RUN: %clang_cc1 -triple thumbv7-apple-macosx10.11.0 -emit-obj -disable-free -disable-llvm-verifier -main-file-name %s -mrelocation-model pic -pic-level 2 -mthread-model posix -mdisable-fp-elim -masm-verbose -target-cpu cortex-a8 -target-feature +soft-float-abi -target-abi apcs-gnu -mfloat-abi soft -target-linker-version 253.9 -dwarf-column-info -O3 -Wnon-modular-include-in-framework-module -Wno-trigraphs -Wno-missing-field-initializers -Wno-missing-prototypes -Wreturn-type -Wunreachable-code -Wdeprecated-objc-isa-usage -Wobjc-root-class -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wconditional-uninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wshorten-64-to-32 -Wno-newline-eof -Wdeprecated-declarations -Wno-sign-conversion -Wpointer-sign -Wno-unneeded-internal-declaration -Wno-enum-conversion -Wno-conditional-uninitialized -Wno-shorten-64-to-32 -ferror-limit 19 -fmessage-length 0 -stack-protector 1 -mstackrealign -fblocks -fobjc-runtime=macosx-10.11.0 -fencode-extended-block-signature -fsjlj-exceptions -fmax-type-align=16 -fdiagnostics-show-option -vectorize-loops -vectorize-slp -x c %s
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c" 1
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 146 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c" 2
# 77 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/config.h" 1

# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/auto-host.h" 1
# 3 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/config.h" 2




struct rtx_def;
typedef struct rtx_def *rtx;
struct rtvec_def;
typedef struct rtvec_def *rtvec;
union tree_node;
typedef union tree_node *tree;

# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/ansidecl.h" 1
# 14 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/config.h" 2
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/biarch64.h" 1
# 15 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/config.h" 2
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/i386.h" 1
# 53 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/i386.h"
struct processor_costs {
  const int add;
  const int lea;
  const int shift_var;
  const int shift_const;
  const int mult_init;
  const int mult_bit;
  const int divide;
  int movsx;
  int movzx;
  const int large_insn;
  const int move_ratio;

  const int movzbl_load;
  const int int_load[3];


  const int int_store[3];

  const int fp_move;
  const int fp_load[3];

  const int fp_store[3];

  const int mmx_move;
  const int mmx_load[2];

  const int mmx_store[2];

  const int sse_move;
  const int sse_load[3];

  const int sse_store[3];

  const int mmxsse_to_integer;

  const int prefetch_block;
  const int simultaneous_prefetches;

};

extern const struct processor_costs *ix86_cost;



extern int target_flags;
# 211 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/i386.h"
extern const int x86_use_leave, x86_push_memory, x86_zero_extend_with_and;
extern const int x86_use_bit_test, x86_cmove, x86_deep_branch;
extern const int x86_branch_hints, x86_unroll_strlen;
extern const int x86_double_with_add, x86_partial_reg_stall, x86_movx;
extern const int x86_use_loop, x86_use_fiop, x86_use_mov0;
extern const int x86_use_cltd, x86_read_modify_write;
extern const int x86_read_modify, x86_split_long_moves;
extern const int x86_promote_QImode, x86_single_stringop;
extern const int x86_himode_math, x86_qimode_math, x86_promote_qi_regs;
extern const int x86_promote_hi_regs, x86_integer_DFmode_moves;
extern const int x86_add_esp_4, x86_add_esp_8, x86_sub_esp_4, x86_sub_esp_8;
extern const int x86_partial_reg_dependency, x86_memory_mismatch_stall;
extern const int x86_accumulate_outgoing_args, x86_prologue_using_move;
extern const int x86_epilogue_using_move, x86_decompose_lea;
extern const int x86_arch_always_fancy_math_387;
extern int x86_prefetch_sse;
# 395 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/i386.h"
enum processor_type
{
  PROCESSOR_I386,
  PROCESSOR_I486,
  PROCESSOR_PENTIUM,
  PROCESSOR_PENTIUMPRO,
  PROCESSOR_K6,
  PROCESSOR_ATHLON,
  PROCESSOR_PENTIUM4,
  PROCESSOR_max
};
enum fpmath_unit
{
  FPMATH_387 = 1,
  FPMATH_SSE = 2
};

extern enum processor_type ix86_cpu;
extern enum fpmath_unit ix86_fpmath;

extern int ix86_arch;
# 1202 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/i386.h"
enum reg_class
{
  NO_REGS,
  AREG, DREG, CREG, BREG, SIREG, DIREG,
  AD_REGS,
  Q_REGS,
  NON_Q_REGS,
  INDEX_REGS,
  LEGACY_REGS,
  GENERAL_REGS,
  FP_TOP_REG, FP_SECOND_REG,
  FLOAT_REGS,
  SSE_REGS,
  MMX_REGS,
  FP_TOP_SSE_REGS,
  FP_SECOND_SSE_REGS,
  FLOAT_SSE_REGS,
  FLOAT_INT_REGS,
  INT_SSE_REGS,
  FLOAT_INT_SSE_REGS,
  ALL_REGS, LIM_REG_CLASSES
};
# 1667 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/i386.h"
typedef struct ix86_args {
  int words;
  int nregs;
  int regno;
  int sse_words;
  int sse_nregs;
  int sse_regno;
  int maybe_vaarg;
} CUMULATIVE_ARGS;
# 2029 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/i386.h"
enum ix86_builtins
{
  IX86_BUILTIN_ADDPS,
  IX86_BUILTIN_ADDSS,
  IX86_BUILTIN_DIVPS,
  IX86_BUILTIN_DIVSS,
  IX86_BUILTIN_MULPS,
  IX86_BUILTIN_MULSS,
  IX86_BUILTIN_SUBPS,
  IX86_BUILTIN_SUBSS,

  IX86_BUILTIN_CMPEQPS,
  IX86_BUILTIN_CMPLTPS,
  IX86_BUILTIN_CMPLEPS,
  IX86_BUILTIN_CMPGTPS,
  IX86_BUILTIN_CMPGEPS,
  IX86_BUILTIN_CMPNEQPS,
  IX86_BUILTIN_CMPNLTPS,
  IX86_BUILTIN_CMPNLEPS,
  IX86_BUILTIN_CMPNGTPS,
  IX86_BUILTIN_CMPNGEPS,
  IX86_BUILTIN_CMPORDPS,
  IX86_BUILTIN_CMPUNORDPS,
  IX86_BUILTIN_CMPNEPS,
  IX86_BUILTIN_CMPEQSS,
  IX86_BUILTIN_CMPLTSS,
  IX86_BUILTIN_CMPLESS,
  IX86_BUILTIN_CMPGTSS,
  IX86_BUILTIN_CMPGESS,
  IX86_BUILTIN_CMPNEQSS,
  IX86_BUILTIN_CMPNLTSS,
  IX86_BUILTIN_CMPNLESS,
  IX86_BUILTIN_CMPNGTSS,
  IX86_BUILTIN_CMPNGESS,
  IX86_BUILTIN_CMPORDSS,
  IX86_BUILTIN_CMPUNORDSS,
  IX86_BUILTIN_CMPNESS,

  IX86_BUILTIN_COMIEQSS,
  IX86_BUILTIN_COMILTSS,
  IX86_BUILTIN_COMILESS,
  IX86_BUILTIN_COMIGTSS,
  IX86_BUILTIN_COMIGESS,
  IX86_BUILTIN_COMINEQSS,
  IX86_BUILTIN_UCOMIEQSS,
  IX86_BUILTIN_UCOMILTSS,
  IX86_BUILTIN_UCOMILESS,
  IX86_BUILTIN_UCOMIGTSS,
  IX86_BUILTIN_UCOMIGESS,
  IX86_BUILTIN_UCOMINEQSS,

  IX86_BUILTIN_CVTPI2PS,
  IX86_BUILTIN_CVTPS2PI,
  IX86_BUILTIN_CVTSI2SS,
  IX86_BUILTIN_CVTSS2SI,
  IX86_BUILTIN_CVTTPS2PI,
  IX86_BUILTIN_CVTTSS2SI,

  IX86_BUILTIN_MAXPS,
  IX86_BUILTIN_MAXSS,
  IX86_BUILTIN_MINPS,
  IX86_BUILTIN_MINSS,

  IX86_BUILTIN_LOADAPS,
  IX86_BUILTIN_LOADUPS,
  IX86_BUILTIN_STOREAPS,
  IX86_BUILTIN_STOREUPS,
  IX86_BUILTIN_LOADSS,
  IX86_BUILTIN_STORESS,
  IX86_BUILTIN_MOVSS,

  IX86_BUILTIN_MOVHLPS,
  IX86_BUILTIN_MOVLHPS,
  IX86_BUILTIN_LOADHPS,
  IX86_BUILTIN_LOADLPS,
  IX86_BUILTIN_STOREHPS,
  IX86_BUILTIN_STORELPS,

  IX86_BUILTIN_MASKMOVQ,
  IX86_BUILTIN_MOVMSKPS,
  IX86_BUILTIN_PMOVMSKB,

  IX86_BUILTIN_MOVNTPS,
  IX86_BUILTIN_MOVNTQ,

  IX86_BUILTIN_PACKSSWB,
  IX86_BUILTIN_PACKSSDW,
  IX86_BUILTIN_PACKUSWB,

  IX86_BUILTIN_PADDB,
  IX86_BUILTIN_PADDW,
  IX86_BUILTIN_PADDD,
  IX86_BUILTIN_PADDSB,
  IX86_BUILTIN_PADDSW,
  IX86_BUILTIN_PADDUSB,
  IX86_BUILTIN_PADDUSW,
  IX86_BUILTIN_PSUBB,
  IX86_BUILTIN_PSUBW,
  IX86_BUILTIN_PSUBD,
  IX86_BUILTIN_PSUBSB,
  IX86_BUILTIN_PSUBSW,
  IX86_BUILTIN_PSUBUSB,
  IX86_BUILTIN_PSUBUSW,

  IX86_BUILTIN_PAND,
  IX86_BUILTIN_PANDN,
  IX86_BUILTIN_POR,
  IX86_BUILTIN_PXOR,

  IX86_BUILTIN_PAVGB,
  IX86_BUILTIN_PAVGW,

  IX86_BUILTIN_PCMPEQB,
  IX86_BUILTIN_PCMPEQW,
  IX86_BUILTIN_PCMPEQD,
  IX86_BUILTIN_PCMPGTB,
  IX86_BUILTIN_PCMPGTW,
  IX86_BUILTIN_PCMPGTD,

  IX86_BUILTIN_PEXTRW,
  IX86_BUILTIN_PINSRW,

  IX86_BUILTIN_PMADDWD,

  IX86_BUILTIN_PMAXSW,
  IX86_BUILTIN_PMAXUB,
  IX86_BUILTIN_PMINSW,
  IX86_BUILTIN_PMINUB,

  IX86_BUILTIN_PMULHUW,
  IX86_BUILTIN_PMULHW,
  IX86_BUILTIN_PMULLW,

  IX86_BUILTIN_PSADBW,
  IX86_BUILTIN_PSHUFW,

  IX86_BUILTIN_PSLLW,
  IX86_BUILTIN_PSLLD,
  IX86_BUILTIN_PSLLQ,
  IX86_BUILTIN_PSRAW,
  IX86_BUILTIN_PSRAD,
  IX86_BUILTIN_PSRLW,
  IX86_BUILTIN_PSRLD,
  IX86_BUILTIN_PSRLQ,
  IX86_BUILTIN_PSLLWI,
  IX86_BUILTIN_PSLLDI,
  IX86_BUILTIN_PSLLQI,
  IX86_BUILTIN_PSRAWI,
  IX86_BUILTIN_PSRADI,
  IX86_BUILTIN_PSRLWI,
  IX86_BUILTIN_PSRLDI,
  IX86_BUILTIN_PSRLQI,

  IX86_BUILTIN_PUNPCKHBW,
  IX86_BUILTIN_PUNPCKHWD,
  IX86_BUILTIN_PUNPCKHDQ,
  IX86_BUILTIN_PUNPCKLBW,
  IX86_BUILTIN_PUNPCKLWD,
  IX86_BUILTIN_PUNPCKLDQ,

  IX86_BUILTIN_SHUFPS,

  IX86_BUILTIN_RCPPS,
  IX86_BUILTIN_RCPSS,
  IX86_BUILTIN_RSQRTPS,
  IX86_BUILTIN_RSQRTSS,
  IX86_BUILTIN_SQRTPS,
  IX86_BUILTIN_SQRTSS,

  IX86_BUILTIN_UNPCKHPS,
  IX86_BUILTIN_UNPCKLPS,

  IX86_BUILTIN_ANDPS,
  IX86_BUILTIN_ANDNPS,
  IX86_BUILTIN_ORPS,
  IX86_BUILTIN_XORPS,

  IX86_BUILTIN_EMMS,
  IX86_BUILTIN_LDMXCSR,
  IX86_BUILTIN_STMXCSR,
  IX86_BUILTIN_SFENCE,


  IX86_BUILTIN_FEMMS,
  IX86_BUILTIN_PAVGUSB,
  IX86_BUILTIN_PF2ID,
  IX86_BUILTIN_PFACC,
  IX86_BUILTIN_PFADD,
  IX86_BUILTIN_PFCMPEQ,
  IX86_BUILTIN_PFCMPGE,
  IX86_BUILTIN_PFCMPGT,
  IX86_BUILTIN_PFMAX,
  IX86_BUILTIN_PFMIN,
  IX86_BUILTIN_PFMUL,
  IX86_BUILTIN_PFRCP,
  IX86_BUILTIN_PFRCPIT1,
  IX86_BUILTIN_PFRCPIT2,
  IX86_BUILTIN_PFRSQIT1,
  IX86_BUILTIN_PFRSQRT,
  IX86_BUILTIN_PFSUB,
  IX86_BUILTIN_PFSUBR,
  IX86_BUILTIN_PI2FD,
  IX86_BUILTIN_PMULHRW,


  IX86_BUILTIN_PF2IW,
  IX86_BUILTIN_PFNACC,
  IX86_BUILTIN_PFPNACC,
  IX86_BUILTIN_PI2FW,
  IX86_BUILTIN_PSWAPDSI,
  IX86_BUILTIN_PSWAPDSF,

  IX86_BUILTIN_SSE_ZERO,
  IX86_BUILTIN_MMX_ZERO,

  IX86_BUILTIN_MAX
};
# 2818 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/i386.h"
extern int const dbx_register_map[53];
extern int const dbx64_register_map[53];
extern int const svr4_dbx_register_map[53];
# 3092 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/i386.h"
enum cmodel {
  CM_32,
  CM_SMALL,
  CM_KERNEL,
  CM_MEDIUM,
  CM_LARGE,
  CM_SMALL_PIC
};





extern const char *ix86_debug_arg_string, *ix86_debug_addr_string;

enum asm_dialect {
  ASM_ATT,
  ASM_INTEL
};
extern const char *ix86_asm_string;
extern enum asm_dialect ix86_asm_dialect;

extern const char *ix86_cmodel_string;
extern enum cmodel ix86_cmodel;


extern const char *ix86_cpu_string;
extern const char *ix86_arch_string;
extern const char *ix86_fpmath_string;
extern const char *ix86_regparm_string;
extern const char *ix86_align_loops_string;
extern const char *ix86_align_jumps_string;
extern const char *ix86_align_funcs_string;
extern const char *ix86_preferred_stack_boundary_string;
extern const char *ix86_branch_cost_string;
extern int ix86_regparm;
extern int ix86_preferred_stack_boundary;
extern int ix86_branch_cost;
extern enum reg_class const regclass_map[53];
extern rtx ix86_compare_op0;
extern rtx ix86_compare_op1;
# 3149 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/i386.h"
enum fp_cw_mode {FP_CW_STORED, FP_CW_UNINITIALIZED, FP_CW_ANY};
# 16 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/config.h" 2
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/att.h" 1
# 22 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/att.h"
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/unix.h" 1
# 23 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/att.h" 2
# 17 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/config.h" 2
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/dbxelf.h" 1
# 18 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/config.h" 2
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/elfos.h" 1
# 19 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/config.h" 2
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/svr4.h" 1
# 20 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/config.h" 2
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/linux.h" 1
# 21 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/config.h" 2
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/x86-64.h" 1
# 22 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/config.h" 2
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/linux64.h" 1
# 23 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/config.h" 2
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/defaults.h" 1
# 24 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/config.h" 2




# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/insn-constants.h" 1
# 29 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/config.h" 2
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/insn-flags.h" 1
# 599 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/insn-flags.h"
struct rtx_def;
extern struct rtx_def *gen_cmpdi_ccno_1_rex64 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_cmpdi_1_insn_rex64 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_cmpqi_ext_3_insn (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_cmpqi_ext_3_insn_rex64 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_x86_fnstsw_1 (struct rtx_def *);
extern struct rtx_def *gen_x86_sahf_1 (struct rtx_def *);
extern struct rtx_def *gen_popsi1 (struct rtx_def *);
extern struct rtx_def *gen_movsi_insv_1 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_pushdi2_rex64 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_popdi1 (struct rtx_def *);
extern struct rtx_def *gen_swapxf (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_swaptf (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_zero_extendhisi2_and (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_zero_extendsidi2_32 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_zero_extendsidi2_rex64 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_zero_extendhidi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_zero_extendqidi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_extendsidi2_rex64 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_extendhidi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_extendqidi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_extendhisi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_extendqihi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_extendqisi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_truncdfsf2_3 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_truncdfsf2_sse_only (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_fix_truncdi_nomemory (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_fix_truncdi_memory (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_fix_truncsfdi_sse (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_fix_truncdfdi_sse (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_fix_truncsi_nomemory (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_fix_truncsi_memory (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_fix_truncsfsi_sse (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_fix_truncdfsi_sse (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_fix_trunchi_nomemory (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_fix_trunchi_memory (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_x86_fnstcw_1 (struct rtx_def *);
extern struct rtx_def *gen_x86_fldcw_1 (struct rtx_def *);
extern struct rtx_def *gen_floathisf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_floathidf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_floathixf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_floathitf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_floatsixf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_floatsitf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_floatdixf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_floatditf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_addqi3_cc (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_addsi_1_zext (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_addqi_ext_1 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_subdi3_carry_rex64 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_subsi3_carry (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_subsi3_carry_zext (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_divqi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_udivqi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_divmodhi4 (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_udivmoddi4 (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_udivmodsi4 (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_testsi_1 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_andqi_ext_0 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_negsf2_memory (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_negsf2_ifs (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_negdf2_memory (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_negdf2_ifs (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_abssf2_memory (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_abssf2_ifs (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_absdf2_memory (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_absdf2_ifs (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_ashldi3_1 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_x86_shld_1 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_ashrdi3_63_rex64 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_ashrdi3_1 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_x86_shrd_1 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_ashrsi3_31 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_lshrdi3_1 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_setcc_2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_jump (struct rtx_def *);
extern struct rtx_def *gen_doloop_end_internal (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_blockage (void);
extern struct rtx_def *gen_return_internal (void);
extern struct rtx_def *gen_return_pop_internal (struct rtx_def *);
extern struct rtx_def *gen_return_indirect_internal (struct rtx_def *);
extern struct rtx_def *gen_nop (void);
extern struct rtx_def *gen_prologue_set_got (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_prologue_get_pc (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_eh_return_si (struct rtx_def *);
extern struct rtx_def *gen_eh_return_di (struct rtx_def *);
extern struct rtx_def *gen_leave (void);
extern struct rtx_def *gen_leave_rex64 (void);
extern struct rtx_def *gen_ffssi_1 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sqrtsf2_1 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sqrtsf2_1_sse_only (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sqrtsf2_i387 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sqrtdf2_1 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sqrtdf2_1_sse_only (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sqrtdf2_i387 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sqrtxf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sqrttf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sindf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sinsf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sinxf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sintf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_cosdf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_cossf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_cosxf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_costf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_cld (void);
extern struct rtx_def *gen_strmovdi_rex_1 (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_strmovsi_1 (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_strmovsi_rex_1 (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_strmovhi_1 (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_strmovhi_rex_1 (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_strmovqi_1 (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_strmovqi_rex_1 (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_rep_movdi_rex64 (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_rep_movsi (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_rep_movsi_rex64 (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_rep_movqi (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_rep_movqi_rex64 (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_strsetdi_rex_1 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_strsetsi_1 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_strsetsi_rex_1 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_strsethi_1 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_strsethi_rex_1 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_strsetqi_1 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_strsetqi_rex_1 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_rep_stosdi_rex64 (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_rep_stossi (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_rep_stossi_rex64 (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_rep_stosqi (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_rep_stosqi_rex64 (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_cmpstrqi_nz_1 (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_cmpstrqi_nz_rex_1 (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_cmpstrqi_1 (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_cmpstrqi_rex_1 (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_strlenqi_1 (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_strlenqi_rex_1 (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_x86_movdicc_0_m1_rex64 (struct rtx_def *);
extern struct rtx_def *gen_x86_movsicc_0_m1 (struct rtx_def *);
extern struct rtx_def *gen_pro_epilogue_adjust_stack_rex64 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sse_movsfcc (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sse_movsfcc_eq (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sse_movdfcc (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sse_movdfcc_eq (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_allocate_stack_worker_1 (struct rtx_def *);
extern struct rtx_def *gen_allocate_stack_worker_rex64 (struct rtx_def *);
extern struct rtx_def *gen_trap (void);
extern struct rtx_def *gen_movv4sf_internal (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movv4si_internal (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movv8qi_internal (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movv4hi_internal (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movv2si_internal (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movv2sf_internal (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movti_internal (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sse_movaps (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sse_movups (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sse_movmskps (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mmx_pmovmskb (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mmx_maskmovq (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mmx_maskmovq_rex (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sse_movntv4sf (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sse_movntdi (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sse_movhlps (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sse_movlhps (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sse_movhps (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sse_movlps (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sse_loadss (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sse_movss (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sse_storess (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sse_shufps (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_addv4sf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_vmaddv4sf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_subv4sf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_vmsubv4sf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mulv4sf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_vmmulv4sf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_divv4sf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_vmdivv4sf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_rcpv4sf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_vmrcpv4sf2 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_rsqrtv4sf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_vmrsqrtv4sf2 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sqrtv4sf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_vmsqrtv4sf2 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sse_andti3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sse_nandti3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sse_iorti3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sse_xorti3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sse_clrv4sf (struct rtx_def *);
extern struct rtx_def *gen_maskcmpv4sf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_maskncmpv4sf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_vmmaskcmpv4sf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_vmmaskncmpv4sf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sse_comi (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sse_ucomi (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sse_unpckhps (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sse_unpcklps (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_smaxv4sf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_vmsmaxv4sf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sminv4sf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_vmsminv4sf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_cvtpi2ps (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_cvtps2pi (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_cvttps2pi (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_cvtsi2ss (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_cvtss2si (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_cvttss2si (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_addv8qi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_addv4hi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_addv2si3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_ssaddv8qi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_ssaddv4hi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_usaddv8qi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_usaddv4hi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_subv8qi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_subv4hi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_subv2si3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sssubv8qi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sssubv4hi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_ussubv8qi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_ussubv4hi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mulv4hi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_smulv4hi3_highpart (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_umulv4hi3_highpart (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mmx_pmaddwd (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mmx_iordi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mmx_xordi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mmx_clrdi (struct rtx_def *);
extern struct rtx_def *gen_mmx_anddi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mmx_nanddi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mmx_uavgv8qi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mmx_uavgv4hi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mmx_psadbw (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mmx_pinsrw (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mmx_pextrw (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mmx_pshufw (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_eqv8qi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_eqv4hi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_eqv2si3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_gtv8qi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_gtv4hi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_gtv2si3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_umaxv8qi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_smaxv4hi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_uminv8qi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sminv4hi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_ashrv4hi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_ashrv2si3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_lshrv4hi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_lshrv2si3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mmx_lshrdi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_ashlv4hi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_ashlv2si3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mmx_ashldi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mmx_packsswb (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mmx_packssdw (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mmx_packuswb (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mmx_punpckhbw (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mmx_punpckhwd (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mmx_punpckhdq (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mmx_punpcklbw (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mmx_punpcklwd (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mmx_punpckldq (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_emms (void);
extern struct rtx_def *gen_ldmxcsr (struct rtx_def *);
extern struct rtx_def *gen_stmxcsr (struct rtx_def *);
extern struct rtx_def *gen_addv2sf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_subv2sf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_subrv2sf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_gtv2sf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_gev2sf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_eqv2sf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_pfmaxv2sf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_pfminv2sf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mulv2sf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_femms (void);
extern struct rtx_def *gen_pf2id (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_pf2iw (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_pfacc (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_pfnacc (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_pfpnacc (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_pi2fw (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_floatv2si2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_pavgusb (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_pfrcpv2sf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_pfrcpit1v2sf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_pfrcpit2v2sf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_pfrsqrtv2sf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_pfrsqit1v2sf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_pmulhrwv4hi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_pswapdv2si2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_pswapdv2sf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_cmpdi (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_cmpsi (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_cmphi (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_cmpqi (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_cmpdi_1_rex64 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_cmpsi_1 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_cmpqi_ext_3 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_cmpxf (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_cmptf (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_cmpdf (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_cmpsf (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movsi (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movhi (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movstricthi (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movqi (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_reload_outqi (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movstrictqi (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movdi (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movsf (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movdf (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movxf (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movtf (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_zero_extendhisi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_zero_extendqihi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_zero_extendqisi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_zero_extendsidi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_extendsidi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_extendsfdf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_extendsfxf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_extendsftf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_extenddfxf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_extenddftf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_truncdfsf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_truncxfsf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_trunctfsf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_truncxfdf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_trunctfdf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_fix_truncxfdi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_fix_trunctfdi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_fix_truncdfdi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_fix_truncsfdi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_fix_truncxfsi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_fix_trunctfsi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_fix_truncdfsi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_fix_truncsfsi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_fix_truncxfhi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_fix_trunctfhi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_fix_truncdfhi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_fix_truncsfhi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_floatsisf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_floatdisf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_floatsidf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_floatdidf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_adddi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_addsi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_addhi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_addqi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_addxf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_addtf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_adddf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_addsf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_subdi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_subsi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_subhi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_subqi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_subxf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_subtf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_subdf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_subsf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_muldi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mulsi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mulhi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mulqi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_umulqihi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mulqihi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_umulditi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_umulsidi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mulditi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mulsidi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_umuldi3_highpart (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_umulsi3_highpart (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_smuldi3_highpart (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_smulsi3_highpart (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mulxf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_multf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_muldf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mulsf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_divxf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_divtf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_divdf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_divsf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_divmoddi4 (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_divmodsi4 (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_udivmodhi4 (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_testsi_ccno_1 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_testqi_ccz_1 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_testqi_ext_ccno_0 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_anddi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_andsi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_andhi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_andqi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_iordi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_iorsi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_iorhi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_iorqi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_xordi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_xorsi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_xorhi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_xorqi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_xorqi_cc_ext_1 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_negdi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_negsi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_neghi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_negqi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_negsf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_negdf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_negxf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_negtf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_abssf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_absdf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_absxf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_abstf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_one_cmpldi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_one_cmplsi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_one_cmplhi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_one_cmplqi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_ashldi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_x86_shift_adj_1 (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_x86_shift_adj_2 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_ashlsi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_ashlhi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_ashlqi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_ashrdi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_x86_shift_adj_3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_ashrsi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_ashrhi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_ashrqi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_lshrdi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_lshrsi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_lshrhi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_lshrqi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_rotldi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_rotlsi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_rotlhi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_rotlqi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_rotrdi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_rotrsi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_rotrhi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_rotrqi3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_extv (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_extzv (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_insv (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_seq (struct rtx_def *);
extern struct rtx_def *gen_sne (struct rtx_def *);
extern struct rtx_def *gen_sgt (struct rtx_def *);
extern struct rtx_def *gen_sgtu (struct rtx_def *);
extern struct rtx_def *gen_slt (struct rtx_def *);
extern struct rtx_def *gen_sltu (struct rtx_def *);
extern struct rtx_def *gen_sge (struct rtx_def *);
extern struct rtx_def *gen_sgeu (struct rtx_def *);
extern struct rtx_def *gen_sle (struct rtx_def *);
extern struct rtx_def *gen_sleu (struct rtx_def *);
extern struct rtx_def *gen_sunordered (struct rtx_def *);
extern struct rtx_def *gen_sordered (struct rtx_def *);
extern struct rtx_def *gen_suneq (struct rtx_def *);
extern struct rtx_def *gen_sunge (struct rtx_def *);
extern struct rtx_def *gen_sungt (struct rtx_def *);
extern struct rtx_def *gen_sunle (struct rtx_def *);
extern struct rtx_def *gen_sunlt (struct rtx_def *);
extern struct rtx_def *gen_sltgt (struct rtx_def *);
extern struct rtx_def *gen_beq (struct rtx_def *);
extern struct rtx_def *gen_bne (struct rtx_def *);
extern struct rtx_def *gen_bgt (struct rtx_def *);
extern struct rtx_def *gen_bgtu (struct rtx_def *);
extern struct rtx_def *gen_blt (struct rtx_def *);
extern struct rtx_def *gen_bltu (struct rtx_def *);
extern struct rtx_def *gen_bge (struct rtx_def *);
extern struct rtx_def *gen_bgeu (struct rtx_def *);
extern struct rtx_def *gen_ble (struct rtx_def *);
extern struct rtx_def *gen_bleu (struct rtx_def *);
extern struct rtx_def *gen_bunordered (struct rtx_def *);
extern struct rtx_def *gen_bordered (struct rtx_def *);
extern struct rtx_def *gen_buneq (struct rtx_def *);
extern struct rtx_def *gen_bunge (struct rtx_def *);
extern struct rtx_def *gen_bungt (struct rtx_def *);
extern struct rtx_def *gen_bunle (struct rtx_def *);
extern struct rtx_def *gen_bunlt (struct rtx_def *);
extern struct rtx_def *gen_bltgt (struct rtx_def *);
extern struct rtx_def *gen_indirect_jump (struct rtx_def *);
extern struct rtx_def *gen_tablejump (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_doloop_end (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);

extern struct rtx_def *gen_call_pop (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);

extern struct rtx_def *gen_call (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_call_exp (struct rtx_def *, struct rtx_def *);

extern struct rtx_def *gen_call_value_pop (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);

extern struct rtx_def *gen_call_value (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_call_value_exp (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_untyped_call (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_return (void);
extern struct rtx_def *gen_prologue (void);
extern struct rtx_def *gen_epilogue (void);
extern struct rtx_def *gen_sibcall_epilogue (void);
extern struct rtx_def *gen_eh_return (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_ffssi2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sqrtsf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sqrtdf2 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movstrsi (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movstrdi (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_strmovdi_rex64 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_strmovsi (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_strmovsi_rex64 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_strmovhi (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_strmovhi_rex64 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_strmovqi (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_strmovqi_rex64 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_clrstrsi (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_clrstrdi (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_strsetdi_rex64 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_strsetsi (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_strsetsi_rex64 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_strsethi (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_strsethi_rex64 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_strsetqi (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_strsetqi_rex64 (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_cmpstrsi (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_cmpintqi (struct rtx_def *);
extern struct rtx_def *gen_strlensi (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_strlendi (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movdicc (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movsicc (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movhicc (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movsfcc (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movdfcc (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movxfcc (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movtfcc (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_minsf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_mindf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_maxsf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_maxdf3 (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_pro_epilogue_adjust_stack (struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_allocate_stack_worker (struct rtx_def *);
extern struct rtx_def *gen_allocate_stack (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_builtin_setjmp_receiver (struct rtx_def *);
extern struct rtx_def *gen_conditional_trap (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movti (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movv4sf (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movv4si (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movv2si (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movv4hi (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movv8qi (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_movv2sf (struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_sfence (void);
extern struct rtx_def *gen_sse_prologue_save (struct rtx_def *, struct rtx_def *, struct rtx_def *, struct rtx_def *);
extern struct rtx_def *gen_prefetch (struct rtx_def *, struct rtx_def *, struct rtx_def *);
# 30 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/config.h" 2
# 78 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c" 2
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/system.h" 1
# 33 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/system.h"
# 1 "/Users/ddunbar/rt/10907846/clang-211.10.1-Slate/Developer/usr/bin/../lib/clang/3.0/include/stdarg.h" 1 3 4
# 30 "/Users/ddunbar/rt/10907846/clang-211.10.1-Slate/Developer/usr/bin/../lib/clang/3.0/include/stdarg.h" 3 4
typedef __builtin_va_list va_list;
# 48 "/Users/ddunbar/rt/10907846/clang-211.10.1-Slate/Developer/usr/bin/../lib/clang/3.0/include/stdarg.h" 3 4
typedef __builtin_va_list __gnuc_va_list;
# 34 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/system.h" 2
# 47 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/system.h"
# 1 "/Users/ddunbar/rt/10907846/clang-211.10.1-Slate/Developer/usr/bin/../lib/clang/3.0/include/stddef.h" 1 3 4
# 31 "/Users/ddunbar/rt/10907846/clang-211.10.1-Slate/Developer/usr/bin/../lib/clang/3.0/include/stddef.h" 3 4
typedef __typeof__(((int*)0)-((int*)0)) ptrdiff_t;



typedef __typeof__(sizeof(int)) size_t;




typedef int wchar_t;
# 48 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/system.h" 2


# 1 "/usr/include/stdio.h" 1 3 4
# 64 "/usr/include/stdio.h" 3 4
# 1 "/usr/include/sys/cdefs.h" 1 3 4
# 417 "/usr/include/sys/cdefs.h" 3 4
# 1 "/usr/include/sys/_symbol_aliasing.h" 1 3 4
# 418 "/usr/include/sys/cdefs.h" 2 3 4
# 494 "/usr/include/sys/cdefs.h" 3 4
# 1 "/usr/include/sys/_posix_availability.h" 1 3 4
# 495 "/usr/include/sys/cdefs.h" 2 3 4
# 65 "/usr/include/stdio.h" 2 3 4
# 1 "/usr/include/Availability.h" 1 3 4
# 141 "/usr/include/Availability.h" 3 4
# 1 "/usr/include/AvailabilityInternal.h" 1 3 4
# 142 "/usr/include/Availability.h" 2 3 4
# 66 "/usr/include/stdio.h" 2 3 4

# 1 "/usr/include/_types.h" 1 3 4
# 27 "/usr/include/_types.h" 3 4
# 1 "/usr/include/sys/_types.h" 1 3 4
# 33 "/usr/include/sys/_types.h" 3 4
# 1 "/usr/include/machine/_types.h" 1 3 4
# 32 "/usr/include/machine/_types.h" 3 4
# 1 "/usr/include/i386/_types.h" 1 3 4
# 37 "/usr/include/i386/_types.h" 3 4
typedef signed char __int8_t;



typedef unsigned char __uint8_t;
typedef short __int16_t;
typedef unsigned short __uint16_t;
typedef int __int32_t;
typedef unsigned int __uint32_t;
typedef long long __int64_t;
typedef unsigned long long __uint64_t;

typedef long __darwin_intptr_t;
typedef unsigned int __darwin_natural_t;
# 70 "/usr/include/i386/_types.h" 3 4
typedef int __darwin_ct_rune_t;





typedef union {
 char __mbstate8[128];
 long long _mbstateL;
} __mbstate_t;

typedef __mbstate_t __darwin_mbstate_t;


typedef long int __darwin_ptrdiff_t;





typedef long unsigned int __darwin_size_t;





typedef __builtin_va_list __darwin_va_list;





typedef int __darwin_wchar_t;




typedef __darwin_wchar_t __darwin_rune_t;


typedef int __darwin_wint_t;




typedef unsigned long __darwin_clock_t;
typedef __uint32_t __darwin_socklen_t;
typedef long __darwin_ssize_t;
typedef long __darwin_time_t;
# 33 "/usr/include/machine/_types.h" 2 3 4
# 34 "/usr/include/sys/_types.h" 2 3 4
# 58 "/usr/include/sys/_types.h" 3 4
struct __darwin_pthread_handler_rec
{
 void (*__routine)(void *);
 void *__arg;
 struct __darwin_pthread_handler_rec *__next;
};
struct _opaque_pthread_attr_t { long __sig; char __opaque[56]; };
struct _opaque_pthread_cond_t { long __sig; char __opaque[40]; };
struct _opaque_pthread_condattr_t { long __sig; char __opaque[8]; };
struct _opaque_pthread_mutex_t { long __sig; char __opaque[56]; };
struct _opaque_pthread_mutexattr_t { long __sig; char __opaque[8]; };
struct _opaque_pthread_once_t { long __sig; char __opaque[8]; };
struct _opaque_pthread_rwlock_t { long __sig; char __opaque[192]; };
struct _opaque_pthread_rwlockattr_t { long __sig; char __opaque[16]; };
struct _opaque_pthread_t { long __sig; struct __darwin_pthread_handler_rec *__cleanup_stack; char __opaque[1168]; };
# 94 "/usr/include/sys/_types.h" 3 4
typedef __int64_t __darwin_blkcnt_t;
typedef __int32_t __darwin_blksize_t;
typedef __int32_t __darwin_dev_t;
typedef unsigned int __darwin_fsblkcnt_t;
typedef unsigned int __darwin_fsfilcnt_t;
typedef __uint32_t __darwin_gid_t;
typedef __uint32_t __darwin_id_t;
typedef __uint64_t __darwin_ino64_t;

typedef __darwin_ino64_t __darwin_ino_t;



typedef __darwin_natural_t __darwin_mach_port_name_t;
typedef __darwin_mach_port_name_t __darwin_mach_port_t;
typedef __uint16_t __darwin_mode_t;
typedef __int64_t __darwin_off_t;
typedef __int32_t __darwin_pid_t;
typedef struct _opaque_pthread_attr_t
   __darwin_pthread_attr_t;
typedef struct _opaque_pthread_cond_t
   __darwin_pthread_cond_t;
typedef struct _opaque_pthread_condattr_t
   __darwin_pthread_condattr_t;
typedef unsigned long __darwin_pthread_key_t;
typedef struct _opaque_pthread_mutex_t
   __darwin_pthread_mutex_t;
typedef struct _opaque_pthread_mutexattr_t
   __darwin_pthread_mutexattr_t;
typedef struct _opaque_pthread_once_t
   __darwin_pthread_once_t;
typedef struct _opaque_pthread_rwlock_t
   __darwin_pthread_rwlock_t;
typedef struct _opaque_pthread_rwlockattr_t
   __darwin_pthread_rwlockattr_t;
typedef struct _opaque_pthread_t
   *__darwin_pthread_t;
typedef __uint32_t __darwin_sigset_t;
typedef __int32_t __darwin_suseconds_t;
typedef __uint32_t __darwin_uid_t;
typedef __uint32_t __darwin_useconds_t;
typedef unsigned char __darwin_uuid_t[16];
typedef char __darwin_uuid_string_t[37];
# 28 "/usr/include/_types.h" 2 3 4
# 39 "/usr/include/_types.h" 3 4
typedef int __darwin_nl_item;
typedef int __darwin_wctrans_t;

typedef __uint32_t __darwin_wctype_t;
# 68 "/usr/include/stdio.h" 2 3 4
# 85 "/usr/include/stdio.h" 3 4
typedef __darwin_off_t fpos_t;
# 96 "/usr/include/stdio.h" 3 4
struct __sbuf {
 unsigned char *_base;
 int _size;
};


struct __sFILEX;
# 130 "/usr/include/stdio.h" 3 4
typedef struct __sFILE {
 unsigned char *_p;
 int _r;
 int _w;
 short _flags;
 short _file;
 struct __sbuf _bf;
 int _lbfsize;


 void *_cookie;
 int (*_close)(void *);
 int (*_read) (void *, char *, int);
 fpos_t (*_seek) (void *, fpos_t, int);
 int (*_write)(void *, const char *, int);


 struct __sbuf _ub;
 struct __sFILEX *_extra;
 int _ur;


 unsigned char _ubuf[3];
 unsigned char _nbuf[1];


 struct __sbuf _lb;


 int _blksize;
 fpos_t _offset;
} FILE;


extern FILE *__stdinp;
extern FILE *__stdoutp;
extern FILE *__stderrp;
# 239 "/usr/include/stdio.h" 3 4
void clearerr(FILE *);
int fclose(FILE *);
int feof(FILE *);
int ferror(FILE *);
int fflush(FILE *);
int fgetc(FILE *);
int fgetpos(FILE * restrict, fpos_t *);
char *fgets(char * restrict, int, FILE *);



FILE *fopen(const char * restrict, const char * restrict) __asm("_" "fopen" );

int fprintf(FILE * restrict, const char * restrict, ...) __attribute__((__format__ (__printf__, 2, 3)));
int fputc(int, FILE *);
int fputs(const char * restrict, FILE * restrict) __asm("_" "fputs" );
size_t fread(void * restrict, size_t, size_t, FILE * restrict);
FILE *freopen(const char * restrict, const char * restrict,
                 FILE * restrict) __asm("_" "freopen" );
int fscanf(FILE * restrict, const char * restrict, ...) __attribute__((__format__ (__scanf__, 2, 3)));
int fseek(FILE *, long, int);
int fsetpos(FILE *, const fpos_t *);
long ftell(FILE *);
size_t fwrite(const void * restrict, size_t, size_t, FILE * restrict) __asm("_" "fwrite" );
int getc(FILE *);
int getchar(void);
char *gets(char *);
void perror(const char *);
int printf(const char * restrict, ...) __attribute__((__format__ (__printf__, 1, 2)));
int putc(int, FILE *);
int putchar(int);
int puts(const char *);
int remove(const char *);
int rename (const char *, const char *);
void rewind(FILE *);
int scanf(const char * restrict, ...) __attribute__((__format__ (__scanf__, 1, 2)));
void setbuf(FILE * restrict, char * restrict);
int setvbuf(FILE * restrict, char * restrict, int, size_t);
int sprintf(char * restrict, const char * restrict, ...) __attribute__((__format__ (__printf__, 2, 3)));
int sscanf(const char * restrict, const char * restrict, ...) __attribute__((__format__ (__scanf__, 2, 3)));
FILE *tmpfile(void);
char *tmpnam(char *);
int ungetc(int, FILE *);
int vfprintf(FILE * restrict, const char * restrict, va_list) __attribute__((__format__ (__printf__, 2, 0)));
int vprintf(const char * restrict, va_list) __attribute__((__format__ (__printf__, 1, 0)));
int vsprintf(char * restrict, const char * restrict, va_list) __attribute__((__format__ (__printf__, 2, 0)));
# 300 "/usr/include/stdio.h" 3 4
char *ctermid(char *);





FILE *fdopen(int, const char *) __asm("_" "fdopen" );

int fileno(FILE *);
# 319 "/usr/include/stdio.h" 3 4
int pclose(FILE *);



FILE *popen(const char *, const char *) __asm("_" "popen" );
# 341 "/usr/include/stdio.h" 3 4
int __srget(FILE *);
int __svfscanf(FILE *, const char *, va_list) __attribute__((__format__ (__scanf__, 2, 0)));
int __swbuf(int, FILE *);
# 352 "/usr/include/stdio.h" 3 4
static __inline int __sputc(int _c, FILE *_p) {
 if (--_p->_w >= 0 || (_p->_w >= _p->_lbfsize && (char)_c != '\n'))
  return (*_p->_p++ = _c);
 else
  return (__swbuf(_c, _p));
}
# 378 "/usr/include/stdio.h" 3 4
void flockfile(FILE *);
int ftrylockfile(FILE *);
void funlockfile(FILE *);
int getc_unlocked(FILE *);
int getchar_unlocked(void);
int putc_unlocked(int, FILE *);
int putchar_unlocked(int);



int getw(FILE *);
int putw(int, FILE *);


char *tempnam(const char *, const char *) __asm("_" "tempnam" );
# 414 "/usr/include/stdio.h" 3 4
typedef __darwin_off_t off_t;



int fseeko(FILE *, off_t, int);
off_t ftello(FILE *);





int snprintf(char * restrict, size_t, const char * restrict, ...) __attribute__((__format__ (__printf__, 3, 4)));
int vfscanf(FILE * restrict, const char * restrict, va_list) __attribute__((__format__ (__scanf__, 2, 0)));
int vscanf(const char * restrict, va_list) __attribute__((__format__ (__scanf__, 1, 0)));
int vsnprintf(char * restrict, size_t, const char * restrict, va_list) __attribute__((__format__ (__printf__, 3, 0)));
int vsscanf(const char * restrict, const char * restrict, va_list) __attribute__((__format__ (__scanf__, 2, 0)));
# 442 "/usr/include/stdio.h" 3 4
typedef __darwin_ssize_t ssize_t;



int dprintf(int, const char * restrict, ...) __attribute__((__format__ (__printf__, 2, 3))) __attribute__((visibility("default")));
int vdprintf(int, const char * restrict, va_list) __attribute__((__format__ (__printf__, 2, 0))) __attribute__((visibility("default")));
ssize_t getdelim(char ** restrict, size_t * restrict, int, FILE * restrict) __attribute__((visibility("default")));
ssize_t getline(char ** restrict, size_t * restrict, FILE * restrict) __attribute__((visibility("default")));
# 459 "/usr/include/stdio.h" 3 4
extern const int sys_nerr;
extern const char *const sys_errlist[];

int asprintf(char **, const char *, ...) __attribute__((__format__ (__printf__, 2, 3)));
char *ctermid_r(char *);
char *fgetln(FILE *, size_t *);
const char *fmtcheck(const char *, const char *);
int fpurge(FILE *);
void setbuffer(FILE *, char *, int);
int setlinebuf(FILE *);
int vasprintf(char **, const char *, va_list) __attribute__((__format__ (__printf__, 2, 0)));
FILE *zopen(const char *, const char *, int);





FILE *funopen(const void *,
                 int (*)(void *, char *, int),
                 int (*)(void *, const char *, int),
                 fpos_t (*)(void *, fpos_t, int),
                 int (*)(void *));
# 499 "/usr/include/stdio.h" 3 4
# 1 "/usr/include/secure/_stdio.h" 1 3 4
# 31 "/usr/include/secure/_stdio.h" 3 4
# 1 "/usr/include/secure/_common.h" 1 3 4
# 32 "/usr/include/secure/_stdio.h" 2 3 4
# 45 "/usr/include/secure/_stdio.h" 3 4
extern int __sprintf_chk (char * restrict, int, size_t,
     const char * restrict, ...)
                                      ;




extern int __snprintf_chk (char * restrict, size_t, int, size_t,
      const char * restrict, ...)
                                       ;





extern int __vsprintf_chk (char * restrict, int, size_t,
      const char * restrict, va_list)
                                       ;




extern int __vsnprintf_chk (char * restrict, size_t, int, size_t,
       const char * restrict, va_list)
                                        ;
# 500 "/usr/include/stdio.h" 2 3 4
# 51 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/system.h" 2
# 110 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/system.h"
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/safe-ctype.h" 1
# 44 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/safe-ctype.h"
enum {

  _sch_isblank = 0x0001,
  _sch_iscntrl = 0x0002,
  _sch_isdigit = 0x0004,
  _sch_islower = 0x0008,
  _sch_isprint = 0x0010,
  _sch_ispunct = 0x0020,
  _sch_isspace = 0x0040,
  _sch_isupper = 0x0080,
  _sch_isxdigit = 0x0100,


  _sch_isidst = 0x0200,
  _sch_isvsp = 0x0400,
  _sch_isnvsp = 0x0800,


  _sch_isalpha = _sch_isupper|_sch_islower,
  _sch_isalnum = _sch_isalpha|_sch_isdigit,
  _sch_isidnum = _sch_isidst|_sch_isdigit,
  _sch_isgraph = _sch_isalnum|_sch_ispunct,
  _sch_iscppsp = _sch_isvsp|_sch_isnvsp,
  _sch_isbasic = _sch_isprint|_sch_iscppsp

};


extern const unsigned short _sch_istable[256];
# 97 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/safe-ctype.h"
extern const unsigned char _sch_toupper[256];
extern const unsigned char _sch_tolower[256];
# 111 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/system.h" 2

# 1 "/usr/include/sys/types.h" 1 3 4
# 72 "/usr/include/sys/types.h" 3 4
# 1 "/usr/include/sys/appleapiopts.h" 1 3 4
# 73 "/usr/include/sys/types.h" 2 3 4





# 1 "/usr/include/machine/types.h" 1 3 4
# 35 "/usr/include/machine/types.h" 3 4
# 1 "/usr/include/i386/types.h" 1 3 4
# 78 "/usr/include/i386/types.h" 3 4
typedef signed char int8_t;

typedef unsigned char u_int8_t;


typedef short int16_t;

typedef unsigned short u_int16_t;


typedef int int32_t;

typedef unsigned int u_int32_t;


typedef long long int64_t;

typedef unsigned long long u_int64_t;


typedef int64_t register_t;






typedef __darwin_intptr_t intptr_t;



typedef unsigned long uintptr_t;




typedef u_int64_t user_addr_t;
typedef u_int64_t user_size_t;
typedef int64_t user_ssize_t;
typedef int64_t user_long_t;
typedef u_int64_t user_ulong_t;
typedef int64_t user_time_t;
typedef int64_t user_off_t;







typedef u_int64_t syscall_arg_t;
# 36 "/usr/include/machine/types.h" 2 3 4
# 79 "/usr/include/sys/types.h" 2 3 4


# 1 "/usr/include/machine/endian.h" 1 3 4
# 35 "/usr/include/machine/endian.h" 3 4
# 1 "/usr/include/i386/endian.h" 1 3 4
# 99 "/usr/include/i386/endian.h" 3 4
# 1 "/usr/include/sys/_endian.h" 1 3 4
# 124 "/usr/include/sys/_endian.h" 3 4
# 1 "/usr/include/libkern/_OSByteOrder.h" 1 3 4
# 66 "/usr/include/libkern/_OSByteOrder.h" 3 4
# 1 "/usr/include/libkern/i386/_OSByteOrder.h" 1 3 4
# 44 "/usr/include/libkern/i386/_OSByteOrder.h" 3 4
static __inline__
__uint16_t
_OSSwapInt16(
    __uint16_t _data
)
{
    return ((_data << 8) | (_data >> 8));
}

static __inline__
__uint32_t
_OSSwapInt32(
    __uint32_t _data
)
{

    return __builtin_bswap32(_data);




}


static __inline__
__uint64_t
_OSSwapInt64(
    __uint64_t _data
)
{
    return __builtin_bswap64(_data);
}
# 67 "/usr/include/libkern/_OSByteOrder.h" 2 3 4
# 125 "/usr/include/sys/_endian.h" 2 3 4
# 100 "/usr/include/i386/endian.h" 2 3 4
# 36 "/usr/include/machine/endian.h" 2 3 4
# 82 "/usr/include/sys/types.h" 2 3 4


typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;

typedef unsigned long u_long;


typedef unsigned short ushort;
typedef unsigned int uint;


typedef u_int64_t u_quad_t;
typedef int64_t quad_t;
typedef quad_t * qaddr_t;

typedef char * caddr_t;
typedef int32_t daddr_t;


typedef __darwin_dev_t dev_t;



typedef u_int32_t fixpt_t;


typedef __darwin_blkcnt_t blkcnt_t;




typedef __darwin_blksize_t blksize_t;




typedef __darwin_gid_t gid_t;





typedef __uint32_t in_addr_t;




typedef __uint16_t in_port_t;



typedef __darwin_ino_t ino_t;





typedef __darwin_ino64_t ino64_t;






typedef __int32_t key_t;



typedef __darwin_mode_t mode_t;




typedef __uint16_t nlink_t;





typedef __darwin_id_t id_t;



typedef __darwin_pid_t pid_t;
# 176 "/usr/include/sys/types.h" 3 4
typedef int32_t segsz_t;
typedef int32_t swblk_t;


typedef __darwin_uid_t uid_t;
# 223 "/usr/include/sys/types.h" 3 4
typedef __darwin_clock_t clock_t;
# 240 "/usr/include/sys/types.h" 3 4
typedef __darwin_time_t time_t;




typedef __darwin_useconds_t useconds_t;




typedef __darwin_suseconds_t suseconds_t;
# 260 "/usr/include/sys/types.h" 3 4
# 1 "/usr/include/sys/_structs.h" 1 3 4
# 184 "/usr/include/sys/_structs.h" 3 4
typedef struct fd_set {
 __int32_t fds_bits[((((1024) % ((sizeof(__int32_t) * 8))) == 0) ? ((1024) / ((sizeof(__int32_t) * 8))) : (((1024) / ((sizeof(__int32_t) * 8))) + 1))];
} fd_set;



static __inline int
__darwin_fd_isset(int _n, const struct fd_set *_p)
{
 return (_p->fds_bits[_n/(sizeof(__int32_t) * 8)] & (1<<(_n % (sizeof(__int32_t) * 8))));
}
# 261 "/usr/include/sys/types.h" 2 3 4




typedef __int32_t fd_mask;
# 318 "/usr/include/sys/types.h" 3 4
typedef __darwin_pthread_attr_t pthread_attr_t;



typedef __darwin_pthread_cond_t pthread_cond_t;



typedef __darwin_pthread_condattr_t pthread_condattr_t;



typedef __darwin_pthread_mutex_t pthread_mutex_t;



typedef __darwin_pthread_mutexattr_t pthread_mutexattr_t;



typedef __darwin_pthread_once_t pthread_once_t;



typedef __darwin_pthread_rwlock_t pthread_rwlock_t;



typedef __darwin_pthread_rwlockattr_t pthread_rwlockattr_t;



typedef __darwin_pthread_t pthread_t;






typedef __darwin_pthread_key_t pthread_key_t;





typedef __darwin_fsblkcnt_t fsblkcnt_t;




typedef __darwin_fsfilcnt_t fsfilcnt_t;
# 113 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/system.h" 2

# 1 "/usr/include/errno.h" 1 3 4
# 23 "/usr/include/errno.h" 3 4
# 1 "/usr/include/sys/errno.h" 1 3 4
# 75 "/usr/include/sys/errno.h" 3 4
extern int * __error(void);
# 24 "/usr/include/errno.h" 2 3 4
# 115 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/system.h" 2
# 125 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/system.h"
# 1 "/usr/include/string.h" 1 3 4
# 80 "/usr/include/string.h" 3 4
void *memchr(const void *, int, size_t);
int memcmp(const void *, const void *, size_t);
void *memcpy(void *, const void *, size_t);
void *memmove(void *, const void *, size_t);
void *memset(void *, int, size_t);
char *strcat(char *, const char *);
char *strchr(const char *, int);
int strcmp(const char *, const char *);
int strcoll(const char *, const char *);
char *strcpy(char *, const char *);
size_t strcspn(const char *, const char *);
char *strerror(int) __asm("_" "strerror" );
size_t strlen(const char *);
char *strncat(char *, const char *, size_t);
int strncmp(const char *, const char *, size_t);
char *strncpy(char *, const char *, size_t);
char *strpbrk(const char *, const char *);
char *strrchr(const char *, int);
size_t strspn(const char *, const char *);
char *strstr(const char *, const char *);
char *strtok(char *, const char *);
size_t strxfrm(char *, const char *, size_t);
# 114 "/usr/include/string.h" 3 4
char *strtok_r(char *, const char *, char **);
# 126 "/usr/include/string.h" 3 4
int strerror_r(int, char *, size_t);
char *strdup(const char *);
void *memccpy(void *, const void *, int, size_t);
# 140 "/usr/include/string.h" 3 4
char *stpcpy(char *, const char *);
char *stpncpy(char *, const char *, size_t) __attribute__((visibility("default")));
char *strndup(const char *, size_t) __attribute__((visibility("default")));
size_t strnlen(const char *, size_t) __attribute__((visibility("default")));
char *strsignal(int sig);
# 159 "/usr/include/string.h" 3 4
void *memmem(const void *, size_t, const void *, size_t) __attribute__((visibility("default")));
void memset_pattern4(void *, const void *, size_t) __attribute__((visibility("default")));
void memset_pattern8(void *, const void *, size_t) __attribute__((visibility("default")));
void memset_pattern16(void *, const void *, size_t) __attribute__((visibility("default")));

char *strcasestr(const char *, const char *);
char *strnstr(const char *, const char *, size_t);
size_t strlcat(char *, const char *, size_t);
size_t strlcpy(char *, const char *, size_t);
void strmode(int, char *);
char *strsep(char **, const char *);


void swab(const void * restrict, void * restrict, ssize_t);








# 1 "/usr/include/strings.h" 1 3 4
# 74 "/usr/include/strings.h" 3 4
int bcmp(const void *, const void *, size_t) ;
void bcopy(const void *, void *, size_t) ;
void bzero(void *, size_t) ;
char *index(const char *, int) ;
char *rindex(const char *, int) ;


int ffs(int);
int strcasecmp(const char *, const char *);
int strncasecmp(const char *, const char *, size_t);





int ffsl(long) __attribute__((visibility("default")));
int fls(int) __attribute__((visibility("default")));
int flsl(long) __attribute__((visibility("default")));



# 1 "/usr/include/string.h" 1 3 4
# 95 "/usr/include/strings.h" 2 3 4
# 181 "/usr/include/string.h" 2 3 4
# 190 "/usr/include/string.h" 3 4
# 1 "/usr/include/secure/_string.h" 1 3 4
# 58 "/usr/include/secure/_string.h" 3 4
static __inline void *
__inline_memcpy_chk (void *__dest, const void *__src, size_t __len)
{
  return __builtin___memcpy_chk (__dest, __src, __len, __builtin_object_size (__dest, 0));
}






static __inline void *
__inline_memmove_chk (void *__dest, const void *__src, size_t __len)
{
  return __builtin___memmove_chk (__dest, __src, __len, __builtin_object_size (__dest, 0));
}






static __inline void *
__inline_memset_chk (void *__dest, int __val, size_t __len)
{
  return __builtin___memset_chk (__dest, __val, __len, __builtin_object_size (__dest, 0));
}






static __inline char *
__inline_strcpy_chk (char *restrict __dest, const char *restrict __src)
{
  return __builtin___strcpy_chk (__dest, __src, __builtin_object_size (__dest, 2 > 1));
}







static __inline char *
__inline_stpcpy_chk (char *__dest, const char *__src)
{
  return __builtin___stpcpy_chk (__dest, __src, __builtin_object_size (__dest, 2 > 1));
}






static __inline char *
__inline_stpncpy_chk (char *restrict __dest, const char *restrict __src,
        size_t __len)
{
  return __builtin___stpncpy_chk (__dest, __src, __len, __builtin_object_size (__dest, 2 > 1));
}







static __inline char *
__inline_strncpy_chk (char *restrict __dest, const char *restrict __src,
        size_t __len)
{
  return __builtin___strncpy_chk (__dest, __src, __len, __builtin_object_size (__dest, 2 > 1));
}






static __inline char *
__inline_strcat_chk (char *restrict __dest, const char *restrict __src)
{
  return __builtin___strcat_chk (__dest, __src, __builtin_object_size (__dest, 2 > 1));
}







static __inline char *
__inline_strncat_chk (char *restrict __dest, const char *restrict __src,
        size_t __len)
{
  return __builtin___strncat_chk (__dest, __src, __len, __builtin_object_size (__dest, 2 > 1));
}
# 191 "/usr/include/string.h" 2 3 4
# 126 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/system.h" 2








# 1 "/usr/include/stdlib.h" 1 3 4
# 65 "/usr/include/stdlib.h" 3 4
# 1 "/usr/include/sys/wait.h" 1 3 4
# 79 "/usr/include/sys/wait.h" 3 4
typedef enum {
 P_ALL,
 P_PID,
 P_PGID
} idtype_t;
# 116 "/usr/include/sys/wait.h" 3 4
# 1 "/usr/include/sys/signal.h" 1 3 4
# 81 "/usr/include/sys/signal.h" 3 4
# 1 "/usr/include/machine/signal.h" 1 3 4
# 32 "/usr/include/machine/signal.h" 3 4
# 1 "/usr/include/i386/signal.h" 1 3 4
# 39 "/usr/include/i386/signal.h" 3 4
typedef int sig_atomic_t;
# 55 "/usr/include/i386/signal.h" 3 4
# 1 "/usr/include/i386/_structs.h" 1 3 4
# 56 "/usr/include/i386/signal.h" 2 3 4
# 33 "/usr/include/machine/signal.h" 2 3 4
# 82 "/usr/include/sys/signal.h" 2 3 4
# 148 "/usr/include/sys/signal.h" 3 4
# 1 "/usr/include/sys/_structs.h" 1 3 4
# 57 "/usr/include/sys/_structs.h" 3 4
# 1 "/usr/include/machine/_structs.h" 1 3 4
# 29 "/usr/include/machine/_structs.h" 3 4
# 1 "/usr/include/i386/_structs.h" 1 3 4
# 38 "/usr/include/i386/_structs.h" 3 4
# 1 "/usr/include/mach/i386/_structs.h" 1 3 4
# 43 "/usr/include/mach/i386/_structs.h" 3 4
struct __darwin_i386_thread_state
{
    unsigned int __eax;
    unsigned int __ebx;
    unsigned int __ecx;
    unsigned int __edx;
    unsigned int __edi;
    unsigned int __esi;
    unsigned int __ebp;
    unsigned int __esp;
    unsigned int __ss;
    unsigned int __eflags;
    unsigned int __eip;
    unsigned int __cs;
    unsigned int __ds;
    unsigned int __es;
    unsigned int __fs;
    unsigned int __gs;
};
# 89 "/usr/include/mach/i386/_structs.h" 3 4
struct __darwin_fp_control
{
    unsigned short __invalid :1,
        __denorm :1,
    __zdiv :1,
    __ovrfl :1,
    __undfl :1,
    __precis :1,
      :2,
    __pc :2,





    __rc :2,






             :1,
      :3;
};
typedef struct __darwin_fp_control __darwin_fp_control_t;
# 147 "/usr/include/mach/i386/_structs.h" 3 4
struct __darwin_fp_status
{
    unsigned short __invalid :1,
        __denorm :1,
    __zdiv :1,
    __ovrfl :1,
    __undfl :1,
    __precis :1,
    __stkflt :1,
    __errsumm :1,
    __c0 :1,
    __c1 :1,
    __c2 :1,
    __tos :3,
    __c3 :1,
    __busy :1;
};
typedef struct __darwin_fp_status __darwin_fp_status_t;
# 191 "/usr/include/mach/i386/_structs.h" 3 4
struct __darwin_mmst_reg
{
 char __mmst_reg[10];
 char __mmst_rsrv[6];
};
# 210 "/usr/include/mach/i386/_structs.h" 3 4
struct __darwin_xmm_reg
{
 char __xmm_reg[16];
};
# 232 "/usr/include/mach/i386/_structs.h" 3 4
struct __darwin_i386_float_state
{
 int __fpu_reserved[2];
 struct __darwin_fp_control __fpu_fcw;
 struct __darwin_fp_status __fpu_fsw;
 __uint8_t __fpu_ftw;
 __uint8_t __fpu_rsrv1;
 __uint16_t __fpu_fop;
 __uint32_t __fpu_ip;
 __uint16_t __fpu_cs;
 __uint16_t __fpu_rsrv2;
 __uint32_t __fpu_dp;
 __uint16_t __fpu_ds;
 __uint16_t __fpu_rsrv3;
 __uint32_t __fpu_mxcsr;
 __uint32_t __fpu_mxcsrmask;
 struct __darwin_mmst_reg __fpu_stmm0;
 struct __darwin_mmst_reg __fpu_stmm1;
 struct __darwin_mmst_reg __fpu_stmm2;
 struct __darwin_mmst_reg __fpu_stmm3;
 struct __darwin_mmst_reg __fpu_stmm4;
 struct __darwin_mmst_reg __fpu_stmm5;
 struct __darwin_mmst_reg __fpu_stmm6;
 struct __darwin_mmst_reg __fpu_stmm7;
 struct __darwin_xmm_reg __fpu_xmm0;
 struct __darwin_xmm_reg __fpu_xmm1;
 struct __darwin_xmm_reg __fpu_xmm2;
 struct __darwin_xmm_reg __fpu_xmm3;
 struct __darwin_xmm_reg __fpu_xmm4;
 struct __darwin_xmm_reg __fpu_xmm5;
 struct __darwin_xmm_reg __fpu_xmm6;
 struct __darwin_xmm_reg __fpu_xmm7;
 char __fpu_rsrv4[14*16];
 int __fpu_reserved1;
};


struct __darwin_i386_avx_state
{
 int __fpu_reserved[2];
 struct __darwin_fp_control __fpu_fcw;
 struct __darwin_fp_status __fpu_fsw;
 __uint8_t __fpu_ftw;
 __uint8_t __fpu_rsrv1;
 __uint16_t __fpu_fop;
 __uint32_t __fpu_ip;
 __uint16_t __fpu_cs;
 __uint16_t __fpu_rsrv2;
 __uint32_t __fpu_dp;
 __uint16_t __fpu_ds;
 __uint16_t __fpu_rsrv3;
 __uint32_t __fpu_mxcsr;
 __uint32_t __fpu_mxcsrmask;
 struct __darwin_mmst_reg __fpu_stmm0;
 struct __darwin_mmst_reg __fpu_stmm1;
 struct __darwin_mmst_reg __fpu_stmm2;
 struct __darwin_mmst_reg __fpu_stmm3;
 struct __darwin_mmst_reg __fpu_stmm4;
 struct __darwin_mmst_reg __fpu_stmm5;
 struct __darwin_mmst_reg __fpu_stmm6;
 struct __darwin_mmst_reg __fpu_stmm7;
 struct __darwin_xmm_reg __fpu_xmm0;
 struct __darwin_xmm_reg __fpu_xmm1;
 struct __darwin_xmm_reg __fpu_xmm2;
 struct __darwin_xmm_reg __fpu_xmm3;
 struct __darwin_xmm_reg __fpu_xmm4;
 struct __darwin_xmm_reg __fpu_xmm5;
 struct __darwin_xmm_reg __fpu_xmm6;
 struct __darwin_xmm_reg __fpu_xmm7;
 char __fpu_rsrv4[14*16];
 int __fpu_reserved1;
 char __avx_reserved1[64];
 struct __darwin_xmm_reg __fpu_ymmh0;
 struct __darwin_xmm_reg __fpu_ymmh1;
 struct __darwin_xmm_reg __fpu_ymmh2;
 struct __darwin_xmm_reg __fpu_ymmh3;
 struct __darwin_xmm_reg __fpu_ymmh4;
 struct __darwin_xmm_reg __fpu_ymmh5;
 struct __darwin_xmm_reg __fpu_ymmh6;
 struct __darwin_xmm_reg __fpu_ymmh7;
};
# 402 "/usr/include/mach/i386/_structs.h" 3 4
struct __darwin_i386_exception_state
{
 __uint16_t __trapno;
 __uint16_t __cpu;
 __uint32_t __err;
 __uint32_t __faultvaddr;
};
# 422 "/usr/include/mach/i386/_structs.h" 3 4
struct __darwin_x86_debug_state32
{
 unsigned int __dr0;
 unsigned int __dr1;
 unsigned int __dr2;
 unsigned int __dr3;
 unsigned int __dr4;
 unsigned int __dr5;
 unsigned int __dr6;
 unsigned int __dr7;
};
# 454 "/usr/include/mach/i386/_structs.h" 3 4
struct __darwin_x86_thread_state64
{
 __uint64_t __rax;
 __uint64_t __rbx;
 __uint64_t __rcx;
 __uint64_t __rdx;
 __uint64_t __rdi;
 __uint64_t __rsi;
 __uint64_t __rbp;
 __uint64_t __rsp;
 __uint64_t __r8;
 __uint64_t __r9;
 __uint64_t __r10;
 __uint64_t __r11;
 __uint64_t __r12;
 __uint64_t __r13;
 __uint64_t __r14;
 __uint64_t __r15;
 __uint64_t __rip;
 __uint64_t __rflags;
 __uint64_t __cs;
 __uint64_t __fs;
 __uint64_t __gs;
};
# 509 "/usr/include/mach/i386/_structs.h" 3 4
struct __darwin_x86_float_state64
{
 int __fpu_reserved[2];
 struct __darwin_fp_control __fpu_fcw;
 struct __darwin_fp_status __fpu_fsw;
 __uint8_t __fpu_ftw;
 __uint8_t __fpu_rsrv1;
 __uint16_t __fpu_fop;


 __uint32_t __fpu_ip;
 __uint16_t __fpu_cs;

 __uint16_t __fpu_rsrv2;


 __uint32_t __fpu_dp;
 __uint16_t __fpu_ds;

 __uint16_t __fpu_rsrv3;
 __uint32_t __fpu_mxcsr;
 __uint32_t __fpu_mxcsrmask;
 struct __darwin_mmst_reg __fpu_stmm0;
 struct __darwin_mmst_reg __fpu_stmm1;
 struct __darwin_mmst_reg __fpu_stmm2;
 struct __darwin_mmst_reg __fpu_stmm3;
 struct __darwin_mmst_reg __fpu_stmm4;
 struct __darwin_mmst_reg __fpu_stmm5;
 struct __darwin_mmst_reg __fpu_stmm6;
 struct __darwin_mmst_reg __fpu_stmm7;
 struct __darwin_xmm_reg __fpu_xmm0;
 struct __darwin_xmm_reg __fpu_xmm1;
 struct __darwin_xmm_reg __fpu_xmm2;
 struct __darwin_xmm_reg __fpu_xmm3;
 struct __darwin_xmm_reg __fpu_xmm4;
 struct __darwin_xmm_reg __fpu_xmm5;
 struct __darwin_xmm_reg __fpu_xmm6;
 struct __darwin_xmm_reg __fpu_xmm7;
 struct __darwin_xmm_reg __fpu_xmm8;
 struct __darwin_xmm_reg __fpu_xmm9;
 struct __darwin_xmm_reg __fpu_xmm10;
 struct __darwin_xmm_reg __fpu_xmm11;
 struct __darwin_xmm_reg __fpu_xmm12;
 struct __darwin_xmm_reg __fpu_xmm13;
 struct __darwin_xmm_reg __fpu_xmm14;
 struct __darwin_xmm_reg __fpu_xmm15;
 char __fpu_rsrv4[6*16];
 int __fpu_reserved1;
};


struct __darwin_x86_avx_state64
{
 int __fpu_reserved[2];
 struct __darwin_fp_control __fpu_fcw;
 struct __darwin_fp_status __fpu_fsw;
 __uint8_t __fpu_ftw;
 __uint8_t __fpu_rsrv1;
 __uint16_t __fpu_fop;


 __uint32_t __fpu_ip;
 __uint16_t __fpu_cs;

 __uint16_t __fpu_rsrv2;


 __uint32_t __fpu_dp;
 __uint16_t __fpu_ds;

 __uint16_t __fpu_rsrv3;
 __uint32_t __fpu_mxcsr;
 __uint32_t __fpu_mxcsrmask;
 struct __darwin_mmst_reg __fpu_stmm0;
 struct __darwin_mmst_reg __fpu_stmm1;
 struct __darwin_mmst_reg __fpu_stmm2;
 struct __darwin_mmst_reg __fpu_stmm3;
 struct __darwin_mmst_reg __fpu_stmm4;
 struct __darwin_mmst_reg __fpu_stmm5;
 struct __darwin_mmst_reg __fpu_stmm6;
 struct __darwin_mmst_reg __fpu_stmm7;
 struct __darwin_xmm_reg __fpu_xmm0;
 struct __darwin_xmm_reg __fpu_xmm1;
 struct __darwin_xmm_reg __fpu_xmm2;
 struct __darwin_xmm_reg __fpu_xmm3;
 struct __darwin_xmm_reg __fpu_xmm4;
 struct __darwin_xmm_reg __fpu_xmm5;
 struct __darwin_xmm_reg __fpu_xmm6;
 struct __darwin_xmm_reg __fpu_xmm7;
 struct __darwin_xmm_reg __fpu_xmm8;
 struct __darwin_xmm_reg __fpu_xmm9;
 struct __darwin_xmm_reg __fpu_xmm10;
 struct __darwin_xmm_reg __fpu_xmm11;
 struct __darwin_xmm_reg __fpu_xmm12;
 struct __darwin_xmm_reg __fpu_xmm13;
 struct __darwin_xmm_reg __fpu_xmm14;
 struct __darwin_xmm_reg __fpu_xmm15;
 char __fpu_rsrv4[6*16];
 int __fpu_reserved1;
 char __avx_reserved1[64];
 struct __darwin_xmm_reg __fpu_ymmh0;
 struct __darwin_xmm_reg __fpu_ymmh1;
 struct __darwin_xmm_reg __fpu_ymmh2;
 struct __darwin_xmm_reg __fpu_ymmh3;
 struct __darwin_xmm_reg __fpu_ymmh4;
 struct __darwin_xmm_reg __fpu_ymmh5;
 struct __darwin_xmm_reg __fpu_ymmh6;
 struct __darwin_xmm_reg __fpu_ymmh7;
 struct __darwin_xmm_reg __fpu_ymmh8;
 struct __darwin_xmm_reg __fpu_ymmh9;
 struct __darwin_xmm_reg __fpu_ymmh10;
 struct __darwin_xmm_reg __fpu_ymmh11;
 struct __darwin_xmm_reg __fpu_ymmh12;
 struct __darwin_xmm_reg __fpu_ymmh13;
 struct __darwin_xmm_reg __fpu_ymmh14;
 struct __darwin_xmm_reg __fpu_ymmh15;
};
# 751 "/usr/include/mach/i386/_structs.h" 3 4
struct __darwin_x86_exception_state64
{
    __uint16_t __trapno;
    __uint16_t __cpu;
    __uint32_t __err;
    __uint64_t __faultvaddr;
};
# 771 "/usr/include/mach/i386/_structs.h" 3 4
struct __darwin_x86_debug_state64
{
 __uint64_t __dr0;
 __uint64_t __dr1;
 __uint64_t __dr2;
 __uint64_t __dr3;
 __uint64_t __dr4;
 __uint64_t __dr5;
 __uint64_t __dr6;
 __uint64_t __dr7;
};
# 39 "/usr/include/i386/_structs.h" 2 3 4
# 48 "/usr/include/i386/_structs.h" 3 4
struct __darwin_mcontext32
{
 struct __darwin_i386_exception_state __es;
 struct __darwin_i386_thread_state __ss;
 struct __darwin_i386_float_state __fs;
};


struct __darwin_mcontext_avx32
{
 struct __darwin_i386_exception_state __es;
 struct __darwin_i386_thread_state __ss;
 struct __darwin_i386_avx_state __fs;
};
# 86 "/usr/include/i386/_structs.h" 3 4
struct __darwin_mcontext64
{
 struct __darwin_x86_exception_state64 __es;
 struct __darwin_x86_thread_state64 __ss;
 struct __darwin_x86_float_state64 __fs;
};


struct __darwin_mcontext_avx64
{
 struct __darwin_x86_exception_state64 __es;
 struct __darwin_x86_thread_state64 __ss;
 struct __darwin_x86_avx_state64 __fs;
};
# 127 "/usr/include/i386/_structs.h" 3 4
typedef struct __darwin_mcontext64 *mcontext_t;
# 30 "/usr/include/machine/_structs.h" 2 3 4
# 58 "/usr/include/sys/_structs.h" 2 3 4
# 75 "/usr/include/sys/_structs.h" 3 4
struct __darwin_sigaltstack
{
 void *ss_sp;
 __darwin_size_t ss_size;
 int ss_flags;
};
# 128 "/usr/include/sys/_structs.h" 3 4
struct __darwin_ucontext
{
 int uc_onstack;
 __darwin_sigset_t uc_sigmask;
 struct __darwin_sigaltstack uc_stack;
 struct __darwin_ucontext *uc_link;
 __darwin_size_t uc_mcsize;
 struct __darwin_mcontext64 *uc_mcontext;



};
# 218 "/usr/include/sys/_structs.h" 3 4
typedef struct __darwin_sigaltstack stack_t;
# 227 "/usr/include/sys/_structs.h" 3 4
typedef struct __darwin_ucontext ucontext_t;
# 149 "/usr/include/sys/signal.h" 2 3 4
# 162 "/usr/include/sys/signal.h" 3 4
typedef __darwin_sigset_t sigset_t;
# 175 "/usr/include/sys/signal.h" 3 4
union sigval {

 int sival_int;
 void *sival_ptr;
};





struct sigevent {
 int sigev_notify;
 int sigev_signo;
 union sigval sigev_value;
 void (*sigev_notify_function)(union sigval);
 pthread_attr_t *sigev_notify_attributes;
};


typedef struct __siginfo {
 int si_signo;
 int si_errno;
 int si_code;
 pid_t si_pid;
 uid_t si_uid;
 int si_status;
 void *si_addr;
 union sigval si_value;
 long si_band;
 unsigned long __pad[7];
} siginfo_t;
# 286 "/usr/include/sys/signal.h" 3 4
union __sigaction_u {
 void (*__sa_handler)(int);
 void (*__sa_sigaction)(int, struct __siginfo *,
         void *);
};


struct __sigaction {
 union __sigaction_u __sigaction_u;
 void (*sa_tramp)(void *, int, int, siginfo_t *, void *);
 sigset_t sa_mask;
 int sa_flags;
};




struct sigaction {
 union __sigaction_u __sigaction_u;
 sigset_t sa_mask;
 int sa_flags;
};
# 348 "/usr/include/sys/signal.h" 3 4
typedef void (*sig_t)(int);
# 365 "/usr/include/sys/signal.h" 3 4
struct sigvec {
 void (*sv_handler)(int);
 int sv_mask;
 int sv_flags;
};
# 384 "/usr/include/sys/signal.h" 3 4
struct sigstack {
 char *ss_sp;
 int ss_onstack;
};
# 407 "/usr/include/sys/signal.h" 3 4
void (*signal(int, void (*)(int)))(int);
# 117 "/usr/include/sys/wait.h" 2 3 4
# 1 "/usr/include/sys/resource.h" 1 3 4
# 77 "/usr/include/sys/resource.h" 3 4
# 1 "/usr/include/sys/_structs.h" 1 3 4
# 100 "/usr/include/sys/_structs.h" 3 4
struct timeval
{
 __darwin_time_t tv_sec;
 __darwin_suseconds_t tv_usec;
};
# 78 "/usr/include/sys/resource.h" 2 3 4
# 89 "/usr/include/sys/resource.h" 3 4
typedef __uint64_t rlim_t;
# 151 "/usr/include/sys/resource.h" 3 4
struct rusage {
 struct timeval ru_utime;
 struct timeval ru_stime;
# 162 "/usr/include/sys/resource.h" 3 4
 long ru_maxrss;

 long ru_ixrss;
 long ru_idrss;
 long ru_isrss;
 long ru_minflt;
 long ru_majflt;
 long ru_nswap;
 long ru_inblock;
 long ru_oublock;
 long ru_msgsnd;
 long ru_msgrcv;
 long ru_nsignals;
 long ru_nvcsw;
 long ru_nivcsw;


};
# 222 "/usr/include/sys/resource.h" 3 4
struct rlimit {
 rlim_t rlim_cur;
 rlim_t rlim_max;
};
# 245 "/usr/include/sys/resource.h" 3 4
int getpriority(int, id_t);

int getiopolicy_np(int, int) __attribute__((visibility("default")));

int getrlimit(int, struct rlimit *) __asm("_" "getrlimit" );
int getrusage(int, struct rusage *);
int setpriority(int, id_t, int);

int setiopolicy_np(int, int, int) __attribute__((visibility("default")));

int setrlimit(int, const struct rlimit *) __asm("_" "setrlimit" );
# 118 "/usr/include/sys/wait.h" 2 3 4
# 201 "/usr/include/sys/wait.h" 3 4
union wait {
 int w_status;



 struct {

  unsigned int w_Termsig:7,
    w_Coredump:1,
    w_Retcode:8,
    w_Filler:16;







 } w_T;





 struct {

  unsigned int w_Stopval:8,
    w_Stopsig:8,
    w_Filler:16;






 } w_S;
};
# 255 "/usr/include/sys/wait.h" 3 4
pid_t wait(int *) __asm("_" "wait" );
pid_t waitpid(pid_t, int *, int) __asm("_" "waitpid" );

int waitid(idtype_t, id_t, siginfo_t *, int) __asm("_" "waitid" );


pid_t wait3(int *, int, struct rusage *);
pid_t wait4(pid_t, int *, int, struct rusage *);
# 66 "/usr/include/stdlib.h" 2 3 4

# 1 "/usr/include/alloca.h" 1 3 4
# 36 "/usr/include/alloca.h" 3 4
void *alloca(size_t);
# 68 "/usr/include/stdlib.h" 2 3 4
# 81 "/usr/include/stdlib.h" 3 4
typedef __darwin_ct_rune_t ct_rune_t;




typedef __darwin_rune_t rune_t;
# 97 "/usr/include/stdlib.h" 3 4
typedef struct {
 int quot;
 int rem;
} div_t;

typedef struct {
 long quot;
 long rem;
} ldiv_t;


typedef struct {
 long long quot;
 long long rem;
} lldiv_t;
# 134 "/usr/include/stdlib.h" 3 4
extern int __mb_cur_max;
# 145 "/usr/include/stdlib.h" 3 4
void abort(void) __attribute__((__noreturn__));
int abs(int) __attribute__((__const__));
int atexit(void (*)(void));
double atof(const char *);
int atoi(const char *);
long atol(const char *);

long long
  atoll(const char *);

void *bsearch(const void *, const void *, size_t,
     size_t, int (*)(const void *, const void *));
void *calloc(size_t, size_t);
div_t div(int, int) __attribute__((__const__));
void exit(int) __attribute__((__noreturn__));
void free(void *);
char *getenv(const char *);
long labs(long) __attribute__((__const__));
ldiv_t ldiv(long, long) __attribute__((__const__));

long long
  llabs(long long);
lldiv_t lldiv(long long, long long);

void *malloc(size_t);
int mblen(const char *, size_t);
size_t mbstowcs(wchar_t * restrict , const char * restrict, size_t);
int mbtowc(wchar_t * restrict, const char * restrict, size_t);
int posix_memalign(void **, size_t, size_t) __attribute__((visibility("default")));
void qsort(void *, size_t, size_t,
     int (*)(const void *, const void *));
int rand(void);
void *realloc(void *, size_t);
void srand(unsigned);
double strtod(const char *, char **) __asm("_" "strtod" );
float strtof(const char *, char **) __asm("_" "strtof" );
long strtol(const char *, char **, int);
long double
  strtold(const char *, char **) ;

long long
  strtoll(const char *, char **, int);

unsigned long
  strtoul(const char *, char **, int);

unsigned long long
  strtoull(const char *, char **, int);

int system(const char *) __asm("_" "system" );
size_t wcstombs(char * restrict, const wchar_t * restrict, size_t);
int wctomb(char *, wchar_t);


void _Exit(int) __attribute__((__noreturn__));
long a64l(const char *);
double drand48(void);
char *ecvt(double, int, int *restrict, int *restrict);
double erand48(unsigned short[3]);
char *fcvt(double, int, int *restrict, int *restrict);
char *gcvt(double, int, char *);
int getsubopt(char **, char * const *, char **);
int grantpt(int);

char *initstate(unsigned, char *, size_t);



long jrand48(unsigned short[3]);
char *l64a(long);
void lcong48(unsigned short[7]);
long lrand48(void);
char *mktemp(char *);
int mkstemp(char *);
long mrand48(void);
long nrand48(unsigned short[3]);
int posix_openpt(int);
char *ptsname(int);
int putenv(char *) __asm("_" "putenv" );
long random(void);
int rand_r(unsigned *);

char *realpath(const char * restrict, char * restrict) __asm("_" "realpath" "$DARWIN_EXTSN");



unsigned short
 *seed48(unsigned short[3]);
int setenv(const char *, const char *, int) __asm("_" "setenv" );

void setkey(const char *) __asm("_" "setkey" );



char *setstate(const char *);
void srand48(long);

void srandom(unsigned);



int unlockpt(int);

int unsetenv(const char *) __asm("_" "unsetenv" );
# 267 "/usr/include/stdlib.h" 3 4
u_int32_t
  arc4random(void);
void arc4random_addrandom(unsigned char * , int );
void arc4random_buf(void * , size_t ) __attribute__((visibility("default")));
void arc4random_stir(void);
u_int32_t
  arc4random_uniform(u_int32_t ) __attribute__((visibility("default")));

int atexit_b(void (^)(void)) __attribute__((visibility("default")));
void *bsearch_b(const void *, const void *, size_t,
     size_t, int (^)(const void *, const void *)) __attribute__((visibility("default")));



char *cgetcap(char *, const char *, int);
int cgetclose(void);
int cgetent(char **, char **, const char *);
int cgetfirst(char **, char **);
int cgetmatch(const char *, const char *);
int cgetnext(char **, char **);
int cgetnum(char *, const char *, long *);
int cgetset(const char *);
int cgetstr(char *, const char *, char **);
int cgetustr(char *, const char *, char **);

int daemon(int, int) __asm("_" "daemon" "$1050") __attribute__((deprecated,visibility("default")));
char *devname(dev_t, mode_t);
char *devname_r(dev_t, mode_t, char *buf, int len);
char *getbsize(int *, long *);
int getloadavg(double [], int);
const char
 *getprogname(void);

int heapsort(void *, size_t, size_t,
     int (*)(const void *, const void *));

int heapsort_b(void *, size_t, size_t,
     int (^)(const void *, const void *)) __attribute__((visibility("default")));

int mergesort(void *, size_t, size_t,
     int (*)(const void *, const void *));

int mergesort_b(void *, size_t, size_t,
     int (^)(const void *, const void *)) __attribute__((visibility("default")));

void psort(void *, size_t, size_t,
     int (*)(const void *, const void *)) __attribute__((visibility("default")));

void psort_b(void *, size_t, size_t,
     int (^)(const void *, const void *)) __attribute__((visibility("default")));

void psort_r(void *, size_t, size_t, void *,
     int (*)(void *, const void *, const void *)) __attribute__((visibility("default")));

void qsort_b(void *, size_t, size_t,
     int (^)(const void *, const void *)) __attribute__((visibility("default")));

void qsort_r(void *, size_t, size_t, void *,
     int (*)(void *, const void *, const void *));
int radixsort(const unsigned char **, int, const unsigned char *,
     unsigned);
void setprogname(const char *);
int sradixsort(const unsigned char **, int, const unsigned char *,
     unsigned);
void sranddev(void);
void srandomdev(void);
void *reallocf(void *, size_t);

long long
  strtoq(const char *, char **, int);
unsigned long long
  strtouq(const char *, char **, int);

extern char *suboptarg;
void *valloc(size_t);
# 135 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/system.h" 2
# 157 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/system.h"
# 1 "/usr/include/unistd.h" 1 3 4
# 72 "/usr/include/unistd.h" 3 4
# 1 "/usr/include/sys/unistd.h" 1 3 4
# 138 "/usr/include/sys/unistd.h" 3 4
struct accessx_descriptor {
 unsigned int ad_name_offset;
 int ad_flags;
 int ad_pad[2];
};
# 73 "/usr/include/unistd.h" 2 3 4
# 469 "/usr/include/unistd.h" 3 4
void _exit(int) __attribute__((__noreturn__));
int access(const char *, int);
unsigned int
  alarm(unsigned int);
int chdir(const char *);
int chown(const char *, uid_t, gid_t);

int close(int) __asm("_" "close" );

int dup(int);
int dup2(int, int);
int execl(const char *, const char *, ...);
int execle(const char *, const char *, ...);
int execlp(const char *, const char *, ...);
int execv(const char *, char * const *);
int execve(const char *, char * const *, char * const *);
int execvp(const char *, char * const *);
pid_t fork(void);
long fpathconf(int, int);
char *getcwd(char *, size_t);
gid_t getegid(void);
uid_t geteuid(void);
gid_t getgid(void);



int getgroups(int, gid_t []);

char *getlogin(void);
pid_t getpgrp(void);
pid_t getpid(void);
pid_t getppid(void);
uid_t getuid(void);
int isatty(int);
int link(const char *, const char *);
off_t lseek(int, off_t, int);
long pathconf(const char *, int);

int pause(void) __asm("_" "pause" );

int pipe(int [2]);

ssize_t read(int, void *, size_t) __asm("_" "read" );

int rmdir(const char *);
int setgid(gid_t);
int setpgid(pid_t, pid_t);
pid_t setsid(void);
int setuid(uid_t);

unsigned int
  sleep(unsigned int) __asm("_" "sleep" );

long sysconf(int);
pid_t tcgetpgrp(int);
int tcsetpgrp(int, pid_t);
char *ttyname(int);


int ttyname_r(int, char *, size_t) __asm("_" "ttyname_r" );




int unlink(const char *);

ssize_t write(int, const void *, size_t) __asm("_" "write" );
# 546 "/usr/include/unistd.h" 3 4
size_t confstr(int, char *, size_t) __asm("_" "confstr" );

int getopt(int, char * const [], const char *) __asm("_" "getopt" );

extern char *optarg;
extern int optind, opterr, optopt;
# 575 "/usr/include/unistd.h" 3 4
void *brk(const void *);
int chroot(const char *) ;


char *crypt(const char *, const char *);






void encrypt(char *, int) __asm("_" "encrypt" );



int fchdir(int);
long gethostid(void);
pid_t getpgid(pid_t);
pid_t getsid(pid_t);



int getdtablesize(void) ;
int getpagesize(void) __attribute__((__const__)) ;
char *getpass(const char *) ;




char *getwd(char *) ;


int lchown(const char *, uid_t, gid_t) __asm("_" "lchown" );

int lockf(int, int, off_t) __asm("_" "lockf" );

int nice(int) __asm("_" "nice" );

ssize_t pread(int, void *, size_t, off_t) __asm("_" "pread" );

ssize_t pwrite(int, const void *, size_t, off_t) __asm("_" "pwrite" );





void *sbrk(int);



pid_t setpgrp(void) __asm("_" "setpgrp" );




int setregid(gid_t, gid_t) __asm("_" "setregid" );

int setreuid(uid_t, uid_t) __asm("_" "setreuid" );

void swab(const void * restrict, void * restrict, ssize_t);
void sync(void);
int truncate(const char *, off_t);
useconds_t ualarm(useconds_t, useconds_t);
int usleep(useconds_t) __asm("_" "usleep" );
pid_t vfork(void);


int fsync(int) __asm("_" "fsync" );

int ftruncate(int, off_t);
int getlogin_r(char *, size_t);
# 658 "/usr/include/unistd.h" 3 4
int fchown(int, uid_t, gid_t);
int gethostname(char *, size_t);
ssize_t readlink(const char * restrict, char * restrict, size_t);
int setegid(gid_t);
int seteuid(uid_t);
int symlink(const char *, const char *);
# 672 "/usr/include/unistd.h" 3 4
# 1 "/usr/include/sys/select.h" 1 3 4
# 78 "/usr/include/sys/select.h" 3 4
# 1 "/usr/include/sys/_structs.h" 1 3 4
# 88 "/usr/include/sys/_structs.h" 3 4
struct timespec
{
 __darwin_time_t tv_sec;
 long tv_nsec;
};
# 79 "/usr/include/sys/select.h" 2 3 4
# 137 "/usr/include/sys/select.h" 3 4
int pselect(int, fd_set * restrict, fd_set * restrict,
  fd_set * restrict, const struct timespec * restrict,
  const sigset_t * restrict)




  __asm("_" "pselect" "$1050")




  ;



# 1 "/usr/include/sys/_select.h" 1 3 4
# 39 "/usr/include/sys/_select.h" 3 4
int select(int, fd_set * restrict, fd_set * restrict,
  fd_set * restrict, struct timeval * restrict)




  __asm("_" "select" "$1050")




  ;
# 153 "/usr/include/sys/select.h" 2 3 4
# 673 "/usr/include/unistd.h" 2 3 4
# 686 "/usr/include/unistd.h" 3 4
typedef __darwin_uuid_t uuid_t;



void _Exit(int) __attribute__((__noreturn__));
int accessx_np(const struct accessx_descriptor *, size_t, int *, uid_t);
int acct(const char *);
int add_profil(char *, size_t, unsigned long, unsigned int);
void endusershell(void);
int execvP(const char *, const char *, char * const *);
char *fflagstostr(unsigned long);
int getdomainname(char *, int);
int getgrouplist(const char *, int, int *, int *);
int gethostuuid(uuid_t, const struct timespec *) __attribute__((visibility("default")));
mode_t getmode(const void *, mode_t);
int getpeereid(int, uid_t *, gid_t *);
int getsgroups_np(int *, uuid_t);
char *getusershell(void);
int getwgroups_np(int *, uuid_t);
int initgroups(const char *, int);
int iruserok(unsigned long, int, const char *, const char *);
int iruserok_sa(const void *, int, int, const char *, const char *);
int issetugid(void);
char *mkdtemp(char *);
int mknod(const char *, mode_t, dev_t);
int mkstemp(char *);
int mkstemps(char *, int);
char *mktemp(char *);
int nfssvc(int, void *);
int profil(char *, size_t, unsigned long, unsigned int);
int pthread_setugid_np(uid_t, gid_t);
int pthread_getugid_np( uid_t *, gid_t *);
int rcmd(char **, int, const char *, const char *, const char *, int *);
int rcmd_af(char **, int, const char *, const char *, const char *, int *,
  int);
int reboot(int);
int revoke(const char *);
int rresvport(int *);
int rresvport_af(int *, int);
int ruserok(const char *, int, const char *, const char *);
int setdomainname(const char *, int);
int setgroups(int, const gid_t *);
void sethostid(long);
int sethostname(const char *, int);

void setkey(const char *) __asm("_" "setkey" );



int setlogin(const char *);
void *setmode(const char *) __asm("_" "setmode" );
int setrgid(gid_t);
int setruid(uid_t);
int setsgroups_np(int, const uuid_t);
void setusershell(void);
int setwgroups_np(int, const uuid_t);
int strtofflags(char **, unsigned long *, unsigned long *);
int swapon(const char *);
int syscall(int, ...);
int ttyslot(void);
int undelete(const char *);
int unwhiteout(const char *);
void *valloc(size_t);

extern char *suboptarg;
int getsubopt(char **, char * const *, char **);



int fgetattrlist(int,void*,void*,size_t,unsigned int) __attribute__((visibility("default")));
int fsetattrlist(int,void*,void*,size_t,unsigned int) __attribute__((visibility("default")));
int getattrlist(const char*,void*,void*,size_t,unsigned int) __asm("_" "getattrlist" );
int setattrlist(const char*,void*,void*,size_t,unsigned int) __asm("_" "setattrlist" );
int exchangedata(const char*,const char*,unsigned int);
int getdirentriesattr(int,void*,void*,size_t,unsigned int*,unsigned int*,unsigned int*,unsigned int);
# 772 "/usr/include/unistd.h" 3 4
struct fssearchblock;
struct searchstate;

int searchfs(const char *, struct fssearchblock *, unsigned long *, unsigned int, unsigned int, struct searchstate *);
int fsctl(const char *,unsigned long,void*,unsigned int);
int ffsctl(int,unsigned long,void*,unsigned int) __attribute__((visibility("default")));

extern int optreset;
# 158 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/system.h" 2
# 167 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/system.h"
# 1 "/Users/ddunbar/rt/10907846/clang-211.10.1-Slate/Developer/usr/bin/../lib/clang/3.0/include/limits.h" 1 3 4
# 38 "/Users/ddunbar/rt/10907846/clang-211.10.1-Slate/Developer/usr/bin/../lib/clang/3.0/include/limits.h" 3 4
# 1 "/usr/include/limits.h" 1 3 4
# 64 "/usr/include/limits.h" 3 4
# 1 "/usr/include/machine/limits.h" 1 3 4





# 1 "/usr/include/i386/limits.h" 1 3 4
# 40 "/usr/include/i386/limits.h" 3 4
# 1 "/usr/include/i386/_limits.h" 1 3 4
# 41 "/usr/include/i386/limits.h" 2 3 4
# 7 "/usr/include/machine/limits.h" 2 3 4
# 65 "/usr/include/limits.h" 2 3 4
# 1 "/usr/include/sys/syslimits.h" 1 3 4
# 66 "/usr/include/limits.h" 2 3 4
# 39 "/Users/ddunbar/rt/10907846/clang-211.10.1-Slate/Developer/usr/bin/../lib/clang/3.0/include/limits.h" 2 3 4
# 168 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/system.h" 2



# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/hwint.h" 1
# 172 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/system.h" 2
# 207 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/system.h"
# 1 "/usr/include/time.h" 1 3 4
# 69 "/usr/include/time.h" 3 4
# 1 "/usr/include/_structs.h" 1 3 4
# 24 "/usr/include/_structs.h" 3 4
# 1 "/usr/include/sys/_structs.h" 1 3 4
# 25 "/usr/include/_structs.h" 2 3 4
# 70 "/usr/include/time.h" 2 3 4
# 90 "/usr/include/time.h" 3 4
struct tm {
 int tm_sec;
 int tm_min;
 int tm_hour;
 int tm_mday;
 int tm_mon;
 int tm_year;
 int tm_wday;
 int tm_yday;
 int tm_isdst;
 long tm_gmtoff;
 char *tm_zone;
};
# 113 "/usr/include/time.h" 3 4
extern char *tzname[];


extern int getdate_err;

extern long timezone __asm("_" "timezone" );

extern int daylight;


char *asctime(const struct tm *);
clock_t clock(void) __asm("_" "clock" );
char *ctime(const time_t *);
double difftime(time_t, time_t);
struct tm *getdate(const char *);
struct tm *gmtime(const time_t *);
struct tm *localtime(const time_t *);
time_t mktime(struct tm *) __asm("_" "mktime" );
size_t strftime(char * restrict, size_t, const char * restrict, const struct tm * restrict) __asm("_" "strftime" );
char *strptime(const char * restrict, const char * restrict, struct tm * restrict) __asm("_" "strptime" );
time_t time(time_t *);


void tzset(void);



char *asctime_r(const struct tm * restrict, char * restrict);
char *ctime_r(const time_t *, char *);
struct tm *gmtime_r(const time_t * restrict, struct tm * restrict);
struct tm *localtime_r(const time_t * restrict, struct tm * restrict);


time_t posix2time(time_t);



void tzsetwall(void);
time_t time2posix(time_t);
time_t timelocal(struct tm * const);
time_t timegm(struct tm * const);



int nanosleep(const struct timespec *, struct timespec *) __asm("_" "nanosleep" );
# 208 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/system.h" 2





# 1 "/usr/include/fcntl.h" 1 3 4
# 23 "/usr/include/fcntl.h" 3 4
# 1 "/usr/include/sys/fcntl.h" 1 3 4
# 339 "/usr/include/sys/fcntl.h" 3 4
struct flock {
 off_t l_start;
 off_t l_len;
 pid_t l_pid;
 short l_type;
 short l_whence;
};
# 355 "/usr/include/sys/fcntl.h" 3 4
struct radvisory {
       off_t ra_offset;
       int ra_count;
};
# 367 "/usr/include/sys/fcntl.h" 3 4
typedef struct fsignatures {
 off_t fs_file_start;
 void *fs_blob_start;
 size_t fs_blob_size;
} fsignatures_t;
# 381 "/usr/include/sys/fcntl.h" 3 4
typedef struct fstore {
 unsigned int fst_flags;
 int fst_posmode;
 off_t fst_offset;
 off_t fst_length;
 off_t fst_bytesalloc;
} fstore_t;



typedef struct fbootstraptransfer {
  off_t fbt_offset;
  size_t fbt_length;
  void *fbt_buffer;
} fbootstraptransfer_t;
# 419 "/usr/include/sys/fcntl.h" 3 4
#pragma pack(4)

struct log2phys {
 unsigned int l2p_flags;
 off_t l2p_contigbytes;


 off_t l2p_devoffset;


};

#pragma pack()
# 442 "/usr/include/sys/fcntl.h" 3 4
struct _filesec;
typedef struct _filesec *filesec_t;


typedef enum {
 FILESEC_OWNER = 1,
 FILESEC_GROUP = 2,
 FILESEC_UUID = 3,
 FILESEC_MODE = 4,
 FILESEC_ACL = 5,
 FILESEC_GRPUUID = 6,


 FILESEC_ACL_RAW = 100,
 FILESEC_ACL_ALLOCSIZE = 101
} filesec_property_t;






int open(const char *, int, ...) __asm("_" "open" );
int creat(const char *, mode_t) __asm("_" "creat" );
int fcntl(int, int, ...) __asm("_" "fcntl" );


int openx_np(const char *, int, filesec_t);
int flock(int, int);
filesec_t filesec_init(void);
filesec_t filesec_dup(filesec_t);
void filesec_free(filesec_t);
int filesec_get_property(filesec_t, filesec_property_t, void *);
int filesec_query_property(filesec_t, filesec_property_t, int *);
int filesec_set_property(filesec_t, filesec_property_t, const void *);
int filesec_unset_property(filesec_t, filesec_property_t) __attribute__((visibility("default")));
# 24 "/usr/include/fcntl.h" 2 3 4
# 214 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/system.h" 2
# 381 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/system.h"
# 1 "/usr/include/sys/stat.h" 1 3 4
# 79 "/usr/include/sys/stat.h" 3 4
# 1 "/usr/include/sys/_structs.h" 1 3 4
# 80 "/usr/include/sys/stat.h" 2 3 4
# 153 "/usr/include/sys/stat.h" 3 4
struct ostat {
 __uint16_t st_dev;
 ino_t st_ino;
 mode_t st_mode;
 nlink_t st_nlink;
 __uint16_t st_uid;
 __uint16_t st_gid;
 __uint16_t st_rdev;
 __int32_t st_size;
 struct timespec st_atimespec;
 struct timespec st_mtimespec;
 struct timespec st_ctimespec;
 __int32_t st_blksize;
 __int32_t st_blocks;
 __uint32_t st_flags;
 __uint32_t st_gen;
};
# 225 "/usr/include/sys/stat.h" 3 4
struct stat { dev_t st_dev; mode_t st_mode; nlink_t st_nlink; __darwin_ino64_t st_ino; uid_t st_uid; gid_t st_gid; dev_t st_rdev; struct timespec st_atimespec; struct timespec st_mtimespec; struct timespec st_ctimespec; struct timespec st_birthtimespec; off_t st_size; blkcnt_t st_blocks; blksize_t st_blksize; __uint32_t st_flags; __uint32_t st_gen; __int32_t st_lspare; __int64_t st_qspare[2]; };
# 264 "/usr/include/sys/stat.h" 3 4
struct stat64 { dev_t st_dev; mode_t st_mode; nlink_t st_nlink; __darwin_ino64_t st_ino; uid_t st_uid; gid_t st_gid; dev_t st_rdev; struct timespec st_atimespec; struct timespec st_mtimespec; struct timespec st_ctimespec; struct timespec st_birthtimespec; off_t st_size; blkcnt_t st_blocks; blksize_t st_blksize; __uint32_t st_flags; __uint32_t st_gen; __int32_t st_lspare; __int64_t st_qspare[2]; };
# 430 "/usr/include/sys/stat.h" 3 4
int chmod(const char *, mode_t) __asm("_" "chmod" );
int fchmod(int, mode_t) __asm("_" "fchmod" );
int fstat(int, struct stat *) __asm("_" "fstat" "$INODE64");
int lstat(const char *, struct stat *) __asm("_" "lstat" "$INODE64");
int mkdir(const char *, mode_t);
int mkfifo(const char *, mode_t);
int stat(const char *, struct stat *) __asm("_" "stat" "$INODE64");
int mknod(const char *, mode_t, dev_t);
mode_t umask(mode_t);







int chflags(const char *, __uint32_t);
int chmodx_np(const char *, filesec_t);
int fchflags(int, __uint32_t);
int fchmodx_np(int, filesec_t);
int fstatx_np(int, struct stat *, filesec_t) __asm("_" "fstatx_np" "$INODE64");
int lchflags(const char *, __uint32_t) __attribute__((visibility("default")));
int lchmod(const char *, mode_t) __attribute__((visibility("default")));
int lstatx_np(const char *, struct stat *, filesec_t) __asm("_" "lstatx_np" "$INODE64");
int mkdirx_np(const char *, filesec_t);
int mkfifox_np(const char *, filesec_t);
int statx_np(const char *, struct stat *, filesec_t) __asm("_" "statx_np" "$INODE64");
int umaskx_np(filesec_t) __attribute__((deprecated,visibility("default")));



int fstatx64_np(int, struct stat64 *, filesec_t) __attribute__((deprecated,visibility("default")));
int lstatx64_np(const char *, struct stat64 *, filesec_t) __attribute__((deprecated,visibility("default")));
int statx64_np(const char *, struct stat64 *, filesec_t) __attribute__((deprecated,visibility("default")));
int fstat64(int, struct stat64 *) __attribute__((deprecated,visibility("default")));
int lstat64(const char *, struct stat64 *) __attribute__((deprecated,visibility("default")));
int stat64(const char *, struct stat64 *) __attribute__((deprecated,visibility("default")));
# 382 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/system.h" 2
# 493 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/system.h"
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/libiberty.h" 1
# 46 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/libiberty.h"
# 1 "/Users/ddunbar/rt/10907846/clang-211.10.1-Slate/Developer/usr/bin/../lib/clang/3.0/include/stddef.h" 1 3 4
# 47 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/libiberty.h" 2
# 59 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/libiberty.h"
extern char **buildargv (const char *) __attribute__ ((__malloc__));



extern void freeargv (char **);




extern char **dupargv (char **) __attribute__ ((__malloc__));
# 91 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/libiberty.h"
extern const char *lbasename (const char *);





extern char *concat (const char *, ...) __attribute__ ((__malloc__));
# 106 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/libiberty.h"
extern char *reconcat (char *, const char *, ...) __attribute__ ((__malloc__));





extern unsigned long concat_length (const char *, ...);






extern char *concat_copy (char *, const char *, ...);






extern char *concat_copy2 (const char *, ...);



extern char *libiberty_concat_ptr;
# 142 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/libiberty.h"
extern int fdmatch (int fd1, int fd2);




extern char * getpwd (void);



extern long get_run_time (void);



extern char *choose_temp_base (void) __attribute__ ((__malloc__));



extern char *make_temp_file (const char *) __attribute__ ((__malloc__));



extern const char *spaces (int count);




extern int errno_max (void);




extern const char *strerrno (int);



extern int strtoerrno (const char *);



extern char *xstrerror (int);




extern int signo_max (void);
# 198 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/libiberty.h"
extern const char *strsigno (int);



extern int strtosigno (const char *);



extern int xatexit (void (*fn) (void));



extern void xexit (int status) __attribute__ ((__noreturn__));



extern void xmalloc_set_program_name (const char *);


extern void xmalloc_failed (size_t) __attribute__ ((__noreturn__));





extern void * xmalloc (size_t) __attribute__ ((__malloc__));





extern void * xrealloc (void *, size_t);




extern void * xcalloc (size_t, size_t) __attribute__ ((__malloc__));



extern char *xstrdup (const char *) __attribute__ ((__malloc__));



extern void * xmemdup (const void *, size_t, size_t) __attribute__ ((__malloc__));


extern void specqsort (void *, int, int, int (*)() );





extern const char _hex_value[256];
extern void hex_init (void);
# 268 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/libiberty.h"
extern int pexecute (const char *, char * const *, const char *, const char *, char **, char **, int);




extern int pwait (int, int *, int);




extern int asprintf (char **, const char *, ...) __attribute__ ((__format__ (__printf__, 2, 3)));




extern int vasprintf (char **, const char *, va_list)
  __attribute__ ((__format__ (__printf__, 2, 0)));
# 494 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/system.h" 2
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/symcat.h" 1
# 495 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/system.h" 2
# 79 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c" 2
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.h" 1
# 25 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.h"
struct function;


# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/machmode.h" 1
# 29 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/machmode.h"
enum machine_mode {

# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/machmode.def" 1
# 74 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/machmode.def"
VOIDmode,

BImode,
QImode,
HImode,
SImode,
DImode,
TImode,
OImode,




PQImode,
PHImode,
PSImode,
PDImode,

QFmode,
HFmode,
TQFmode,
SFmode,
DFmode,
XFmode,
TFmode,


QCmode,
HCmode,
SCmode,
DCmode,
XCmode,
TCmode,

CQImode,
CHImode,
CSImode,
CDImode,
CTImode,
COImode,







V2QImode,
V2HImode,
V2SImode,
V2DImode,

V4QImode,
V4HImode,
V4SImode,
V4DImode,

V8QImode,
V8HImode,
V8SImode,
V8DImode,

V16QImode,

V2SFmode,
V2DFmode,

V4SFmode,
V4DFmode,

V8SFmode,
V8DFmode,
V16SFmode,



BLKmode,
# 159 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/machmode.def"
CCmode,


CCGCmode, CCGOCmode, CCNOmode, CCZmode, CCFPmode, CCFPUmode,
# 31 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/machmode.h" 2
MAX_MACHINE_MODE };
# 41 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/machmode.h"
extern const char * const mode_name[(int) MAX_MACHINE_MODE];


enum mode_class { MODE_RANDOM, MODE_INT, MODE_FLOAT, MODE_PARTIAL_INT, MODE_CC,
    MODE_COMPLEX_INT, MODE_COMPLEX_FLOAT,
    MODE_VECTOR_INT, MODE_VECTOR_FLOAT,
    MAX_MODE_CLASS};




extern const enum mode_class mode_class[(int) MAX_MACHINE_MODE];
# 80 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/machmode.h"
extern const unsigned char mode_size[(int) MAX_MACHINE_MODE];




extern const unsigned char mode_unit_size[(int) MAX_MACHINE_MODE];
# 96 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/machmode.h"
extern const unsigned short mode_bitsize[(int) MAX_MACHINE_MODE];
# 106 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/machmode.h"
extern const unsigned long long mode_mask_array[(int) MAX_MACHINE_MODE];



extern const enum machine_mode inner_mode_array[(int) MAX_MACHINE_MODE];
# 123 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/machmode.h"
extern const unsigned char mode_wider_mode[(int) MAX_MACHINE_MODE];






extern enum machine_mode mode_for_size (unsigned int, enum mode_class, int);




extern enum machine_mode smallest_mode_for_size
    (unsigned int, enum mode_class);





extern enum machine_mode int_mode_for_mode (enum machine_mode);



extern enum machine_mode get_best_mode (int, int, unsigned int, enum machine_mode, int);




extern unsigned get_mode_alignment (enum machine_mode);





extern const enum machine_mode class_narrowest_mode[(int) MAX_MODE_CLASS];





extern enum machine_mode byte_mode;
extern enum machine_mode word_mode;
extern enum machine_mode ptr_mode;
# 28 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.h" 2
# 41 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.h"
enum rtx_code {



# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def" 1
# 70 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
UNKNOWN ,



NIL ,




INCLUDE ,






EXPR_LIST ,



INSN_LIST ,
# 129 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
MATCH_OPERAND ,






MATCH_SCRATCH ,




MATCH_DUP ,







MATCH_OPERATOR ,
# 158 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
MATCH_PARALLEL ,




MATCH_OP_DUP ,




MATCH_PAR_DUP ,




MATCH_INSN ,
# 192 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
DEFINE_INSN ,







DEFINE_PEEPHOLE ,
# 211 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
DEFINE_SPLIT ,
# 239 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
DEFINE_INSN_AND_SPLIT ,



DEFINE_PEEPHOLE2 ,



DEFINE_COMBINE ,
# 260 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
DEFINE_EXPAND ,
# 276 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
DEFINE_DELAY ,
# 317 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
DEFINE_FUNCTION_UNIT ,


DEFINE_ASM_ATTRIBUTES ,
# 333 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
DEFINE_COND_EXEC ,





SEQUENCE ,


ADDRESS ,
# 353 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
DEFINE_ATTR ,


ATTR ,







SET_ATTR ,
# 379 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
SET_ATTR_ALTERNATIVE ,




EQ_ATTR ,
# 394 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
ATTR_FLAG ,
# 407 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
INSN ,



JUMP_INSN ,






CALL_INSN ,


BARRIER ,
# 430 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
CODE_LABEL ,






NOTE ,
# 450 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
COND_EXEC ,


PARALLEL ,







ASM_INPUT ,
# 475 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
ASM_OPERANDS ,
# 486 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
UNSPEC ,


UNSPEC_VOLATILE ,



ADDR_VEC ,
# 518 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
ADDR_DIFF_VEC ,
# 529 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
PREFETCH ,
# 541 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
SET ,




USE ,




CLOBBER ,





CALL ,



RETURN ,





TRAP_IF ,




RESX ,






CONST_INT ,






CONST_DOUBLE ,


CONST_VECTOR ,


CONST_STRING ,





CONST ,



PC ,


VALUE ,
# 614 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
REG ,






SCRATCH ,
# 631 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
SUBREG ,
# 644 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
STRICT_LOW_PART ,





CONCAT ,




MEM ,





LABEL_REF ,





SYMBOL_REF ,






CC0 ,
# 683 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
ADDRESSOF ,
# 701 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
QUEUED ,
# 713 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
IF_THEN_ELSE ,
# 722 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
COND ,


COMPARE ,


PLUS ,


MINUS ,


NEG ,

MULT ,


DIV ,

MOD ,


UDIV ,
UMOD ,


AND ,

IOR ,

XOR ,

NOT ,




ASHIFT ,
ROTATE ,
ASHIFTRT ,
LSHIFTRT ,
ROTATERT ,





SMIN ,
SMAX ,
UMIN ,
UMAX ,
# 781 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
PRE_DEC ,
PRE_INC ,
POST_DEC ,
POST_INC ,
# 798 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
PRE_MODIFY ,
POST_MODIFY ,



NE ,
EQ ,
GE ,
GT ,
LE ,
LT ,
GEU ,
GTU ,
LEU ,
LTU ,


UNORDERED ,
ORDERED ,


UNEQ ,
UNGE ,
UNGT ,
UNLE ,
UNLT ,


LTGT ,




SIGN_EXTEND ,


ZERO_EXTEND ,


TRUNCATE ,


FLOAT_EXTEND ,
FLOAT_TRUNCATE ,


FLOAT ,







FIX ,


UNSIGNED_FLOAT ,




UNSIGNED_FIX ,


ABS ,


SQRT ,




FFS ,
# 882 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
SIGN_EXTRACT ,


ZERO_EXTRACT ,




HIGH ,



LO_SUM ,
# 907 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
RANGE_INFO ,
# 922 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
RANGE_REG ,





RANGE_VAR ,



RANGE_LIVE ,




CONSTANT_P_RTX ,
# 958 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
CALL_PLACEHOLDER ,






VEC_MERGE ,





VEC_SELECT ,




VEC_CONCAT ,





VEC_DUPLICATE ,


SS_PLUS ,


US_PLUS ,


SS_MINUS ,


US_MINUS ,


SS_TRUNCATE ,


US_TRUNCATE ,
# 1014 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.def"
PHI ,
# 45 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.h" 2


  LAST_AND_UNUSED_RTX_CODE};






extern const unsigned char rtx_length[((int) LAST_AND_UNUSED_RTX_CODE)];


extern const char * const rtx_name[((int) LAST_AND_UNUSED_RTX_CODE)];


extern const char * const rtx_format[((int) LAST_AND_UNUSED_RTX_CODE)];


extern const char rtx_class[((int) LAST_AND_UNUSED_RTX_CODE)];




typedef struct
{

  unsigned min_align: 8;

  unsigned base_after_vec: 1;
  unsigned min_after_vec: 1;

  unsigned max_after_vec: 1;

  unsigned min_after_base: 1;

  unsigned max_after_base: 1;


  unsigned offset_unsigned: 1;
  unsigned : 2;
  unsigned scale : 8;
} addr_diff_vec_flags;





typedef struct
{
  long long alias;
  tree expr;
  rtx offset;
  rtx size;
  unsigned int align;
} mem_attrs;



typedef union rtunion_def
{
  long long rtwint;
  int rtint;
  unsigned int rtuint;
  const char *rtstr;
  rtx rtx;
  rtvec rtvec;
  enum machine_mode rttype;
  addr_diff_vec_flags rt_addr_diff_vec_flags;
  struct cselib_val_struct *rt_cselib;
  struct bitmap_head_def *rtbit;
  tree rttree;
  struct basic_block_def *bb;
  mem_attrs *rtmem;
} rtunion;



struct rtx_def
{

  enum rtx_code code: 16;


  enum machine_mode mode : 8;






  unsigned int jump : 1;


  unsigned int call : 1;
# 149 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.h"
  unsigned int unchanging : 1;







  unsigned int volatil : 1;
# 175 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.h"
  unsigned int in_struct : 1;






  unsigned int used : 1;




  unsigned integrated : 1;
# 196 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.h"
  unsigned frame_related : 1;




  rtunion fld[1];
};
# 222 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.h"
struct rtvec_def {
  int num_elem;
  rtx elem[1];
};
# 451 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.h"
enum reg_note
{



  REG_DEAD = 1,


  REG_INC,
# 470 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.h"
  REG_EQUIV,




  REG_EQUAL,





  REG_WAS_0,





  REG_RETVAL,




  REG_LIBCALL,






  REG_NONNEG,



  REG_NO_CONFLICT,


  REG_UNUSED,
# 515 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.h"
  REG_CC_SETTER, REG_CC_USER,




  REG_LABEL,





  REG_DEP_ANTI, REG_DEP_OUTPUT,





  REG_BR_PROB,




  REG_EXEC_COUNT,



  REG_NOALIAS,



  REG_SAVE_AREA,





  REG_BR_PRED,




  REG_FRAME_RELATED_EXPR,




  REG_EH_CONTEXT,





  REG_EH_REGION,


  REG_SAVE_NOTE,





  REG_MAYBE_DEAD,


  REG_NORETURN,



  REG_NON_LOCAL_GOTO,



  REG_SETJMP,


  REG_ALWAYS_RETURN,



  REG_VTABLE_REF
};
# 607 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.h"
extern const char * const reg_note_name[];
# 663 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.h"
enum insn_note
{

  NOTE_INSN_BIAS = -100,



  NOTE_INSN_DELETED,



  NOTE_INSN_BLOCK_BEG,
  NOTE_INSN_BLOCK_END,


  NOTE_INSN_LOOP_BEG,
  NOTE_INSN_LOOP_END,


  NOTE_INSN_LOOP_CONT,

  NOTE_INSN_LOOP_VTOP,





  NOTE_INSN_LOOP_END_TOP_COND,






  NOTE_INSN_FUNCTION_END,


  NOTE_INSN_PROLOGUE_END,


  NOTE_INSN_EPILOGUE_BEG,


  NOTE_INSN_DELETED_LABEL,




  NOTE_INSN_FUNCTION_BEG,



  NOTE_INSN_EH_REGION_BEG,
  NOTE_INSN_EH_REGION_END,




  NOTE_INSN_REPEATED_LINE_NUMBER,



  NOTE_INSN_RANGE_BEG,
  NOTE_INSN_RANGE_END,


  NOTE_INSN_LIVE,


  NOTE_INSN_BASIC_BLOCK,



  NOTE_INSN_EXPECTED_VALUE,

  NOTE_INSN_MAX
};



extern const char * const note_insn_name[NOTE_INSN_MAX - NOTE_INSN_BIAS];
# 840 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.h"
extern unsigned int subreg_lsb (rtx);
extern unsigned int subreg_regno_offset (unsigned int, enum machine_mode, unsigned int, enum machine_mode);



extern unsigned int subreg_regno (rtx);
# 1211 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.h"
extern int rtx_equal_function_value_matters;


extern int generating_concat_p;




extern int ceil_log2 (unsigned long long);




extern rtx expand_builtin_expect_jump (tree, rtx, rtx);


extern void set_stack_check_libfunc (rtx);
extern long long trunc_int_for_mode (long long, enum machine_mode);

extern rtx plus_constant_wide (rtx, long long);
extern rtx plus_constant_for_output_wide (rtx, long long);
extern void optimize_save_area_alloca (rtx);


extern rtx gen_rtx (enum rtx_code, enum machine_mode, ...);

extern rtvec gen_rtvec (int, ...);
extern rtx copy_insn_1 (rtx);
extern rtx copy_insn (rtx);
extern rtx gen_int_mode (long long, enum machine_mode);



extern rtx rtx_alloc (enum rtx_code);
extern rtvec rtvec_alloc (int);
extern rtx copy_rtx (rtx);


extern rtx copy_rtx_if_shared (rtx);


extern rtx copy_most_rtx (rtx, rtx);
extern rtx shallow_copy_rtx (rtx);
extern int rtx_equal_p (rtx, rtx);


extern rtvec gen_rtvec_v (int, rtx *);
extern rtx gen_reg_rtx (enum machine_mode);
extern rtx gen_label_rtx (void);
extern int subreg_hard_regno (rtx, int);
extern rtx gen_lowpart_common (enum machine_mode, rtx);
extern rtx gen_lowpart (enum machine_mode, rtx);


extern rtx gen_lowpart_if_possible (enum machine_mode, rtx);


extern rtx gen_highpart (enum machine_mode, rtx);
extern rtx gen_highpart_mode (enum machine_mode, enum machine_mode, rtx);

extern rtx gen_realpart (enum machine_mode, rtx);
extern rtx gen_imagpart (enum machine_mode, rtx);
extern rtx operand_subword (rtx, unsigned int, int, enum machine_mode);

extern rtx constant_subword (rtx, int, enum machine_mode);



extern rtx operand_subword_force (rtx, unsigned int, enum machine_mode);

extern int subreg_lowpart_p (rtx);
extern unsigned int subreg_lowpart_offset (enum machine_mode, enum machine_mode);

extern unsigned int subreg_highpart_offset (enum machine_mode, enum machine_mode);

extern rtx make_safe_from (rtx, rtx);
extern rtx convert_memory_address (enum machine_mode, rtx);
extern rtx get_insns (void);
extern const char *get_insn_name (int);
extern rtx get_last_insn (void);
extern rtx get_last_insn_anywhere (void);
extern void start_sequence (void);
extern void push_to_sequence (rtx);
extern void end_sequence (void);
extern void push_to_full_sequence (rtx, rtx);
extern void end_full_sequence (rtx*, rtx*);
extern rtx gen_sequence (void);


extern rtx immed_double_const (long long, long long, enum machine_mode);
extern rtx mem_for_const_double (rtx);
extern rtx force_const_mem (enum machine_mode, rtx);


extern rtx get_pool_constant (rtx);
extern rtx get_pool_constant_mark (rtx, _Bool *);
extern enum machine_mode get_pool_mode (rtx);
extern rtx get_pool_constant_for_function (struct function *, rtx);
extern enum machine_mode get_pool_mode_for_function (struct function *, rtx);
extern int get_pool_offset (rtx);
extern rtx simplify_subtraction (rtx);


extern rtx assign_stack_local (enum machine_mode, long long, int);

extern rtx assign_stack_temp (enum machine_mode, long long, int);

extern rtx assign_stack_temp_for_type (enum machine_mode, long long, int, tree);

extern rtx assign_temp (tree, int, int, int);

extern rtx emit_insn_before (rtx, rtx);
extern rtx emit_jump_insn_before (rtx, rtx);
extern rtx emit_call_insn_before (rtx, rtx);
extern rtx emit_barrier_before (rtx);
extern rtx emit_label_before (rtx, rtx);
extern rtx emit_note_before (int, rtx);
extern rtx emit_insn_after (rtx, rtx);
extern rtx emit_jump_insn_after (rtx, rtx);
extern rtx emit_barrier_after (rtx);
extern rtx emit_label_after (rtx, rtx);
extern rtx emit_note_after (int, rtx);
extern rtx emit_line_note_after (const char *, int, rtx);
extern rtx emit_insn (rtx);
extern rtx emit_insns (rtx);
extern rtx emit_insns_before (rtx, rtx);
extern rtx emit_insns_after (rtx, rtx);
extern rtx emit_jump_insn (rtx);
extern rtx emit_call_insn (rtx);
extern rtx emit_label (rtx);
extern rtx emit_barrier (void);
extern rtx emit_line_note (const char *, int);
extern rtx emit_note (const char *, int);
extern rtx emit_line_note_force (const char *, int);
extern rtx make_insn_raw (rtx);
extern rtx previous_insn (rtx);
extern rtx next_insn (rtx);
extern rtx prev_nonnote_insn (rtx);
extern rtx next_nonnote_insn (rtx);
extern rtx prev_real_insn (rtx);
extern rtx next_real_insn (rtx);
extern rtx prev_active_insn (rtx);
extern rtx next_active_insn (rtx);
extern int active_insn_p (rtx);
extern rtx prev_label (rtx);
extern rtx next_label (rtx);
extern rtx next_cc0_user (rtx);
extern rtx prev_cc0_setter (rtx);


extern rtx next_nondeleted_insn (rtx);
extern enum rtx_code reverse_condition (enum rtx_code);
extern enum rtx_code reverse_condition_maybe_unordered (enum rtx_code);
extern enum rtx_code swap_condition (enum rtx_code);
extern enum rtx_code unsigned_condition (enum rtx_code);
extern enum rtx_code signed_condition (enum rtx_code);
extern void mark_jump_label (rtx, rtx, int);
extern void cleanup_barriers (void);


extern _Bool squeeze_notes (rtx *, rtx *);
extern rtx delete_related_insns (rtx);
extern void delete_jump (rtx);
extern void delete_barrier (rtx);
extern rtx get_label_before (rtx);
extern rtx get_label_after (rtx);
extern rtx follow_jumps (rtx);


extern rtx *find_constant_term_loc (rtx *);


extern rtx try_split (rtx, rtx, int);
extern int split_branch_probability;


extern rtx split_insns (rtx, rtx);


extern rtx simplify_unary_operation (enum rtx_code, enum machine_mode, rtx, enum machine_mode);


extern rtx simplify_binary_operation (enum rtx_code, enum machine_mode, rtx, rtx);


extern rtx simplify_ternary_operation (enum rtx_code, enum machine_mode, enum machine_mode, rtx, rtx, rtx);



extern rtx simplify_relational_operation (enum rtx_code, enum machine_mode, rtx, rtx);


extern rtx simplify_gen_binary (enum rtx_code, enum machine_mode, rtx, rtx);


extern rtx simplify_gen_unary (enum rtx_code, enum machine_mode, rtx, enum machine_mode);


extern rtx simplify_gen_ternary (enum rtx_code, enum machine_mode, enum machine_mode, rtx, rtx, rtx);



extern rtx simplify_gen_relational (enum rtx_code, enum machine_mode, enum machine_mode, rtx, rtx);



extern rtx simplify_subreg (enum machine_mode, rtx, enum machine_mode, unsigned int);



extern rtx simplify_gen_subreg (enum machine_mode, rtx, enum machine_mode, unsigned int);



extern rtx simplify_replace_rtx (rtx, rtx, rtx);
extern rtx simplify_rtx (rtx);
extern rtx avoid_constant_pool_reference (rtx);


extern rtx gen_mem_addressof (rtx, tree);


extern enum machine_mode choose_hard_reg_mode (unsigned int, unsigned int);



extern rtx set_unique_reg_note (rtx, enum reg_note, rtx);
# 1448 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.h"
extern int rtx_addr_can_trap_p (rtx);
extern int rtx_unstable_p (rtx);
extern int rtx_varies_p (rtx, int);
extern int rtx_addr_varies_p (rtx, int);
extern long long get_integer_term (rtx);
extern rtx get_related_value (rtx);
extern rtx get_jump_table_offset (rtx, rtx *);
extern int reg_mentioned_p (rtx, rtx);
extern int count_occurrences (rtx, rtx, int);
extern int reg_referenced_p (rtx, rtx);
extern int reg_used_between_p (rtx, rtx, rtx);
extern int reg_referenced_between_p (rtx, rtx, rtx);
extern int reg_set_between_p (rtx, rtx, rtx);
extern int regs_set_between_p (rtx, rtx, rtx);
extern int commutative_operand_precedence (rtx);
extern int swap_commutative_operands_p (rtx, rtx);
extern int modified_between_p (rtx, rtx, rtx);
extern int no_labels_between_p (rtx, rtx);
extern int no_jumps_between_p (rtx, rtx);
extern int modified_in_p (rtx, rtx);
extern int insn_dependent_p (rtx, rtx);
extern int reg_set_p (rtx, rtx);
extern rtx single_set_2 (rtx, rtx);
extern int multiple_sets (rtx);
extern int set_noop_p (rtx);
extern int noop_move_p (rtx);
extern rtx find_last_value (rtx, rtx *, rtx, int);
extern int refers_to_regno_p (unsigned int, unsigned int, rtx, rtx *);

extern int reg_overlap_mentioned_p (rtx, rtx);
extern rtx set_of (rtx, rtx);
extern void note_stores (rtx, void (*) (rtx, rtx, void *), void *);


extern void note_uses (rtx *, void (*) (rtx *, void *), void *);


extern rtx reg_set_last (rtx, rtx);
extern int dead_or_set_p (rtx, rtx);
extern int dead_or_set_regno_p (rtx, unsigned int);
extern rtx find_reg_note (rtx, enum reg_note, rtx);
extern rtx find_regno_note (rtx, enum reg_note, unsigned int);

extern rtx find_reg_equal_equiv_note (rtx);
extern int find_reg_fusage (rtx, enum rtx_code, rtx);
extern int find_regno_fusage (rtx, enum rtx_code, unsigned int);

extern int pure_call_p (rtx);
extern void remove_note (rtx, rtx);
extern int side_effects_p (rtx);
extern int volatile_refs_p (rtx);
extern int volatile_insn_p (rtx);
extern int may_trap_p (rtx);
extern int inequality_comparisons_p (rtx);
extern rtx replace_rtx (rtx, rtx, rtx);
extern rtx replace_regs (rtx, rtx *, unsigned int, int);

extern int computed_jump_p (rtx);
typedef int (*rtx_function) (rtx *, void *);
extern int for_each_rtx (rtx *, rtx_function, void *);
extern rtx regno_use_in (unsigned int, rtx);
extern int auto_inc_p (rtx);
extern int in_expr_list_p (rtx, rtx);
extern void remove_node_from_expr_list (rtx, rtx *);
extern int insns_safe_to_move_p (rtx, rtx, rtx *);
extern int loc_mentioned_in_p (rtx *, rtx);
extern rtx find_first_parameter_load (rtx, rtx);



extern rtx find_use_as_address (rtx, rtx, long long);
void init_EXPR_INSN_LIST_cache (void);
void free_EXPR_LIST_list (rtx *);
void free_INSN_LIST_list (rtx *);
void free_EXPR_LIST_node (rtx);
void free_INSN_LIST_node (rtx);
rtx alloc_INSN_LIST (rtx, rtx);
rtx alloc_EXPR_LIST (int, rtx, rtx);







extern int max_parallel;


extern void free_reg_info (void);


extern int asm_noperands (rtx);
extern const char *decode_asm_operands (rtx, rtx *, rtx **, const char **, enum machine_mode *);



extern enum reg_class reg_preferred_class (int);
extern enum reg_class reg_alternate_class (int);

extern rtx get_first_nonparm_insn (void);

extern void split_all_insns (int);
extern void split_all_insns_noflow (void);


extern rtx const_int_rtx[64 * 2 + 1];





extern rtx const_true_rtx;

extern rtx const_tiny_rtx[3][(int) MAX_MACHINE_MODE];
# 1584 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.h"
enum global_rtl_index
{
  GR_PC,
  GR_CC0,
  GR_STACK_POINTER,
  GR_FRAME_POINTER,
# 1599 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.h"
  GR_HARD_FRAME_POINTER,





  GR_ARG_POINTER,


  GR_VIRTUAL_INCOMING_ARGS,
  GR_VIRTUAL_STACK_ARGS,
  GR_VIRTUAL_STACK_DYNAMIC,
  GR_VIRTUAL_OUTGOING_ARGS,
  GR_VIRTUAL_CFA,

  GR_MAX
};


extern rtx global_rtl[GR_MAX];
# 1632 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.h"
extern rtx pic_offset_table_rtx;
extern rtx struct_value_rtx;
extern rtx struct_value_incoming_rtx;
extern rtx static_chain_rtx;
extern rtx static_chain_incoming_rtx;
extern rtx return_address_pointer_rtx;





# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/genrtl.h" 1





extern rtx gen_rtx_fmt_s (enum rtx_code, enum machine_mode mode, const char *arg0);

extern rtx gen_rtx_fmt_ee (enum rtx_code, enum machine_mode mode, rtx arg0, rtx arg1);

extern rtx gen_rtx_fmt_ue (enum rtx_code, enum machine_mode mode, rtx arg0, rtx arg1);

extern rtx gen_rtx_fmt_iss (enum rtx_code, enum machine_mode mode, int arg0, const char *arg1, const char *arg2);


extern rtx gen_rtx_fmt_is (enum rtx_code, enum machine_mode mode, int arg0, const char *arg1);

extern rtx gen_rtx_fmt_i (enum rtx_code, enum machine_mode mode, int arg0);

extern rtx gen_rtx_fmt_isE (enum rtx_code, enum machine_mode mode, int arg0, const char *arg1, rtvec arg2);


extern rtx gen_rtx_fmt_iE (enum rtx_code, enum machine_mode mode, int arg0, rtvec arg1);

extern rtx gen_rtx_fmt_Ess (enum rtx_code, enum machine_mode mode, rtvec arg0, const char *arg1, const char *arg2);


extern rtx gen_rtx_fmt_sEss (enum rtx_code, enum machine_mode mode, const char *arg0, rtvec arg1, const char *arg2, const char *arg3);


extern rtx gen_rtx_fmt_eE (enum rtx_code, enum machine_mode mode, rtx arg0, rtvec arg1);

extern rtx gen_rtx_fmt_E (enum rtx_code, enum machine_mode mode, rtvec arg0);

extern rtx gen_rtx_fmt_e (enum rtx_code, enum machine_mode mode, rtx arg0);

extern rtx gen_rtx_fmt_sse (enum rtx_code, enum machine_mode mode, const char *arg0, const char *arg1, rtx arg2);


extern rtx gen_rtx_fmt_ss (enum rtx_code, enum machine_mode mode, const char *arg0, const char *arg1);

extern rtx gen_rtx_fmt_sE (enum rtx_code, enum machine_mode mode, const char *arg0, rtvec arg1);

extern rtx gen_rtx_fmt_iuueiee (enum rtx_code, enum machine_mode mode, int arg0, rtx arg1, rtx arg2, rtx arg3, int arg4, rtx arg5, rtx arg6);



extern rtx gen_rtx_fmt_iuueiee0 (enum rtx_code, enum machine_mode mode, int arg0, rtx arg1, rtx arg2, rtx arg3, int arg4, rtx arg5, rtx arg6);



extern rtx gen_rtx_fmt_iuueieee (enum rtx_code, enum machine_mode mode, int arg0, rtx arg1, rtx arg2, rtx arg3, int arg4, rtx arg5, rtx arg6, rtx arg7);



extern rtx gen_rtx_fmt_iuu (enum rtx_code, enum machine_mode mode, int arg0, rtx arg1, rtx arg2);

extern rtx gen_rtx_fmt_iuu00iss (enum rtx_code, enum machine_mode mode, int arg0, rtx arg1, rtx arg2, int arg3, const char *arg4, const char *arg5);



extern rtx gen_rtx_fmt_ssiEEsi (enum rtx_code, enum machine_mode mode, const char *arg0, const char *arg1, int arg2, rtvec arg3, rtvec arg4, const char *arg5, int arg6);



extern rtx gen_rtx_fmt_Ei (enum rtx_code, enum machine_mode mode, rtvec arg0, int arg1);

extern rtx gen_rtx_fmt_eEee0 (enum rtx_code, enum machine_mode mode, rtx arg0, rtvec arg1, rtx arg2, rtx arg3);


extern rtx gen_rtx_fmt_eee (enum rtx_code, enum machine_mode mode, rtx arg0, rtx arg1, rtx arg2);

extern rtx gen_rtx_fmt_ (enum rtx_code, enum machine_mode mode);
extern rtx gen_rtx_fmt_w (enum rtx_code, enum machine_mode mode, long long arg0);

extern rtx gen_rtx_fmt_0www (enum rtx_code, enum machine_mode mode, long long arg0, long long arg1, long long arg2);



extern rtx gen_rtx_fmt_0 (enum rtx_code, enum machine_mode mode);
extern rtx gen_rtx_fmt_i0 (enum rtx_code, enum machine_mode mode, int arg0);

extern rtx gen_rtx_fmt_ei (enum rtx_code, enum machine_mode mode, rtx arg0, int arg1);

extern rtx gen_rtx_fmt_e0 (enum rtx_code, enum machine_mode mode, rtx arg0);

extern rtx gen_rtx_fmt_u00 (enum rtx_code, enum machine_mode mode, rtx arg0);

extern rtx gen_rtx_fmt_eit (enum rtx_code, enum machine_mode mode, rtx arg0, int arg1, union tree_node *arg2);


extern rtx gen_rtx_fmt_eeeee (enum rtx_code, enum machine_mode mode, rtx arg0, rtx arg1, rtx arg2, rtx arg3, rtx arg4);


extern rtx gen_rtx_fmt_Ee (enum rtx_code, enum machine_mode mode, rtvec arg0, rtx arg1);

extern rtx gen_rtx_fmt_uuEiiiiiibbii (enum rtx_code, enum machine_mode mode, rtx arg0, rtx arg1, rtvec arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, struct bitmap_head_def *arg9, struct bitmap_head_def *arg10, int arg11, int arg12);






extern rtx gen_rtx_fmt_iiiiiiiitt (enum rtx_code, enum machine_mode mode, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, union tree_node *arg8, union tree_node *arg9);





extern rtx gen_rtx_fmt_eti (enum rtx_code, enum machine_mode mode, rtx arg0, union tree_node *arg1, int arg2);


extern rtx gen_rtx_fmt_bi (enum rtx_code, enum machine_mode mode, struct bitmap_head_def *arg0, int arg1);


extern rtx gen_rtx_fmt_uuuu (enum rtx_code, enum machine_mode mode, rtx arg0, rtx arg1, rtx arg2, rtx arg3);
# 1643 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.h" 2







extern rtx gen_rtx_CONST_DOUBLE (enum machine_mode, long long, long long);

extern rtx gen_rtx_CONST_INT (enum machine_mode, long long);
extern rtx gen_raw_REG (enum machine_mode, int);
extern rtx gen_rtx_REG (enum machine_mode, int);
extern rtx gen_rtx_SUBREG (enum machine_mode, rtx, int);
extern rtx gen_rtx_MEM (enum machine_mode, rtx);

extern rtx gen_lowpart_SUBREG (enum machine_mode, rtx);
# 1728 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.h"
extern rtx find_next_ref (rtx, rtx);

extern rtx output_constant_def (tree, int);
extern rtx immed_real_const (tree);
# 1741 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.h"
extern int flow2_completed;




extern int reload_completed;




extern int reload_in_progress;







extern int cse_not_expected;



extern int no_new_pseudos;





extern int rtx_to_tree_code (enum rtx_code);


struct obstack;
extern void gcc_obstack_init (struct obstack *);


struct cse_basic_block_data;
# 1787 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/rtl.h"
extern int rtx_cost (rtx, enum rtx_code);
extern int address_cost (rtx, enum machine_mode);
extern void delete_trivially_dead_insns (rtx, int, int);

extern int cse_main (rtx, int, int, FILE *);

extern void cse_end_of_basic_block (rtx, struct cse_basic_block_data *, int, int, int);




extern int comparison_dominates_p (enum rtx_code, enum rtx_code);
extern int condjump_p (rtx);
extern int any_condjump_p (rtx);
extern int any_uncondjump_p (rtx);
extern int safe_to_remove_jump_p (rtx);
extern rtx pc_set (rtx);
extern rtx condjump_label (rtx);
extern int simplejump_p (rtx);
extern int returnjump_p (rtx);
extern int onlyjump_p (rtx);
extern int only_sets_cc0_p (rtx);
extern int sets_cc0_p (rtx);
extern int invert_jump_1 (rtx, rtx);
extern int invert_jump (rtx, rtx, int);
extern int rtx_renumbered_equal_p (rtx, rtx);
extern int true_regnum (rtx);
extern int redirect_jump_1 (rtx, rtx);
extern int redirect_jump (rtx, rtx, int);
extern void rebuild_jump_labels (rtx);
extern enum rtx_code reversed_comparison_code (rtx, rtx);
extern enum rtx_code reversed_comparison_code_parts (enum rtx_code, rtx, rtx, rtx);

extern void delete_for_peephole (rtx, rtx);
extern int condjump_in_parallel_p (rtx);
extern void never_reached_warning (rtx, rtx);
extern void purge_line_number_notes (rtx);
extern void copy_loop_headers (rtx);


extern int max_reg_num (void);
extern int max_label_num (void);
extern int get_first_label_num (void);
extern void delete_insns_since (rtx);
extern void mark_reg_pointer (rtx, int);
extern void mark_user_reg (rtx);
extern void reset_used_flags (rtx);
extern void reorder_insns (rtx, rtx, rtx);
extern void reorder_insns_nobb (rtx, rtx, rtx);
extern int get_max_uid (void);
extern int in_sequence_p (void);
extern void force_next_line_note (void);
extern void clear_emit_caches (void);
extern void init_emit (void);
extern void init_emit_once (int);
extern void push_topmost_sequence (void);
extern void pop_topmost_sequence (void);
extern int subreg_realpart_p (rtx);
extern void reverse_comparison (rtx);
extern void set_new_first_and_last_insn (rtx, rtx);
extern void set_new_first_and_last_label_num (int, int);
extern void set_new_last_label_num (int);
extern void unshare_all_rtl_again (rtx);
extern void set_last_insn (rtx);
extern void link_cc0_insns (rtx);
extern void add_insn (rtx);
extern void add_insn_before (rtx, rtx);
extern void add_insn_after (rtx, rtx);
extern void remove_insn (rtx);
extern void reorder_insns_with_line_notes (rtx, rtx, rtx);
extern void emit_insn_after_with_line_notes (rtx, rtx, rtx);
extern enum rtx_code classify_insn (rtx);
extern rtx emit (rtx);



int force_line_numbers (void);
void restore_line_number_status (int old_value);
extern void renumber_insns (FILE *);
extern void remove_unnecessary_notes (void);
extern rtx delete_insn (rtx);
extern void delete_insn_chain (rtx, rtx);


extern int combine_instructions (rtx, unsigned int);
extern unsigned int extended_count (rtx, enum machine_mode, int);
extern rtx remove_death (unsigned int, rtx);

extern void dump_combine_stats (FILE *);
extern void dump_combine_total_stats (FILE *);




extern void schedule_insns (FILE *);
extern void schedule_ebbs (FILE *);

extern void fix_sched_param (const char *, const char *);


extern const char *print_rtx_head;
extern void debug_rtx (rtx);
extern void debug_rtx_list (rtx, int);
extern void debug_rtx_range (rtx, rtx);
extern rtx debug_rtx_find (rtx, int);

extern void print_mem_expr (FILE *, tree);
extern void print_rtl (FILE *, rtx);
extern void print_simple_rtl (FILE *, rtx);
extern int print_rtl_single (FILE *, rtx);
extern void print_inline_rtx (FILE *, rtx, int);



extern void init_loop (void);
extern rtx libcall_other_reg (rtx, rtx);

extern void loop_optimize (rtx, FILE *, int);

extern void record_excess_regs (rtx, rtx, rtx *);


extern void reposition_prologue_and_epilogue_notes (rtx);
extern void thread_prologue_and_epilogue_insns (rtx);
extern int prologue_epilogue_contains (rtx);
extern int sibcall_epilogue_contains (rtx);
extern void preserve_rtl_expr_result (rtx);
extern void mark_temp_addr_taken (rtx);
extern void update_temp_slot_address (rtx, rtx);
extern void purge_addressof (rtx);
extern void purge_hard_subreg_sets (rtx);


extern void set_file_and_line_for_stmt (const char *, int);
extern void expand_null_return (void);
extern void emit_jump (rtx);
extern int preserve_subexpressions_p (void);


extern void move_by_pieces (rtx, rtx, unsigned long long, unsigned int);




extern void recompute_reg_usage (rtx, int);
extern int initialize_uninitialized_subregs (void);

extern void print_rtl_with_bb (FILE *, rtx);
extern void dump_flow_info (FILE *);



extern void init_expmed (void);
extern void expand_inc (rtx, rtx);
extern void expand_dec (rtx, rtx);
extern rtx expand_mult_highpart (enum machine_mode, rtx, unsigned long long, rtx, int, int);





extern int gcse_main (rtx, FILE *);



extern void mark_elimination (int, int);

extern int global_alloc (FILE *);
extern void dump_global_regs (FILE *);




extern void build_insn_chain (rtx);


extern int reg_classes_intersect_p (enum reg_class, enum reg_class);
extern int reg_class_subset_p (enum reg_class, enum reg_class);
extern void globalize_reg (int);
extern void init_regs (void);
extern void init_reg_sets (void);
extern void regset_release_memory (void);
extern void regclass_init (void);
extern void regclass (rtx, int, FILE *);
extern void reg_scan (rtx, unsigned int, int);
extern void reg_scan_update (rtx, rtx, unsigned int);
extern void fix_register (const char *, int, int);

extern void delete_null_pointer_checks (rtx);



extern void regmove_optimize (rtx, int, FILE *);

extern void combine_stack_adjustments (void);



extern void dbr_schedule (rtx, FILE *);




extern void dump_local_alloc (FILE *);

extern int local_alloc (void);
extern int function_invariant_p (rtx);


extern void init_branch_prob (const char *);
extern void branch_prob (void);
extern void end_branch_prob (void);
extern void output_func_start_profiler (void);



extern void reg_to_stack (rtx, FILE *);



extern int add_double (unsigned long long, long long, unsigned long long, long long, unsigned long long *, long long *);



extern int neg_double (unsigned long long, long long, unsigned long long *, long long *);


extern int mul_double (unsigned long long, long long, unsigned long long, long long, unsigned long long *, long long *);




extern void lshift_double (unsigned long long, long long, long long, unsigned int, unsigned long long *, long long *, int);



extern void rshift_double (unsigned long long, long long, long long, unsigned int, unsigned long long *, long long *, int);



extern void lrotate_double (unsigned long long, long long, long long, unsigned int, unsigned long long *, long long *);



extern void rrotate_double (unsigned long long, long long, long long, unsigned int, unsigned long long *, long long *);





enum libcall_type
{
  LCT_NORMAL = 0,
  LCT_CONST = 1,
  LCT_PURE = 2,
  LCT_CONST_MAKE_BLOCK = 3,
  LCT_PURE_MAKE_BLOCK = 4,
  LCT_NORETURN = 5,
  LCT_THROW = 6,
  LCT_ALWAYS_RETURN = 7,
  LCT_RETURNS_TWICE = 8
};

extern void emit_library_call (rtx, enum libcall_type, enum machine_mode, int, ...);


extern rtx emit_library_call_value (rtx, rtx, enum libcall_type, enum machine_mode, int, ...);




extern int set_dominates_use (int, int, int, rtx, rtx);


extern int in_data_section (void);
extern void init_varasm_once (void);


extern void init_rtl (void);
extern void traverse_md_constants (int (*) (void **, void *), void *);

struct md_constant { char *name, *value; };


extern int read_skip_spaces (FILE *);
extern rtx read_rtx (FILE *);


extern const char *read_rtx_filename;
extern int read_rtx_lineno;







extern void fancy_abort (const char *, int, const char *)
    __attribute__ ((__noreturn__));



extern void clear_reg_alias_info (rtx);
extern rtx canon_rtx (rtx);
extern int true_dependence (rtx, enum machine_mode, rtx, int (*)(rtx, int));

extern rtx get_addr (rtx);
extern int canon_true_dependence (rtx, enum machine_mode, rtx, rtx, int (*)(rtx, int));

extern int read_dependence (rtx, rtx);
extern int anti_dependence (rtx, rtx);
extern int output_dependence (rtx, rtx);
extern void mark_constant_function (void);
extern void init_alias_once (void);
extern void init_alias_analysis (void);
extern void end_alias_analysis (void);
extern rtx addr_side_effect_eval (rtx, int, int);


typedef enum {
  sibcall_use_normal = 1,
  sibcall_use_tail_recursion,
  sibcall_use_sibcall
} sibcall_use_t;

extern void optimize_sibling_and_tail_recursive_calls (void);
extern void replace_call_placeholder (rtx, sibcall_use_t);


extern int stack_regs_mentioned (rtx insn);



extern rtx stack_limit_rtx;


extern void regrename_optimize (void);
extern void copyprop_hardreg_forward (void);


extern void if_convert (int);


extern void invert_br_probabilities (rtx);
extern _Bool expensive_function_p (int);
# 80 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c" 2
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/tm_p.h" 1
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/i386-protos.h" 1
# 23 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/i386-protos.h"
extern void override_options (void);
extern void optimization_options (int, int);

extern int ix86_can_use_return_insn_p (void);
extern int ix86_frame_pointer_required (void);
extern void ix86_setup_frame_addresses (void);

extern void ix86_asm_file_end (FILE *);
extern void load_pic_register (void);
extern long long ix86_initial_elimination_offset (int, int);
extern void ix86_expand_prologue (void);
extern void ix86_expand_epilogue (int);

extern void ix86_output_addr_vec_elt (FILE *, int);
extern void ix86_output_addr_diff_elt (FILE *, int, int);


extern int ix86_aligned_p (rtx);

extern int standard_80387_constant_p (rtx);
extern int standard_sse_constant_p (rtx);
extern int symbolic_reference_mentioned_p (rtx);

extern int x86_64_general_operand (rtx, enum machine_mode);
extern int x86_64_szext_general_operand (rtx, enum machine_mode);
extern int x86_64_nonmemory_operand (rtx, enum machine_mode);
extern int x86_64_szext_nonmemory_operand (rtx, enum machine_mode);
extern int x86_64_immediate_operand (rtx, enum machine_mode);
extern int x86_64_zext_immediate_operand (rtx, enum machine_mode);
extern int const_int_1_operand (rtx, enum machine_mode);
extern int symbolic_operand (rtx, enum machine_mode);
extern int pic_symbolic_operand (rtx, enum machine_mode);
extern int call_insn_operand (rtx, enum machine_mode);
extern int constant_call_address_operand (rtx, enum machine_mode);
extern int const0_operand (rtx, enum machine_mode);
extern int const1_operand (rtx, enum machine_mode);
extern int const248_operand (rtx, enum machine_mode);
extern int incdec_operand (rtx, enum machine_mode);
extern int reg_no_sp_operand (rtx, enum machine_mode);
extern int mmx_reg_operand (rtx, enum machine_mode);
extern int general_no_elim_operand (rtx, enum machine_mode);
extern int nonmemory_no_elim_operand (rtx, enum machine_mode);
extern int q_regs_operand (rtx, enum machine_mode);
extern int non_q_regs_operand (rtx, enum machine_mode);
extern int sse_comparison_operator (rtx, enum machine_mode);
extern int fcmov_comparison_operator (rtx, enum machine_mode);
extern int cmp_fp_expander_operand (rtx, enum machine_mode);
extern int ix86_comparison_operator (rtx, enum machine_mode);
extern int ext_register_operand (rtx, enum machine_mode);
extern int binary_fp_operator (rtx, enum machine_mode);
extern int mult_operator (rtx, enum machine_mode);
extern int div_operator (rtx, enum machine_mode);
extern int arith_or_logical_operator (rtx, enum machine_mode);
extern int promotable_binary_operator (rtx, enum machine_mode);
extern int memory_displacement_operand (rtx, enum machine_mode);
extern int cmpsi_operand (rtx, enum machine_mode);
extern int long_memory_operand (rtx, enum machine_mode);
extern int aligned_operand (rtx, enum machine_mode);
extern enum machine_mode ix86_cc_mode (enum rtx_code, rtx, rtx);

extern int ix86_expand_movstr (rtx, rtx, rtx, rtx);
extern int ix86_expand_clrstr (rtx, rtx, rtx);
extern int ix86_expand_strlen (rtx, rtx, rtx, rtx);

extern int legitimate_pic_address_disp_p (rtx);
extern int legitimate_address_p (enum machine_mode, rtx, int);
extern rtx legitimize_pic_address (rtx, rtx);
extern rtx legitimize_address (rtx, rtx, enum machine_mode);

extern void print_reg (rtx, int, FILE*);
extern void print_operand (FILE*, rtx, int);
extern void print_operand_address (FILE*, rtx);

extern void split_di (rtx[], int, rtx[], rtx[]);
extern void split_ti (rtx[], int, rtx[], rtx[]);

extern const char *output_387_binary_op (rtx, rtx*);
extern const char *output_fix_trunc (rtx, rtx*);
extern const char *output_fp_compare (rtx, rtx*, int, int);

extern void i386_dwarf_output_addr_const (FILE*, rtx);
extern rtx i386_simplify_dwarf_addr (rtx);

extern void ix86_expand_clear (rtx);
extern void ix86_expand_move (enum machine_mode, rtx[]);
extern void ix86_expand_vector_move (enum machine_mode, rtx[]);
extern void ix86_expand_binary_operator (enum rtx_code, enum machine_mode, rtx[]);

extern int ix86_binary_operator_ok (enum rtx_code, enum machine_mode, rtx[]);

extern void ix86_expand_unary_operator (enum rtx_code, enum machine_mode, rtx[]);

extern int ix86_unary_operator_ok (enum rtx_code, enum machine_mode, rtx[]);

extern int ix86_match_ccmode (rtx, enum machine_mode);
extern rtx ix86_expand_compare (enum rtx_code, rtx *, rtx *);
extern int ix86_use_fcomi_compare (enum rtx_code);
extern void ix86_expand_branch (enum rtx_code, rtx);
extern int ix86_expand_setcc (enum rtx_code, rtx);
extern int ix86_expand_int_movcc (rtx[]);
extern int ix86_expand_fp_movcc (rtx[]);
extern void x86_initialize_trampoline (rtx, rtx, rtx);
extern rtx ix86_zero_extend_to_Pmode (rtx);
extern void ix86_split_long_move (rtx[]);
extern void ix86_split_ashldi (rtx *, rtx);
extern void ix86_split_ashrdi (rtx *, rtx);
extern void ix86_split_lshrdi (rtx *, rtx);
extern int ix86_address_cost (rtx);
extern rtx ix86_find_base_term (rtx);

extern rtx assign_386_stack_local (enum machine_mode, int);
extern int ix86_attr_length_immediate_default (rtx, int);
extern int ix86_attr_length_address_default (rtx);

extern enum machine_mode ix86_fp_compare_mode (enum rtx_code);

extern int x86_64_sign_extended_value (rtx);
extern int x86_64_zero_extended_value (rtx);
extern rtx ix86_libcall_value (enum machine_mode);
extern _Bool ix86_function_value_regno_p (int);
extern _Bool ix86_function_arg_regno_p (int);
extern int ix86_function_arg_boundary (enum machine_mode, tree);
extern int ix86_return_in_memory (tree);
extern void ix86_va_start (int, tree, rtx);
extern rtx ix86_va_arg (tree, tree);
extern void ix86_setup_incoming_varargs (CUMULATIVE_ARGS *, enum machine_mode, tree, int *, int);



extern rtx ix86_force_to_memory (enum machine_mode, rtx);
extern void ix86_free_from_memory (enum machine_mode);
extern void ix86_split_fp_branch (enum rtx_code code, rtx, rtx, rtx, rtx, rtx);

extern int ix86_hard_regno_mode_ok (int, enum machine_mode);
extern int ix86_register_move_cost (enum machine_mode, enum reg_class, enum reg_class);

extern int ix86_secondary_memory_needed (enum reg_class, enum reg_class, enum machine_mode, int);


extern enum reg_class ix86_preferred_reload_class (rtx, enum reg_class);

extern int ix86_memory_move_cost (enum machine_mode, enum reg_class, int);

extern void ix86_set_move_mem_attrs (rtx, rtx, rtx, rtx, rtx);
extern void emit_i387_cw_initialization (rtx, rtx);
extern _Bool ix86_fp_jump_nontrivial_p (enum rtx_code);
extern void x86_order_regs_for_local_alloc (void);
# 2 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/tm_p.h" 2
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/tm-preds.h" 1







extern int x86_64_immediate_operand (rtx, enum machine_mode);
extern int x86_64_nonmemory_operand (rtx, enum machine_mode);
extern int x86_64_movabs_operand (rtx, enum machine_mode);
extern int x86_64_szext_nonmemory_operand (rtx, enum machine_mode);
extern int x86_64_general_operand (rtx, enum machine_mode);
extern int x86_64_szext_general_operand (rtx, enum machine_mode);
extern int x86_64_zext_immediate_operand (rtx, enum machine_mode);
extern int shiftdi_operand (rtx, enum machine_mode);
extern int const_int_1_operand (rtx, enum machine_mode);
extern int const_int_1_31_operand (rtx, enum machine_mode);
extern int symbolic_operand (rtx, enum machine_mode);
extern int aligned_operand (rtx, enum machine_mode);
extern int pic_symbolic_operand (rtx, enum machine_mode);
extern int call_insn_operand (rtx, enum machine_mode);
extern int constant_call_address_operand (rtx, enum machine_mode);
extern int const0_operand (rtx, enum machine_mode);
extern int const1_operand (rtx, enum machine_mode);
extern int const248_operand (rtx, enum machine_mode);
extern int incdec_operand (rtx, enum machine_mode);
extern int mmx_reg_operand (rtx, enum machine_mode);
extern int reg_no_sp_operand (rtx, enum machine_mode);
extern int general_no_elim_operand (rtx, enum machine_mode);
extern int nonmemory_no_elim_operand (rtx, enum machine_mode);
extern int q_regs_operand (rtx, enum machine_mode);
extern int non_q_regs_operand (rtx, enum machine_mode);
extern int fcmov_comparison_operator (rtx, enum machine_mode);
extern int sse_comparison_operator (rtx, enum machine_mode);
extern int ix86_comparison_operator (rtx, enum machine_mode);
extern int cmp_fp_expander_operand (rtx, enum machine_mode);
extern int ext_register_operand (rtx, enum machine_mode);
extern int binary_fp_operator (rtx, enum machine_mode);
extern int mult_operator (rtx, enum machine_mode);
extern int div_operator (rtx, enum machine_mode);
extern int arith_or_logical_operator (rtx, enum machine_mode);
extern int promotable_binary_operator (rtx, enum machine_mode);
extern int memory_displacement_operand (rtx, enum machine_mode);
extern int cmpsi_operand (rtx, enum machine_mode);
extern int long_memory_operand (rtx, enum machine_mode);
# 3 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/tm_p.h" 2
# 81 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c" 2
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/flags.h" 1
# 26 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/flags.h"
extern const char *main_input_filename;

enum debug_info_type
{
  NO_DEBUG,
  DBX_DEBUG,
  SDB_DEBUG,
  DWARF_DEBUG,
  DWARF2_DEBUG,
  XCOFF_DEBUG,
  VMS_DEBUG,
  VMS_AND_DWARF2_DEBUG

};


extern enum debug_info_type write_symbols;

enum debug_info_level
{
  DINFO_LEVEL_NONE,
  DINFO_LEVEL_TERSE,
  DINFO_LEVEL_NORMAL,
  DINFO_LEVEL_VERBOSE
};


extern enum debug_info_level debug_info_level;



extern int use_gnu_debug_info_extensions;



extern int optimize;



extern int optimize_size;




extern int quiet_flag;



extern int time_report;




extern int mem_report;



extern int inhibit_warnings;



extern int warn_system_headers;



extern int extra_warnings;





extern void set_Wunused (int setting);

extern int warn_unused_function;
extern int warn_unused_label;
extern int warn_unused_parameter;
extern int warn_unused_variable;
extern int warn_unused_value;



extern int warn_notreached;



extern int warn_inline;



extern int warn_uninitialized;







extern int warn_unknown_pragmas;



extern int warn_shadow;



extern int warn_switch;




extern int warn_return_type;



extern int warn_missing_noreturn;





extern int warn_cast_align;





extern int warn_larger_than;
extern long long larger_than_size;




extern int warn_aggregate_return;



extern int warn_packed;



extern int warn_padded;



extern int warn_disabled_optimization;




extern int warn_deprecated_decl;



extern int profile_flag;



extern int profile_arc_flag;



extern int flag_test_coverage;



extern int flag_branch_probabilities;



extern int flag_reorder_blocks;



extern int flag_rename_registers;




extern int pedantic;




extern int in_system_header;




extern int flag_print_asm_name;





extern int flag_signed_char;



extern int flag_short_enums;





extern int flag_caller_saves;



extern int flag_pcc_struct_return;




extern int flag_force_mem;




extern int flag_force_addr;




extern int flag_defer_pop;




extern int flag_float_store;



extern int flag_strength_reduce;






extern int flag_unroll_loops;




extern int flag_unroll_all_loops;




extern int flag_move_all_movables;



extern int flag_prefetch_loop_arrays;




extern int flag_reduce_all_givs;




extern int flag_cse_follow_jumps;




extern int flag_cse_skip_blocks;



extern int flag_expensive_optimizations;




extern int flag_writable_strings;





extern int flag_no_function_cse;




extern int flag_omit_frame_pointer;



extern int flag_no_peephole;



extern int flag_volatile;



extern int flag_volatile_global;



extern int flag_volatile_static;



extern int flag_optimize_sibling_calls;




extern int flag_errno_math;






extern int flag_unsafe_math_optimizations;





extern int flag_trapping_math;





extern int flag_complex_divide_method;



extern int flag_rerun_loop_opt;




extern int flag_inline_functions;





extern int flag_keep_inline_functions;







extern int flag_no_inline;




extern int flag_really_no_inline;



extern int flag_syntax_only;



extern int flag_gen_aux_info;



extern int flag_shared_data;






extern int flag_schedule_insns;
extern int flag_schedule_insns_after_reload;
# 418 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/flags.h"
extern int flag_schedule_interblock;
extern int flag_schedule_speculative;
extern int flag_schedule_speculative_load;
extern int flag_schedule_speculative_load_dangerous;



extern int flag_branch_on_count_reg;





extern int flag_single_precision_constant;



extern int flag_delayed_branch;




extern int flag_dump_unnumbered;





extern int flag_pretend_float;




extern int flag_pedantic_errors;




extern int flag_pic;




extern int flag_exceptions;



extern int flag_unwind_tables;



extern int flag_asynchronous_unwind_tables;




extern int flag_no_common;





extern int flag_inhibit_size_directive;




extern int flag_function_sections;



extern int flag_data_sections;
# 498 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/flags.h"
extern int flag_verbose_asm;
# 507 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/flags.h"
extern int flag_debug_asm;

extern int flag_dump_rtl_in_asm;



extern int flag_gnu_linker;


extern int flag_pack_struct;
# 525 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/flags.h"
extern int flag_argument_noalias;





extern int flag_strict_aliasing;



extern int flag_stack_check;


extern int flag_regmove;


extern int flag_instrument_function_entry_exit;


extern int flag_peephole2;


extern int flag_guess_branch_prob;






extern int flag_bounded_pointers;







extern int flag_bounds_check;




extern int flag_merge_constants;




extern int flag_renumber_insns;







extern int frame_pointer_needed;



extern int flag_trapv;


extern int g_switch_value;
extern int g_switch_set;






extern int align_loops;
extern int align_loops_log;
extern int align_loops_max_skip;
extern int align_jumps;
extern int align_jumps_log;
extern int align_jumps_max_skip;
extern int align_labels;
extern int align_labels_log;
extern int align_labels_max_skip;
extern int align_functions;
extern int align_functions_log;


extern int dump_for_graph;


enum graph_dump_types
{
  no_graph = 0,
  vcg
};
extern enum graph_dump_types graph_dump_format;





extern int flag_no_ident;



extern int flag_gcse_lm;



extern int flag_gcse_sm;




extern int flag_eliminate_dwarf2_dups;



extern int flag_detailed_statistics;


extern int flag_non_call_exceptions;
# 82 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c" 2
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/regs.h" 1
# 23 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/regs.h"
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/varray.h" 1
# 36 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/varray.h"
struct const_equiv_data {
# 50 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/varray.h"
  struct rtx_def *rtx;



  unsigned age;
};


typedef union varray_data_tag {
  char c[1];
  unsigned char uc[1];
  short s[1];
  unsigned short us[1];
  int i[1];
  unsigned int u[1];
  long l[1];
  unsigned long ul[1];
  long long hint[1];
  unsigned long long uhint[1];
  void * generic[1];
  char *cptr[1];
  struct rtx_def *rtx[1];
  struct rtvec_def *rtvec[1];
  union tree_node *tree[1];
  struct bitmap_head_def *bitmap[1];
  struct sched_info_tag *sched[1];
  struct reg_info_def *reg[1];
  struct const_equiv_data const_equiv[1];
  struct basic_block_def *bb[1];
  struct elt_list *te[1];
} varray_data;


typedef struct varray_head_tag {
  size_t num_elements;
  size_t elements_used;

  size_t element_size;
  const char *name;
  varray_data data;
} *varray_type;



extern varray_type varray_init (size_t, size_t, const char *);
# 165 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/varray.h"
extern varray_type varray_grow (varray_type, size_t);
# 24 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/regs.h" 2
# 42 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/regs.h"
extern int max_regno;


typedef struct reg_info_def
{
  int first_uid;
  int last_uid;
  int last_note_uid;


  int sets;


  int refs;
  int freq;
  int deaths;
  int live_length;
  int calls_crossed;
  int basic_block;
  char changes_mode;

} reg_info;

extern varray_type reg_n_info;
# 157 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/regs.h"
extern short *reg_renumber;




extern char regs_ever_live[53];



extern const char * reg_names[53];






extern enum machine_mode reg_raw_mode[53];
# 196 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/regs.h"
extern rtx regs_may_share;




extern int caller_save_needed;
# 230 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/regs.h"
extern void allocate_reg_info (size_t, int, int);
# 83 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c" 2
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/hard-reg-set.h" 1
# 41 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/hard-reg-set.h"
typedef unsigned long long HARD_REG_ELT_TYPE;
# 395 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/hard-reg-set.h"
extern char fixed_regs[53];



extern HARD_REG_ELT_TYPE fixed_reg_set;






extern char call_used_regs[53];



extern HARD_REG_ELT_TYPE call_used_reg_set;


extern HARD_REG_ELT_TYPE losing_caller_save_reg_set;







extern char call_fixed_regs[53];



extern HARD_REG_ELT_TYPE call_fixed_reg_set;






extern char global_regs[53];
# 441 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/hard-reg-set.h"
extern HARD_REG_ELT_TYPE regs_invalidated_by_call;




extern int reg_alloc_order[53];



extern int inv_reg_alloc_order[53];




extern HARD_REG_ELT_TYPE reg_class_contents[((int) LIM_REG_CLASSES)];



extern unsigned int reg_class_size[((int) LIM_REG_CLASSES)];



extern enum reg_class reg_class_superclasses[((int) LIM_REG_CLASSES)][((int) LIM_REG_CLASSES)];



extern enum reg_class reg_class_subclasses[((int) LIM_REG_CLASSES)][((int) LIM_REG_CLASSES)];




extern enum reg_class reg_class_subunion[((int) LIM_REG_CLASSES)][((int) LIM_REG_CLASSES)];




extern enum reg_class reg_class_superunion[((int) LIM_REG_CLASSES)][((int) LIM_REG_CLASSES)];



extern int n_non_fixed_regs;



extern const char * reg_names[53];
# 84 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c" 2
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/basic-block.h" 1
# 25 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/basic-block.h"
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/bitmap.h" 1
# 43 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/bitmap.h"
typedef struct bitmap_element_def
{
  struct bitmap_element_def *next;
  struct bitmap_element_def *prev;
  unsigned int indx;
  unsigned long long bits[2];
} bitmap_element;


typedef struct bitmap_head_def {
  bitmap_element *first;
  bitmap_element *current;
  unsigned int indx;

} bitmap_head, *bitmap;


enum bitmap_bits {
  BITMAP_AND,
  BITMAP_AND_COMPL,
  BITMAP_IOR,
  BITMAP_XOR,
  BITMAP_IOR_COMPL
};


extern bitmap_element bitmap_zero_bits;


extern void bitmap_clear (bitmap);


extern void bitmap_copy (bitmap, bitmap);


extern int bitmap_equal_p (bitmap, bitmap);


extern int bitmap_operation (bitmap, bitmap, bitmap, enum bitmap_bits);



extern void bitmap_ior_and_compl (bitmap, bitmap, bitmap);


extern void bitmap_clear_bit (bitmap, int);


extern void bitmap_set_bit (bitmap, int);


extern int bitmap_bit_p (bitmap, int);


extern void debug_bitmap (bitmap);
extern void debug_bitmap_file (FILE *, bitmap);


extern void bitmap_print (FILE *, bitmap, const char *, const char *);


extern bitmap bitmap_initialize (bitmap);


extern void bitmap_release_memory (void);






extern int bitmap_union_of_diff (bitmap, bitmap, bitmap, bitmap);
extern int bitmap_first_set_bit (bitmap);
extern int bitmap_last_set_bit (bitmap);
# 26 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/basic-block.h" 2
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/sbitmap.h" 1
# 31 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/sbitmap.h"
typedef struct simple_bitmap_def
{
  unsigned int n_bits;
  unsigned int size;
  unsigned int bytes;
  unsigned long long elms[1];
} *sbitmap;

typedef unsigned long long *sbitmap_ptr;
# 91 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/sbitmap.h"
struct int_list;

extern void dump_sbitmap (FILE *, sbitmap);
extern void dump_sbitmap_vector (FILE *, const char *, const char *, sbitmap *, int);


extern sbitmap sbitmap_alloc (unsigned int);
extern sbitmap *sbitmap_vector_alloc (unsigned int, unsigned int);
extern void sbitmap_copy (sbitmap, sbitmap);
extern int sbitmap_equal (sbitmap, sbitmap);
extern void sbitmap_zero (sbitmap);
extern void sbitmap_ones (sbitmap);
extern void sbitmap_vector_zero (sbitmap *, unsigned int);
extern void sbitmap_vector_ones (sbitmap *, unsigned int);

extern int sbitmap_union_of_diff (sbitmap, sbitmap, sbitmap, sbitmap);

extern void sbitmap_difference (sbitmap, sbitmap, sbitmap);
extern void sbitmap_not (sbitmap, sbitmap);
extern int sbitmap_a_or_b_and_c (sbitmap, sbitmap, sbitmap, sbitmap);

extern int sbitmap_a_and_b_or_c (sbitmap, sbitmap, sbitmap, sbitmap);

extern int sbitmap_a_and_b (sbitmap, sbitmap, sbitmap);
extern int sbitmap_a_or_b (sbitmap, sbitmap, sbitmap);
extern int sbitmap_a_xor_b (sbitmap, sbitmap, sbitmap);
extern int sbitmap_a_subset_b_p (sbitmap, sbitmap);

extern int sbitmap_first_set_bit (sbitmap);
extern int sbitmap_last_set_bit (sbitmap);

extern void sbitmap_intersect_of_predsucc (sbitmap, sbitmap *, int, struct int_list **);




extern void sbitmap_union_of_predsucc (sbitmap, sbitmap *, int, struct int_list **);







extern void sbitmap_intersection_of_succs (sbitmap, sbitmap *, int);
extern void sbitmap_intersection_of_preds (sbitmap, sbitmap *, int);
extern void sbitmap_union_of_succs (sbitmap, sbitmap *, int);
extern void sbitmap_union_of_preds (sbitmap, sbitmap *, int);

extern void debug_sbitmap (sbitmap);
# 27 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/basic-block.h" 2

# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/partition.h" 1
# 46 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/partition.h"
struct partition_elem
{


  int class_element;


  struct partition_elem* next;


  unsigned class_count;
};

typedef struct partition_def
{

  int num_elements;

  struct partition_elem elements[1];
} *partition;

extern partition partition_new (int);
extern void partition_delete (partition);
extern int partition_union (partition, int, int);


extern void partition_print (partition, FILE*);
# 29 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/basic-block.h" 2


typedef bitmap_head regset_head;

typedef bitmap regset;
# 74 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/basic-block.h"
extern void reg_set_to_hard_reg_set (HARD_REG_ELT_TYPE *, bitmap);
# 116 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/basic-block.h"
typedef long long gcov_type;


typedef struct edge_def {

  struct edge_def *pred_next, *succ_next;


  struct basic_block_def *src, *dest;


  rtx insns;


  void *aux;

  int flags;
  int probability;
  gcov_type count;

} *edge;
# 174 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/basic-block.h"
typedef struct basic_block_def {

  rtx head, end;


  tree head_tree;
  tree end_tree;


  edge pred, succ;




  regset local_set;



  regset cond_local_set;





  regset global_live_at_start;

  regset global_live_at_end;


  void *aux;


  int index;


  int loop_depth;


  gcov_type count;


  int frequency;


  int flags;
} *basic_block;
# 228 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/basic-block.h"
extern int n_basic_blocks;



extern int n_edges;



extern varray_type basic_block_info;





extern regset regs_live_at_setjmp;



extern rtx label_value_list, tail_recursion_label_list;

extern struct obstack flow_obstack;
# 278 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/basic-block.h"
extern struct basic_block_def entry_exit_blocks[2];



extern varray_type basic_block_for_insn;



extern void compute_bb_for_insn (int);
extern void free_bb_for_insn (void);
extern void update_bb_for_insn (basic_block);
extern void set_block_for_insn (rtx, basic_block);

extern void free_basic_block_vars (int);

extern edge split_block (basic_block, rtx);
extern basic_block split_edge (edge);
extern void insert_insn_on_edge (rtx, edge);
extern void commit_edge_insertions (void);
extern void remove_fake_edges (void);
extern void add_noreturn_fake_exit_edges (void);
extern void connect_infinite_loops_to_exit (void);
extern int flow_call_edges_add (sbitmap);
extern edge cached_make_edge (sbitmap *, basic_block, basic_block, int);

extern edge make_edge (basic_block, basic_block, int);

extern edge make_single_succ_edge (basic_block, basic_block, int);

extern void remove_edge (edge);
extern void redirect_edge_succ (edge, basic_block);
extern edge redirect_edge_succ_nodup (edge, basic_block);
extern void redirect_edge_pred (edge, basic_block);
extern basic_block create_basic_block_structure (int, rtx, rtx, rtx);
extern basic_block create_basic_block (int, rtx, rtx);
extern int flow_delete_block (basic_block);
extern int flow_delete_block_noexpunge (basic_block);
extern void merge_blocks_nomove (basic_block, basic_block);
extern void tidy_fallthru_edge (edge, basic_block, basic_block);

extern void tidy_fallthru_edges (void);
extern void flow_reverse_top_sort_order_compute (int *);
extern int flow_depth_first_order_compute (int *, int *);
extern void flow_preorder_transversal_compute (int *);
extern void dump_edge_info (FILE *, edge, int);
extern void clear_edges (void);
extern void mark_critical_edges (void);
extern rtx first_insn_after_basic_block_note (basic_block);


struct loop
{

  int num;


  basic_block header;


  basic_block latch;


  basic_block pre_header;




  edge *pre_header_edges;


  int num_pre_header_edges;



  basic_block first;



  basic_block last;


  sbitmap nodes;


  int num_nodes;


  edge *entry_edges;


  int num_entries;


  edge *exit_edges;


  int num_exits;


  sbitmap exits_doms;


  int depth;



  int level;


  struct loop *outer;


  struct loop *inner;


  struct loop *next;


  int shared;


  int invalid;


  void *aux;





  rtx vtop;



  rtx cont;


  rtx cont_dominator;


  rtx start;


  rtx end;



  rtx top;


  rtx scan_start;


  rtx sink;
# 442 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/basic-block.h"
  rtx exit_labels;



  int exit_count;
};



struct loops
{

  int num;


  int levels;



  struct loop *array;


  struct loop *tree_root;


  struct cfg
  {

    sbitmap *dom;


    int *dfs_order;



    int *rc_order;
  } cfg;


  sbitmap shared_headers;
};

extern int flow_loops_find (struct loops *, int flags);
extern int flow_loops_update (struct loops *, int flags);
extern void flow_loops_free (struct loops *);
extern void flow_loops_dump (const struct loops *, FILE *, void (*)(const struct loop *, FILE *, int), int);


extern void flow_loop_dump (const struct loop *, FILE *, void (*)(const struct loop *, FILE *, int), int);


extern int flow_loop_scan (struct loops *, struct loop *, int);


struct edge_list
{
  int num_blocks;
  int num_edges;
  edge *index_to_edge;
};
# 539 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/basic-block.h"
struct edge_list * create_edge_list (void);
void free_edge_list (struct edge_list *);
void print_edge_list (FILE *, struct edge_list *);
void verify_edge_list (FILE *, struct edge_list *);
int find_edge_index (struct edge_list *, basic_block, basic_block);



enum update_life_extent
{
  UPDATE_LIFE_LOCAL = 0,
  UPDATE_LIFE_GLOBAL = 1,
  UPDATE_LIFE_GLOBAL_RM_NOTES = 2
};
# 588 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/basic-block.h"
extern void life_analysis (rtx, FILE *, int);
extern void update_life_info (sbitmap, enum update_life_extent, int);

extern int count_or_remove_death_notes (sbitmap, int);
extern int propagate_block (basic_block, regset, regset, regset, int);


struct propagate_block_info;
extern rtx propagate_one_insn (struct propagate_block_info *, rtx);
extern struct propagate_block_info *init_propagate_block_info
  (basic_block, regset, regset, regset, int);
extern void free_propagate_block_info (struct propagate_block_info *);


extern struct edge_list *pre_edge_lcm (FILE *, int, sbitmap *, sbitmap *, sbitmap *, sbitmap *, sbitmap **, sbitmap **);



extern struct edge_list *pre_edge_rev_lcm (FILE *, int, sbitmap *, sbitmap *, sbitmap *, sbitmap *, sbitmap **, sbitmap **);



extern void compute_available (sbitmap *, sbitmap *, sbitmap *, sbitmap *);

extern int optimize_mode_switching (FILE *);


extern rtx emit_block_insn_after (rtx, rtx, basic_block);
extern rtx emit_block_insn_before (rtx, rtx, basic_block);


extern void estimate_probability (struct loops *);
extern void expected_value_to_br_prob (void);


extern void init_flow (void);
extern void reorder_basic_blocks (void);
extern void dump_bb (basic_block, FILE *);
extern void debug_bb (basic_block);
extern void debug_bb_n (int);
extern void dump_regset (regset, FILE *);
extern void debug_regset (regset);
extern void allocate_reg_life_data (void);
extern void allocate_bb_life_data (void);
extern void expunge_block (basic_block);
extern void expunge_block_nocompact (basic_block);
extern basic_block alloc_block (void);
extern void find_unreachable_blocks (void);
extern void delete_noop_moves (rtx);
extern basic_block redirect_edge_and_branch_force (edge, basic_block);
extern basic_block force_nonfallthru (edge);
extern _Bool redirect_edge_and_branch (edge, basic_block);
extern rtx block_label (basic_block);
extern _Bool forwarder_block_p (basic_block);
extern _Bool purge_all_dead_edges (int);
extern _Bool purge_dead_edges (basic_block);
extern void find_sub_basic_blocks (basic_block);
extern void find_many_sub_basic_blocks (sbitmap);
extern _Bool can_fallthru (basic_block, basic_block);
extern void flow_nodes_print (const char *, const sbitmap, FILE *);

extern void flow_edge_list_print (const char *, const edge *, int, FILE *);

extern void alloc_aux_for_block (basic_block, int);
extern void alloc_aux_for_blocks (int);
extern void clear_aux_for_blocks (void);
extern void free_aux_for_blocks (void);
extern void alloc_aux_for_edge (edge, int);
extern void alloc_aux_for_edges (int);
extern void clear_aux_for_edges (void);
extern void free_aux_for_edges (void);




extern void verify_flow_info (void);
extern int flow_loop_outside_edge_p (const struct loop *, edge);

typedef struct conflict_graph_def *conflict_graph;




typedef int (*conflict_graph_enum_fn) (int, int, void *);




extern conflict_graph conflict_graph_new
                                        (int);
extern void conflict_graph_delete (conflict_graph);
extern int conflict_graph_add (conflict_graph, int, int);

extern int conflict_graph_conflict_p (conflict_graph, int, int);

extern void conflict_graph_enum (conflict_graph, int, conflict_graph_enum_fn, void *);


extern void conflict_graph_merge_regs (conflict_graph, int, int);

extern void conflict_graph_print (conflict_graph, FILE*);
extern conflict_graph conflict_graph_compute
                                        (regset, partition);

extern _Bool mark_dfs_back_edges (void);
extern void update_br_prob_note (basic_block);
extern void fixup_abnormal_edges (void);



enum cdi_direction
{
  CDI_DOMINATORS,
  CDI_POST_DOMINATORS
};

extern void calculate_dominance_info (int *, sbitmap *, enum cdi_direction);
# 85 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c" 2
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/insn-config.h" 1
# 86 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c" 2
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/function.h" 1
# 22 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/function.h"
struct var_refs_queue
{
  rtx modified;
  enum machine_mode promoted_mode;
  int unsignedp;
  struct var_refs_queue *next;
};






struct sequence_stack
{

  rtx first, last;
  tree sequence_rtl_expr;
  struct sequence_stack *next;
};

extern struct sequence_stack *sequence_stack;



struct simple_obstack_stack
{
  struct obstack *obstack;
  struct simple_obstack_stack *next;
};

struct emit_status
{


  int x_reg_rtx_no;


  int x_first_label_num;






  rtx x_first_insn;
  rtx x_last_insn;




  tree sequence_rtl_expr;





  struct sequence_stack *sequence_stack;



  int x_cur_insn_uid;



  int x_last_linenum;
  const char *x_last_filename;





  int regno_pointer_align_length;




  unsigned char *regno_pointer_align;



  tree *regno_decl;



  rtx *x_regno_reg_rtx;
};
# 119 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/function.h"
struct expr_status
{


  int x_pending_stack_adjust;
# 140 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/function.h"
  int x_inhibit_defer_pop;





  int x_stack_pointer_delta;




  rtx x_saveregs_value;


  rtx x_apply_args_value;


  rtx x_forced_labels;


  rtx x_pending_chain;
};
# 174 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/function.h"
struct function
{
  struct eh_status *eh;
  struct stmt_status *stmt;
  struct expr_status *expr;
  struct emit_status *emit;
  struct varasm_status *varasm;




  const char *name;


  tree decl;


  struct function *outer;




  int pops_args;




  int args_size;




  int pretend_args_size;



  int outgoing_args_size;



  rtx arg_offset_rtx;



  CUMULATIVE_ARGS args_info;





  rtx return_rtx;


  rtx internal_arg_pointer;



  const char *cannot_inline;



  struct initial_value_struct *hard_reg_initial_vals;


  int x_function_call_count;




  tree x_nonlocal_labels;





  rtx x_nonlocal_goto_handler_slots;



  rtx x_nonlocal_goto_handler_labels;




  rtx x_nonlocal_goto_stack_level;





  rtx x_cleanup_label;




  rtx x_return_label;



  rtx x_save_expr_regs;



  rtx x_stack_slot_list;


  tree x_rtl_expr_chain;



  rtx x_tail_recursion_label;


  rtx x_tail_recursion_reentry;





  rtx x_arg_pointer_save_area;




  rtx x_clobber_return_insn;




  long long x_frame_offset;




  tree x_context_display;
# 317 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/function.h"
  tree x_trampoline_list;


  rtx x_parm_birth_insn;



  rtx x_last_parm_insn;



  unsigned int x_max_parm_reg;





  rtx *x_parm_reg_stack_loc;


  struct temp_slot *x_temp_slots;


  int x_temp_slot_level;


  int x_var_temp_slot_level;





  int x_target_temp_slot_level;



  struct var_refs_queue *fixup_var_refs_queue;


  int inlinable;
  int no_debugging_symbols;

  void *original_arg_vector;
  tree original_decl_initial;


  rtx inl_last_parm_insn;

  int inl_max_label_num;


  int profile_label_no;




  struct machine_function *machine;

  int stack_alignment_needed;

  int preferred_stack_boundary;


  struct language_function *language;





  rtx epilogue_delay_list;





  unsigned int returns_struct : 1;



  unsigned int returns_pcc_struct : 1;


  unsigned int returns_pointer : 1;


  unsigned int needs_context : 1;


  unsigned int calls_setjmp : 1;


  unsigned int calls_longjmp : 1;



  unsigned int calls_alloca : 1;


  unsigned int calls_eh_return : 1;



  unsigned int has_nonlocal_label : 1;



  unsigned int has_nonlocal_goto : 1;


  unsigned int contains_functions : 1;


  unsigned int has_computed_jump : 1;




  unsigned int is_thunk : 1;



  unsigned int instrument_entry_exit : 1;


  unsigned int profile : 1;



  unsigned int limit_stack : 1;



  unsigned int varargs : 1;



  unsigned int stdarg : 1;





  unsigned int x_whole_function_mode_p : 1;
# 468 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/function.h"
  unsigned int x_dont_save_pending_sizes_p : 1;


  unsigned int uses_const_pool : 1;


  unsigned int uses_pic_offset_table : 1;


  unsigned int uses_eh_lsda : 1;


  unsigned int arg_pointer_save_area_init : 1;
};


extern struct function *cfun;


extern int virtuals_instantiated;
# 549 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/function.h"
extern tree inline_function_decl;



struct function *find_function_data (tree);


extern void identify_blocks (void);



extern void reorder_blocks (void);


extern void number_blocks (tree);




extern long long get_frame_size (void);

extern long long get_func_frame_size (struct function *);



extern void (*init_machine_status) (struct function *);
extern void (*free_machine_status) (struct function *);



extern void (*mark_machine_status) (struct function *);


extern void (*init_lang_status) (struct function *);
extern void (*mark_lang_status) (struct function *);
extern void (*save_lang_status) (struct function *);
extern void (*restore_lang_status) (struct function *);
extern void (*free_lang_status) (struct function *);


extern void restore_emit_status (struct function *);
extern void free_after_parsing (struct function *);
extern void free_after_compilation (struct function *);

extern void init_varasm_status (struct function *);
extern void free_varasm_status (struct function *);
extern void free_emit_status (struct function *);
extern void free_stmt_status (struct function *);
extern void free_eh_status (struct function *);
extern void free_expr_status (struct function *);

extern rtx get_first_block_beg (void);


extern void diddle_return_value (void (*)(rtx, void*), void*);
extern void clobber_return_register (void);
extern void use_return_register (void);


extern rtx get_arg_pointer_save_area (struct function *);

extern void init_virtual_regs (struct emit_status *);


extern void init_function_once (void);
# 87 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c" 2

# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/expr.h" 1
# 52 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/expr.h"
enum expand_modifier {EXPAND_NORMAL, EXPAND_SUM, EXPAND_CONST_ADDRESS,
   EXPAND_INITIALIZER, EXPAND_WRITE};
# 119 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/expr.h"
enum direction {none, upward, downward};
# 256 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/expr.h"
enum optab_methods
{
  OPTAB_DIRECT,
  OPTAB_LIB,
  OPTAB_WIDEN,
  OPTAB_LIB_WIDEN,
  OPTAB_MUST_WIDEN
};




extern rtx expand_simple_binop (enum machine_mode, enum rtx_code, rtx, rtx, rtx, int, enum optab_methods);

extern rtx expand_simple_unop (enum machine_mode, enum rtx_code, rtx, rtx, int);




extern int have_insn_for (enum rtx_code, enum machine_mode);


extern void emit_libcall_block (rtx, rtx, rtx, rtx);





extern rtx gen_add2_insn (rtx, rtx);
extern rtx gen_add3_insn (rtx, rtx, rtx);
extern rtx gen_sub2_insn (rtx, rtx);
extern rtx gen_sub3_insn (rtx, rtx, rtx);
extern rtx gen_move_insn (rtx, rtx);
extern int have_add2_insn (rtx, rtx);
extern int have_sub2_insn (rtx, rtx);



extern void emit_cmp_and_jump_insns (rtx, rtx, enum rtx_code, rtx, enum machine_mode, int, rtx);



extern void emit_indirect_jump (rtx);



rtx emit_conditional_move (rtx, enum rtx_code, rtx, rtx, enum machine_mode, rtx, rtx, enum machine_mode, int);




int can_conditionally_move_p (enum machine_mode mode);
# 316 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/expr.h"
extern rtx negate_rtx (enum machine_mode, rtx);


extern rtx expand_and (enum machine_mode, rtx, rtx, rtx);


extern rtx emit_store_flag (rtx, enum rtx_code, rtx, rtx, enum machine_mode, int, int);



extern rtx emit_store_flag_force (rtx, enum rtx_code, rtx, rtx, enum machine_mode, int, int);






extern rtx canonicalize_condition (rtx, rtx, int, rtx *, rtx);



extern rtx get_condition (rtx, rtx *);


extern rtx gen_cond_trap (enum rtx_code, rtx, rtx, rtx);


extern rtx expand_builtin (tree, rtx, rtx, enum machine_mode, int);
extern void std_expand_builtin_va_start (int, tree, rtx);
extern rtx std_expand_builtin_va_arg (tree, tree);
extern rtx expand_builtin_va_arg (tree, tree);
extern void default_init_builtins (void);
extern rtx default_expand_builtin (tree, rtx, rtx, enum machine_mode, int);

extern void expand_builtin_setjmp_setup (rtx, rtx);
extern void expand_builtin_setjmp_receiver (rtx);
extern void expand_builtin_longjmp (rtx, rtx);
extern rtx expand_builtin_saveregs (void);
extern void expand_builtin_trap (void);
extern long long get_varargs_alias_set (void);
extern long long get_frame_alias_set (void);
extern void record_base_value (unsigned int, rtx, int);
extern void record_alias_subset (long long, long long);

extern long long new_alias_set (void);
extern int can_address_p (tree);





extern void init_expr_once (void);


extern void init_expr (void);



extern void init_stor_layout_once (void);


extern void finish_expr_for_function (void);



extern rtx protect_from_queue (rtx, int);


extern void emit_queue (void);


extern int queued_subexp_p (rtx);



extern void convert_move (rtx, rtx, int);


extern rtx convert_to_mode (enum machine_mode, rtx, int);


extern rtx convert_modes (enum machine_mode, enum machine_mode, rtx, int);



extern rtx emit_block_move (rtx, rtx, rtx);



extern void move_block_to_reg (int, rtx, int, enum machine_mode);



extern void move_block_from_reg (int, rtx, int, int);



extern void emit_group_load (rtx, rtx, int);



extern void emit_group_store (rtx, rtx, int);







extern void use_reg (rtx *, rtx);



extern void use_regs (rtx *, int, int);


extern void use_group_regs (rtx *, rtx);



extern rtx clear_storage (rtx, rtx);






extern int can_store_by_pieces (unsigned long long, rtx (*) (void *, long long, enum machine_mode), void *, unsigned int);
# 452 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/expr.h"
extern void store_by_pieces (rtx, unsigned long long, rtx (*) (void *, long long, enum machine_mode), void *, unsigned int);





extern rtx emit_move_insn (rtx, rtx);


extern rtx emit_move_insn_1 (rtx, rtx);



extern rtx push_block (rtx, int, int);
# 486 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/expr.h"
extern rtx force_operand (rtx, rtx);
# 495 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/expr.h"
extern tree find_placeholder (tree, tree *);




extern rtx expand_expr (tree, rtx, enum machine_mode, enum expand_modifier);




extern void init_pending_stack_adjust (void);



extern void clear_pending_stack_adjust (void);


extern void do_pending_stack_adjust (void);
# 531 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/expr.h"
extern rtx compare_from_rtx (rtx, rtx, enum rtx_code, int, enum machine_mode, rtx);

extern void do_compare_rtx_and_jump (rtx, rtx, enum rtx_code, int, enum machine_mode, rtx, rtx, rtx);




extern int try_casesi (tree, tree, tree, tree, rtx, rtx);
extern int try_tablejump (tree, tree, tree, tree, rtx, rtx);



extern unsigned int case_values_threshold (void);
# 584 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/expr.h"
extern rtx promoted_input_arg (unsigned int, enum machine_mode *, int *);





extern rtx eliminate_constant_term (rtx, rtx *);



extern rtx memory_address (enum machine_mode, rtx);


extern rtx memory_address_noforce (enum machine_mode, rtx);


extern void set_mem_alias_set (rtx, long long);


extern void set_mem_align (rtx, unsigned int);


extern void set_mem_expr (rtx, tree);


extern void set_mem_offset (rtx, rtx);





extern rtx change_address (rtx, enum machine_mode, rtx);
# 636 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/expr.h"
extern rtx adjust_address_1 (rtx, enum machine_mode, long long, int, int);

extern rtx adjust_automodify_address_1 (rtx, enum machine_mode, rtx, long long, int);





extern rtx offset_address (rtx, rtx, long long);





extern rtx replace_equiv_address (rtx, rtx);


extern rtx replace_equiv_address_nv (rtx, rtx);



extern rtx widen_memory_access (rtx, enum machine_mode, long long);



extern rtx validize_mem (rtx);
# 676 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/expr.h"
extern rtx assemble_trampoline_template (void);



extern rtx stabilize (rtx);



extern rtx copy_all_regs (rtx);


extern rtx copy_to_reg (rtx);


extern rtx copy_addr_to_reg (rtx);


extern rtx copy_to_mode_reg (enum machine_mode, rtx);


extern rtx copy_to_suggested_reg (rtx, rtx, enum machine_mode);



extern rtx force_reg (enum machine_mode, rtx);


extern rtx force_not_mem (rtx);
# 712 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/expr.h"
extern void adjust_stack (rtx);


extern void anti_adjust_stack (rtx);


enum save_level {SAVE_BLOCK, SAVE_FUNCTION, SAVE_NONLOCAL};


extern void emit_stack_save (enum save_level, rtx *, rtx);


extern void emit_stack_restore (enum save_level, rtx, rtx);



extern rtx allocate_dynamic_stack_space (rtx, rtx, int);






extern void probe_stack_range (long long, rtx);



extern rtx hard_libcall_value (enum machine_mode);



extern rtx round_push (rtx);





enum extraction_pattern { EP_insv, EP_extv, EP_extzv };
extern enum machine_mode
mode_for_extraction (enum extraction_pattern, int);

extern rtx store_bit_field (rtx, unsigned long long, unsigned long long, enum machine_mode, rtx, long long);


extern rtx extract_bit_field (rtx, unsigned long long, unsigned long long, int, rtx, enum machine_mode, enum machine_mode, long long);



extern rtx expand_mult (enum machine_mode, rtx, rtx, rtx, int);
extern rtx expand_mult_add (rtx, rtx, rtx, rtx,enum machine_mode, int);
extern rtx expand_mult_highpart_adjust (enum machine_mode, rtx, rtx, rtx, rtx, int);

extern rtx assemble_static_space (int);




extern rtx (*lang_expand_expr) (union tree_node *, rtx, enum machine_mode, enum expand_modifier modifier);



extern int safe_from_p (rtx, tree, int);



extern void init_optabs (void);
extern void init_all_optabs (void);


extern rtx init_one_libfunc (const char *);

extern void do_jump_by_parts_equality_rtx (rtx, rtx, rtx);
extern void do_jump_by_parts_greater_rtx (enum machine_mode, int, rtx, rtx, rtx, rtx);
# 89 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c" 2
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/insn-attr.h" 1
# 10 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/insn-attr.h"
enum attr_cpu {CPU_I386, CPU_I486, CPU_PENTIUM, CPU_PENTIUMPRO, CPU_K6, CPU_ATHLON, CPU_PENTIUM4};
extern enum attr_cpu get_attr_cpu (void);


enum attr_type {TYPE_OTHER, TYPE_MULTI, TYPE_ALU1, TYPE_NEGNOT, TYPE_ALU, TYPE_ICMP, TYPE_TEST, TYPE_IMOV, TYPE_IMOVX, TYPE_LEA, TYPE_INCDEC, TYPE_ISHIFT, TYPE_IMUL, TYPE_IDIV, TYPE_IBR, TYPE_SETCC, TYPE_PUSH, TYPE_POP, TYPE_CALL, TYPE_CALLV, TYPE_ICMOV, TYPE_FMOV, TYPE_FOP, TYPE_FOP1, TYPE_FSGN, TYPE_FMUL, TYPE_FDIV, TYPE_FPSPC, TYPE_FCMOV, TYPE_FCMP, TYPE_FXCH, TYPE_STR, TYPE_CLD, TYPE_SSE, TYPE_MMX, TYPE_FISTP};
extern enum attr_type get_attr_type (rtx);


enum attr_mode {MODE_UNKNOWN, MODE_NONE, MODE_QI, MODE_HI, MODE_SI, MODE_DI, MODE_UNKNOWNFP, MODE_SF, MODE_DF, MODE_XF, MODE_TI};
extern enum attr_mode get_attr_mode (rtx);


extern int get_attr_i387 (rtx);

extern int get_attr_length_immediate (rtx);

extern int get_attr_length_address (rtx);

extern int get_attr_prefix_data16 (rtx);

extern int get_attr_prefix_rep (rtx);

extern int get_attr_prefix_0f (rtx);

extern int get_attr_modrm (rtx);

extern int get_attr_length (rtx);
extern void shorten_branches (rtx);
extern int insn_default_length (rtx);
extern int insn_variable_length_p (rtx);
extern int insn_current_length (rtx);


# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/insn-addr.h" 1
# 26 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/insn-addr.h"
extern varray_type insn_addresses_;
extern int insn_current_address;
# 43 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/insn-attr.h" 2


enum attr_memory {MEMORY_NONE, MEMORY_LOAD, MEMORY_STORE, MEMORY_BOTH, MEMORY_UNKNOWN};
extern enum attr_memory get_attr_memory (rtx);


enum attr_imm_disp {IMM_DISP_FALSE, IMM_DISP_TRUE, IMM_DISP_UNKNOWN};
extern enum attr_imm_disp get_attr_imm_disp (rtx);


enum attr_fp_int_src {FP_INT_SRC_FALSE, FP_INT_SRC_TRUE};
extern enum attr_fp_int_src get_attr_fp_int_src (rtx);


enum attr_pent_prefix {PENT_PREFIX_FALSE, PENT_PREFIX_TRUE};
extern enum attr_pent_prefix get_attr_pent_prefix (rtx);


enum attr_pent_pair {PENT_PAIR_UV, PENT_PAIR_PU, PENT_PAIR_PV, PENT_PAIR_NP};
extern enum attr_pent_pair get_attr_pent_pair (rtx);


enum attr_ppro_uops {PPRO_UOPS_ONE, PPRO_UOPS_FEW, PPRO_UOPS_MANY};
extern enum attr_ppro_uops get_attr_ppro_uops (rtx);


enum attr_athlon_decode {ATHLON_DECODE_DIRECT, ATHLON_DECODE_VECTOR};
extern enum attr_athlon_decode get_attr_athlon_decode (rtx);


enum attr_athlon_fpunits {ATHLON_FPUNITS_NONE, ATHLON_FPUNITS_STORE, ATHLON_FPUNITS_MUL, ATHLON_FPUNITS_ADD, ATHLON_FPUNITS_MULADD, ATHLON_FPUNITS_ANY};
extern enum attr_athlon_fpunits get_attr_athlon_fpunits (rtx);



extern int result_ready_cost (rtx);
extern int function_units_used (rtx);

extern const struct function_unit_desc
{
  const char *const name;
  const int bitmask;
  const int multiplicity;
  const int simultaneity;
  const int default_cost;
  const int max_issue_delay;
  int (*const ready_cost_function) (rtx);
  int (*const conflict_cost_function) (rtx, rtx);
  const int max_blockage;
  unsigned int (*const blockage_range_function) (rtx);
  int (*const blockage_function) (rtx, rtx);
} function_units[];
# 90 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c" 2
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/recog.h" 1
# 28 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/recog.h"
enum op_type {
  OP_IN,
  OP_OUT,
  OP_INOUT
};

struct operand_alternative
{


  const char *constraint;


  enum reg_class class;



  unsigned int reject;


  int matches;



  int matched;


  unsigned int earlyclobber:1;

  unsigned int memory_ok:1;

  unsigned int offmem_ok:1;

  unsigned int nonoffmem_ok:1;

  unsigned int decmem_ok:1;

  unsigned int incmem_ok:1;

  unsigned int is_address:1;


  unsigned int anything_ok:1;
};


extern void init_recog (void);
extern void init_recog_no_volatile (void);
extern int recog_memoized_1 (rtx);
extern int check_asm_operands (rtx);
extern int asm_operand_ok (rtx, const char *);
extern int validate_change (rtx, rtx *, rtx, int);
extern int insn_invalid_p (rtx);
extern int apply_change_group (void);
extern int num_validated_changes (void);
extern void cancel_changes (int);
extern int constrain_operands (int);
extern int constrain_operands_cached (int);
extern int memory_address_p (enum machine_mode, rtx);
extern int strict_memory_address_p (enum machine_mode, rtx);
extern int validate_replace_rtx_subexp (rtx, rtx, rtx, rtx *);
extern int validate_replace_rtx (rtx, rtx, rtx);
extern void validate_replace_rtx_group (rtx, rtx, rtx);
extern int validate_replace_src (rtx, rtx, rtx);



extern int reg_fits_class_p (rtx, enum reg_class, int, enum machine_mode);

extern rtx *find_single_use (rtx, rtx, rtx *);

extern int general_operand (rtx, enum machine_mode);
extern int address_operand (rtx, enum machine_mode);
extern int register_operand (rtx, enum machine_mode);
extern int pmode_register_operand (rtx, enum machine_mode);
extern int scratch_operand (rtx, enum machine_mode);
extern int immediate_operand (rtx, enum machine_mode);
extern int const_int_operand (rtx, enum machine_mode);
extern int const_double_operand (rtx, enum machine_mode);
extern int nonimmediate_operand (rtx, enum machine_mode);
extern int nonmemory_operand (rtx, enum machine_mode);
extern int push_operand (rtx, enum machine_mode);
extern int pop_operand (rtx, enum machine_mode);
extern int memory_operand (rtx, enum machine_mode);
extern int indirect_operand (rtx, enum machine_mode);
extern int mode_independent_operand (rtx, enum machine_mode);
extern int comparison_operator (rtx, enum machine_mode);

extern int offsettable_memref_p (rtx);
extern int offsettable_nonstrict_memref_p (rtx);
extern int offsettable_address_p (int, enum machine_mode, rtx);
extern int mode_dependent_address_p (rtx);

extern int recog (rtx, rtx, int *);
extern void add_clobbers (rtx, int);
extern int added_clobbers_hard_reg_p (int);
extern void insn_extract (rtx);
extern void extract_insn (rtx);
extern void extract_constrain_insn_cached (rtx);
extern void extract_insn_cached (rtx);
extern void preprocess_constraints (void);
extern rtx peep2_next_insn (int);
extern int peep2_regno_dead_p (int, int);
extern int peep2_reg_dead_p (int, rtx);

extern rtx peep2_find_free_register (int, int, const char *, enum machine_mode, HARD_REG_ELT_TYPE *);



extern void peephole2_optimize (FILE *);
extern rtx peephole2_insns (rtx, rtx, int *);


extern int volatile_ok;



extern int which_alternative;



struct recog_data
{







  rtx operand[30];


  rtx *operand_loc[30];


  const char *constraints[30];


  enum machine_mode operand_mode[30];


  enum op_type operand_type[30];



  rtx *dup_loc[4];



  char dup_num[4];
# 191 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/recog.h"
  char n_operands;


  char n_dups;


  char n_alternatives;


  rtx insn;
};

extern struct recog_data recog_data;



extern struct operand_alternative recog_op_alt[30][30];




typedef int (*insn_operand_predicate_fn) (rtx, enum machine_mode);
typedef const char * (*insn_output_fn) (rtx *, rtx);
typedef rtx (*insn_gen_fn) (rtx, ...);

struct insn_operand_data
{
  const insn_operand_predicate_fn predicate;

  const char *const constraint;

  const enum machine_mode mode : 16;

  const char strict_low;

  const char eliminable;
};
# 236 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/recog.h"
struct insn_data
{
  const char *const name;
  const void * output;
  const insn_gen_fn genfun;
  const struct insn_operand_data *const operand;

  const char n_operands;
  const char n_dups;
  const char n_alternatives;
  const char output_format;
};

extern const struct insn_data insn_data[];
# 91 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c" 2
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/real.h" 1
# 107 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/real.h"
typedef struct {
  long long r[(19 + sizeof (long long))/(sizeof (long long))];
} realvaluetype;
# 131 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/real.h"
extern unsigned int significand_size (enum machine_mode);
# 144 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/real.h"
extern void earith (realvaluetype *, int, realvaluetype *, realvaluetype *);

extern realvaluetype etrunci (realvaluetype);
extern realvaluetype etruncui (realvaluetype);
extern realvaluetype ereal_negate (realvaluetype);
extern long long efixi (realvaluetype);
extern unsigned long long efixui (realvaluetype);
extern void ereal_from_int (realvaluetype *, long long, long long, enum machine_mode);


extern void ereal_from_uint (realvaluetype *, unsigned long long, unsigned long long, enum machine_mode);



extern void ereal_to_int (long long *, long long *, realvaluetype);

extern realvaluetype ereal_ldexp (realvaluetype, int);

extern void etartdouble (realvaluetype, long *);
extern void etarldouble (realvaluetype, long *);
extern void etardouble (realvaluetype, long *);
extern long etarsingle (realvaluetype);
extern void ereal_to_decimal (realvaluetype, char *);
extern int ereal_cmp (realvaluetype, realvaluetype);
extern int ereal_isneg (realvaluetype);
extern realvaluetype ereal_unto_float (long);
extern realvaluetype ereal_unto_double (long *);
extern realvaluetype ereal_from_float (long long);
extern realvaluetype ereal_from_double (long long *);
# 406 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/real.h"
extern realvaluetype real_value_truncate (enum machine_mode, realvaluetype);
# 437 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/real.h"
extern realvaluetype dconst0;
extern realvaluetype dconst1;
extern realvaluetype dconst2;
extern realvaluetype dconstm1;




union real_extract
{
  realvaluetype d;
  long long i[sizeof (realvaluetype) / sizeof (long long)];
};




union tree_node;
realvaluetype real_value_from_int_cst (union tree_node *, union tree_node *);
# 466 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/real.h"
extern struct rtx_def *immed_real_const_1 (realvaluetype, enum machine_mode);
# 478 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/real.h"
extern int exact_real_inverse (enum machine_mode, realvaluetype *);
extern int target_isnan (realvaluetype);
extern int target_isinf (realvaluetype);
extern int target_negative (realvaluetype);
extern void debug_real (realvaluetype);
extern realvaluetype ereal_atof (const char *, enum machine_mode);
# 92 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c" 2
# 1 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/toplev.h" 1
# 29 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/toplev.h"
extern int toplev_main (int, char **);
extern int read_integral_parameter (const char *, const char *, const int);

extern int count_error (int);
extern void strip_off_ending (char *, int);
extern void print_time (const char *, long);
extern const char *trim_filename (const char *);
extern void internal_error (const char *, ...)
            __attribute__ ((__noreturn__));
extern void fatal_io_error (const char *, ...)
            __attribute__ ((__noreturn__));
extern void _fatal_insn_not_found (struct rtx_def *, const char *, int, const char *)


            __attribute__ ((__noreturn__));
extern void _fatal_insn (const char *, struct rtx_def *, const char *, int, const char *)



           __attribute__ ((__noreturn__));
# 58 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/toplev.h"
extern void warning (const char *, ...);
extern void error (const char *, ...);
extern void fatal_error (const char *, ...)
            __attribute__ ((__noreturn__));
extern void pedwarn (const char *, ...);
extern void pedwarn_with_file_and_line (const char *, int, const char *, ...);

extern void warning_with_file_and_line (const char *, int, const char *, ...);

extern void error_with_file_and_line (const char *, int, const char *, ...);

extern void sorry (const char *, ...);
extern void report_error_function (const char *);

extern void rest_of_decl_compilation (union tree_node *, const char *, int, int);

extern void rest_of_type_compilation (union tree_node *, int);
extern void rest_of_compilation (union tree_node *);

extern void pedwarn_with_decl (union tree_node *, const char *, ...);

extern void warning_with_decl (union tree_node *, const char *, ...);

extern void error_with_decl (union tree_node *, const char *, ...);


extern void announce_function (union tree_node *);

extern void error_for_asm (struct rtx_def *, const char *, ...);

extern void warning_for_asm (struct rtx_def *, const char *, ...);

extern void warn_deprecated_use (union tree_node *);
extern int do_float_handler (void (*) (void *), void *);


extern void output_quoted_string (FILE *, const char *);
extern void output_file_directive (FILE *, const char *);

extern void do_abort (void) __attribute__ ((__noreturn__));
extern void botch (const char *)
  __attribute__ ((__noreturn__));




extern void fnotice (FILE *, const char *, ...)
            __attribute__ ((__format__ (__printf__, 2, 3)));


extern int wrapup_global_declarations (union tree_node **, int);
extern void check_global_declarations (union tree_node **, int);

extern const char *progname;
extern const char *dump_base_name;


extern struct ht *ident_hash;




extern void set_fast_math_flags (void);
extern void set_no_fast_math_flags (void);
# 130 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/toplev.h"
extern int exact_log2_wide (unsigned long long);
extern int floor_log2_wide (unsigned long long);
# 93 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c" 2







static int combine_attempts;



static int combine_merges;



static int combine_extras;



static int combine_successes;



static int total_attempts, total_merges, total_extras, total_successes;
# 126 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static int *uid_cuid;
static int max_uid_cuid;
# 142 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static unsigned int combine_max_regno;



static rtx *reg_last_death;



static rtx *reg_last_set;




static int mem_last_set;




static int last_call_cuid;







static rtx subst_insn;




static rtx subst_prev_insn;
# 182 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static int subst_low_cuid;




static HARD_REG_ELT_TYPE newpat_used_regs;





static rtx added_links_insn;


static int this_basic_block;




static sbitmap refresh_blocks;
static int need_refresh;
# 250 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static rtx *reg_last_set_value;




static int *reg_last_set_label;




static int *reg_last_set_table_tick;




static char *reg_last_set_invalid;



static int label_tick;
# 281 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static unsigned long long *reg_nonzero_bits;




static enum machine_mode nonzero_bits_mode;




static unsigned char *reg_sign_bit_copies;






static int nonzero_sign_valid;






static enum machine_mode *reg_last_set_mode;
static unsigned long long *reg_last_set_nonzero_bits;
static char *reg_last_set_sign_bit_copies;





struct undo
{
  struct undo *next;
  int is_int;
  union {rtx r; unsigned int i;} old_contents;
  union {rtx *r; unsigned int *i;} where;
};







struct undobuf
{
  struct undo *undos;
  struct undo *frees;
  rtx other_insn;
};

static struct undobuf undobuf;




static int n_occurrences;

static void do_SUBST (rtx *, rtx);
static void do_SUBST_INT (unsigned int *, unsigned int);

static void init_reg_last_arrays (void);
static void setup_incoming_promotions (void);
static void set_nonzero_bits_and_sign_copies (rtx, rtx, void *);
static int cant_combine_insn_p (rtx);
static int can_combine_p (rtx, rtx, rtx, rtx, rtx *, rtx *);
static int sets_function_arg_p (rtx);
static int combinable_i3pat (rtx, rtx *, rtx, rtx, int, rtx *);
static int contains_muldiv (rtx);
static rtx try_combine (rtx, rtx, rtx, int *);
static void undo_all (void);
static void undo_commit (void);
static rtx *find_split_point (rtx *, rtx);
static rtx subst (rtx, rtx, rtx, int, int);
static rtx combine_simplify_rtx (rtx, enum machine_mode, int, int);
static rtx simplify_if_then_else (rtx);
static rtx simplify_set (rtx);
static rtx simplify_logical (rtx, int);
static rtx expand_compound_operation (rtx);
static rtx expand_field_assignment (rtx);
static rtx make_extraction (enum machine_mode, rtx, long long, rtx, unsigned long long, int, int, int);


static rtx extract_left_shift (rtx, int);
static rtx make_compound_operation (rtx, enum rtx_code);
static int get_pos_from_mask (unsigned long long, unsigned long long *);

static rtx force_to_mode (rtx, enum machine_mode, unsigned long long, rtx, int);

static rtx if_then_else_cond (rtx, rtx *, rtx *);
static rtx known_cond (rtx, enum rtx_code, rtx, rtx);
static int rtx_equal_for_field_assignment_p (rtx, rtx);
static rtx make_field_assignment (rtx);
static rtx apply_distributive_law (rtx);
static rtx simplify_and_const_int (rtx, enum machine_mode, rtx, unsigned long long);

static unsigned long long nonzero_bits (rtx, enum machine_mode);
static unsigned int num_sign_bit_copies (rtx, enum machine_mode);
static int merge_outer_ops (enum rtx_code *, long long *, enum rtx_code, long long, enum machine_mode, int *);


static rtx simplify_shift_const (rtx, enum rtx_code, enum machine_mode, rtx, int);

static int recog_for_combine (rtx *, rtx, rtx *);
static rtx gen_lowpart_for_combine (enum machine_mode, rtx);
static rtx gen_binary (enum rtx_code, enum machine_mode, rtx, rtx);

static enum rtx_code simplify_comparison (enum rtx_code, rtx *, rtx *);
static void update_table_tick (rtx);
static void record_value_for_reg (rtx, rtx, rtx);
static void check_promoted_subreg (rtx, rtx);
static void record_dead_and_set_regs_1 (rtx, rtx, void *);
static void record_dead_and_set_regs (rtx);
static int get_last_value_validate (rtx *, rtx, int, int);
static rtx get_last_value (rtx);
static int use_crosses_set_p (rtx, int);
static void reg_dead_at_p_1 (rtx, rtx, void *);
static int reg_dead_at_p (rtx, rtx);
static void move_deaths (rtx, rtx, int, rtx, rtx *);
static int reg_bitfield_target_p (rtx, rtx);
static void distribute_notes (rtx, rtx, rtx, rtx, rtx, rtx);
static void distribute_links (rtx);
static void mark_used_regs_combine (rtx);
static int insn_cuid (rtx);
static void record_promoted_value (rtx, rtx);
static rtx reversed_comparison (rtx, enum machine_mode, rtx, rtx);
static enum rtx_code combine_reversed_comparison_code (rtx);







static void
do_SUBST (into, newval)
     rtx *into, newval;
{
  struct undo *buf;
  rtx oldval = *into;

  if (oldval == newval)
    return;






  if ((mode_class[(int) (((enum machine_mode) (oldval)->mode))]) == MODE_INT
      && ((enum rtx_code) (newval)->code) == CONST_INT)
    {


      if ((((newval)->fld[0]).rtwint) != trunc_int_for_mode ((((newval)->fld[0]).rtwint),
       ((enum machine_mode) (oldval)->mode)))
 fancy_abort ("/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c", 439, __FUNCTION__);







      if ((((enum rtx_code) (oldval)->code) == SUBREG
    && ((enum rtx_code) ((((oldval)->fld[0]).rtx))->code) == CONST_INT)
   || (((enum rtx_code) (oldval)->code) == ZERO_EXTEND
       && ((enum rtx_code) ((((oldval)->fld[0]).rtx))->code) == CONST_INT))
 fancy_abort ("/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c", 451, __FUNCTION__);
     }

  if (undobuf.frees)
    buf = undobuf.frees, undobuf.frees = buf->next;
  else
    buf = (struct undo *) xmalloc (sizeof (struct undo));

  buf->is_int = 0;
  buf->where.r = into;
  buf->old_contents.r = oldval;
  *into = newval;

  buf->next = undobuf.undos, undobuf.undos = buf;
}







static void
do_SUBST_INT (into, newval)
     unsigned int *into, newval;
{
  struct undo *buf;
  unsigned int oldval = *into;

  if (oldval == newval)
    return;

  if (undobuf.frees)
    buf = undobuf.frees, undobuf.frees = buf->next;
  else
    buf = (struct undo *) xmalloc (sizeof (struct undo));

  buf->is_int = 1;
  buf->where.i = into;
  buf->old_contents.i = oldval;
  *into = newval;

  buf->next = undobuf.undos, undobuf.undos = buf;
}
# 504 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
int
combine_instructions (f, nregs)
     rtx f;
     unsigned int nregs;
{
  rtx insn, next;



  int i;
  rtx links, nextlinks;

  int new_direct_jump_p = 0;

  combine_attempts = 0;
  combine_merges = 0;
  combine_extras = 0;
  combine_successes = 0;

  combine_max_regno = nregs;

  reg_nonzero_bits = ((unsigned long long *)
        xcalloc (nregs, sizeof (unsigned long long)));
  reg_sign_bit_copies
    = (unsigned char *) xcalloc (nregs, sizeof (unsigned char));

  reg_last_death = (rtx *) xmalloc (nregs * sizeof (rtx));
  reg_last_set = (rtx *) xmalloc (nregs * sizeof (rtx));
  reg_last_set_value = (rtx *) xmalloc (nregs * sizeof (rtx));
  reg_last_set_table_tick = (int *) xmalloc (nregs * sizeof (int));
  reg_last_set_label = (int *) xmalloc (nregs * sizeof (int));
  reg_last_set_invalid = (char *) xmalloc (nregs * sizeof (char));
  reg_last_set_mode
    = (enum machine_mode *) xmalloc (nregs * sizeof (enum machine_mode));
  reg_last_set_nonzero_bits
    = (unsigned long long *) xmalloc (nregs * sizeof (long long));
  reg_last_set_sign_bit_copies
    = (char *) xmalloc (nregs * sizeof (char));

  init_reg_last_arrays ();

  init_recog_no_volatile ();



  for (insn = f, i = 0; insn; insn = (((insn)->fld[2]).rtx))
    if ((((insn)->fld[0]).rtint) > i)
      i = (((insn)->fld[0]).rtint);

  uid_cuid = (int *) xmalloc ((i + 1) * sizeof (int));
  max_uid_cuid = i;

  nonzero_bits_mode = mode_for_size ((8 * 8), MODE_INT, 0);




  nonzero_sign_valid = 0;
# 574 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  label_tick = 1;



  subst_prev_insn = (rtx) 0;

  setup_incoming_promotions ();

  refresh_blocks = sbitmap_alloc (n_basic_blocks);
  sbitmap_zero (refresh_blocks);
  need_refresh = 0;

  for (insn = f, i = 0; insn; insn = (((insn)->fld[2]).rtx))
    {
      uid_cuid[(((insn)->fld[0]).rtint)] = ++i;
      subst_low_cuid = i;
      subst_insn = insn;

      if (((rtx_class[(int) (((enum rtx_code) (insn)->code))]) == 'i'))
 {
   note_stores ((((insn)->fld[3]).rtx), set_nonzero_bits_and_sign_copies,
         ((void*)0));
   record_dead_and_set_regs (insn);







 }

      if (((enum rtx_code) (insn)->code) == CODE_LABEL)
 label_tick++;
    }

  nonzero_sign_valid = 1;



  this_basic_block = -1;
  label_tick = 1;
  last_call_cuid = 0;
  mem_last_set = 0;
  init_reg_last_arrays ();
  setup_incoming_promotions ();

  for (insn = f; insn; insn = next ? next : (((insn)->fld[2]).rtx))
    {
      next = 0;


      if (this_basic_block + 1 < n_basic_blocks
   && ((((basic_block_info)->data.bb[(this_basic_block + 1)]))->head) == insn)
 this_basic_block++;

      if (((enum rtx_code) (insn)->code) == CODE_LABEL)
 label_tick++;

      else if (((rtx_class[(int) (((enum rtx_code) (insn)->code))]) == 'i'))
 {


   check_promoted_subreg (insn, (((insn)->fld[3]).rtx));



   for (links = (((insn)->fld[5]).rtx); links; links = (((links)->fld[1]).rtx))
     if ((next = try_combine (insn, (((links)->fld[0]).rtx),
         (rtx) 0, &new_direct_jump_p)) != 0)
       goto retry;



   for (links = (((insn)->fld[5]).rtx); links; links = (((links)->fld[1]).rtx))
     {
       rtx link = (((links)->fld[0]).rtx);



       if (((enum rtx_code) (link)->code) == NOTE)
  continue;

       for (nextlinks = (((link)->fld[5]).rtx);
     nextlinks;
     nextlinks = (((nextlinks)->fld[1]).rtx))
  if ((next = try_combine (insn, link,
      (((nextlinks)->fld[0]).rtx),
      &new_direct_jump_p)) != 0)
    goto retry;
     }
# 728 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
   for (links = (((insn)->fld[5]).rtx); links; links = (((links)->fld[1]).rtx))
     for (nextlinks = (((links)->fld[1]).rtx); nextlinks;
   nextlinks = (((nextlinks)->fld[1]).rtx))
       if ((next = try_combine (insn, (((links)->fld[0]).rtx),
           (((nextlinks)->fld[0]).rtx),
           &new_direct_jump_p)) != 0)
  goto retry;

   if (((enum rtx_code) (insn)->code) != NOTE)
     record_dead_and_set_regs (insn);

 retry:
   ;
 }
    }

  delete_noop_moves (f);

  if (need_refresh)
    {
      update_life_info (refresh_blocks, UPDATE_LIFE_GLOBAL_RM_NOTES,
   1);
    }


  free(refresh_blocks);
  free (reg_nonzero_bits);
  free (reg_sign_bit_copies);
  free (reg_last_death);
  free (reg_last_set);
  free (reg_last_set_value);
  free (reg_last_set_table_tick);
  free (reg_last_set_label);
  free (reg_last_set_invalid);
  free (reg_last_set_mode);
  free (reg_last_set_nonzero_bits);
  free (reg_last_set_sign_bit_copies);
  free (uid_cuid);

  {
    struct undo *undo, *next;
    for (undo = undobuf.frees; undo; undo = next)
      {
 next = undo->next;
 free (undo);
      }
    undobuf.frees = 0;
  }

  total_attempts += combine_attempts;
  total_merges += combine_merges;
  total_extras += combine_extras;
  total_successes += combine_successes;

  nonzero_sign_valid = 0;


  init_recog ();

  return new_direct_jump_p;
}



static void
init_reg_last_arrays ()
{
  unsigned int nregs = combine_max_regno;

  ((__builtin_object_size ((char *) reg_last_death, 0) != (size_t) -1) ? __builtin___memset_chk ((char *) reg_last_death, 0, nregs * sizeof (rtx), __builtin_object_size ((char *) reg_last_death, 0)) : __inline_memset_chk ((char *) reg_last_death, 0, nregs * sizeof (rtx)));
  ((__builtin_object_size ((char *) reg_last_set, 0) != (size_t) -1) ? __builtin___memset_chk ((char *) reg_last_set, 0, nregs * sizeof (rtx), __builtin_object_size ((char *) reg_last_set, 0)) : __inline_memset_chk ((char *) reg_last_set, 0, nregs * sizeof (rtx)));
  ((__builtin_object_size ((char *) reg_last_set_value, 0) != (size_t) -1) ? __builtin___memset_chk ((char *) reg_last_set_value, 0, nregs * sizeof (rtx), __builtin_object_size ((char *) reg_last_set_value, 0)) : __inline_memset_chk ((char *) reg_last_set_value, 0, nregs * sizeof (rtx)));
  ((__builtin_object_size ((char *) reg_last_set_table_tick, 0) != (size_t) -1) ? __builtin___memset_chk ((char *) reg_last_set_table_tick, 0, nregs * sizeof (int), __builtin_object_size ((char *) reg_last_set_table_tick, 0)) : __inline_memset_chk ((char *) reg_last_set_table_tick, 0, nregs * sizeof (int)));
  ((__builtin_object_size ((char *) reg_last_set_label, 0) != (size_t) -1) ? __builtin___memset_chk ((char *) reg_last_set_label, 0, nregs * sizeof (int), __builtin_object_size ((char *) reg_last_set_label, 0)) : __inline_memset_chk ((char *) reg_last_set_label, 0, nregs * sizeof (int)));
  ((__builtin_object_size (reg_last_set_invalid, 0) != (size_t) -1) ? __builtin___memset_chk (reg_last_set_invalid, 0, nregs * sizeof (char), __builtin_object_size (reg_last_set_invalid, 0)) : __inline_memset_chk (reg_last_set_invalid, 0, nregs * sizeof (char)));
  ((__builtin_object_size ((char *) reg_last_set_mode, 0) != (size_t) -1) ? __builtin___memset_chk ((char *) reg_last_set_mode, 0, nregs * sizeof (enum machine_mode), __builtin_object_size ((char *) reg_last_set_mode, 0)) : __inline_memset_chk ((char *) reg_last_set_mode, 0, nregs * sizeof (enum machine_mode)));
  ((__builtin_object_size ((char *) reg_last_set_nonzero_bits, 0) != (size_t) -1) ? __builtin___memset_chk ((char *) reg_last_set_nonzero_bits, 0, nregs * sizeof (long long), __builtin_object_size ((char *) reg_last_set_nonzero_bits, 0)) : __inline_memset_chk ((char *) reg_last_set_nonzero_bits, 0, nregs * sizeof (long long)));
  ((__builtin_object_size (reg_last_set_sign_bit_copies, 0) != (size_t) -1) ? __builtin___memset_chk (reg_last_set_sign_bit_copies, 0, nregs * sizeof (char), __builtin_object_size (reg_last_set_sign_bit_copies, 0)) : __inline_memset_chk (reg_last_set_sign_bit_copies, 0, nregs * sizeof (char)));
}



static void
setup_incoming_promotions ()
{
# 837 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
}
# 850 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static void
set_nonzero_bits_and_sign_copies (x, set, data)
     rtx x;
     rtx set;
     void *data __attribute__ ((__unused__));
{
  unsigned int num;

  if (((enum rtx_code) (x)->code) == REG
      && (((x)->fld[0]).rtuint) >= 53


      && ! bitmap_bit_p ((((basic_block_info)->data.bb[(0)]))->global_live_at_start, (((x)->fld[0]).rtuint))
      && (mode_bitsize[(int) (((enum machine_mode) (x)->mode))]) <= (8 * 8))
    {
      if (set == 0 || ((enum rtx_code) (set)->code) == CLOBBER)
 {
   reg_nonzero_bits[(((x)->fld[0]).rtuint)] = mode_mask_array[(int) (((enum machine_mode) (x)->mode))];
   reg_sign_bit_copies[(((x)->fld[0]).rtuint)] = 1;
   return;
 }



      set = expand_field_assignment (set);




      if ((((set)->fld[0]).rtx) == x
   || (((enum rtx_code) ((((set)->fld[0]).rtx))->code) == SUBREG
       && ((mode_size[(int) (((enum machine_mode) ((((set)->fld[0]).rtx))->mode))])
    > (mode_size[(int) (((enum machine_mode) (((((((set)->fld[0]).rtx))->fld[0]).rtx))->mode))]))
       && ((((((set)->fld[0]).rtx))->fld[0]).rtx) == x))
 {
   rtx src = (((set)->fld[1]).rtx);
# 909 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
   if (reg_nonzero_bits[(((x)->fld[0]).rtuint)] != ~(unsigned long long) 0)
     reg_nonzero_bits[(((x)->fld[0]).rtuint)]
       |= nonzero_bits (src, nonzero_bits_mode);
   num = num_sign_bit_copies ((((set)->fld[1]).rtx), ((enum machine_mode) (x)->mode));
   if (reg_sign_bit_copies[(((x)->fld[0]).rtuint)] == 0
       || reg_sign_bit_copies[(((x)->fld[0]).rtuint)] > num)
     reg_sign_bit_copies[(((x)->fld[0]).rtuint)] = num;
 }
      else
 {
   reg_nonzero_bits[(((x)->fld[0]).rtuint)] = mode_mask_array[(int) (((enum machine_mode) (x)->mode))];
   reg_sign_bit_copies[(((x)->fld[0]).rtuint)] = 1;
 }
    }
}
# 935 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static int
can_combine_p (insn, i3, pred, succ, pdest, psrc)
     rtx insn;
     rtx i3;
     rtx pred __attribute__ ((__unused__));
     rtx succ;
     rtx *pdest, *psrc;
{
  int i;
  rtx set = 0, src, dest;
  rtx p;



  int all_adjacent = (succ ? (next_active_insn (insn) == succ
         && next_active_insn (succ) == i3)
        : next_active_insn (insn) == i3);
# 968 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  if (((enum rtx_code) ((((insn)->fld[3]).rtx))->code) == SET)
    set = (((insn)->fld[3]).rtx);
  else if (((enum rtx_code) ((((insn)->fld[3]).rtx))->code) == PARALLEL
    && ((enum rtx_code) (((((((((insn)->fld[3]).rtx))->fld[0]).rtvec))->elem[0]))->code) == SET)
    {
      for (i = 0; i < ((((((((insn)->fld[3]).rtx))->fld[0]).rtvec))->num_elem); i++)
 {
   rtx elt = ((((((((insn)->fld[3]).rtx))->fld[0]).rtvec))->elem[i]);

   switch (((enum rtx_code) (elt)->code))
     {


     case USE:
# 995 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
       if (((enum rtx_code) ((((elt)->fld[0]).rtx))->code) == REG
    && ((enum rtx_code) ((((i3)->fld[3]).rtx))->code) == PARALLEL)
  {
    rtx i3pat = (((i3)->fld[3]).rtx);
    int i = (((((i3pat)->fld[0]).rtvec))->num_elem) - 1;
    unsigned int regno = ((((((elt)->fld[0]).rtx))->fld[0]).rtuint);

    do
      {
        rtx i3elt = (((((i3pat)->fld[0]).rtvec))->elem[i]);

        if (((enum rtx_code) (i3elt)->code) == USE
     && ((enum rtx_code) ((((i3elt)->fld[0]).rtx))->code) == REG
     && (((((((i3elt)->fld[0]).rtx))->fld[0]).rtuint) == regno
         ? reg_set_between_p ((((elt)->fld[0]).rtx),
         (((insn)->fld[1]).rtx), i3)
         : regno >= 53))
   return 0;
      }
    while (--i >= 0);
  }
       break;


     case CLOBBER:
       break;

     case SET:


       if (find_reg_note (insn, REG_UNUSED, (((elt)->fld[0]).rtx))
    && ! side_effects_p (elt))
  break;



       if (set)
  return 0;

       set = elt;
       break;

     default:

       return 0;
     }
 }

      if (set == 0


   || ((enum rtx_code) ((((set)->fld[1]).rtx))->code) == ASM_OPERANDS)
 return 0;
    }
  else
    return 0;

  if (set == 0)
    return 0;

  set = expand_field_assignment (set);
  src = (((set)->fld[1]).rtx), dest = (((set)->fld[0]).rtx);


  if (dest == (global_rtl[GR_STACK_POINTER])

      || ((enum rtx_code) (dest)->code) == ZERO_EXTRACT || ((enum rtx_code) (dest)->code) == STRICT_LOW_PART


      || (rtx_equal_p (src, dest) && find_reg_note (insn, REG_EQUAL, (rtx) 0))

      || ((enum rtx_code) (src)->code) == ASM_OPERANDS

      || ((enum rtx_code) (src)->code) == CALL

      || (((enum rtx_code) (i3)->code) == CALL_INSN
   && (find_reg_fusage (i3, USE, dest)
       || (((enum rtx_code) (dest)->code) == REG
    && (((dest)->fld[0]).rtuint) < 53
    && global_regs[(((dest)->fld[0]).rtuint)])))

      || 0
      || (succ && 0)
# 1087 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
      || (succ && ! all_adjacent
   && reg_used_between_p (dest, succ, i3))
# 1098 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
      || (! all_adjacent
   && (((((enum rtx_code) (src)->code) != MEM
  || ! find_reg_note (insn, REG_EQUIV, src))
        && use_crosses_set_p (src, ((((insn)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (insn) : uid_cuid[(((insn)->fld[0]).rtint)])))
       || (((enum rtx_code) (src)->code) == ASM_OPERANDS && ((src)->volatil))
       || ((enum rtx_code) (src)->code) == UNSPEC_VOLATILE))


      || find_reg_note (i3, REG_NO_CONFLICT, dest)
      || (succ && find_reg_note (succ, REG_NO_CONFLICT, dest))





      || (((((insn)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (insn) : uid_cuid[(((insn)->fld[0]).rtint)]) < last_call_cuid && ! (((enum rtx_code) (src)->code) == LABEL_REF || ((enum rtx_code) (src)->code) == SYMBOL_REF || ((enum rtx_code) (src)->code) == CONST_INT || ((enum rtx_code) (src)->code) == CONST_DOUBLE || ((enum rtx_code) (src)->code) == CONST || ((enum rtx_code) (src)->code) == HIGH || ((enum rtx_code) (src)->code) == CONST_VECTOR || ((enum rtx_code) (src)->code) == CONSTANT_P_RTX)))
    return 0;


  if (((enum rtx_code) (dest)->code) == REG)
    {
# 1128 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
      if (((enum rtx_code) (src)->code) == REG
   && (((((dest)->fld[0]).rtuint) < 53
        && ! ix86_hard_regno_mode_ok (((((dest)->fld[0]).rtuint)), (((enum machine_mode) (dest)->mode))))







       || ((((src)->fld[0]).rtuint) < 53
    && ! ix86_hard_regno_mode_ok (((((src)->fld[0]).rtuint)), (((enum machine_mode) (src)->mode))))))
 return 0;
    }
  else if (((enum rtx_code) (dest)->code) != CC0)
    return 0;




  if (((enum rtx_code) ((((i3)->fld[3]).rtx))->code) == PARALLEL)
    for (i = ((((((((i3)->fld[3]).rtx))->fld[0]).rtvec))->num_elem) - 1; i >= 0; i--)
      if (((enum rtx_code) (((((((((i3)->fld[3]).rtx))->fld[0]).rtvec))->elem[i]))->code) == CLOBBER
   && (reg_overlap_mentioned_p ((((((((((((i3)->fld[3]).rtx))->fld[0]).rtvec))->elem[i]))->fld[0]).rtx),
           src)
       || rtx_equal_p ((((((((((((i3)->fld[3]).rtx))->fld[0]).rtvec))->elem[i]))->fld[0]).rtx), dest)))
 return 0;




  if (((enum rtx_code) (src)->code) == ASM_OPERANDS || volatile_refs_p (src))
    {

      if (succ != 0 && volatile_refs_p ((((succ)->fld[3]).rtx)))
        return 0;

      for (p = (((insn)->fld[2]).rtx); p != i3; p = (((p)->fld[2]).rtx))
        if (((rtx_class[(int) (((enum rtx_code) (p)->code))]) == 'i') && p != succ && volatile_refs_p ((((p)->fld[3]).rtx)))
   return 0;
    }




  if (((enum rtx_code) (src)->code) == ASM_OPERANDS
      && ((enum rtx_code) (dest)->code) == REG && (((dest)->fld[0]).rtuint) < 53)
    return 0;




  for (p = (((insn)->fld[2]).rtx); p != i3; p = (((p)->fld[2]).rtx))
    if (((rtx_class[(int) (((enum rtx_code) (p)->code))]) == 'i') && p != succ && volatile_insn_p ((((p)->fld[3]).rtx)))
      return 0;
# 1218 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  *pdest = dest;
  *psrc = src;

  return 1;
}




static int
sets_function_arg_p (pat)
     rtx pat;
{
  int i;
  rtx inner_dest;

  switch (((enum rtx_code) (pat)->code))
    {
    case INSN:
      return sets_function_arg_p ((((pat)->fld[3]).rtx));

    case PARALLEL:
      for (i = (((((pat)->fld[0]).rtvec))->num_elem); --i >= 0;)
 if (sets_function_arg_p ((((((pat)->fld[0]).rtvec))->elem[i])))
   return 1;

      break;

    case SET:
      inner_dest = (((pat)->fld[0]).rtx);
      while (((enum rtx_code) (inner_dest)->code) == STRICT_LOW_PART
      || ((enum rtx_code) (inner_dest)->code) == SUBREG
      || ((enum rtx_code) (inner_dest)->code) == ZERO_EXTRACT)
 inner_dest = (((inner_dest)->fld[0]).rtx);

      return (((enum rtx_code) (inner_dest)->code) == REG
       && (((inner_dest)->fld[0]).rtuint) < 53
       && ix86_function_arg_regno_p ((((inner_dest)->fld[0]).rtuint)));

    default:
      break;
    }

  return 0;
}
# 1301 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static int
combinable_i3pat (i3, loc, i2dest, i1dest, i1_not_in_src, pi3dest_killed)
     rtx i3;
     rtx *loc;
     rtx i2dest;
     rtx i1dest;
     int i1_not_in_src;
     rtx *pi3dest_killed;
{
  rtx x = *loc;

  if (((enum rtx_code) (x)->code) == SET)
    {
      rtx set = expand_field_assignment (x);
      rtx dest = (((set)->fld[0]).rtx);
      rtx src = (((set)->fld[1]).rtx);
      rtx inner_dest = dest;





      do_SUBST(&(*loc), (set));

      while (((enum rtx_code) (inner_dest)->code) == STRICT_LOW_PART
      || ((enum rtx_code) (inner_dest)->code) == SUBREG
      || ((enum rtx_code) (inner_dest)->code) == ZERO_EXTRACT)
 inner_dest = (((inner_dest)->fld[0]).rtx);
# 1356 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
      if ((inner_dest != dest
    && (reg_overlap_mentioned_p (i2dest, inner_dest)
        || (i1dest && reg_overlap_mentioned_p (i1dest, inner_dest))))
# 1370 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
   || (((enum rtx_code) (inner_dest)->code) == REG
       && (((inner_dest)->fld[0]).rtuint) < 53
       && (! ix86_hard_regno_mode_ok (((((inner_dest)->fld[0]).rtuint)), (((enum machine_mode) (inner_dest)->mode)))))

   || (i1_not_in_src && reg_overlap_mentioned_p (i1dest, src)))
 return 0;






      if (pi3dest_killed && ((enum rtx_code) (dest)->code) == REG
   && reg_referenced_p (dest, (((i3)->fld[3]).rtx))
   && (((dest)->fld[0]).rtuint) != 20

   && (((dest)->fld[0]).rtuint) != 6


   && ((((dest)->fld[0]).rtuint) != 16
       || ! fixed_regs [(((dest)->fld[0]).rtuint)])

   && (((dest)->fld[0]).rtuint) != 7)
 {
   if (*pi3dest_killed)
     return 0;

   *pi3dest_killed = dest;
 }
    }

  else if (((enum rtx_code) (x)->code) == PARALLEL)
    {
      int i;

      for (i = 0; i < (((((x)->fld[0]).rtvec))->num_elem); i++)
 if (! combinable_i3pat (i3, &(((((x)->fld[0]).rtvec))->elem[i]), i2dest, i1dest,
    i1_not_in_src, pi3dest_killed))
   return 0;
    }

  return 1;
}




static int
contains_muldiv (x)
     rtx x;
{
  switch (((enum rtx_code) (x)->code))
    {
    case MOD: case DIV: case UMOD: case UDIV:
      return 1;

    case MULT:
      return ! (((enum rtx_code) ((((x)->fld[1]).rtx))->code) == CONST_INT
  && exact_log2_wide ((unsigned long long) (((((((x)->fld[1]).rtx))->fld[0]).rtwint))) >= 0);
    default:
      switch ((rtx_class[(int) (((enum rtx_code) (x)->code))]))
 {
 case 'c': case '<': case '2':
   return contains_muldiv ((((x)->fld[0]).rtx))
     || contains_muldiv ((((x)->fld[1]).rtx));

 case '1':
   return contains_muldiv ((((x)->fld[0]).rtx));

 default:
   return 0;
 }
    }
}





static int
cant_combine_insn_p (insn)
     rtx insn;
{
  rtx set;
  rtx src, dest;




  if (! ((rtx_class[(int) (((enum rtx_code) (insn)->code))]) == 'i'))
    return 1;
# 1469 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  set = (((rtx_class[(int) (((enum rtx_code) (insn)->code))]) == 'i') ? (((enum rtx_code) ((((insn)->fld[3]).rtx))->code) == SET ? (((insn)->fld[3]).rtx) : single_set_2 (insn, (((insn)->fld[3]).rtx))) : (rtx) 0);
  if (! set)
    return 0;
  src = (((set)->fld[1]).rtx);
  dest = (((set)->fld[0]).rtx);
  if (((enum rtx_code) (src)->code) == SUBREG)
    src = (((src)->fld[0]).rtx);
  if (((enum rtx_code) (dest)->code) == SUBREG)
    dest = (((dest)->fld[0]).rtx);
  if ((((enum rtx_code) (src)->code) == REG) && (((enum rtx_code) (dest)->code) == REG)
      && (((((src)->fld[0]).rtuint) < 53
    && ! fixed_regs[(((src)->fld[0]).rtuint)])
   || ((((dest)->fld[0]).rtuint) < 53
       && ! fixed_regs[(((dest)->fld[0]).rtuint)])))
    return 1;

  return 0;
}
# 1504 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static rtx
try_combine (i3, i2, i1, new_direct_jump_p)
     rtx i3, i2, i1;
     int *new_direct_jump_p;
{

  rtx newpat, newi2pat = 0;
  int substed_i2 = 0, substed_i1 = 0;

  int added_sets_1, added_sets_2;

  int total_sets;

  int i2_is_used;

  int insn_code_number, i2_code_number = 0, other_code_number = 0;



  rtx i3dest_killed = 0;

  rtx i2dest, i2src, i1dest = 0, i1src = 0;

  rtx i2pat;

  int i2dest_in_i2src = 0, i1dest_in_i1src = 0, i2dest_in_i1src = 0;
  int i1_feeds_i3 = 0;

  rtx new_i3_notes, new_i2_notes;

  int i3_subst_into_i2 = 0;

  int have_mult = 0;

  int maxreg;
  rtx temp;
  rtx link;
  int i;



  if (cant_combine_insn_p (i3)
      || cant_combine_insn_p (i2)
      || (i1 && cant_combine_insn_p (i1))
# 1556 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
      )
    return 0;

  combine_attempts++;
  undobuf.other_insn = 0;


  ((newpat_used_regs) = ((HARD_REG_ELT_TYPE) (0)));



  if (i1 && ((((i1)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (i1) : uid_cuid[(((i1)->fld[0]).rtint)]) > ((((i2)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (i2) : uid_cuid[(((i2)->fld[0]).rtint)]))
    temp = i1, i1 = i2, i2 = temp;

  added_links_insn = 0;
# 1588 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  if (i1 == 0 && ((enum rtx_code) (i3)->code) == INSN && ((enum rtx_code) ((((i3)->fld[3]).rtx))->code) == SET
      && ((enum rtx_code) (((((((i3)->fld[3]).rtx))->fld[1]).rtx))->code) == REG
      && (((((((((i3)->fld[3]).rtx))->fld[1]).rtx))->fld[0]).rtuint) >= 53
      && find_reg_note (i3, REG_DEAD, ((((((i3)->fld[3]).rtx))->fld[1]).rtx))
      && ((enum rtx_code) ((((i2)->fld[3]).rtx))->code) == PARALLEL
      && ! side_effects_p (((((((i3)->fld[3]).rtx))->fld[0]).rtx))




      && ((enum rtx_code) (((((((i3)->fld[3]).rtx))->fld[0]).rtx))->code) != ZERO_EXTRACT
      && ((enum rtx_code) (((((((i3)->fld[3]).rtx))->fld[0]).rtx))->code) != STRICT_LOW_PART
      && ! reg_overlap_mentioned_p (((((((i3)->fld[3]).rtx))->fld[1]).rtx),
        ((((((i3)->fld[3]).rtx))->fld[0]).rtx))
      && next_real_insn (i2) == i3)
    {
      rtx p2 = (((i2)->fld[3]).rtx);
# 1616 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
      for (i = 0; i < (((((p2)->fld[0]).rtvec))->num_elem); i++)
 if ((((enum rtx_code) ((((((p2)->fld[0]).rtvec))->elem[i]))->code) == SET
      || ((enum rtx_code) ((((((p2)->fld[0]).rtvec))->elem[i]))->code) == CLOBBER)
     && reg_overlap_mentioned_p (((((((i3)->fld[3]).rtx))->fld[0]).rtx),
     ((((((((p2)->fld[0]).rtvec))->elem[i]))->fld[0]).rtx)))
   break;

      if (i == (((((p2)->fld[0]).rtvec))->num_elem))
 for (i = 0; i < (((((p2)->fld[0]).rtvec))->num_elem); i++)
   if ((((enum rtx_code) ((((((p2)->fld[0]).rtvec))->elem[i]))->code) == SET
        || ((enum rtx_code) ((((((p2)->fld[0]).rtvec))->elem[i]))->code) == CLOBBER)
       && ((((((((p2)->fld[0]).rtvec))->elem[i]))->fld[0]).rtx) == ((((((i3)->fld[3]).rtx))->fld[1]).rtx))
     {
       combine_merges++;

       subst_insn = i3;
       subst_low_cuid = ((((i2)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (i2) : uid_cuid[(((i2)->fld[0]).rtint)]);

       added_sets_2 = added_sets_1 = 0;
       i2dest = ((((((i3)->fld[3]).rtx))->fld[1]).rtx);




       do_SUBST(&(((((((((p2)->fld[0]).rtvec))->elem[i]))->fld[0]).rtx)), (((((((i3)->fld[3]).rtx))->fld[0]).rtx)));


       newpat = p2;
       i3_subst_into_i2 = 1;
       goto validate_replacement;
     }
    }




  if (i1 == 0
      && (temp = (((rtx_class[(int) (((enum rtx_code) (i2)->code))]) == 'i') ? (((enum rtx_code) ((((i2)->fld[3]).rtx))->code) == SET ? (((i2)->fld[3]).rtx) : single_set_2 (i2, (((i2)->fld[3]).rtx))) : (rtx) 0)) != 0
      && (((enum rtx_code) ((((temp)->fld[1]).rtx))->code) == CONST_INT
   || ((enum rtx_code) ((((temp)->fld[1]).rtx))->code) == CONST_DOUBLE)
      && ((enum rtx_code) ((((temp)->fld[0]).rtx))->code) == REG
      && (mode_class[(int) (((enum machine_mode) ((((temp)->fld[0]).rtx))->mode))]) == MODE_INT
      && (mode_size[(int) (((enum machine_mode) ((((temp)->fld[0]).rtx))->mode))]) == 2 * ((target_flags & 0x02000000) ? 8 : 4)
      && ((enum rtx_code) ((((i3)->fld[3]).rtx))->code) == SET
      && ((enum rtx_code) (((((((i3)->fld[3]).rtx))->fld[0]).rtx))->code) == SUBREG
      && (((((((((i3)->fld[3]).rtx))->fld[0]).rtx))->fld[0]).rtx) == (((temp)->fld[0]).rtx)
      && (mode_class[(int) (((enum machine_mode) (((((((i3)->fld[3]).rtx))->fld[0]).rtx))->mode))]) == MODE_INT
      && (mode_size[(int) (((enum machine_mode) (((((((i3)->fld[3]).rtx))->fld[0]).rtx))->mode))]) == ((target_flags & 0x02000000) ? 8 : 4)
      && ((enum rtx_code) (((((((i3)->fld[3]).rtx))->fld[1]).rtx))->code) == CONST_INT)
    {
      long long lo, hi;

      if (((enum rtx_code) ((((temp)->fld[1]).rtx))->code) == CONST_INT)
 lo = ((((((temp)->fld[1]).rtx))->fld[0]).rtwint), hi = lo < 0 ? -1 : 0;
      else
 {
   lo = ((((((temp)->fld[1]).rtx))->fld[1]).rtwint);
   hi = ((((((temp)->fld[1]).rtx))->fld[2]).rtwint);
 }

      if (subreg_lowpart_p (((((((i3)->fld[3]).rtx))->fld[0]).rtx)))
 {


   if ((8 * 8) < ((target_flags & 0x02000000) ? 64 : 32))
     fancy_abort ("/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c", 1681, __FUNCTION__);

   lo &= ~((((unsigned long long) (1) << (((target_flags & 0x02000000) ? 64 : 32) - 1)) << 1) - 1);
   lo |= ((((((((((i3)->fld[3]).rtx))->fld[1]).rtx))->fld[0]).rtwint)
   & ((((unsigned long long) (1) << (((target_flags & 0x02000000) ? 64 : 32) - 1)) << 1) - 1));
 }
      else if ((8 * 8) == ((target_flags & 0x02000000) ? 64 : 32))
 hi = (((((((((i3)->fld[3]).rtx))->fld[1]).rtx))->fld[0]).rtwint);
      else if ((8 * 8) >= 2 * ((target_flags & 0x02000000) ? 64 : 32))
 {
   int sign = -(int) ((unsigned long long) lo
        >> ((8 * 8) - 1));

   lo &= ~ ((((unsigned long long) ((((unsigned long long) (1) << (((target_flags & 0x02000000) ? 64 : 32) - 1)) << 1) - 1) << (((target_flags & 0x02000000) ? 64 : 32) - 1)) << 1));

   lo |= ((((unsigned long long) ((((((((((i3)->fld[3]).rtx))->fld[1]).rtx))->fld[0]).rtwint)) << (((target_flags & 0x02000000) ? 64 : 32) - 1)) << 1));

   if (hi == sign)
     hi = lo < 0 ? -1 : 0;
 }
      else


 fancy_abort ("/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c", 1704, __FUNCTION__);

      combine_merges++;
      subst_insn = i3;
      subst_low_cuid = ((((i2)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (i2) : uid_cuid[(((i2)->fld[0]).rtint)]);
      added_sets_2 = added_sets_1 = 0;
      i2dest = (((temp)->fld[0]).rtx);

      do_SUBST(&((((temp)->fld[1]).rtx)), (immed_double_const (lo, hi, ((enum machine_mode) ((((temp)->fld[0]).rtx))->mode))));


      newpat = (((i2)->fld[3]).rtx);
      goto validate_replacement;
    }
# 1733 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  if (i1 == 0 && ((enum rtx_code) ((((i2)->fld[3]).rtx))->code) == PARALLEL
      && ((((((((i2)->fld[3]).rtx))->fld[0]).rtvec))->num_elem) >= 2
      && ((enum rtx_code) (((((((((i2)->fld[3]).rtx))->fld[0]).rtvec))->elem[0]))->code) == SET
      && ((mode_class[(int) (((enum machine_mode) ((((((((((((i2)->fld[3]).rtx))->fld[0]).rtvec))->elem[0]))->fld[0]).rtx))->mode))])
   == MODE_CC)
      && ((enum rtx_code) ((((((((((((i2)->fld[3]).rtx))->fld[0]).rtvec))->elem[0]))->fld[1]).rtx))->code) == COMPARE
      && ((((((((((((((i2)->fld[3]).rtx))->fld[0]).rtvec))->elem[0]))->fld[1]).rtx))->fld[1]).rtx) == (const_int_rtx[64])
      && ((enum rtx_code) (((((((((i2)->fld[3]).rtx))->fld[0]).rtvec))->elem[1]))->code) == SET
      && ((enum rtx_code) ((((((((((((i2)->fld[3]).rtx))->fld[0]).rtvec))->elem[1]))->fld[0]).rtx))->code) == REG
      && rtx_equal_p (((((((((((((((i2)->fld[3]).rtx))->fld[0]).rtvec))->elem[0]))->fld[1]).rtx))->fld[0]).rtx),
        (((((((((((i2)->fld[3]).rtx))->fld[0]).rtvec))->elem[1]))->fld[1]).rtx)))
    {
      for (i = ((((((((i2)->fld[3]).rtx))->fld[0]).rtvec))->num_elem) - 1; i >= 2; i--)
 if (((enum rtx_code) (((((((((i2)->fld[3]).rtx))->fld[0]).rtvec))->elem[i]))->code) != CLOBBER)
   break;

      if (i == 1)
 {





   subst_prev_insn = i1
     = gen_rtx_fmt_iuueiee (INSN, (VOIDmode), ((((i2)->fld[0]).rtint)), ((rtx) 0), (i2), (((((((((i2)->fld[3]).rtx))->fld[0]).rtvec))->elem[1])), (-1), ((rtx) 0), ((rtx) 0));



   do_SUBST(&((((i2)->fld[3]).rtx)), (((((((((i2)->fld[3]).rtx))->fld[0]).rtvec))->elem[0])));
   do_SUBST(&((((((((((i2)->fld[3]).rtx))->fld[1]).rtx))->fld[0]).rtx)), (((((((i1)->fld[3]).rtx))->fld[0]).rtx)));

 }
    }



  if (! can_combine_p (i2, i3, i1, (rtx) 0, &i2dest, &i2src)
      || (i1 && ! can_combine_p (i1, i3, (rtx) 0, i2, &i1dest, &i1src)))
    {
      undo_all ();
      return 0;
    }



  i2dest_in_i2src = reg_overlap_mentioned_p (i2dest, i2src);
  i1dest_in_i1src = i1 && reg_overlap_mentioned_p (i1dest, i1src);
  i2dest_in_i1src = i1 && reg_overlap_mentioned_p (i2dest, i1src);



  i1_feeds_i3 = i1 && ! reg_overlap_mentioned_p (i1dest, i2src);


  if (! combinable_i3pat (i3, &(((i3)->fld[3]).rtx), i2dest, i1dest,
     i1 && i2dest_in_i1src && i1_feeds_i3,
     &i3dest_killed))
    {
      undo_all ();
      return 0;
    }




  if (((enum rtx_code) (i2src)->code) == MULT
      || (i1 != 0 && ((enum rtx_code) (i1src)->code) == MULT)
      || (((enum rtx_code) ((((i3)->fld[3]).rtx))->code) == SET
   && ((enum rtx_code) (((((((i3)->fld[3]).rtx))->fld[1]).rtx))->code) == MULT))
    have_mult = 1;
# 1842 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  added_sets_2 = ! dead_or_set_p (i3, i2dest);

  added_sets_1
    = i1 && ! (i1_feeds_i3 ? dead_or_set_p (i3, i1dest)
        : (dead_or_set_p (i3, i1dest) || dead_or_set_p (i2, i1dest)));
# 1855 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  i2pat = (((enum rtx_code) ((((i2)->fld[3]).rtx))->code) == PARALLEL
    ? gen_rtx_fmt_ee (SET, (VOIDmode), (i2dest), (i2src))
    : (((i2)->fld[3]).rtx));

  if (added_sets_2)
    i2pat = copy_rtx (i2pat);

  combine_merges++;



  maxreg = max_reg_num ();

  subst_insn = i3;
# 1884 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  if (flag_expensive_optimizations)
    {



      if (i1)
 {
   subst_low_cuid = ((((i1)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (i1) : uid_cuid[(((i1)->fld[0]).rtint)]);
   i1src = subst (i1src, (global_rtl[GR_PC]), (global_rtl[GR_PC]), 0, 0);
 }
      else
 {
   subst_low_cuid = ((((i2)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (i2) : uid_cuid[(((i2)->fld[0]).rtint)]);
   i2src = subst (i2src, (global_rtl[GR_PC]), (global_rtl[GR_PC]), 0, 0);
 }
    }
# 1913 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  if (i1 == 0 && added_sets_2 && ((enum rtx_code) ((((i3)->fld[3]).rtx))->code) == SET
      && ((enum rtx_code) (((((((i3)->fld[3]).rtx))->fld[1]).rtx))->code) == COMPARE
      && (((((((((i3)->fld[3]).rtx))->fld[1]).rtx))->fld[1]).rtx) == (const_int_rtx[64])
      && rtx_equal_p ((((((((((i3)->fld[3]).rtx))->fld[1]).rtx))->fld[0]).rtx), i2dest))
    {

      rtx *cc_use;
      enum machine_mode compare_mode;


      newpat = (((i3)->fld[3]).rtx);
      do_SUBST(&(((((((newpat)->fld[1]).rtx))->fld[0]).rtx)), (i2src));

      i2_is_used = 1;







      if (undobuf.other_insn == 0
   && (cc_use = find_single_use ((((newpat)->fld[0]).rtx), i3,
     &undobuf.other_insn))
   && ((compare_mode = ix86_cc_mode ((((enum rtx_code) (*cc_use)->code)), (i2src), ((const_int_rtx[64]))))

       != ((enum machine_mode) ((((newpat)->fld[0]).rtx))->mode)))
 {
   unsigned int regno = ((((((newpat)->fld[0]).rtx))->fld[0]).rtuint);
   rtx new_dest = gen_rtx_REG (compare_mode, regno);

   if (regno < 53
       || ((((reg_n_info)->data.reg[regno])->sets) == 1 && ! added_sets_2
    && ! (((((newpat)->fld[0]).rtx))->volatil)))
     {
       if (regno >= 53)
  do_SUBST(&((cfun->emit->x_regno_reg_rtx)[regno]), (new_dest));

       do_SUBST(&((((newpat)->fld[0]).rtx)), (new_dest));
       do_SUBST(&((((*cc_use)->fld[0]).rtx)), (new_dest));
       do_SUBST(&((((newpat)->fld[1]).rtx)), (gen_rtx_fmt_ee (COMPARE, (compare_mode), (i2src), ((const_int_rtx[64])))));

     }
   else
     undobuf.other_insn = 0;
 }

    }
  else

    {
      n_occurrences = 0;





      subst_low_cuid = ((((i2)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (i2) : uid_cuid[(((i2)->fld[0]).rtint)]);
      newpat = subst ((((i3)->fld[3]).rtx), i2dest, i2src, 0,
        ! i1_feeds_i3 && i1dest_in_i1src);
      substed_i2 = 1;


      i2_is_used = n_occurrences;
    }




  if (i1 && ((enum rtx_code) (newpat)->code) != CLOBBER)
    {




      if (! combinable_i3pat ((rtx) 0, &newpat, i1dest, (rtx) 0,
         0, (rtx*) 0))
 {
   undo_all ();
   return 0;
 }

      n_occurrences = 0;
      subst_low_cuid = ((((i1)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (i1) : uid_cuid[(((i1)->fld[0]).rtint)]);
      newpat = subst (newpat, i1dest, i1src, 0, 0);
      substed_i1 = 1;
    }



  if ((0 != 0
       && i2_is_used + added_sets_2 > 1)
      || (i1 != 0 && 0 != 0
   && (n_occurrences + added_sets_1 + (added_sets_2 && ! i1_feeds_i3)
       > 1))


      || max_reg_num () != maxreg

      || ((enum rtx_code) (newpat)->code) == CLOBBER


      || (((enum rtx_code) (newpat)->code) == SET && ((enum rtx_code) ((((newpat)->fld[1]).rtx))->code) == MULT
   && ! have_mult))
    {
      undo_all ();
      return 0;
    }






  if (added_sets_1 || added_sets_2)
    {
      combine_extras++;

      if (((enum rtx_code) (newpat)->code) == PARALLEL)
 {
   rtvec old = (((newpat)->fld[0]).rtvec);
   total_sets = (((((newpat)->fld[0]).rtvec))->num_elem) + added_sets_1 + added_sets_2;
   newpat = gen_rtx_fmt_E (PARALLEL, (VOIDmode), (rtvec_alloc (total_sets)));
   ((__builtin_object_size ((((newpat)->fld[0]).rtvec)->elem, 0) != (size_t) -1) ? __builtin___memcpy_chk ((((newpat)->fld[0]).rtvec)->elem, &old->elem[0], sizeof (old->elem[0]) * old->num_elem, __builtin_object_size ((((newpat)->fld[0]).rtvec)->elem, 0)) : __inline_memcpy_chk ((((newpat)->fld[0]).rtvec)->elem, &old->elem[0], sizeof (old->elem[0]) * old->num_elem));

 }
      else
 {
   rtx old = newpat;
   total_sets = 1 + added_sets_1 + added_sets_2;
   newpat = gen_rtx_fmt_E (PARALLEL, (VOIDmode), (rtvec_alloc (total_sets)));
   (((((newpat)->fld[0]).rtvec))->elem[0]) = old;
 }

      if (added_sets_1)
 (((((newpat)->fld[0]).rtvec))->elem[--total_sets])
   = (((enum rtx_code) ((((i1)->fld[3]).rtx))->code) == PARALLEL
      ? gen_rtx_fmt_ee (SET, (VOIDmode), (i1dest), (i1src)) : (((i1)->fld[3]).rtx));

      if (added_sets_2)
 {



   if (i1 == 0)
     (((((newpat)->fld[0]).rtvec))->elem[--total_sets]) = i2pat;
   else

     (((((newpat)->fld[0]).rtvec))->elem[--total_sets])
       = subst (i2pat, i1dest, i1src, 0, 0);
 }
    }



 validate_replacement:


  mark_used_regs_combine (newpat);


  insn_code_number = recog_for_combine (&newpat, i3, &new_i3_notes);
# 2085 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  if (insn_code_number < 0 && ((enum rtx_code) (newpat)->code) == PARALLEL
      && (((((newpat)->fld[0]).rtvec))->num_elem) == 2
      && ((enum rtx_code) ((((((newpat)->fld[0]).rtvec))->elem[0]))->code) == SET
      && ((enum rtx_code) ((((((newpat)->fld[0]).rtvec))->elem[1]))->code) == SET
      && ((enum rtx_code) (((((((((newpat)->fld[0]).rtvec))->elem[1]))->fld[0]).rtx))->code) == REG
      && find_reg_note (i3, REG_UNUSED, ((((((((newpat)->fld[0]).rtvec))->elem[1]))->fld[0]).rtx))
      && ! side_effects_p (((((((((newpat)->fld[0]).rtvec))->elem[1]))->fld[1]).rtx))
      && asm_noperands (newpat) < 0)
    {
      newpat = (((((newpat)->fld[0]).rtvec))->elem[0]);
      insn_code_number = recog_for_combine (&newpat, i3, &new_i3_notes);
    }

  else if (insn_code_number < 0 && ((enum rtx_code) (newpat)->code) == PARALLEL
    && (((((newpat)->fld[0]).rtvec))->num_elem) == 2
    && ((enum rtx_code) ((((((newpat)->fld[0]).rtvec))->elem[0]))->code) == SET
    && ((enum rtx_code) ((((((newpat)->fld[0]).rtvec))->elem[1]))->code) == SET
    && ((enum rtx_code) (((((((((newpat)->fld[0]).rtvec))->elem[0]))->fld[0]).rtx))->code) == REG
    && find_reg_note (i3, REG_UNUSED, ((((((((newpat)->fld[0]).rtvec))->elem[0]))->fld[0]).rtx))
    && ! side_effects_p (((((((((newpat)->fld[0]).rtvec))->elem[0]))->fld[1]).rtx))
    && asm_noperands (newpat) < 0)
    {
      newpat = (((((newpat)->fld[0]).rtvec))->elem[1]);
      insn_code_number = recog_for_combine (&newpat, i3, &new_i3_notes);
    }







  if (i1 && insn_code_number < 0 && ((enum rtx_code) (newpat)->code) == SET
      && asm_noperands (newpat) < 0)
    {
      rtx m_split, *split;
      rtx ni2dest = i2dest;





      m_split = split_insns (newpat, i3);
# 2136 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
      if (m_split == 0 && ! reg_overlap_mentioned_p (ni2dest, newpat))
 {


   if (((enum machine_mode) ((((newpat)->fld[0]).rtx))->mode) != ((enum machine_mode) (i2dest)->mode)
       && ((enum machine_mode) ((((newpat)->fld[0]).rtx))->mode) != VOIDmode
       && ((enum rtx_code) (i2dest)->code) == REG
       && ((((i2dest)->fld[0]).rtuint) < 53
    || ((((reg_n_info)->data.reg[(((i2dest)->fld[0]).rtuint)])->sets) == 1 && ! added_sets_2
        && ! ((i2dest)->volatil))))
     ni2dest = gen_rtx_REG (((enum machine_mode) ((((newpat)->fld[0]).rtx))->mode),
       (((i2dest)->fld[0]).rtuint));

   m_split = split_insns (gen_rtx_fmt_E (PARALLEL, (VOIDmode), (gen_rtvec (2, newpat, gen_rtx_fmt_e (CLOBBER, (VOIDmode), (ni2dest))))),




     i3);


   if (! m_split && ni2dest != i2dest)
     {
       ni2dest = i2dest;
       m_split = split_insns (gen_rtx_fmt_E (PARALLEL, (VOIDmode), (gen_rtvec (2, newpat, gen_rtx_fmt_e (CLOBBER, (VOIDmode), (i2dest))))),




         i3);
     }
 }



      if (m_split && ((enum rtx_code) (m_split)->code) == SEQUENCE
   && (((((m_split)->fld[0]).rtvec))->num_elem) == 1)
 m_split = ((((((((m_split)->fld[0]).rtvec))->elem[0]))->fld[3]).rtx);

      if (m_split && ((enum rtx_code) (m_split)->code) != SEQUENCE)
 {
   insn_code_number = recog_for_combine (&m_split, i3, &new_i3_notes);
   if (insn_code_number >= 0)
     newpat = m_split;
 }
      else if (m_split && ((enum rtx_code) (m_split)->code) == SEQUENCE
        && (((((m_split)->fld[0]).rtvec))->num_elem) == 2
        && (next_real_insn (i2) == i3
     || ! use_crosses_set_p (((((((((m_split)->fld[0]).rtvec))->elem[0]))->fld[3]).rtx),
        ((((i2)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (i2) : uid_cuid[(((i2)->fld[0]).rtint)]))))
 {
   rtx i2set, i3set;
   rtx newi3pat = ((((((((m_split)->fld[0]).rtvec))->elem[1]))->fld[3]).rtx);
   newi2pat = ((((((((m_split)->fld[0]).rtvec))->elem[0]))->fld[3]).rtx);

   i3set = (((rtx_class[(int) (((enum rtx_code) ((((((m_split)->fld[0]).rtvec))->elem[1]))->code))]) == 'i') ? (((enum rtx_code) (((((((((m_split)->fld[0]).rtvec))->elem[1]))->fld[3]).rtx))->code) == SET ? ((((((((m_split)->fld[0]).rtvec))->elem[1]))->fld[3]).rtx) : single_set_2 ((((((m_split)->fld[0]).rtvec))->elem[1]), ((((((((m_split)->fld[0]).rtvec))->elem[1]))->fld[3]).rtx))) : (rtx) 0);
   i2set = (((rtx_class[(int) (((enum rtx_code) ((((((m_split)->fld[0]).rtvec))->elem[0]))->code))]) == 'i') ? (((enum rtx_code) (((((((((m_split)->fld[0]).rtvec))->elem[0]))->fld[3]).rtx))->code) == SET ? ((((((((m_split)->fld[0]).rtvec))->elem[0]))->fld[3]).rtx) : single_set_2 ((((((m_split)->fld[0]).rtvec))->elem[0]), ((((((((m_split)->fld[0]).rtvec))->elem[0]))->fld[3]).rtx))) : (rtx) 0);





   if ((((i2dest)->fld[0]).rtuint) >= 53)
     do_SUBST(&((cfun->emit->x_regno_reg_rtx)[(((i2dest)->fld[0]).rtuint)]), (ni2dest));

   i2_code_number = recog_for_combine (&newi2pat, i2, &new_i2_notes);





   if (i2_code_number >= 0 && i2set && i3set
       && (next_real_insn (i2) == i3
    || ! reg_used_between_p ((((i2set)->fld[0]).rtx), i2, i3)))
     insn_code_number = recog_for_combine (&newi3pat, i3,
        &new_i3_notes);
   if (insn_code_number >= 0)
     newpat = newi3pat;




   if (insn_code_number >= 0)
     {
       rtx new_i3_dest = (((i3set)->fld[0]).rtx);
       rtx new_i2_dest = (((i2set)->fld[0]).rtx);

       while (((enum rtx_code) (new_i3_dest)->code) == ZERO_EXTRACT
       || ((enum rtx_code) (new_i3_dest)->code) == STRICT_LOW_PART
       || ((enum rtx_code) (new_i3_dest)->code) == SUBREG)
  new_i3_dest = (((new_i3_dest)->fld[0]).rtx);

       while (((enum rtx_code) (new_i2_dest)->code) == ZERO_EXTRACT
       || ((enum rtx_code) (new_i2_dest)->code) == STRICT_LOW_PART
       || ((enum rtx_code) (new_i2_dest)->code) == SUBREG)
  new_i2_dest = (((new_i2_dest)->fld[0]).rtx);

       if (((enum rtx_code) (new_i3_dest)->code) == REG
    && ((enum rtx_code) (new_i2_dest)->code) == REG
    && (((new_i3_dest)->fld[0]).rtuint) == (((new_i2_dest)->fld[0]).rtuint))
  (((reg_n_info)->data.reg[(((new_i2_dest)->fld[0]).rtuint)])->sets)++;
     }
 }




      if (insn_code_number < 0 && (split = find_split_point (&newpat, i3)) != 0





   && (((enum machine_mode) (*split)->mode) == ((enum machine_mode) (i2dest)->mode)
       || ((enum machine_mode) (*split)->mode) == VOIDmode
       || (((i2dest)->fld[0]).rtuint) < 53
       || ((((reg_n_info)->data.reg[(((i2dest)->fld[0]).rtuint)])->sets) == 1 && ! added_sets_2
    && ! ((i2dest)->volatil)))
   && (next_real_insn (i2) == i3
       || ! use_crosses_set_p (*split, ((((i2)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (i2) : uid_cuid[(((i2)->fld[0]).rtint)])))


   && ! reg_referenced_p (i2dest, newpat))
 {
   rtx newdest = i2dest;
   enum rtx_code split_code = ((enum rtx_code) (*split)->code);
   enum machine_mode split_mode = ((enum machine_mode) (*split)->mode);



   if (((enum machine_mode) (i2dest)->mode) != split_mode && split_mode != VOIDmode)
     {
       newdest = gen_rtx_REG (split_mode, (((i2dest)->fld[0]).rtuint));

       if ((((i2dest)->fld[0]).rtuint) >= 53)
  do_SUBST(&((cfun->emit->x_regno_reg_rtx)[(((i2dest)->fld[0]).rtuint)]), (newdest));
     }




   if (split_code == MULT
       && ((enum rtx_code) ((((*split)->fld[1]).rtx))->code) == CONST_INT
       && ((((((*split)->fld[1]).rtx))->fld[0]).rtwint) > 0
       && (i = exact_log2_wide ((unsigned long long) (((((((*split)->fld[1]).rtx))->fld[0]).rtwint)))) >= 0)
     {
       do_SUBST(&(*split), (gen_rtx_fmt_ee (ASHIFT, (split_mode), ((((*split)->fld[0]).rtx)), (gen_rtx_CONST_INT (VOIDmode, (long long) (i))))));



       split_code = ((enum rtx_code) (*split)->code);
     }




   if (split_code == SUBREG && ((enum rtx_code) ((((*split)->fld[0]).rtx))->code) == MEM)
     do_SUBST(&(*split), (gen_rtx_fmt_e (ZERO_EXTEND, (split_mode), ((((*split)->fld[0]).rtx)))));



   newi2pat = gen_rtx_fmt_ee (SET, (VOIDmode), (newdest), (*split));
   do_SUBST(&(*split), (newdest));
   i2_code_number = recog_for_combine (&newi2pat, i2, &new_i2_notes);



   if (i2_code_number >= 0 && ! (split_code == MULT && ! have_mult))
     insn_code_number = recog_for_combine (&newpat, i3, &new_i3_notes);
 }
    }
# 2319 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  else if (i1 && insn_code_number < 0 && asm_noperands (newpat) < 0
    && ((enum rtx_code) (newpat)->code) == PARALLEL
    && (((((newpat)->fld[0]).rtvec))->num_elem) == 2
    && ((enum rtx_code) ((((((newpat)->fld[0]).rtvec))->elem[0]))->code) == SET
    && ((enum rtx_code) (((((((((newpat)->fld[0]).rtvec))->elem[0]))->fld[1]).rtx))->code) == SIGN_EXTEND
    && ((enum rtx_code) ((((((newpat)->fld[0]).rtvec))->elem[1]))->code) == SET
    && rtx_equal_p (((((((((newpat)->fld[0]).rtvec))->elem[1]))->fld[1]).rtx),
      (((((((((((newpat)->fld[0]).rtvec))->elem[0]))->fld[1]).rtx))->fld[0]).rtx))
    && ! use_crosses_set_p (((((((((newpat)->fld[0]).rtvec))->elem[1]))->fld[1]).rtx),
       ((((i2)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (i2) : uid_cuid[(((i2)->fld[0]).rtint)]))
    && ((enum rtx_code) (((((((((newpat)->fld[0]).rtvec))->elem[1]))->fld[0]).rtx))->code) != ZERO_EXTRACT
    && ((enum rtx_code) (((((((((newpat)->fld[0]).rtvec))->elem[1]))->fld[0]).rtx))->code) != STRICT_LOW_PART
    && ! (temp = ((((((((newpat)->fld[0]).rtvec))->elem[1]))->fld[0]).rtx),
   (((enum rtx_code) (temp)->code) == REG
    && reg_nonzero_bits[(((temp)->fld[0]).rtuint)] != 0
    && (mode_bitsize[(int) (((enum machine_mode) (temp)->mode))]) < ((target_flags & 0x02000000) ? 64 : 32)
    && (mode_bitsize[(int) (((enum machine_mode) (temp)->mode))]) < (8 * 4)
    && (reg_nonzero_bits[(((temp)->fld[0]).rtuint)]
        != mode_mask_array[(int) (word_mode)])))
    && ! (((enum rtx_code) (((((((((newpat)->fld[0]).rtvec))->elem[1]))->fld[0]).rtx))->code) == SUBREG
   && (temp = (((((((((((newpat)->fld[0]).rtvec))->elem[1]))->fld[0]).rtx))->fld[0]).rtx),
       (((enum rtx_code) (temp)->code) == REG
        && reg_nonzero_bits[(((temp)->fld[0]).rtuint)] != 0
        && (mode_bitsize[(int) (((enum machine_mode) (temp)->mode))]) < ((target_flags & 0x02000000) ? 64 : 32)
        && (mode_bitsize[(int) (((enum machine_mode) (temp)->mode))]) < (8 * 4)
        && (reg_nonzero_bits[(((temp)->fld[0]).rtuint)]
     != mode_mask_array[(int) (word_mode)]))))
    && ! reg_overlap_mentioned_p (((((((((newpat)->fld[0]).rtvec))->elem[1]))->fld[0]).rtx),
      ((((((((newpat)->fld[0]).rtvec))->elem[1]))->fld[1]).rtx))
    && ! find_reg_note (i3, REG_UNUSED,
          ((((((((newpat)->fld[0]).rtvec))->elem[0]))->fld[0]).rtx)))
    {
      rtx ni2dest;

      newi2pat = (((((newpat)->fld[0]).rtvec))->elem[0]);
      ni2dest = ((((((((newpat)->fld[0]).rtvec))->elem[0]))->fld[0]).rtx);
      newpat = (((((newpat)->fld[0]).rtvec))->elem[1]);
      do_SUBST(&((((newpat)->fld[1]).rtx)), (gen_lowpart_for_combine (((enum machine_mode) ((((newpat)->fld[1]).rtx))->mode), ni2dest)));

      i2_code_number = recog_for_combine (&newi2pat, i2, &new_i2_notes);

      if (i2_code_number >= 0)
 insn_code_number = recog_for_combine (&newpat, i3, &new_i3_notes);

      if (insn_code_number >= 0)
 {
   rtx insn;
   rtx link;
# 2377 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
   (((i3)->fld[3]).rtx) = newpat;
   distribute_links (gen_rtx_fmt_ue (INSN_LIST, (VOIDmode), (i3), ((rtx) 0)));
# 2389 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
   for (insn = (((i3)->fld[2]).rtx);
        insn && (this_basic_block == n_basic_blocks - 1
   || insn != ((((basic_block_info)->data.bb[(this_basic_block + 1)]))->head));
        insn = (((insn)->fld[2]).rtx))
     {
       if (((rtx_class[(int) (((enum rtx_code) (insn)->code))]) == 'i') && reg_referenced_p (ni2dest, (((insn)->fld[3]).rtx)))
  {
    for (link = (((insn)->fld[5]).rtx); link;
         link = (((link)->fld[1]).rtx))
      if ((((link)->fld[0]).rtx) == i3)
        (((link)->fld[0]).rtx) = i1;

    break;
  }
     }
 }
    }






  else if (i1 && insn_code_number < 0 && asm_noperands (newpat) < 0
    && ((enum rtx_code) (newpat)->code) == PARALLEL
    && (((((newpat)->fld[0]).rtvec))->num_elem) == 2
    && ((enum rtx_code) ((((((newpat)->fld[0]).rtvec))->elem[0]))->code) == SET
    && ((enum rtx_code) (((((((((newpat)->fld[0]).rtvec))->elem[0]))->fld[0]).rtx))->code) != ZERO_EXTRACT
    && ((enum rtx_code) (((((((((newpat)->fld[0]).rtvec))->elem[0]))->fld[0]).rtx))->code) != STRICT_LOW_PART
    && ((enum rtx_code) ((((((newpat)->fld[0]).rtvec))->elem[1]))->code) == SET
    && ((enum rtx_code) (((((((((newpat)->fld[0]).rtvec))->elem[1]))->fld[0]).rtx))->code) != ZERO_EXTRACT
    && ((enum rtx_code) (((((((((newpat)->fld[0]).rtvec))->elem[1]))->fld[0]).rtx))->code) != STRICT_LOW_PART
    && ! use_crosses_set_p (((((((((newpat)->fld[0]).rtvec))->elem[1]))->fld[1]).rtx),
       ((((i2)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (i2) : uid_cuid[(((i2)->fld[0]).rtint)]))

    && ((enum rtx_code) (((((((((newpat)->fld[0]).rtvec))->elem[1]))->fld[0]).rtx))->code) != USE
    && ((enum rtx_code) (((((((((newpat)->fld[0]).rtvec))->elem[0]))->fld[0]).rtx))->code) != USE
    && ! reg_referenced_p (((((((((newpat)->fld[0]).rtvec))->elem[1]))->fld[0]).rtx),
      (((((newpat)->fld[0]).rtvec))->elem[0]))
    && ! reg_referenced_p (((((((((newpat)->fld[0]).rtvec))->elem[0]))->fld[0]).rtx),
      (((((newpat)->fld[0]).rtvec))->elem[1]))
    && ! (contains_muldiv (((((((((newpat)->fld[0]).rtvec))->elem[0]))->fld[1]).rtx))
   && contains_muldiv (((((((((newpat)->fld[0]).rtvec))->elem[1]))->fld[1]).rtx))))
    {
# 2444 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
 {
   newi2pat = (((((newpat)->fld[0]).rtvec))->elem[1]);
   newpat = (((((newpat)->fld[0]).rtvec))->elem[0]);
 }

      i2_code_number = recog_for_combine (&newi2pat, i2, &new_i2_notes);

      if (i2_code_number >= 0)
 insn_code_number = recog_for_combine (&newpat, i3, &new_i3_notes);
    }



  if ((insn_code_number < 0

       && (! check_asm_operands (newpat) || added_sets_1 || added_sets_2)))
    {
      undo_all ();
      return 0;
    }


  if (undobuf.other_insn)
    {
      rtx other_pat = (((undobuf.other_insn)->fld[3]).rtx);
      rtx new_other_notes;
      rtx note, next;

      ((newpat_used_regs) = ((HARD_REG_ELT_TYPE) (0)));

      other_code_number = recog_for_combine (&other_pat, undobuf.other_insn,
          &new_other_notes);

      if (other_code_number < 0 && ! check_asm_operands (other_pat))
 {
   undo_all ();
   return 0;
 }

      (((undobuf.other_insn)->fld[3]).rtx) = other_pat;




      for (note = (((undobuf.other_insn)->fld[6]).rtx); note; note = next)
 {
   next = (((note)->fld[1]).rtx);

   if (((enum reg_note) ((enum machine_mode) (note)->mode)) == REG_UNUSED
       && ! reg_set_p ((((note)->fld[0]).rtx), (((undobuf.other_insn)->fld[3]).rtx)))
     {
       if (((enum rtx_code) ((((note)->fld[0]).rtx))->code) == REG)
  (((reg_n_info)->data.reg[((((((note)->fld[0]).rtx))->fld[0]).rtuint)])->deaths)--;

       remove_note (undobuf.other_insn, note);
     }
 }

      for (note = new_other_notes; note; note = (((note)->fld[1]).rtx))
 if (((enum rtx_code) ((((note)->fld[0]).rtx))->code) == REG)
   (((reg_n_info)->data.reg[((((((note)->fld[0]).rtx))->fld[0]).rtuint)])->deaths)++;

      distribute_notes (new_other_notes, undobuf.other_insn,
   undobuf.other_insn, (rtx) 0, (rtx) 0, (rtx) 0);
    }
# 2526 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  {
    rtx i3notes, i2notes, i1notes = 0;
    rtx i3links, i2links, i1links = 0;
    rtx midnotes = 0;
    unsigned int regno;



    rtx elim_i2 = ((newi2pat && reg_set_p (i2dest, newi2pat))
     || i2dest_in_i2src || i2dest_in_i1src
     ? 0 : i2dest);
    rtx elim_i1 = (i1 == 0 || i1dest_in_i1src
     || (newi2pat && reg_set_p (i1dest, newi2pat))
     ? 0 : i1dest);



    i3notes = (((i3)->fld[6]).rtx), i3links = (((i3)->fld[5]).rtx);
    i2notes = (((i2)->fld[6]).rtx), i2links = (((i2)->fld[5]).rtx);
    if (i1)
      i1notes = (((i1)->fld[6]).rtx), i1links = (((i1)->fld[5]).rtx);





    reset_used_flags (i3notes);
    reset_used_flags (i2notes);
    reset_used_flags (i1notes);
    reset_used_flags (newpat);
    reset_used_flags (newi2pat);
    if (undobuf.other_insn)
      reset_used_flags ((((undobuf.other_insn)->fld[3]).rtx));

    i3notes = copy_rtx_if_shared (i3notes);
    i2notes = copy_rtx_if_shared (i2notes);
    i1notes = copy_rtx_if_shared (i1notes);
    newpat = copy_rtx_if_shared (newpat);
    newi2pat = copy_rtx_if_shared (newi2pat);
    if (undobuf.other_insn)
      reset_used_flags ((((undobuf.other_insn)->fld[3]).rtx));

    (((i3)->fld[4]).rtint) = insn_code_number;
    (((i3)->fld[3]).rtx) = newpat;

    if (((enum rtx_code) (i3)->code) == CALL_INSN && (((i3)->fld[7]).rtx))
      {
 rtx call_usage = (((i3)->fld[7]).rtx);

 reset_used_flags (call_usage);
 call_usage = copy_rtx (call_usage);

 if (substed_i2)
   replace_rtx (call_usage, i2dest, i2src);

 if (substed_i1)
   replace_rtx (call_usage, i1dest, i1src);

 (((i3)->fld[7]).rtx) = call_usage;
      }

    if (undobuf.other_insn)
      (((undobuf.other_insn)->fld[4]).rtint) = other_code_number;
# 2599 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
    if (i3_subst_into_i2)
      {
 for (i = 0; i < ((((((((i2)->fld[3]).rtx))->fld[0]).rtvec))->num_elem); i++)
   if (((enum rtx_code) (((((((((i2)->fld[3]).rtx))->fld[0]).rtvec))->elem[i]))->code) != USE
       && ((enum rtx_code) ((((((((((((i2)->fld[3]).rtx))->fld[0]).rtvec))->elem[i]))->fld[0]).rtx))->code) == REG
       && (((((((((((i2)->fld[3]).rtx))->fld[0]).rtvec))->elem[i]))->fld[0]).rtx) != i2dest
       && ! find_reg_note (i2, REG_UNUSED,
      (((((((((((i2)->fld[3]).rtx))->fld[0]).rtvec))->elem[i]))->fld[0]).rtx)))
     for (temp = (((i2)->fld[2]).rtx);
   temp && (this_basic_block == n_basic_blocks - 1
     || ((((basic_block_info)->data.bb[(this_basic_block)]))->head) != temp);
   temp = (((temp)->fld[2]).rtx))
       if (temp != i3 && ((rtx_class[(int) (((enum rtx_code) (temp)->code))]) == 'i'))
  for (link = (((temp)->fld[5]).rtx); link; link = (((link)->fld[1]).rtx))
    if ((((link)->fld[0]).rtx) == i2)
      (((link)->fld[0]).rtx) = i3;

 if (i3notes)
   {
     rtx link = i3notes;
     while ((((link)->fld[1]).rtx))
       link = (((link)->fld[1]).rtx);
     (((link)->fld[1]).rtx) = i2notes;
   }
 else
   i3notes = i2notes;
 i2notes = 0;
      }

    (((i3)->fld[5]).rtx) = 0;
    (((i3)->fld[6]).rtx) = 0;
    (((i2)->fld[5]).rtx) = 0;
    (((i2)->fld[6]).rtx) = 0;

    if (newi2pat)
      {
 (((i2)->fld[4]).rtint) = i2_code_number;
 (((i2)->fld[3]).rtx) = newi2pat;
      }
    else
      {
 ((i2)->code = (enum rtx_code) (NOTE));
 (((i2)->fld[4]).rtint) = NOTE_INSN_DELETED;
 (((i2)->fld[3]).rtstr) = 0;
      }

    if (i1)
      {
 (((i1)->fld[5]).rtx) = 0;
 (((i1)->fld[6]).rtx) = 0;
 ((i1)->code = (enum rtx_code) (NOTE));
 (((i1)->fld[4]).rtint) = NOTE_INSN_DELETED;
 (((i1)->fld[3]).rtstr) = 0;
      }






    if (newi2pat)
      {
 move_deaths (newi2pat, (rtx) 0, ((((i1)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (i1) : uid_cuid[(((i1)->fld[0]).rtint)]), i2, &midnotes);
 move_deaths (newpat, newi2pat, ((((i1)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (i1) : uid_cuid[(((i1)->fld[0]).rtint)]), i3, &midnotes);
      }
    else
      move_deaths (newpat, (rtx) 0, i1 ? ((((i1)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (i1) : uid_cuid[(((i1)->fld[0]).rtint)]) : ((((i2)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (i2) : uid_cuid[(((i2)->fld[0]).rtint)]),
     i3, &midnotes);


    if (i3notes)
      distribute_notes (i3notes, i3, i3, newi2pat ? i2 : (rtx) 0,
   elim_i2, elim_i1);
    if (i2notes)
      distribute_notes (i2notes, i2, i3, newi2pat ? i2 : (rtx) 0,
   elim_i2, elim_i1);
    if (i1notes)
      distribute_notes (i1notes, i1, i3, newi2pat ? i2 : (rtx) 0,
   elim_i2, elim_i1);
    if (midnotes)
      distribute_notes (midnotes, (rtx) 0, i3, newi2pat ? i2 : (rtx) 0,
   elim_i2, elim_i1);






    if (newi2pat && new_i2_notes)
      {
 for (temp = new_i2_notes; temp; temp = (((temp)->fld[1]).rtx))
   if (((enum rtx_code) ((((temp)->fld[0]).rtx))->code) == REG)
     (((reg_n_info)->data.reg[((((((temp)->fld[0]).rtx))->fld[0]).rtuint)])->deaths)++;

 distribute_notes (new_i2_notes, i2, i2, (rtx) 0, (rtx) 0, (rtx) 0);
      }

    if (new_i3_notes)
      {
 for (temp = new_i3_notes; temp; temp = (((temp)->fld[1]).rtx))
   if (((enum rtx_code) ((((temp)->fld[0]).rtx))->code) == REG)
     (((reg_n_info)->data.reg[((((((temp)->fld[0]).rtx))->fld[0]).rtuint)])->deaths)++;

 distribute_notes (new_i3_notes, i3, i3, (rtx) 0, (rtx) 0, (rtx) 0);
      }
# 2712 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
    if (i3dest_killed)
      {
 if (((enum rtx_code) (i3dest_killed)->code) == REG)
   (((reg_n_info)->data.reg[(((i3dest_killed)->fld[0]).rtuint)])->deaths)++;

 if (newi2pat && reg_set_p (i3dest_killed, newi2pat))
   distribute_notes (gen_rtx_fmt_ee (EXPR_LIST, (REG_DEAD), (i3dest_killed), ((rtx) 0)),

       (rtx) 0, i2, (rtx) 0, elim_i2, elim_i1);
 else
   distribute_notes (gen_rtx_fmt_ee (EXPR_LIST, (REG_DEAD), (i3dest_killed), ((rtx) 0)),

       (rtx) 0, i3, newi2pat ? i2 : (rtx) 0,
       elim_i2, elim_i1);
      }

    if (i2dest_in_i2src)
      {
 if (((enum rtx_code) (i2dest)->code) == REG)
   (((reg_n_info)->data.reg[(((i2dest)->fld[0]).rtuint)])->deaths)++;

 if (newi2pat && reg_set_p (i2dest, newi2pat))
   distribute_notes (gen_rtx_fmt_ee (EXPR_LIST, (REG_DEAD), (i2dest), ((rtx) 0)),
       (rtx) 0, i2, (rtx) 0, (rtx) 0, (rtx) 0);
 else
   distribute_notes (gen_rtx_fmt_ee (EXPR_LIST, (REG_DEAD), (i2dest), ((rtx) 0)),
       (rtx) 0, i3, newi2pat ? i2 : (rtx) 0,
       (rtx) 0, (rtx) 0);
      }

    if (i1dest_in_i1src)
      {
 if (((enum rtx_code) (i1dest)->code) == REG)
   (((reg_n_info)->data.reg[(((i1dest)->fld[0]).rtuint)])->deaths)++;

 if (newi2pat && reg_set_p (i1dest, newi2pat))
   distribute_notes (gen_rtx_fmt_ee (EXPR_LIST, (REG_DEAD), (i1dest), ((rtx) 0)),
       (rtx) 0, i2, (rtx) 0, (rtx) 0, (rtx) 0);
 else
   distribute_notes (gen_rtx_fmt_ee (EXPR_LIST, (REG_DEAD), (i1dest), ((rtx) 0)),
       (rtx) 0, i3, newi2pat ? i2 : (rtx) 0,
       (rtx) 0, (rtx) 0);
      }

    distribute_links (i3links);
    distribute_links (i2links);
    distribute_links (i1links);

    if (((enum rtx_code) (i2dest)->code) == REG)
      {
 rtx link;
 rtx i2_insn = 0, i2_val = 0, set;
# 2773 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
 for (link = (((i3)->fld[5]).rtx); link; link = (((link)->fld[1]).rtx))
   if ((set = (((rtx_class[(int) (((enum rtx_code) ((((link)->fld[0]).rtx))->code))]) == 'i') ? (((enum rtx_code) (((((((link)->fld[0]).rtx))->fld[3]).rtx))->code) == SET ? ((((((link)->fld[0]).rtx))->fld[3]).rtx) : single_set_2 ((((link)->fld[0]).rtx), ((((((link)->fld[0]).rtx))->fld[3]).rtx))) : (rtx) 0)) != 0
       && rtx_equal_p (i2dest, (((set)->fld[0]).rtx)))
     i2_insn = (((link)->fld[0]).rtx), i2_val = (((set)->fld[1]).rtx);

 record_value_for_reg (i2dest, i2_insn, i2_val);



 if (! added_sets_2
     && (newi2pat == 0 || ! reg_mentioned_p (i2dest, newi2pat))
     && ! i2dest_in_i2src)
   {
     regno = (((i2dest)->fld[0]).rtuint);
     (((reg_n_info)->data.reg[regno])->sets)--;
   }
      }

    if (i1 && ((enum rtx_code) (i1dest)->code) == REG)
      {
 rtx link;
 rtx i1_insn = 0, i1_val = 0, set;

 for (link = (((i3)->fld[5]).rtx); link; link = (((link)->fld[1]).rtx))
   if ((set = (((rtx_class[(int) (((enum rtx_code) ((((link)->fld[0]).rtx))->code))]) == 'i') ? (((enum rtx_code) (((((((link)->fld[0]).rtx))->fld[3]).rtx))->code) == SET ? ((((((link)->fld[0]).rtx))->fld[3]).rtx) : single_set_2 ((((link)->fld[0]).rtx), ((((((link)->fld[0]).rtx))->fld[3]).rtx))) : (rtx) 0)) != 0
       && rtx_equal_p (i1dest, (((set)->fld[0]).rtx)))
     i1_insn = (((link)->fld[0]).rtx), i1_val = (((set)->fld[1]).rtx);

 record_value_for_reg (i1dest, i1_insn, i1_val);

 regno = (((i1dest)->fld[0]).rtuint);
 if (! added_sets_1 && ! i1dest_in_i1src)
   (((reg_n_info)->data.reg[regno])->sets)--;
      }




    if (newi2pat)
      note_stores (newi2pat, set_nonzero_bits_and_sign_copies, ((void*)0));
    note_stores (newpat, set_nonzero_bits_and_sign_copies, ((void*)0));
# 2822 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
    if (((enum rtx_code) (newpat)->code) == RETURN || any_uncondjump_p (i3))
      {
 *new_direct_jump_p = 1;

 if ((temp = next_nonnote_insn (i3)) == (rtx) 0
     || ((enum rtx_code) (temp)->code) != BARRIER)
   emit_barrier_after (i3);
      }


    if (((enum rtx_code) (newpat)->code) == SET
 && (((newpat)->fld[1]).rtx) == (global_rtl[GR_PC])
 && (((newpat)->fld[0]).rtx) == (global_rtl[GR_PC]))
      *new_direct_jump_p = 1;
  }

  combine_successes++;
  undo_commit ();



  subst_prev_insn = (rtx) 0;

  if (added_links_insn
      && (newi2pat == 0 || ((((added_links_insn)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (added_links_insn) : uid_cuid[(((added_links_insn)->fld[0]).rtint)]) < ((((i2)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (i2) : uid_cuid[(((i2)->fld[0]).rtint)]))
      && ((((added_links_insn)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (added_links_insn) : uid_cuid[(((added_links_insn)->fld[0]).rtint)]) < ((((i3)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (i3) : uid_cuid[(((i3)->fld[0]).rtint)]))
    return added_links_insn;
  else
    return newi2pat ? i2 : i3;
}



static void
undo_all ()
{
  struct undo *undo, *next;

  for (undo = undobuf.undos; undo; undo = next)
    {
      next = undo->next;
      if (undo->is_int)
 *undo->where.i = undo->old_contents.i;
      else
 *undo->where.r = undo->old_contents.r;

      undo->next = undobuf.frees;
      undobuf.frees = undo;
    }

  undobuf.undos = 0;



  subst_prev_insn = (rtx) 0;
}




static void
undo_commit ()
{
  struct undo *undo, *next;

  for (undo = undobuf.undos; undo; undo = next)
    {
      next = undo->next;
      undo->next = undobuf.frees;
      undobuf.frees = undo;
    }
  undobuf.undos = 0;
}
# 2904 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static rtx *
find_split_point (loc, insn)
     rtx *loc;
     rtx insn;
{
  rtx x = *loc;
  enum rtx_code code = ((enum rtx_code) (x)->code);
  rtx *split;
  unsigned long long len = 0;
  long long pos = 0;
  int unsignedp = 0;
  rtx inner = (rtx) 0;


  switch (code)
    {
    case SUBREG:



      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == MEM)
 return loc;

      return find_split_point (&(((x)->fld[0]).rtx), insn);

    case MEM:
# 2949 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == PLUS
   && ((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[1]).rtx))->code) == CONST_INT
   && ! memory_address_p (((enum machine_mode) (x)->mode), (((x)->fld[0]).rtx)))
 {
   rtx reg = (cfun->emit->x_regno_reg_rtx)[53];
   rtx seq = split_insns (gen_rtx_fmt_ee (SET, (VOIDmode), (reg), ((((x)->fld[0]).rtx))),
     subst_insn);






   if (seq && (((((seq)->fld[0]).rtvec))->num_elem) == 2
       && ((enum rtx_code) ((((((seq)->fld[0]).rtvec))->elem[0]))->code) == INSN
       && ((enum rtx_code) (((((((((seq)->fld[0]).rtvec))->elem[0]))->fld[3]).rtx))->code) == SET
       && (((((((((((seq)->fld[0]).rtvec))->elem[0]))->fld[3]).rtx))->fld[0]).rtx) == reg
       && ! reg_mentioned_p (reg,
        (((((((((((seq)->fld[0]).rtvec))->elem[0]))->fld[3]).rtx))->fld[1]).rtx))
       && ((enum rtx_code) ((((((seq)->fld[0]).rtvec))->elem[1]))->code) == INSN
       && ((enum rtx_code) (((((((((seq)->fld[0]).rtvec))->elem[1]))->fld[3]).rtx))->code) == SET
       && (((((((((((seq)->fld[0]).rtvec))->elem[1]))->fld[3]).rtx))->fld[0]).rtx) == reg
       && memory_address_p (((enum machine_mode) (x)->mode),
       (((((((((((seq)->fld[0]).rtvec))->elem[1]))->fld[3]).rtx))->fld[1]).rtx)))
     {
       rtx src1 = (((((((((((seq)->fld[0]).rtvec))->elem[0]))->fld[3]).rtx))->fld[1]).rtx);
       rtx src2 = (((((((((((seq)->fld[0]).rtvec))->elem[1]))->fld[3]).rtx))->fld[1]).rtx);






       src2 = replace_rtx (src2, reg, src1);
       split = 0;
       if ((((src2)->fld[0]).rtx) == src1)
  split = &(((src2)->fld[0]).rtx);
       else if ((rtx_format[(int) (((enum rtx_code) ((((src2)->fld[0]).rtx))->code))])[0] == 'e'
         && ((((((src2)->fld[0]).rtx))->fld[0]).rtx) == src1)
  split = &((((((src2)->fld[0]).rtx))->fld[0]).rtx);

       if (split)
  {
    do_SUBST(&((((x)->fld[0]).rtx)), (src2));
    return split;
  }
     }






   else if ((rtx_class[(int) (((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->code))]) != 'o'
     && ! (((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->code) == SUBREG
    && ((rtx_class[(int) (((enum rtx_code) ((((((((((x)->fld[0]).rtx))->fld[0]).rtx))->fld[0]).rtx))->code))])
        == 'o')))
     return &((((((x)->fld[0]).rtx))->fld[0]).rtx);
 }
      break;

    case SET:
# 3027 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
      split = find_split_point (&(((x)->fld[1]).rtx), insn);
      if (split && split != &(((x)->fld[1]).rtx))
 return split;


      split = find_split_point (&(((x)->fld[0]).rtx), insn);
      if (split && split != &(((x)->fld[0]).rtx))
 return split;



      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == ZERO_EXTRACT
   && ((mode_bitsize[(int) (((enum machine_mode) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->mode))])
       <= (8 * 8))
   && ((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[1]).rtx))->code) == CONST_INT
   && ((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[2]).rtx))->code) == CONST_INT
   && ((enum rtx_code) ((((x)->fld[1]).rtx))->code) == CONST_INT
   && (((((((((((x)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint)
        + (((((((((x)->fld[0]).rtx))->fld[2]).rtx))->fld[0]).rtwint))
       <= (mode_bitsize[(int) (((enum machine_mode) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->mode))]))
   && ! side_effects_p (((((((x)->fld[0]).rtx))->fld[0]).rtx)))
 {
   long long pos = (((((((((x)->fld[0]).rtx))->fld[2]).rtx))->fld[0]).rtwint);
   unsigned long long len = (((((((((x)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint);
   unsigned long long src = ((((((x)->fld[1]).rtx))->fld[0]).rtwint);
   rtx dest = ((((((x)->fld[0]).rtx))->fld[0]).rtx);
   enum machine_mode mode = ((enum machine_mode) (dest)->mode);
   unsigned long long mask = ((long long) 1 << len) - 1;

   if (0)
     pos = (mode_bitsize[(int) (mode)]) - len - pos;

   if (src == mask)
     do_SUBST(&((((x)->fld[1]).rtx)), (gen_binary (IOR, mode, dest, gen_rtx_CONST_INT (VOIDmode, (long long) (src << pos)))));

   else
     do_SUBST(&((((x)->fld[1]).rtx)), (gen_binary (IOR, mode, gen_binary (AND, mode, dest, gen_rtx_CONST_INT (VOIDmode, (long long) (trunc_int_for_mode (~(mask << pos) & mode_mask_array[(int) (mode)], mode)))), gen_rtx_CONST_INT (VOIDmode, (long long) (src << pos)))));
# 3073 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
   do_SUBST(&((((x)->fld[0]).rtx)), (dest));

   split = find_split_point (&(((x)->fld[1]).rtx), insn);
   if (split && split != &(((x)->fld[1]).rtx))
     return split;
 }



      code = ((enum rtx_code) ((((x)->fld[1]).rtx))->code);

      switch (code)
 {
 case AND:
# 3096 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
   if (((enum rtx_code) (((((((x)->fld[1]).rtx))->fld[1]).rtx))->code) == CONST_INT
       && ((enum rtx_code) (((((((x)->fld[1]).rtx))->fld[0]).rtx))->code) == REG
       && (pos = exact_log2_wide ((unsigned long long) ((((((((((x)->fld[1]).rtx))->fld[1]).rtx))->fld[0]).rtwint)))) >= 7
       && ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == REG
       && (split = find_single_use ((((x)->fld[0]).rtx), insn, (rtx*) 0)) != 0
       && (((enum rtx_code) (*split)->code) == EQ || ((enum rtx_code) (*split)->code) == NE)
       && (((*split)->fld[0]).rtx) == (((x)->fld[0]).rtx)
       && (((*split)->fld[1]).rtx) == (const_int_rtx[64]))
     {
       rtx extraction = make_extraction (((enum machine_mode) ((((x)->fld[0]).rtx))->mode),
      ((((((x)->fld[1]).rtx))->fld[0]).rtx),
      pos, (rtx) 0, 1, 1, 0, 0);
       if (extraction != 0)
  {
    do_SUBST(&((((x)->fld[1]).rtx)), (extraction));
    return find_split_point (loc, insn);
  }
     }
   break;

 case NE:


   if (1 == -1 && ((((((x)->fld[1]).rtx))->fld[1]).rtx) == (const_int_rtx[64])
       && ((enum machine_mode) ((((x)->fld[1]).rtx))->mode) == ((enum machine_mode) (((((((x)->fld[1]).rtx))->fld[0]).rtx))->mode)
       && 1 <= (pos = exact_log2_wide ((unsigned long long) (nonzero_bits (((((((x)->fld[1]).rtx))->fld[0]).rtx), ((enum machine_mode) (((((((x)->fld[1]).rtx))->fld[0]).rtx))->mode))))))


     {
       enum machine_mode mode = ((enum machine_mode) (((((((x)->fld[1]).rtx))->fld[0]).rtx))->mode);

       do_SUBST(&((((x)->fld[1]).rtx)), (gen_rtx_fmt_e (NEG, (mode), (gen_rtx_fmt_ee (LSHIFTRT, (mode), (((((((x)->fld[1]).rtx))->fld[0]).rtx)), (gen_rtx_CONST_INT (VOIDmode, (long long) (pos))))))));





       split = find_split_point (&(((x)->fld[1]).rtx), insn);
       if (split && split != &(((x)->fld[1]).rtx))
  return split;
     }
   break;

 case SIGN_EXTEND:
   inner = ((((((x)->fld[1]).rtx))->fld[0]).rtx);




   if ((mode_class[(int) (((enum machine_mode) (inner)->mode))]) == MODE_PARTIAL_INT
       || (mode_class[(int) (((enum machine_mode) ((((x)->fld[1]).rtx))->mode))]) == MODE_PARTIAL_INT)
     break;

   pos = 0;
   len = (mode_bitsize[(int) (((enum machine_mode) (inner)->mode))]);
   unsignedp = 0;
   break;

 case SIGN_EXTRACT:
 case ZERO_EXTRACT:
   if (((enum rtx_code) (((((((x)->fld[1]).rtx))->fld[1]).rtx))->code) == CONST_INT
       && ((enum rtx_code) (((((((x)->fld[1]).rtx))->fld[2]).rtx))->code) == CONST_INT)
     {
       inner = ((((((x)->fld[1]).rtx))->fld[0]).rtx);
       len = (((((((((x)->fld[1]).rtx))->fld[1]).rtx))->fld[0]).rtwint);
       pos = (((((((((x)->fld[1]).rtx))->fld[2]).rtx))->fld[0]).rtwint);

       if (0)
  pos = (mode_bitsize[(int) (((enum machine_mode) (inner)->mode))]) - len - pos;
       unsignedp = (code == ZERO_EXTRACT);
     }
   break;

 default:
   break;
 }

      if (len && pos >= 0 && pos + len <= (mode_bitsize[(int) (((enum machine_mode) (inner)->mode))]))
 {
   enum machine_mode mode = ((enum machine_mode) ((((x)->fld[1]).rtx))->mode);







   if (unsignedp && len <= 8)
     {
       do_SUBST(&((((x)->fld[1]).rtx)), (gen_rtx_fmt_ee (AND, (mode), (gen_rtx_fmt_ee (LSHIFTRT, (mode), (gen_lowpart_for_combine (mode, inner)), (gen_rtx_CONST_INT (VOIDmode, (long long) (pos))))), (gen_rtx_CONST_INT (VOIDmode, (long long) (((long long) 1 << len) - 1))))));






       split = find_split_point (&(((x)->fld[1]).rtx), insn);
       if (split && split != &(((x)->fld[1]).rtx))
  return split;
     }
   else
     {
       do_SUBST(&((((x)->fld[1]).rtx)), (gen_rtx_fmt_ee (unsignedp ? LSHIFTRT : ASHIFTRT, mode, gen_rtx_fmt_ee (ASHIFT, (mode), (gen_lowpart_for_combine (mode, inner)), (gen_rtx_CONST_INT (VOIDmode, (long long) ((mode_bitsize[(int) (mode)]) - len - pos)))), gen_rtx_CONST_INT (VOIDmode, (long long) ((mode_bitsize[(int) (mode)]) - len)))));
# 3207 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
       split = find_split_point (&(((x)->fld[1]).rtx), insn);
       if (split && split != &(((x)->fld[1]).rtx))
  return split;
     }
 }




      if (((rtx_class[(int) (((enum rtx_code) ((((x)->fld[1]).rtx))->code))]) == '2'
    || (rtx_class[(int) (((enum rtx_code) ((((x)->fld[1]).rtx))->code))]) == 'c'
    || (rtx_class[(int) (((enum rtx_code) ((((x)->fld[1]).rtx))->code))]) == '<')
   && (((enum rtx_code) (((((((x)->fld[1]).rtx))->fld[1]).rtx))->code) == LABEL_REF || ((enum rtx_code) (((((((x)->fld[1]).rtx))->fld[1]).rtx))->code) == SYMBOL_REF || ((enum rtx_code) (((((((x)->fld[1]).rtx))->fld[1]).rtx))->code) == CONST_INT || ((enum rtx_code) (((((((x)->fld[1]).rtx))->fld[1]).rtx))->code) == CONST_DOUBLE || ((enum rtx_code) (((((((x)->fld[1]).rtx))->fld[1]).rtx))->code) == CONST || ((enum rtx_code) (((((((x)->fld[1]).rtx))->fld[1]).rtx))->code) == HIGH || ((enum rtx_code) (((((((x)->fld[1]).rtx))->fld[1]).rtx))->code) == CONST_VECTOR || ((enum rtx_code) (((((((x)->fld[1]).rtx))->fld[1]).rtx))->code) == CONSTANT_P_RTX)
   && ((rtx_class[(int) (((enum rtx_code) (((((((x)->fld[1]).rtx))->fld[0]).rtx))->code))]) == 'o'
       || (((enum rtx_code) (((((((x)->fld[1]).rtx))->fld[0]).rtx))->code) == SUBREG
    && ((rtx_class[(int) (((enum rtx_code) ((((((((((x)->fld[1]).rtx))->fld[0]).rtx))->fld[0]).rtx))->code))])
        == 'o'))))
 return &((((((x)->fld[1]).rtx))->fld[1]).rtx);






      if (((rtx_class[(int) (((enum rtx_code) ((((x)->fld[1]).rtx))->code))]) == '2'
    || (rtx_class[(int) (((enum rtx_code) ((((x)->fld[1]).rtx))->code))]) == 'c'
    || (rtx_class[(int) (((enum rtx_code) ((((x)->fld[1]).rtx))->code))]) == '<'
    || (rtx_class[(int) (((enum rtx_code) ((((x)->fld[1]).rtx))->code))]) == '1')
   && ! register_operand (((((((x)->fld[1]).rtx))->fld[0]).rtx), VOIDmode))
 return &((((((x)->fld[1]).rtx))->fld[0]).rtx);

      return 0;

    case AND:
    case IOR:



      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == NOT && ((enum rtx_code) ((((x)->fld[1]).rtx))->code) == NOT)
 {
   do_SUBST(&(*loc), (gen_rtx_fmt_e (NOT, (((enum machine_mode) (x)->mode)), (gen_rtx_fmt_ee (code == IOR ? AND : IOR, ((enum machine_mode) (x)->mode), ((((((x)->fld[0]).rtx))->fld[0]).rtx), ((((((x)->fld[1]).rtx))->fld[0]).rtx))))));





   return find_split_point (loc, insn);
 }




      if (((enum rtx_code) ((((x)->fld[1]).rtx))->code) == NOT)
 {
   rtx tem = (((x)->fld[0]).rtx);
   do_SUBST(&((((x)->fld[0]).rtx)), ((((x)->fld[1]).rtx)));
   do_SUBST(&((((x)->fld[1]).rtx)), (tem));
 }
      break;

    default:
      break;
    }


  switch ((rtx_class[(int) (code)]))
    {
    case 'b':
    case '3':
      split = find_split_point (&(((x)->fld[2]).rtx), insn);
      if (split)
 return split;

    case '2':
    case 'c':
    case '<':
      split = find_split_point (&(((x)->fld[1]).rtx), insn);
      if (split)
 return split;

    case '1':


      if (((enum rtx_code) (x)->code) != AND && ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == AND)
 return &(((x)->fld[0]).rtx);

      split = find_split_point (&(((x)->fld[0]).rtx), insn);
      if (split)
 return split;
      return loc;
    }


  return 0;
}
# 3322 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static rtx
subst (x, from, to, in_dest, unique_copy)
     rtx x, from, to;
     int in_dest;
     int unique_copy;
{
  enum rtx_code code = ((enum rtx_code) (x)->code);
  enum machine_mode op0_mode = VOIDmode;
  const char *fmt;
  int len, i;
  rtx new;
# 3343 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  if (! in_dest && ((x) == (from) || (((enum rtx_code) (x)->code) == REG && ((enum rtx_code) (from)->code) == REG && (((x)->fld[0]).rtuint) == (((from)->fld[0]).rtuint) && ((enum machine_mode) (x)->mode) == ((enum machine_mode) (from)->mode))))
    {
      n_occurrences++;
      return (unique_copy && n_occurrences > 1 ? copy_rtx (to) : to);
    }
# 3356 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  if (! in_dest && code == REG && ((enum rtx_code) (from)->code) == REG
      && (((x)->fld[0]).rtuint) == (((from)->fld[0]).rtuint))
    return gen_rtx_fmt_e (CLOBBER, (((enum machine_mode) (x)->mode)), ((const_int_rtx[64])));



  if (code != MEM && code != LO_SUM && (rtx_class[(int) (code)]) == 'o')
    return x;







  if (((x) == (to) || (((enum rtx_code) (x)->code) == REG && ((enum rtx_code) (to)->code) == REG && (((x)->fld[0]).rtuint) == (((to)->fld[0]).rtuint) && ((enum machine_mode) (x)->mode) == ((enum machine_mode) (to)->mode))))
    return to;
# 3383 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  if (code == PARALLEL
      && ((enum rtx_code) ((((((x)->fld[0]).rtvec))->elem[0]))->code) == SET
      && ((enum rtx_code) (((((((((x)->fld[0]).rtvec))->elem[0]))->fld[1]).rtx))->code) == ASM_OPERANDS)
    {
      new = subst ((((((x)->fld[0]).rtvec))->elem[0]), from, to, 0, unique_copy);


      if (((enum rtx_code) (new)->code) == CLOBBER
   && (((new)->fld[0]).rtx) == (const_int_rtx[64]))
 return new;

      do_SUBST(&((((((x)->fld[0]).rtvec))->elem[0])), (new));

      for (i = (((((x)->fld[0]).rtvec))->num_elem) - 1; i >= 1; i--)
 {
   rtx dest = ((((((((x)->fld[0]).rtvec))->elem[i]))->fld[0]).rtx);

   if (((enum rtx_code) (dest)->code) != REG
       && ((enum rtx_code) (dest)->code) != CC0
       && ((enum rtx_code) (dest)->code) != PC)
     {
       new = subst (dest, from, to, 0, unique_copy);


       if (((enum rtx_code) (new)->code) == CLOBBER
    && (((new)->fld[0]).rtx) == (const_int_rtx[64]))
  return new;

       do_SUBST(&(((((((((x)->fld[0]).rtvec))->elem[i]))->fld[0]).rtx)), (new));
     }
 }
    }
  else
    {
      len = (rtx_length[(int) (code)]);
      fmt = (rtx_format[(int) (code)]);





      if (code == SET
   && (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == REG
       || ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == CC0
       || ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == PC))
 fmt = "ie";



      if (fmt[0] == 'e')
 op0_mode = ((enum machine_mode) ((((x)->fld[0]).rtx))->mode);

      for (i = 0; i < len; i++)
 {
   if (fmt[i] == 'E')
     {
       int j;
       for (j = (((((x)->fld[i]).rtvec))->num_elem) - 1; j >= 0; j--)
  {
    if ((((((((x)->fld[i]).rtvec))->elem[j])) == (from) || (((enum rtx_code) ((((((x)->fld[i]).rtvec))->elem[j]))->code) == REG && ((enum rtx_code) (from)->code) == REG && ((((((((x)->fld[i]).rtvec))->elem[j]))->fld[0]).rtuint) == (((from)->fld[0]).rtuint) && ((enum machine_mode) ((((((x)->fld[i]).rtvec))->elem[j]))->mode) == ((enum machine_mode) (from)->mode))))
      {
        new = (unique_copy && n_occurrences
        ? copy_rtx (to) : to);
        n_occurrences++;
      }
    else
      {
        new = subst ((((((x)->fld[i]).rtvec))->elem[j]), from, to, 0,
       unique_copy);



        if (((enum rtx_code) (new)->code) == CLOBBER
     && (((new)->fld[0]).rtx) == (const_int_rtx[64]))
   return new;
      }

    do_SUBST(&((((((x)->fld[i]).rtvec))->elem[j])), (new));
  }
     }
   else if (fmt[i] == 'e')
     {

       new = (((x)->fld[i]).rtx);
       if (in_dest
    && (code == SUBREG || code == STRICT_LOW_PART
        || code == ZERO_EXTRACT)
    && i == 0
    && ((enum rtx_code) (new)->code) == REG)
  ;

       else if ((((((x)->fld[i]).rtx)) == (from) || (((enum rtx_code) ((((x)->fld[i]).rtx))->code) == REG && ((enum rtx_code) (from)->code) == REG && ((((((x)->fld[i]).rtx))->fld[0]).rtuint) == (((from)->fld[0]).rtuint) && ((enum machine_mode) ((((x)->fld[i]).rtx))->mode) == ((enum machine_mode) (from)->mode))))
  {
# 3489 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
    if (((enum rtx_code) (to)->code) == SUBREG
        && ! ((((enum machine_mode) (to)->mode)) == (((enum machine_mode) ((((to)->fld[0]).rtx))->mode)) || (((((enum machine_mode) (to)->mode)) == HImode || (((enum machine_mode) (to)->mode)) == SImode || ((((enum machine_mode) (to)->mode)) == QImode && ((target_flags & 0x02000000) || !(x86_partial_reg_stall & (1 << ix86_cpu)))) || ((((enum machine_mode) (to)->mode)) == DImode && (target_flags & 0x02000000))) && ((((enum machine_mode) ((((to)->fld[0]).rtx))->mode)) == HImode || (((enum machine_mode) ((((to)->fld[0]).rtx))->mode)) == SImode || ((((enum machine_mode) (to)->mode)) == QImode && ((target_flags & 0x02000000) || !(x86_partial_reg_stall & (1 << ix86_cpu)))) || ((((enum machine_mode) ((((to)->fld[0]).rtx))->mode)) == DImode && (target_flags & 0x02000000)))))

        && ! (code == SUBREG
       && ((((enum machine_mode) (x)->mode)) == (((enum machine_mode) ((((to)->fld[0]).rtx))->mode)) || (((((enum machine_mode) (x)->mode)) == HImode || (((enum machine_mode) (x)->mode)) == SImode || ((((enum machine_mode) (x)->mode)) == QImode && ((target_flags & 0x02000000) || !(x86_partial_reg_stall & (1 << ix86_cpu)))) || ((((enum machine_mode) (x)->mode)) == DImode && (target_flags & 0x02000000))) && ((((enum machine_mode) ((((to)->fld[0]).rtx))->mode)) == HImode || (((enum machine_mode) ((((to)->fld[0]).rtx))->mode)) == SImode || ((((enum machine_mode) (x)->mode)) == QImode && ((target_flags & 0x02000000) || !(x86_partial_reg_stall & (1 << ix86_cpu)))) || ((((enum machine_mode) ((((to)->fld[0]).rtx))->mode)) == DImode && (target_flags & 0x02000000))))))




        )
      return gen_rtx_fmt_e (CLOBBER, (VOIDmode), ((const_int_rtx[64])));
# 3513 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
    new = (unique_copy && n_occurrences ? copy_rtx (to) : to);
    n_occurrences++;
  }
       else
# 3525 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  new = subst ((((x)->fld[i]).rtx), from, to,
        (((in_dest
    && (code == SUBREG || code == STRICT_LOW_PART
        || code == ZERO_EXTRACT))
          || code == SET)
         && i == 0), unique_copy);







       if (((enum rtx_code) (new)->code) == CLOBBER && (((new)->fld[0]).rtx) == (const_int_rtx[64]))
  return new;

       if (((enum rtx_code) (new)->code) == CONST_INT && ((enum rtx_code) (x)->code) == SUBREG)
  {
    enum machine_mode mode = ((enum machine_mode) (x)->mode);
    x = simplify_subreg (mode, new,
           ((enum machine_mode) ((((x)->fld[0]).rtx))->mode),
           (((x)->fld[1]).rtuint));
    if (! x)
      x = gen_rtx_fmt_e (CLOBBER, (mode), ((const_int_rtx[64])));
  }
       else if (((enum rtx_code) (new)->code) == CONST_INT
         && ((enum rtx_code) (x)->code) == ZERO_EXTEND)
  {
    x = simplify_unary_operation (ZERO_EXTEND, ((enum machine_mode) (x)->mode),
      new, ((enum machine_mode) ((((x)->fld[0]).rtx))->mode));
    if (! x)
      fancy_abort ("/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c", 3556, __FUNCTION__);
  }
       else
  do_SUBST(&((((x)->fld[i]).rtx)), (new));
     }
 }
    }





  for (i = 0; i < 4; i++)
    {


      if (code != CONST_INT && code != REG && code != CLOBBER)
 x = combine_simplify_rtx (x, op0_mode, i == 3, in_dest);

      if (((enum rtx_code) (x)->code) == code)
 break;

      code = ((enum rtx_code) (x)->code);



      op0_mode = VOIDmode;
    }

  return x;
}
# 3596 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static rtx
combine_simplify_rtx (x, op0_mode, last, in_dest)
     rtx x;
     enum machine_mode op0_mode;
     int last;
     int in_dest;
{
  enum rtx_code code = ((enum rtx_code) (x)->code);
  enum machine_mode mode = ((enum machine_mode) (x)->mode);
  rtx temp;
  rtx reversed;
  int i;



  if ((rtx_class[(int) (code)]) == 'c'
      && swap_commutative_operands_p ((((x)->fld[0]).rtx), (((x)->fld[1]).rtx)))
    {
      temp = (((x)->fld[0]).rtx);
      do_SUBST(&((((x)->fld[0]).rtx)), ((((x)->fld[1]).rtx)));
      do_SUBST(&((((x)->fld[1]).rtx)), (temp));
    }
# 3637 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  if ((code == PLUS || code == MINUS || code == MULT)
      && ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == ASHIFTRT
      && ((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->code) == PLUS
      && ((enum rtx_code) ((((((((((x)->fld[0]).rtx))->fld[0]).rtx))->fld[0]).rtx))->code) == ASHIFT
      && ((enum rtx_code) (((((((((((((x)->fld[0]).rtx))->fld[0]).rtx))->fld[0]).rtx))->fld[1]).rtx))->code) == CONST_INT
      && ((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[1]).rtx))->code) == CONST_INT
      && ((((((((((((x)->fld[0]).rtx))->fld[0]).rtx))->fld[0]).rtx))->fld[1]).rtx) == ((((((x)->fld[0]).rtx))->fld[1]).rtx)
      && ((enum rtx_code) ((((((((((x)->fld[0]).rtx))->fld[0]).rtx))->fld[1]).rtx))->code) == CONST_INT
      && (temp = simplify_binary_operation (ASHIFTRT, mode,
         (((((((((x)->fld[0]).rtx))->fld[0]).rtx))->fld[1]).rtx),
         ((((((x)->fld[0]).rtx))->fld[1]).rtx))) != 0)
    {
      rtx new
 = simplify_shift_const ((rtx) 0, ASHIFT, mode,
    ((((((((((((x)->fld[0]).rtx))->fld[0]).rtx))->fld[0]).rtx))->fld[0]).rtx),
    (((((((((x)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint));

      new = simplify_shift_const ((rtx) 0, ASHIFTRT, mode, new,
      (((((((((x)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint));

      do_SUBST(&((((x)->fld[0]).rtx)), (gen_binary (PLUS, mode, new, temp)));
    }
# 3667 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  if ((((rtx_class[(int) (code)]) == '2' || (rtx_class[(int) (code)]) == 'c'
 || (rtx_class[(int) (code)]) == '<')
       && (((rtx_class[(int) (((enum rtx_code) ((((x)->fld[0]).rtx))->code))]) != 'o'
     && ! (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == SUBREG
    && ((rtx_class[(int) (((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->code))])
        == 'o')))
    || ((rtx_class[(int) (((enum rtx_code) ((((x)->fld[1]).rtx))->code))]) != 'o'
        && ! (((enum rtx_code) ((((x)->fld[1]).rtx))->code) == SUBREG
       && ((rtx_class[(int) (((enum rtx_code) (((((((x)->fld[1]).rtx))->fld[0]).rtx))->code))])
    == 'o')))))
      || ((rtx_class[(int) (code)]) == '1'
   && (((rtx_class[(int) (((enum rtx_code) ((((x)->fld[0]).rtx))->code))]) != 'o'
        && ! (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == SUBREG
       && ((rtx_class[(int) (((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->code))])
    == 'o'))))))
    {
      rtx cond, true_rtx, false_rtx;

      cond = if_then_else_cond (x, &true_rtx, &false_rtx);
      if (cond != 0


   && ! ((rtx_class[(int) (code)]) == '<'
  && ((rtx_class[(int) (((enum rtx_code) (true_rtx)->code))]) == '<'
      || (rtx_class[(int) (((enum rtx_code) (false_rtx)->code))]) == '<')))
 {
   rtx cop1 = (const_int_rtx[64]);
   enum rtx_code cond_code = simplify_comparison (NE, &cond, &cop1);

   if (cond_code == NE && (rtx_class[(int) (((enum rtx_code) (cond)->code))]) == '<')
     return x;



   true_rtx = subst (true_rtx, (global_rtl[GR_PC]), (global_rtl[GR_PC]), 0, 0);
   false_rtx = subst (false_rtx, (global_rtl[GR_PC]), (global_rtl[GR_PC]), 0, 0);



   if (general_operand (true_rtx, VOIDmode)
       && general_operand (false_rtx, VOIDmode))
     {





       if (true_rtx == const_true_rtx && false_rtx == (const_int_rtx[64]))
  x = gen_binary (cond_code, mode, cond, cop1);
       else if (true_rtx == (const_int_rtx[64]) && false_rtx == const_true_rtx
         && reverse_condition (cond_code) != UNKNOWN)
  x = gen_binary (reverse_condition (cond_code),
    mode, cond, cop1);



       else if (((enum rtx_code) (true_rtx)->code) == CONST_INT
         && (((true_rtx)->fld[0]).rtwint) == - 1
         && false_rtx == (const_int_rtx[64]))
  x = simplify_gen_unary (NEG, mode,
     gen_binary (cond_code, mode, cond,
          cop1),
     mode);
       else if (((enum rtx_code) (false_rtx)->code) == CONST_INT
         && (((false_rtx)->fld[0]).rtwint) == - 1
         && true_rtx == (const_int_rtx[64]))
  x = simplify_gen_unary (NEG, mode,
     gen_binary (reverse_condition
          (cond_code),
          mode, cond, cop1),
     mode);
       else
  return gen_rtx_fmt_eee (IF_THEN_ELSE, (mode), (gen_binary (cond_code, VOIDmode, cond, cop1)), (true_rtx), (false_rtx));




       code = ((enum rtx_code) (x)->code);
       op0_mode = VOIDmode;
     }
 }
    }



  temp = 0;
  switch ((rtx_class[(int) (code)]))
    {
    case '1':
      temp = simplify_unary_operation (code, mode, (((x)->fld[0]).rtx), op0_mode);
      break;
    case '<':
      {
 enum machine_mode cmp_mode = ((enum machine_mode) ((((x)->fld[0]).rtx))->mode);
 if (cmp_mode == VOIDmode)
   {
     cmp_mode = ((enum machine_mode) ((((x)->fld[1]).rtx))->mode);
     if (cmp_mode == VOIDmode)
       cmp_mode = op0_mode;
   }
 temp = simplify_relational_operation (code, cmp_mode,
           (((x)->fld[0]).rtx), (((x)->fld[1]).rtx));
      }
# 3779 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
      break;
    case 'c':
    case '2':
      temp = simplify_binary_operation (code, mode, (((x)->fld[0]).rtx), (((x)->fld[1]).rtx));
      break;
    case 'b':
    case '3':
      temp = simplify_ternary_operation (code, mode, op0_mode, (((x)->fld[0]).rtx),
      (((x)->fld[1]).rtx), (((x)->fld[2]).rtx));
      break;
    }

  if (temp)
    {
      x = temp;
      code = ((enum rtx_code) (temp)->code);
      op0_mode = VOIDmode;
      mode = ((enum machine_mode) (temp)->mode);
    }


  if (code == PLUS || code == MINUS
      || code == AND || code == IOR || code == XOR)
    {
      x = apply_distributive_law (x);
      code = ((enum rtx_code) (x)->code);
      op0_mode = VOIDmode;
    }




  if ((code == PLUS || code == MINUS || code == MULT || code == DIV
       || code == AND || code == IOR || code == XOR
       || code == SMAX || code == SMIN || code == UMAX || code == UMIN)
      && ((((mode_class[(int) (mode)]) == MODE_INT || (mode_class[(int) (mode)]) == MODE_PARTIAL_INT || (mode_class[(int) (mode)]) == MODE_COMPLEX_INT || (mode_class[(int) (mode)]) == MODE_VECTOR_INT) && code != DIV)
   || (flag_unsafe_math_optimizations && ((mode_class[(int) (mode)]) == MODE_FLOAT || (mode_class[(int) (mode)]) == MODE_COMPLEX_FLOAT || (mode_class[(int) (mode)]) == MODE_VECTOR_FLOAT))))
    {
      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == code)
 {
   rtx other = ((((((x)->fld[0]).rtx))->fld[0]).rtx);
   rtx inner_op0 = ((((((x)->fld[0]).rtx))->fld[1]).rtx);
   rtx inner_op1 = (((x)->fld[1]).rtx);
   rtx inner;



   if ((((enum rtx_code) (inner_op0)->code) == LABEL_REF || ((enum rtx_code) (inner_op0)->code) == SYMBOL_REF || ((enum rtx_code) (inner_op0)->code) == CONST_INT || ((enum rtx_code) (inner_op0)->code) == CONST_DOUBLE || ((enum rtx_code) (inner_op0)->code) == CONST || ((enum rtx_code) (inner_op0)->code) == HIGH || ((enum rtx_code) (inner_op0)->code) == CONST_VECTOR || ((enum rtx_code) (inner_op0)->code) == CONSTANT_P_RTX) && (rtx_class[(int) (code)]) == 'c')
     {
       rtx tem = inner_op0;
       inner_op0 = inner_op1;
       inner_op1 = tem;
     }
   inner = simplify_binary_operation (code == MINUS ? PLUS
          : code == DIV ? MULT
          : code,
          mode, inner_op0, inner_op1);



   if (inner == 0 && (rtx_class[(int) (code)]) == 'c')
     {
       other = ((((((x)->fld[0]).rtx))->fld[1]).rtx);
       inner = simplify_binary_operation (code, mode,
       ((((((x)->fld[0]).rtx))->fld[0]).rtx),
       (((x)->fld[1]).rtx));
     }

   if (inner)
     return gen_binary (code, mode, other, inner);
 }
    }


  switch (code)
    {
    case MEM:


      temp = make_compound_operation ((((x)->fld[0]).rtx), MEM);
      do_SUBST(&((((x)->fld[0]).rtx)), (temp));
      break;

    case SUBREG:
      if (op0_mode == VOIDmode)
 op0_mode = ((enum machine_mode) ((((x)->fld[0]).rtx))->mode);


      if ((((enum rtx_code) ((((x)->fld[0]).rtx))->code) == LABEL_REF || ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == SYMBOL_REF || ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == CONST_INT || ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == CONST_DOUBLE || ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == CONST || ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == HIGH || ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == CONST_VECTOR || ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == CONSTANT_P_RTX)
   && subreg_lowpart_offset (mode, op0_mode) == (((x)->fld[1]).rtuint))
 return gen_lowpart_for_combine (mode, (((x)->fld[0]).rtx));

      if ((mode_class[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))]) == MODE_CC)
        break;
      {
 rtx temp;
 temp = simplify_subreg (mode, (((x)->fld[0]).rtx), op0_mode,
    (((x)->fld[1]).rtuint));
 if (temp)
   return temp;
      }



      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == MEM
   && ((((((x)->fld[0]).rtx))->volatil)
       || mode_dependent_address_p (((((((x)->fld[0]).rtx))->fld[0]).rtx))))
 return gen_rtx_fmt_e (CLOBBER, (mode), ((const_int_rtx[64])));





      break;

    case NOT:

      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == PLUS
   && ((((((x)->fld[0]).rtx))->fld[1]).rtx) == (const_int_rtx[64 -1]))
 return gen_rtx_fmt_e (NEG, (mode), (((((((x)->fld[0]).rtx))->fld[0]).rtx)));


      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == NEG)
 return gen_rtx_fmt_ee (PLUS, (mode), (((((((x)->fld[0]).rtx))->fld[0]).rtx)), ((const_int_rtx[64 -1])));


      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == XOR
   && ((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[1]).rtx))->code) == CONST_INT
   && (temp = simplify_unary_operation (NOT, mode,
            ((((((x)->fld[0]).rtx))->fld[1]).rtx),
            mode)) != 0)
 return gen_binary (XOR, mode, ((((((x)->fld[0]).rtx))->fld[0]).rtx), temp);





      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == ASHIFT
   && ((((((x)->fld[0]).rtx))->fld[0]).rtx) == (const_int_rtx[64 +1]))
 return gen_rtx_fmt_ee (ROTATE, (mode), (simplify_gen_unary (NOT, mode, (const_int_rtx[64 +1]), mode)), (((((((x)->fld[0]).rtx))->fld[1]).rtx)));



      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == SUBREG
   && subreg_lowpart_p ((((x)->fld[0]).rtx))
   && ((mode_size[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))])
       < (mode_size[(int) (((enum machine_mode) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->mode))]))
   && ((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->code) == ASHIFT
   && (((((((((x)->fld[0]).rtx))->fld[0]).rtx))->fld[0]).rtx) == (const_int_rtx[64 +1]))
 {
   enum machine_mode inner_mode = ((enum machine_mode) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->mode);

   x = gen_rtx_fmt_ee (ROTATE, (inner_mode), (simplify_gen_unary (NOT, inner_mode, (const_int_rtx[64 +1]), inner_mode)), ((((((((((x)->fld[0]).rtx))->fld[0]).rtx))->fld[1]).rtx)));



   return gen_lowpart_for_combine (mode, x);
 }



      if (1 == -1
   && (rtx_class[(int) (((enum rtx_code) ((((x)->fld[0]).rtx))->code))]) == '<'
   && (reversed = reversed_comparison (x, mode, ((((((x)->fld[0]).rtx))->fld[0]).rtx),
           ((((((x)->fld[0]).rtx))->fld[1]).rtx))))
 return reversed;





      if (1 == -1
   && ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == ASHIFTRT
   && ((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[1]).rtx))->code) == CONST_INT
   && (((((((((x)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint) == (mode_bitsize[(int) (mode)]) - 1)
 return gen_rtx_fmt_ee (GE, (mode), (((((((x)->fld[0]).rtx))->fld[0]).rtx)), ((const_int_rtx[64])));






      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == IOR || ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == AND)
 {
   rtx in1 = ((((((x)->fld[0]).rtx))->fld[0]).rtx), in2 = ((((((x)->fld[0]).rtx))->fld[1]).rtx);
   enum machine_mode op_mode;

   op_mode = ((enum machine_mode) (in1)->mode);
   in1 = simplify_gen_unary (NOT, op_mode, in1, op_mode);

   op_mode = ((enum machine_mode) (in2)->mode);
   if (op_mode == VOIDmode)
     op_mode = mode;
   in2 = simplify_gen_unary (NOT, op_mode, in2, op_mode);

   if (((enum rtx_code) (in2)->code) == NOT && ((enum rtx_code) (in1)->code) != NOT)
     {
       rtx tem = in2;
       in2 = in1; in1 = tem;
     }

   return gen_rtx_fmt_ee (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == IOR ? AND : IOR,
     mode, in1, in2);
 }
      break;

    case NEG:

      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == PLUS
   && ((((((x)->fld[0]).rtx))->fld[1]).rtx) == (const_int_rtx[64 +1]))
 return gen_rtx_fmt_e (NOT, (mode), (((((((x)->fld[0]).rtx))->fld[0]).rtx)));


      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == NOT)
 return plus_constant_wide ((((((((x)->fld[0]).rtx))->fld[0]).rtx)), (long long) (1));


      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == MINUS
   && (! ((mode_class[(int) (mode)]) == MODE_FLOAT || (mode_class[(int) (mode)]) == MODE_COMPLEX_FLOAT || (mode_class[(int) (mode)]) == MODE_VECTOR_FLOAT)

       || 1 != 1
       || flag_unsafe_math_optimizations))
 return gen_binary (MINUS, mode, ((((((x)->fld[0]).rtx))->fld[1]).rtx),
      ((((((x)->fld[0]).rtx))->fld[0]).rtx));


      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == XOR && ((((((x)->fld[0]).rtx))->fld[1]).rtx) == (const_int_rtx[64 +1])
   && nonzero_bits (((((((x)->fld[0]).rtx))->fld[0]).rtx), mode) == 1)
 return gen_binary (PLUS, mode, ((((((x)->fld[0]).rtx))->fld[0]).rtx), (const_int_rtx[64 -1]));





      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == ASHIFT)
 {
   temp = simplify_unary_operation (NEG, mode,
        ((((((x)->fld[0]).rtx))->fld[0]).rtx), mode);
   if (temp)
     return gen_binary (ASHIFT, mode, temp, ((((((x)->fld[0]).rtx))->fld[1]).rtx));
 }

      temp = expand_compound_operation ((((x)->fld[0]).rtx));





      if (((enum rtx_code) (temp)->code) == ASHIFTRT
   && ((enum rtx_code) ((((temp)->fld[1]).rtx))->code) == CONST_INT
   && ((((((temp)->fld[1]).rtx))->fld[0]).rtwint) == (mode_bitsize[(int) (mode)]) - 1)
 return simplify_shift_const (temp, LSHIFTRT, mode, (((temp)->fld[0]).rtx),
         ((((((temp)->fld[1]).rtx))->fld[0]).rtwint));
# 4040 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
      if (((enum rtx_code) (temp)->code) != REG
   && ! (((enum rtx_code) (temp)->code) == SUBREG
  && ((enum rtx_code) ((((temp)->fld[0]).rtx))->code) == REG)
   && (i = exact_log2_wide ((unsigned long long) (nonzero_bits (temp, mode)))) >= 0)
 {
   rtx temp1 = simplify_shift_const
     ((rtx) 0, ASHIFTRT, mode,
      simplify_shift_const ((rtx) 0, ASHIFT, mode, temp,
       (mode_bitsize[(int) (mode)]) - 1 - i),
      (mode_bitsize[(int) (mode)]) - 1 - i);




   if (((enum rtx_code) (temp1)->code) != ASHIFTRT
       || ((enum rtx_code) ((((temp1)->fld[0]).rtx))->code) != ASHIFT
       || ((((((temp1)->fld[0]).rtx))->fld[0]).rtx) != temp)
     return temp1;
 }
      break;

    case TRUNCATE:



      if ((mode_class[(int) (mode)]) == MODE_PARTIAL_INT)
 break;

      if ((mode_bitsize[(int) (mode)]) <= (8 * 8)
   && 1)

 do_SUBST(&((((x)->fld[0]).rtx)), (force_to_mode ((((x)->fld[0]).rtx), ((enum machine_mode) ((((x)->fld[0]).rtx))->mode), mode_mask_array[(int) (mode)], (rtx) 0, 0)));




      if ((((enum rtx_code) ((((x)->fld[0]).rtx))->code) == SIGN_EXTEND
    || ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == ZERO_EXTEND)
   && ((enum machine_mode) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->mode) == mode)
 return ((((((x)->fld[0]).rtx))->fld[0]).rtx);



      if ((((enum rtx_code) ((((x)->fld[0]).rtx))->code) == ABS
    || ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == NEG)
   && (((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->code) == SIGN_EXTEND
       || ((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->code) == ZERO_EXTEND)
   && ((enum machine_mode) ((((((((((x)->fld[0]).rtx))->fld[0]).rtx))->fld[0]).rtx))->mode) == mode)
 return simplify_gen_unary (((enum rtx_code) ((((x)->fld[0]).rtx))->code), mode,
       (((((((((x)->fld[0]).rtx))->fld[0]).rtx))->fld[0]).rtx), mode);



      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == SUBREG
   && ((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->code) == TRUNCATE
   && subreg_lowpart_p ((((x)->fld[0]).rtx)))
 return ((((((x)->fld[0]).rtx))->fld[0]).rtx);






      if (1

   && num_sign_bit_copies ((((x)->fld[0]).rtx), ((enum machine_mode) ((((x)->fld[0]).rtx))->mode))
      >= (mode_bitsize[(int) (mode)]) + 1
   && ! (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == LSHIFTRT
  && ((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->code) == MULT))
 return gen_lowpart_for_combine (mode, (((x)->fld[0]).rtx));





      if ((mode_bitsize[(int) (mode)]) <= (8 * 8)
   && (rtx_class[(int) (((enum rtx_code) ((((x)->fld[0]).rtx))->code))]) == '<'
   && ((long long) 1 & ~mode_mask_array[(int) (mode)]) == 0)
 return gen_lowpart_for_combine (mode, (((x)->fld[0]).rtx));




      if ((mode_bitsize[(int) (mode)]) <= (8 * 8)
   && ((long long) 1 & ~mode_mask_array[(int) (mode)]) == 0
   && (temp = get_last_value ((((x)->fld[0]).rtx)))
   && (rtx_class[(int) (((enum rtx_code) (temp)->code))]) == '<')
 return gen_lowpart_for_combine (mode, (((x)->fld[0]).rtx));

      break;

    case FLOAT_TRUNCATE:

      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == FLOAT_EXTEND
   && ((enum machine_mode) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->mode) == mode)
 return ((((((x)->fld[0]).rtx))->fld[0]).rtx);



      if ((((enum rtx_code) ((((x)->fld[0]).rtx))->code) == ABS
    || ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == NEG)
   && ((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->code) == FLOAT_EXTEND
   && ((enum machine_mode) ((((((((((x)->fld[0]).rtx))->fld[0]).rtx))->fld[0]).rtx))->mode) == mode)
 return simplify_gen_unary (((enum rtx_code) ((((x)->fld[0]).rtx))->code), mode,
       (((((((((x)->fld[0]).rtx))->fld[0]).rtx))->fld[0]).rtx), mode);



      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == SUBREG
   && subreg_lowpart_p ((((x)->fld[0]).rtx))
   && ((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->code) == FLOAT_TRUNCATE)
 return ((((((x)->fld[0]).rtx))->fld[0]).rtx);
      break;
# 4171 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
    case CONST:



      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == CONST)
 do_SUBST(&((((x)->fld[0]).rtx)), (((((((x)->fld[0]).rtx))->fld[0]).rtx)));
      break;
# 4190 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
    case PLUS:




      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == PLUS
   && (((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[1]).rtx))->code) == LABEL_REF || ((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[1]).rtx))->code) == SYMBOL_REF || ((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[1]).rtx))->code) == CONST_INT || ((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[1]).rtx))->code) == CONST || ((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[1]).rtx))->code) == CONST_DOUBLE))
 return gen_binary (PLUS, mode,
      gen_binary (PLUS, mode, ((((((x)->fld[0]).rtx))->fld[0]).rtx),
           (((x)->fld[1]).rtx)),
      ((((((x)->fld[0]).rtx))->fld[1]).rtx));






      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == XOR
   && ((enum rtx_code) ((((x)->fld[1]).rtx))->code) == CONST_INT
   && ((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[1]).rtx))->code) == CONST_INT
   && ((((((x)->fld[1]).rtx))->fld[0]).rtwint) == -(((((((((x)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint)
   && ((i = exact_log2_wide ((unsigned long long) ((((((((((x)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint)))) >= 0
       || (i = exact_log2_wide ((unsigned long long) (((((((x)->fld[1]).rtx))->fld[0]).rtwint)))) >= 0)
   && (mode_bitsize[(int) (mode)]) <= (8 * 8)
   && ((((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->code) == AND
        && ((enum rtx_code) ((((((((((x)->fld[0]).rtx))->fld[0]).rtx))->fld[1]).rtx))->code) == CONST_INT
        && (((((((((((((x)->fld[0]).rtx))->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint)
     == ((long long) 1 << (i + 1)) - 1))
       || (((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->code) == ZERO_EXTEND
    && ((mode_bitsize[(int) (((enum machine_mode) ((((((((((x)->fld[0]).rtx))->fld[0]).rtx))->fld[0]).rtx))->mode))])
        == (unsigned int) i + 1))))
 return simplify_shift_const
   ((rtx) 0, ASHIFTRT, mode,
    simplify_shift_const ((rtx) 0, ASHIFT, mode,
     (((((((((x)->fld[0]).rtx))->fld[0]).rtx))->fld[0]).rtx),
     (mode_bitsize[(int) (mode)]) - (i + 1)),
    (mode_bitsize[(int) (mode)]) - (i + 1));





      if ((rtx_class[(int) (((enum rtx_code) ((((x)->fld[0]).rtx))->code))]) == '<'
   && ((1 == -1 && (((x)->fld[1]).rtx) == (const_int_rtx[64 +1]))
       || (1 == 1 && (((x)->fld[1]).rtx) == (const_int_rtx[64 -1])))
   && (reversed = reversed_comparison ((((x)->fld[0]).rtx), mode,
           ((((((x)->fld[0]).rtx))->fld[0]).rtx),
           ((((((x)->fld[0]).rtx))->fld[1]).rtx))))
 return
   simplify_gen_unary (NEG, mode, reversed, mode);





      if ((((x)->fld[1]).rtx) == (const_int_rtx[64 -1])
   && ((enum rtx_code) ((((x)->fld[0]).rtx))->code) != REG
   && ! (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == SUBREG
  && ((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->code) == REG)
   && nonzero_bits ((((x)->fld[0]).rtx), mode) == 1)
 return simplify_shift_const ((rtx) 0, ASHIFTRT, mode,
    simplify_shift_const ((rtx) 0, ASHIFT, mode,
     gen_rtx_fmt_ee (XOR, (mode), ((((x)->fld[0]).rtx)), ((const_int_rtx[64 +1]))),
     (mode_bitsize[(int) (mode)]) - 1),
    (mode_bitsize[(int) (mode)]) - 1);






      if ((mode_bitsize[(int) (mode)]) <= (8 * 8)
   && (nonzero_bits ((((x)->fld[0]).rtx), mode)
       & nonzero_bits ((((x)->fld[1]).rtx), mode)) == 0)
 {

   rtx tor = gen_binary (IOR, mode, (((x)->fld[0]).rtx), (((x)->fld[1]).rtx));
   temp = combine_simplify_rtx (tor, mode, last, in_dest);




   if (((enum rtx_code) (temp)->code) != CLOBBER && temp != tor)
     return temp;
 }
      break;

    case MINUS:


      if (1 == 1
   && (((x)->fld[0]).rtx) == (const_int_rtx[64 +1])
   && (rtx_class[(int) (((enum rtx_code) ((((x)->fld[1]).rtx))->code))]) == '<'
   && (reversed = reversed_comparison ((((x)->fld[1]).rtx), mode,
           ((((((x)->fld[1]).rtx))->fld[0]).rtx),
           ((((((x)->fld[1]).rtx))->fld[1]).rtx))))
 return reversed;



      if (((enum rtx_code) ((((x)->fld[1]).rtx))->code) == AND
   && ((enum rtx_code) (((((((x)->fld[1]).rtx))->fld[1]).rtx))->code) == CONST_INT
   && exact_log2_wide ((unsigned long long) (-(((((((((x)->fld[1]).rtx))->fld[1]).rtx))->fld[0]).rtwint))) >= 0
   && rtx_equal_p (((((((x)->fld[1]).rtx))->fld[0]).rtx), (((x)->fld[0]).rtx)))
 return simplify_and_const_int ((rtx) 0, mode, (((x)->fld[0]).rtx),
           -(((((((((x)->fld[1]).rtx))->fld[1]).rtx))->fld[0]).rtwint) - 1);



      if (((enum rtx_code) ((((x)->fld[1]).rtx))->code) == PLUS && ((mode_class[(int) (mode)]) == MODE_INT || (mode_class[(int) (mode)]) == MODE_PARTIAL_INT || (mode_class[(int) (mode)]) == MODE_COMPLEX_INT || (mode_class[(int) (mode)]) == MODE_VECTOR_INT))
 return gen_binary (MINUS, mode,
      gen_binary (MINUS, mode, (((x)->fld[0]).rtx),
           ((((((x)->fld[1]).rtx))->fld[0]).rtx)),
      ((((((x)->fld[1]).rtx))->fld[1]).rtx));
      break;

    case MULT:




      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == PLUS)
 {
   x = apply_distributive_law
     (gen_binary (PLUS, mode,
    gen_binary (MULT, mode,
         ((((((x)->fld[0]).rtx))->fld[0]).rtx), (((x)->fld[1]).rtx)),
    gen_binary (MULT, mode,
         ((((((x)->fld[0]).rtx))->fld[1]).rtx),
         copy_rtx ((((x)->fld[1]).rtx)))));

   if (((enum rtx_code) (x)->code) != MULT)
     return x;
 }

      if (((mode_class[(int) (mode)]) == MODE_FLOAT || (mode_class[(int) (mode)]) == MODE_COMPLEX_FLOAT || (mode_class[(int) (mode)]) == MODE_VECTOR_FLOAT) && flag_unsafe_math_optimizations
   && ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == DIV)
 {
   rtx tem = simplify_binary_operation (MULT, mode,
            ((((((x)->fld[0]).rtx))->fld[0]).rtx),
            (((x)->fld[1]).rtx));
   if (tem)
     return gen_binary (DIV, mode, tem, ((((((x)->fld[0]).rtx))->fld[1]).rtx));
 }
      break;

    case UDIV:


      if (((enum rtx_code) ((((x)->fld[1]).rtx))->code) == CONST_INT
   && (i = exact_log2_wide ((unsigned long long) (((((((x)->fld[1]).rtx))->fld[0]).rtwint)))) >= 0
   && (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == ASHIFT
       || ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == LSHIFTRT
       || ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == ASHIFTRT
       || ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == ROTATE
       || ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == ROTATERT))
 return simplify_shift_const ((rtx) 0, LSHIFTRT, mode, (((x)->fld[0]).rtx), i);
      break;

    case EQ: case NE:
    case GT: case GTU: case GE: case GEU:
    case LT: case LTU: case LE: case LEU:
    case UNEQ: case LTGT:
    case UNGT: case UNGE:
    case UNLT: case UNLE:
    case UNORDERED: case ORDERED:


      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == COMPARE
   || ((mode_class[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))]) != MODE_CC



       ))
 {
   rtx op0 = (((x)->fld[0]).rtx);
   rtx op1 = (((x)->fld[1]).rtx);
   enum rtx_code new_code;

   if (((enum rtx_code) (op0)->code) == COMPARE)
     op1 = (((op0)->fld[1]).rtx), op0 = (((op0)->fld[0]).rtx);


   new_code = simplify_comparison (code, &op0, &op1);
# 4387 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
   if (1 == 1
       && new_code == NE && (mode_class[(int) (mode)]) == MODE_INT
       && op1 == (const_int_rtx[64])
       && mode == ((enum machine_mode) (op0)->mode)
       && nonzero_bits (op0, mode) == 1)
     return gen_lowpart_for_combine (mode,
         expand_compound_operation (op0));

   else if (1 == 1
     && new_code == NE && (mode_class[(int) (mode)]) == MODE_INT
     && op1 == (const_int_rtx[64])
     && mode == ((enum machine_mode) (op0)->mode)
     && (num_sign_bit_copies (op0, mode)
         == (mode_bitsize[(int) (mode)])))
     {
       op0 = expand_compound_operation (op0);
       return simplify_gen_unary (NEG, mode,
      gen_lowpart_for_combine (mode, op0),
      mode);
     }

   else if (1 == 1
     && new_code == EQ && (mode_class[(int) (mode)]) == MODE_INT
     && op1 == (const_int_rtx[64])
     && mode == ((enum machine_mode) (op0)->mode)
     && nonzero_bits (op0, mode) == 1)
     {
       op0 = expand_compound_operation (op0);
       return gen_binary (XOR, mode,
     gen_lowpart_for_combine (mode, op0),
     (const_int_rtx[64 +1]));
     }

   else if (1 == 1
     && new_code == EQ && (mode_class[(int) (mode)]) == MODE_INT
     && op1 == (const_int_rtx[64])
     && mode == ((enum machine_mode) (op0)->mode)
     && (num_sign_bit_copies (op0, mode)
         == (mode_bitsize[(int) (mode)])))
     {
       op0 = expand_compound_operation (op0);
       return plus_constant_wide ((gen_lowpart_for_combine (mode, op0)), (long long) (1));
     }



   if (1 == -1
       && new_code == NE && (mode_class[(int) (mode)]) == MODE_INT
       && op1 == (const_int_rtx[64])
       && (num_sign_bit_copies (op0, mode)
    == (mode_bitsize[(int) (mode)])))
     return gen_lowpart_for_combine (mode,
         expand_compound_operation (op0));

   else if (1 == -1
     && new_code == NE && (mode_class[(int) (mode)]) == MODE_INT
     && op1 == (const_int_rtx[64])
     && mode == ((enum machine_mode) (op0)->mode)
     && nonzero_bits (op0, mode) == 1)
     {
       op0 = expand_compound_operation (op0);
       return simplify_gen_unary (NEG, mode,
      gen_lowpart_for_combine (mode, op0),
      mode);
     }

   else if (1 == -1
     && new_code == EQ && (mode_class[(int) (mode)]) == MODE_INT
     && op1 == (const_int_rtx[64])
     && mode == ((enum machine_mode) (op0)->mode)
     && (num_sign_bit_copies (op0, mode)
         == (mode_bitsize[(int) (mode)])))
     {
       op0 = expand_compound_operation (op0);
       return simplify_gen_unary (NOT, mode,
      gen_lowpart_for_combine (mode, op0),
      mode);
     }


   else if (1 == -1
     && new_code == EQ && (mode_class[(int) (mode)]) == MODE_INT
     && op1 == (const_int_rtx[64])
     && mode == ((enum machine_mode) (op0)->mode)
     && nonzero_bits (op0, mode) == 1)
     {
       op0 = expand_compound_operation (op0);
       return plus_constant_wide ((gen_lowpart_for_combine (mode, op0)), (long long) (-1));
     }






   if (new_code == NE && (mode_class[(int) (mode)]) == MODE_INT
       && (mode_bitsize[(int) (mode)]) <= (8 * 8)
       && ((1 & mode_mask_array[(int) (mode)])
    == (unsigned long long) 1 << ((mode_bitsize[(int) (mode)])-1))
       && op1 == (const_int_rtx[64])
       && mode == ((enum machine_mode) (op0)->mode)
       && (i = exact_log2_wide ((unsigned long long) (nonzero_bits (op0, mode)))) >= 0)
     {
       x = simplify_shift_const ((rtx) 0, ASHIFT, mode,
     expand_compound_operation (op0),
     (mode_bitsize[(int) (mode)]) - 1 - i);
       if (((enum rtx_code) (x)->code) == AND && (((x)->fld[1]).rtx) == const_true_rtx)
  return (((x)->fld[0]).rtx);
       else
  return x;
     }


   if (new_code != code)
     return gen_rtx_fmt_ee (new_code, mode, op0, op1);



   do_SUBST(&((((x)->fld[0]).rtx)), (op0));
   do_SUBST(&((((x)->fld[1]).rtx)), (op1));
 }
      break;

    case IF_THEN_ELSE:
      return simplify_if_then_else (x);

    case ZERO_EXTRACT:
    case SIGN_EXTRACT:
    case ZERO_EXTEND:
    case SIGN_EXTEND:

      if (in_dest)
 return x;

      return expand_compound_operation (x);

    case SET:
      return simplify_set (x);

    case AND:
    case IOR:
    case XOR:
      return simplify_logical (x, last);

    case ABS:

      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == NEG)
 do_SUBST(&((((x)->fld[0]).rtx)), (((((((x)->fld[0]).rtx))->fld[0]).rtx)));



      if (((enum machine_mode) ((((x)->fld[0]).rtx))->mode) == VOIDmode)
 break;


      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == FFS || ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == ABS
   || (((mode_bitsize[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))])
        <= (8 * 8))
       && ((nonzero_bits ((((x)->fld[0]).rtx), ((enum machine_mode) ((((x)->fld[0]).rtx))->mode))
     & ((long long) 1
        << ((mode_bitsize[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))]) - 1)))
    == 0)))
 return (((x)->fld[0]).rtx);


      if (num_sign_bit_copies ((((x)->fld[0]).rtx), mode) == (mode_bitsize[(int) (mode)]))
 return gen_rtx_fmt_e (NEG, (mode), ((((x)->fld[0]).rtx)));

      break;

    case FFS:

      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == SIGN_EXTEND
   || ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == ZERO_EXTEND)
 do_SUBST(&((((x)->fld[0]).rtx)), (((((((x)->fld[0]).rtx))->fld[0]).rtx)));
      break;

    case FLOAT:

      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == SIGN_EXTEND)
 do_SUBST(&((((x)->fld[0]).rtx)), (((((((x)->fld[0]).rtx))->fld[0]).rtx)));
      break;

    case ASHIFT:
    case LSHIFTRT:
    case ASHIFTRT:
    case ROTATE:
    case ROTATERT:

      if (((enum rtx_code) ((((x)->fld[1]).rtx))->code) == CONST_INT)
 return simplify_shift_const (x, code, mode, (((x)->fld[0]).rtx),
         ((((((x)->fld[1]).rtx))->fld[0]).rtwint));
# 4590 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
      break;

    case VEC_SELECT:
      {
 rtx op0 = (((x)->fld[0]).rtx);
 rtx op1 = (((x)->fld[1]).rtx);
 int len;

 if (((enum rtx_code) (op1)->code) != PARALLEL)
   fancy_abort ("/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c", 4599, __FUNCTION__);
 len = (((((op1)->fld[0]).rtvec))->num_elem);
 if (len == 1
     && ((enum rtx_code) ((((((op1)->fld[0]).rtvec))->elem[0]))->code) == CONST_INT
     && ((enum rtx_code) (op0)->code) == VEC_CONCAT)
   {
     int offset = ((((((((op1)->fld[0]).rtvec))->elem[0]))->fld[0]).rtwint) * (mode_size[(int) (((enum machine_mode) (x)->mode))]);


     for (;;)
       {
  if (((enum machine_mode) (op0)->mode) == ((enum machine_mode) (x)->mode))
    return op0;
  if (((enum rtx_code) (op0)->code) == VEC_CONCAT)
    {
      long long op0_size = (mode_size[(int) (((enum machine_mode) ((((op0)->fld[0]).rtx))->mode))]);
      if (op0_size < offset)
        op0 = (((op0)->fld[0]).rtx);
      else
        {
   offset -= op0_size;
   op0 = (((op0)->fld[1]).rtx);
        }
    }
  else
    break;
       }
   }
      }

      break;

    default:
      break;
    }

  return x;
}



static rtx
simplify_if_then_else (x)
     rtx x;
{
  enum machine_mode mode = ((enum machine_mode) (x)->mode);
  rtx cond = (((x)->fld[0]).rtx);
  rtx true_rtx = (((x)->fld[1]).rtx);
  rtx false_rtx = (((x)->fld[2]).rtx);
  enum rtx_code true_code = ((enum rtx_code) (cond)->code);
  int comparison_p = (rtx_class[(int) (true_code)]) == '<';
  rtx temp;
  int i;
  enum rtx_code false_code;
  rtx reversed;


  if (comparison_p && true_rtx == const_true_rtx && false_rtx == (const_int_rtx[64]))
    return gen_binary (true_code, mode, (((cond)->fld[0]).rtx), (((cond)->fld[1]).rtx));


  if (comparison_p
      && true_rtx == (const_int_rtx[64]) && false_rtx == const_true_rtx
      && (reversed = reversed_comparison (cond, mode, (((cond)->fld[0]).rtx),
       (((cond)->fld[1]).rtx))))
    return reversed;





  if (comparison_p
      && ((false_code = combine_reversed_comparison_code (cond))
   != UNKNOWN)
      && ((enum rtx_code) ((((cond)->fld[0]).rtx))->code) == REG)
    {
      long long nzb;
      rtx from = (((cond)->fld[0]).rtx);
      rtx true_val = (((cond)->fld[1]).rtx);
      rtx false_val = true_val;
      int swapped = 0;



      if (false_code == EQ)
 {
   swapped = 1, true_code = EQ, false_code = NE;
   temp = true_rtx, true_rtx = false_rtx, false_rtx = temp;
 }





      if (true_code == EQ && true_val == (const_int_rtx[64])
   && exact_log2_wide ((unsigned long long) (nzb = nonzero_bits (from, ((enum machine_mode) (from)->mode)))) >= 0)
 false_code = EQ, false_val = gen_rtx_CONST_INT (VOIDmode, (long long) (nzb));
      else if (true_code == EQ && true_val == (const_int_rtx[64])
        && (num_sign_bit_copies (from, ((enum machine_mode) (from)->mode))
     == (mode_bitsize[(int) (((enum machine_mode) (from)->mode))])))
 false_code = EQ, false_val = (const_int_rtx[64 -1]);





      if (reg_mentioned_p (from, true_rtx))
 true_rtx = subst (known_cond (copy_rtx (true_rtx), true_code,
          from, true_val),
        (global_rtl[GR_PC]), (global_rtl[GR_PC]), 0, 0);
      if (reg_mentioned_p (from, false_rtx))
 false_rtx = subst (known_cond (copy_rtx (false_rtx), false_code,
       from, false_val),
         (global_rtl[GR_PC]), (global_rtl[GR_PC]), 0, 0);

      do_SUBST(&((((x)->fld[1]).rtx)), (swapped ? false_rtx : true_rtx));
      do_SUBST(&((((x)->fld[2]).rtx)), (swapped ? true_rtx : false_rtx));

      true_rtx = (((x)->fld[1]).rtx);
      false_rtx = (((x)->fld[2]).rtx);
      true_code = ((enum rtx_code) (cond)->code);
    }







  if (comparison_p
      && combine_reversed_comparison_code (cond) != UNKNOWN
      && (true_rtx == (global_rtl[GR_PC])
   || ((((enum rtx_code) (true_rtx)->code) == LABEL_REF || ((enum rtx_code) (true_rtx)->code) == SYMBOL_REF || ((enum rtx_code) (true_rtx)->code) == CONST_INT || ((enum rtx_code) (true_rtx)->code) == CONST_DOUBLE || ((enum rtx_code) (true_rtx)->code) == CONST || ((enum rtx_code) (true_rtx)->code) == HIGH || ((enum rtx_code) (true_rtx)->code) == CONST_VECTOR || ((enum rtx_code) (true_rtx)->code) == CONSTANT_P_RTX)
       && ((enum rtx_code) (false_rtx)->code) != CONST_INT && false_rtx != (global_rtl[GR_PC]))
   || true_rtx == (const_int_rtx[64])
   || ((rtx_class[(int) (((enum rtx_code) (true_rtx)->code))]) == 'o'
       && (rtx_class[(int) (((enum rtx_code) (false_rtx)->code))]) != 'o')
   || (((enum rtx_code) (true_rtx)->code) == SUBREG
       && (rtx_class[(int) (((enum rtx_code) ((((true_rtx)->fld[0]).rtx))->code))]) == 'o'
       && (rtx_class[(int) (((enum rtx_code) (false_rtx)->code))]) != 'o')
   || reg_mentioned_p (true_rtx, false_rtx)
   || rtx_equal_p (false_rtx, (((cond)->fld[0]).rtx))))
    {
      true_code = reversed_comparison_code (cond, ((void*)0));
      do_SUBST(&((((x)->fld[0]).rtx)), (reversed_comparison (cond, ((enum machine_mode) (cond)->mode), (((cond)->fld[0]).rtx), (((cond)->fld[1]).rtx))));



      do_SUBST(&((((x)->fld[1]).rtx)), (false_rtx));
      do_SUBST(&((((x)->fld[2]).rtx)), (true_rtx));

      temp = true_rtx, true_rtx = false_rtx, false_rtx = temp;
      cond = (((x)->fld[0]).rtx);


      true_code = ((enum rtx_code) (cond)->code);
      comparison_p = (rtx_class[(int) (true_code)]) == '<';
    }



  if (rtx_equal_p (true_rtx, false_rtx) && ! side_effects_p (cond))
    return true_rtx;


  if (true_code == EQ && ! side_effects_p (cond)
      && (! ((mode_class[(int) (mode)]) == MODE_FLOAT || (mode_class[(int) (mode)]) == MODE_COMPLEX_FLOAT || (mode_class[(int) (mode)]) == MODE_VECTOR_FLOAT) || flag_unsafe_math_optimizations)
      && rtx_equal_p ((((cond)->fld[0]).rtx), false_rtx)
      && rtx_equal_p ((((cond)->fld[1]).rtx), true_rtx))
    return false_rtx;
  else if (true_code == NE && ! side_effects_p (cond)
    && (! ((mode_class[(int) (mode)]) == MODE_FLOAT || (mode_class[(int) (mode)]) == MODE_COMPLEX_FLOAT || (mode_class[(int) (mode)]) == MODE_VECTOR_FLOAT) || flag_unsafe_math_optimizations)
    && rtx_equal_p ((((cond)->fld[0]).rtx), true_rtx)
    && rtx_equal_p ((((cond)->fld[1]).rtx), false_rtx))
    return true_rtx;



  if ((mode_class[(int) (mode)]) == MODE_INT
      && ((enum rtx_code) (false_rtx)->code) == NEG
      && rtx_equal_p (true_rtx, (((false_rtx)->fld[0]).rtx))
      && comparison_p
      && rtx_equal_p (true_rtx, (((cond)->fld[0]).rtx))
      && ! side_effects_p (true_rtx))
    switch (true_code)
      {
      case GT:
      case GE:
 return simplify_gen_unary (ABS, mode, true_rtx, mode);
      case LT:
      case LE:
 return
   simplify_gen_unary (NEG, mode,
         simplify_gen_unary (ABS, mode, true_rtx, mode),
         mode);
      default:
 break;
      }



  if ((! ((mode_class[(int) (mode)]) == MODE_FLOAT || (mode_class[(int) (mode)]) == MODE_COMPLEX_FLOAT || (mode_class[(int) (mode)]) == MODE_VECTOR_FLOAT) || flag_unsafe_math_optimizations)
      && comparison_p
      && rtx_equal_p ((((cond)->fld[0]).rtx), true_rtx)
      && rtx_equal_p ((((cond)->fld[1]).rtx), false_rtx)
      && ! side_effects_p (cond))
    switch (true_code)
      {
      case GE:
      case GT:
 return gen_binary (SMAX, mode, true_rtx, false_rtx);
      case LE:
      case LT:
 return gen_binary (SMIN, mode, true_rtx, false_rtx);
      case GEU:
      case GTU:
 return gen_binary (UMAX, mode, true_rtx, false_rtx);
      case LEU:
      case LTU:
 return gen_binary (UMIN, mode, true_rtx, false_rtx);
      default:
 break;
      }
# 4830 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  if ((1 == 1 || 1 == -1)
      && comparison_p && mode != VOIDmode && ! side_effects_p (x))
    {
      rtx t = make_compound_operation (true_rtx, SET);
      rtx f = make_compound_operation (false_rtx, SET);
      rtx cond_op0 = (((cond)->fld[0]).rtx);
      rtx cond_op1 = (((cond)->fld[1]).rtx);
      enum rtx_code op = NIL, extend_op = NIL;
      enum machine_mode m = mode;
      rtx z = 0, c1 = (rtx) 0;

      if ((((enum rtx_code) (t)->code) == PLUS || ((enum rtx_code) (t)->code) == MINUS
    || ((enum rtx_code) (t)->code) == IOR || ((enum rtx_code) (t)->code) == XOR
    || ((enum rtx_code) (t)->code) == ASHIFT
    || ((enum rtx_code) (t)->code) == LSHIFTRT || ((enum rtx_code) (t)->code) == ASHIFTRT)
   && rtx_equal_p ((((t)->fld[0]).rtx), f))
 c1 = (((t)->fld[1]).rtx), op = ((enum rtx_code) (t)->code), z = f;



      else if ((((enum rtx_code) (t)->code) == PLUS || ((enum rtx_code) (t)->code) == IOR
  || ((enum rtx_code) (t)->code) == XOR)
        && rtx_equal_p ((((t)->fld[1]).rtx), f))
 c1 = (((t)->fld[0]).rtx), op = ((enum rtx_code) (t)->code), z = f;
      else if (((enum rtx_code) (t)->code) == SIGN_EXTEND
        && (((enum rtx_code) ((((t)->fld[0]).rtx))->code) == PLUS
     || ((enum rtx_code) ((((t)->fld[0]).rtx))->code) == MINUS
     || ((enum rtx_code) ((((t)->fld[0]).rtx))->code) == IOR
     || ((enum rtx_code) ((((t)->fld[0]).rtx))->code) == XOR
     || ((enum rtx_code) ((((t)->fld[0]).rtx))->code) == ASHIFT
     || ((enum rtx_code) ((((t)->fld[0]).rtx))->code) == LSHIFTRT
     || ((enum rtx_code) ((((t)->fld[0]).rtx))->code) == ASHIFTRT)
        && ((enum rtx_code) (((((((t)->fld[0]).rtx))->fld[0]).rtx))->code) == SUBREG
        && subreg_lowpart_p (((((((t)->fld[0]).rtx))->fld[0]).rtx))
        && rtx_equal_p ((((((((((t)->fld[0]).rtx))->fld[0]).rtx))->fld[0]).rtx), f)
        && (num_sign_bit_copies (f, ((enum machine_mode) (f)->mode))
     > ((mode_bitsize[(int) (mode)])
        - (mode_bitsize[(int) (((enum machine_mode) (((((((t)->fld[0]).rtx))->fld[0]).rtx))->mode))]))))
 {
   c1 = ((((((t)->fld[0]).rtx))->fld[1]).rtx); z = f; op = ((enum rtx_code) ((((t)->fld[0]).rtx))->code);
   extend_op = SIGN_EXTEND;
   m = ((enum machine_mode) ((((t)->fld[0]).rtx))->mode);
 }
      else if (((enum rtx_code) (t)->code) == SIGN_EXTEND
        && (((enum rtx_code) ((((t)->fld[0]).rtx))->code) == PLUS
     || ((enum rtx_code) ((((t)->fld[0]).rtx))->code) == IOR
     || ((enum rtx_code) ((((t)->fld[0]).rtx))->code) == XOR)
        && ((enum rtx_code) (((((((t)->fld[0]).rtx))->fld[1]).rtx))->code) == SUBREG
        && subreg_lowpart_p (((((((t)->fld[0]).rtx))->fld[1]).rtx))
        && rtx_equal_p ((((((((((t)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtx), f)
        && (num_sign_bit_copies (f, ((enum machine_mode) (f)->mode))
     > ((mode_bitsize[(int) (mode)])
        - (mode_bitsize[(int) (((enum machine_mode) (((((((t)->fld[0]).rtx))->fld[1]).rtx))->mode))]))))
 {
   c1 = ((((((t)->fld[0]).rtx))->fld[0]).rtx); z = f; op = ((enum rtx_code) ((((t)->fld[0]).rtx))->code);
   extend_op = SIGN_EXTEND;
   m = ((enum machine_mode) ((((t)->fld[0]).rtx))->mode);
 }
      else if (((enum rtx_code) (t)->code) == ZERO_EXTEND
        && (((enum rtx_code) ((((t)->fld[0]).rtx))->code) == PLUS
     || ((enum rtx_code) ((((t)->fld[0]).rtx))->code) == MINUS
     || ((enum rtx_code) ((((t)->fld[0]).rtx))->code) == IOR
     || ((enum rtx_code) ((((t)->fld[0]).rtx))->code) == XOR
     || ((enum rtx_code) ((((t)->fld[0]).rtx))->code) == ASHIFT
     || ((enum rtx_code) ((((t)->fld[0]).rtx))->code) == LSHIFTRT
     || ((enum rtx_code) ((((t)->fld[0]).rtx))->code) == ASHIFTRT)
        && ((enum rtx_code) (((((((t)->fld[0]).rtx))->fld[0]).rtx))->code) == SUBREG
        && (mode_bitsize[(int) (mode)]) <= (8 * 8)
        && subreg_lowpart_p (((((((t)->fld[0]).rtx))->fld[0]).rtx))
        && rtx_equal_p ((((((((((t)->fld[0]).rtx))->fld[0]).rtx))->fld[0]).rtx), f)
        && ((nonzero_bits (f, ((enum machine_mode) (f)->mode))
      & ~mode_mask_array[(int) (((enum machine_mode) (((((((t)->fld[0]).rtx))->fld[0]).rtx))->mode))])
     == 0))
 {
   c1 = ((((((t)->fld[0]).rtx))->fld[1]).rtx); z = f; op = ((enum rtx_code) ((((t)->fld[0]).rtx))->code);
   extend_op = ZERO_EXTEND;
   m = ((enum machine_mode) ((((t)->fld[0]).rtx))->mode);
 }
      else if (((enum rtx_code) (t)->code) == ZERO_EXTEND
        && (((enum rtx_code) ((((t)->fld[0]).rtx))->code) == PLUS
     || ((enum rtx_code) ((((t)->fld[0]).rtx))->code) == IOR
     || ((enum rtx_code) ((((t)->fld[0]).rtx))->code) == XOR)
        && ((enum rtx_code) (((((((t)->fld[0]).rtx))->fld[1]).rtx))->code) == SUBREG
        && (mode_bitsize[(int) (mode)]) <= (8 * 8)
        && subreg_lowpart_p (((((((t)->fld[0]).rtx))->fld[1]).rtx))
        && rtx_equal_p ((((((((((t)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtx), f)
        && ((nonzero_bits (f, ((enum machine_mode) (f)->mode))
      & ~mode_mask_array[(int) (((enum machine_mode) (((((((t)->fld[0]).rtx))->fld[1]).rtx))->mode))])
     == 0))
 {
   c1 = ((((((t)->fld[0]).rtx))->fld[0]).rtx); z = f; op = ((enum rtx_code) ((((t)->fld[0]).rtx))->code);
   extend_op = ZERO_EXTEND;
   m = ((enum machine_mode) ((((t)->fld[0]).rtx))->mode);
 }

      if (z)
 {
   temp = subst (gen_binary (true_code, m, cond_op0, cond_op1),
   (global_rtl[GR_PC]), (global_rtl[GR_PC]), 0, 0);
   temp = gen_binary (MULT, m, temp,
        gen_binary (MULT, m, c1, const_true_rtx));
   temp = subst (temp, (global_rtl[GR_PC]), (global_rtl[GR_PC]), 0, 0);
   temp = gen_binary (op, m, gen_lowpart_for_combine (m, z), temp);

   if (extend_op != NIL)
     temp = simplify_gen_unary (extend_op, mode, temp, m);

   return temp;
 }
    }






  if (true_code == NE && (((cond)->fld[1]).rtx) == (const_int_rtx[64])
      && false_rtx == (const_int_rtx[64]) && ((enum rtx_code) (true_rtx)->code) == CONST_INT
      && ((1 == nonzero_bits ((((cond)->fld[0]).rtx), mode)
    && (i = exact_log2_wide ((unsigned long long) ((((true_rtx)->fld[0]).rtwint)))) >= 0)
   || ((num_sign_bit_copies ((((cond)->fld[0]).rtx), mode)
        == (mode_bitsize[(int) (mode)]))
       && (i = exact_log2_wide ((unsigned long long) (-(((true_rtx)->fld[0]).rtwint)))) >= 0)))
    return
      simplify_shift_const ((rtx) 0, ASHIFT, mode,
       gen_lowpart_for_combine (mode, (((cond)->fld[0]).rtx)), i);

  return x;
}



static rtx
simplify_set (x)
     rtx x;
{
  rtx src = (((x)->fld[1]).rtx);
  rtx dest = (((x)->fld[0]).rtx);
  enum machine_mode mode
    = ((enum machine_mode) (src)->mode) != VOIDmode ? ((enum machine_mode) (src)->mode) : ((enum machine_mode) (dest)->mode);
  rtx other_insn;
  rtx *cc_use;


  if (((enum rtx_code) (dest)->code) == PC && ((enum rtx_code) (src)->code) == RETURN)
    return src;





  if ((mode_class[(int) (mode)]) == MODE_INT)
    {
      src = force_to_mode (src, mode, ~(long long) 0, (rtx) 0, 0);
      do_SUBST(&((((x)->fld[1]).rtx)), (src));
    }




  if ((((enum rtx_code) (src)->code) == COMPARE



       )
      && (cc_use = find_single_use (dest, subst_insn, &other_insn)) != 0
      && (undobuf.other_insn == 0 || other_insn == undobuf.other_insn)
      && (rtx_class[(int) (((enum rtx_code) (*cc_use)->code))]) == '<'
      && rtx_equal_p ((((*cc_use)->fld[0]).rtx), dest))
    {
      enum rtx_code old_code = ((enum rtx_code) (*cc_use)->code);
      enum rtx_code new_code;
      rtx op0, op1;
      int other_changed = 0;
      enum machine_mode compare_mode = ((enum machine_mode) (dest)->mode);

      if (((enum rtx_code) (src)->code) == COMPARE)
 op0 = (((src)->fld[0]).rtx), op1 = (((src)->fld[1]).rtx);
      else
 op0 = src, op1 = (const_int_rtx[64]);


      new_code = simplify_comparison (old_code, &op0, &op1);




      compare_mode = ix86_cc_mode ((new_code), (op0), (op1));
# 5026 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
      if (compare_mode != ((enum machine_mode) (dest)->mode))
 {
   unsigned int regno = (((dest)->fld[0]).rtuint);
   rtx new_dest = gen_rtx_REG (compare_mode, regno);

   if (regno < 53
       || ((((reg_n_info)->data.reg[regno])->sets) == 1 && ! ((dest)->volatil)))
     {
       if (regno >= 53)
  do_SUBST(&((cfun->emit->x_regno_reg_rtx)[regno]), (new_dest));

       do_SUBST(&((((x)->fld[0]).rtx)), (new_dest));
       do_SUBST(&((((*cc_use)->fld[0]).rtx)), (new_dest));
       other_changed = 1;

       dest = new_dest;
     }
 }




      if (new_code != old_code)
 {
   unsigned long long mask;

   do_SUBST(&(*cc_use), (gen_rtx_fmt_ee (new_code, ((enum machine_mode) (*cc_use)->mode), dest, (const_int_rtx[64]))));
# 5061 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
   if (((old_code == NE && new_code == EQ)
        || (old_code == EQ && new_code == NE))
       && ! other_changed && op1 == (const_int_rtx[64])
       && (mode_bitsize[(int) (((enum machine_mode) (op0)->mode))]) <= (8 * 8)
       && exact_log2_wide ((unsigned long long) (mask = nonzero_bits (op0, ((enum machine_mode) (op0)->mode)))) >= 0)
     {
       rtx pat = (((other_insn)->fld[3]).rtx), note = 0;

       if ((recog_for_combine (&pat, other_insn, &note) < 0
     && ! check_asm_operands (pat)))
  {
    ((*cc_use)->code = (enum rtx_code) (old_code));
    other_insn = 0;

    op0 = gen_binary (XOR, ((enum machine_mode) (op0)->mode), op0, gen_rtx_CONST_INT (VOIDmode, (long long) (mask)));
  }
     }

   other_changed = 1;
 }

      if (other_changed)
 undobuf.other_insn = other_insn;
# 5098 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
      if (((enum rtx_code) (src)->code) != COMPARE || ((enum machine_mode) (src)->mode) != compare_mode)
 {
   do_SUBST(&((((x)->fld[1]).rtx)), (gen_rtx_fmt_ee (COMPARE, (compare_mode), (op0), (op1))));
   src = (((x)->fld[1]).rtx);
 }
      else
 {

   do_SUBST(&((((src)->fld[0]).rtx)), (op0));
   do_SUBST(&((((src)->fld[1]).rtx)), (op1));
 }
    }
  else
    {


      src = make_compound_operation (src, SET);
      do_SUBST(&((((x)->fld[1]).rtx)), (src));
    }
# 5130 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  if (((enum rtx_code) (src)->code) == SUBREG && subreg_lowpart_p (src)
      && (rtx_class[(int) (((enum rtx_code) ((((src)->fld[0]).rtx))->code))]) != 'o'
      && ((((mode_size[(int) (((enum machine_mode) (src)->mode))]) + (((target_flags & 0x02000000) ? 8 : 4) - 1))
    / ((target_flags & 0x02000000) ? 8 : 4))
   == (((mode_size[(int) (((enum machine_mode) ((((src)->fld[0]).rtx))->mode))])
        + (((target_flags & 0x02000000) ? 8 : 4) - 1)) / ((target_flags & 0x02000000) ? 8 : 4)))

      && ((mode_size[(int) (((enum machine_mode) (src)->mode))])
   < (mode_size[(int) (((enum machine_mode) ((((src)->fld[0]).rtx))->mode))]))
# 5148 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
      && (((enum rtx_code) (dest)->code) == REG
   || (((enum rtx_code) (dest)->code) == SUBREG
       && ((enum rtx_code) ((((dest)->fld[0]).rtx))->code) == REG)))
    {
      do_SUBST(&((((x)->fld[0]).rtx)), (gen_lowpart_for_combine (((enum machine_mode) ((((src)->fld[0]).rtx))->mode), dest)));


      do_SUBST(&((((x)->fld[1]).rtx)), ((((src)->fld[0]).rtx)));

      src = (((x)->fld[1]).rtx), dest = (((x)->fld[0]).rtx);
    }
# 5186 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  if (((enum rtx_code) (dest)->code) != PC
      && ((enum rtx_code) (src)->code) == IF_THEN_ELSE
      && (mode_class[(int) (((enum machine_mode) (src)->mode))]) == MODE_INT
      && (((enum rtx_code) ((((src)->fld[0]).rtx))->code) == EQ || ((enum rtx_code) ((((src)->fld[0]).rtx))->code) == NE)
      && ((((((src)->fld[0]).rtx))->fld[1]).rtx) == (const_int_rtx[64])
      && ((enum machine_mode) (src)->mode) == ((enum machine_mode) (((((((src)->fld[0]).rtx))->fld[0]).rtx))->mode)

      && ! can_conditionally_move_p (((enum machine_mode) (src)->mode))

      && (num_sign_bit_copies (((((((src)->fld[0]).rtx))->fld[0]).rtx),
          ((enum machine_mode) (((((((src)->fld[0]).rtx))->fld[0]).rtx))->mode))
   == (mode_bitsize[(int) (((enum machine_mode) (((((((src)->fld[0]).rtx))->fld[0]).rtx))->mode))]))
      && ! side_effects_p (src))
    {
      rtx true_rtx = (((enum rtx_code) ((((src)->fld[0]).rtx))->code) == NE
        ? (((src)->fld[1]).rtx) : (((src)->fld[2]).rtx));
      rtx false_rtx = (((enum rtx_code) ((((src)->fld[0]).rtx))->code) == NE
     ? (((src)->fld[2]).rtx) : (((src)->fld[1]).rtx));
      rtx term1 = (const_int_rtx[64]), term2, term3;

      if (((enum rtx_code) (true_rtx)->code) == IOR
   && rtx_equal_p ((((true_rtx)->fld[0]).rtx), false_rtx))
 term1 = false_rtx, true_rtx = (((true_rtx)->fld[1]).rtx), false_rtx = (const_int_rtx[64]);
      else if (((enum rtx_code) (true_rtx)->code) == IOR
        && rtx_equal_p ((((true_rtx)->fld[1]).rtx), false_rtx))
 term1 = false_rtx, true_rtx = (((true_rtx)->fld[0]).rtx), false_rtx = (const_int_rtx[64]);
      else if (((enum rtx_code) (false_rtx)->code) == IOR
        && rtx_equal_p ((((false_rtx)->fld[0]).rtx), true_rtx))
 term1 = true_rtx, false_rtx = (((false_rtx)->fld[1]).rtx), true_rtx = (const_int_rtx[64]);
      else if (((enum rtx_code) (false_rtx)->code) == IOR
        && rtx_equal_p ((((false_rtx)->fld[1]).rtx), true_rtx))
 term1 = true_rtx, false_rtx = (((false_rtx)->fld[0]).rtx), true_rtx = (const_int_rtx[64]);

      term2 = gen_binary (AND, ((enum machine_mode) (src)->mode),
     ((((((src)->fld[0]).rtx))->fld[0]).rtx), true_rtx);
      term3 = gen_binary (AND, ((enum machine_mode) (src)->mode),
     simplify_gen_unary (NOT, ((enum machine_mode) (src)->mode),
           ((((((src)->fld[0]).rtx))->fld[0]).rtx),
           ((enum machine_mode) (src)->mode)),
     false_rtx);

      do_SUBST(&((((x)->fld[1]).rtx)), (gen_binary (IOR, ((enum machine_mode) (src)->mode), gen_binary (IOR, ((enum machine_mode) (src)->mode), term1, term2), term3)));




      src = (((x)->fld[1]).rtx);
    }



  if (((enum rtx_code) (src)->code) == CLOBBER && (((src)->fld[0]).rtx) == (const_int_rtx[64]))
    return src;
  else if (((enum rtx_code) (dest)->code) == CLOBBER && (((dest)->fld[0]).rtx) == (const_int_rtx[64]))
    return dest;
  else

    return make_field_assignment (x);
}




static rtx
simplify_logical (x, last)
     rtx x;
     int last;
{
  enum machine_mode mode = ((enum machine_mode) (x)->mode);
  rtx op0 = (((x)->fld[0]).rtx);
  rtx op1 = (((x)->fld[1]).rtx);
  rtx reversed;

  switch (((enum rtx_code) (x)->code))
    {
    case AND:


      if (((enum rtx_code) (op0)->code) == XOR
   && rtx_equal_p ((((op0)->fld[0]).rtx), op1)
   && ! side_effects_p (op1))
 x = gen_binary (AND, mode,
   simplify_gen_unary (NOT, mode, (((op0)->fld[1]).rtx), mode),
   op1);

      if (((enum rtx_code) (op0)->code) == XOR
   && rtx_equal_p ((((op0)->fld[1]).rtx), op1)
   && ! side_effects_p (op1))
 x = gen_binary (AND, mode,
   simplify_gen_unary (NOT, mode, (((op0)->fld[0]).rtx), mode),
   op1);


      if (((enum rtx_code) (op0)->code) == NOT
   && ((enum rtx_code) ((((op0)->fld[0]).rtx))->code) == XOR
   && rtx_equal_p (((((((op0)->fld[0]).rtx))->fld[0]).rtx), op1)
   && ! side_effects_p (op1))
 x = gen_binary (AND, mode, ((((((op0)->fld[0]).rtx))->fld[1]).rtx), op1);

      if (((enum rtx_code) (op0)->code) == NOT
   && ((enum rtx_code) ((((op0)->fld[0]).rtx))->code) == XOR
   && rtx_equal_p (((((((op0)->fld[0]).rtx))->fld[1]).rtx), op1)
   && ! side_effects_p (op1))
 x = gen_binary (AND, mode, ((((((op0)->fld[0]).rtx))->fld[0]).rtx), op1);




      if (((enum rtx_code) (op1)->code) == CONST_INT
   && ((mode_bitsize[(int) (mode)]) <= (8 * 8)
       || (((op1)->fld[0]).rtwint) > 0))
 {
   x = simplify_and_const_int (x, mode, op0, (((op1)->fld[0]).rtwint));




   if (last
       && ((enum rtx_code) (x)->code) == IOR && ((enum rtx_code) (op0)->code) == AND
       && ((enum rtx_code) ((((op0)->fld[1]).rtx))->code) == CONST_INT
       && ((enum rtx_code) (op1)->code) == CONST_INT)
     return gen_binary (IOR, mode,
          gen_binary (AND, mode, (((op0)->fld[0]).rtx),
        gen_rtx_CONST_INT (VOIDmode, (long long) (((((((op0)->fld[1]).rtx))->fld[0]).rtwint) & ~(((op1)->fld[0]).rtwint)))), op1);


   if (((enum rtx_code) (x)->code) != AND)
     return x;

   if ((rtx_class[(int) (((enum rtx_code) (x)->code))]) == 'c'
       || (rtx_class[(int) (((enum rtx_code) (x)->code))]) == '2')
     op0 = (((x)->fld[0]).rtx), op1 = (((x)->fld[1]).rtx);
 }


      if (((enum rtx_code) (op0)->code) == IOR
   && (rtx_equal_p ((((op0)->fld[0]).rtx), op1)
       || rtx_equal_p ((((op0)->fld[1]).rtx), op1))
   && ! side_effects_p ((((op0)->fld[0]).rtx))
   && ! side_effects_p ((((op0)->fld[1]).rtx)))
 return op1;
# 5343 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
      if (((enum rtx_code) (op0)->code) == IOR || ((enum rtx_code) (op0)->code) == XOR)
 {
   x = apply_distributive_law
     (gen_binary (((enum rtx_code) (op0)->code), mode,
    gen_binary (AND, mode, (((op0)->fld[0]).rtx), op1),
    gen_binary (AND, mode, (((op0)->fld[1]).rtx),
         copy_rtx (op1))));
   if (((enum rtx_code) (x)->code) != AND)
     return x;
 }

      if (((enum rtx_code) (op1)->code) == IOR || ((enum rtx_code) (op1)->code) == XOR)
 return apply_distributive_law
   (gen_binary (((enum rtx_code) (op1)->code), mode,
         gen_binary (AND, mode, (((op1)->fld[0]).rtx), op0),
         gen_binary (AND, mode, (((op1)->fld[1]).rtx),
       copy_rtx (op0))));




      if (((enum rtx_code) (op0)->code) == NOT && ((enum rtx_code) (op1)->code) == XOR)
 return apply_distributive_law
   (gen_binary (XOR, mode,
         gen_binary (IOR, mode, (((op0)->fld[0]).rtx), (((op1)->fld[0]).rtx)),
         gen_binary (IOR, mode, copy_rtx ((((op0)->fld[0]).rtx)),
       (((op1)->fld[1]).rtx))));

      else if (((enum rtx_code) (op1)->code) == NOT && ((enum rtx_code) (op0)->code) == XOR)
 return apply_distributive_law
   (gen_binary (XOR, mode,
         gen_binary (IOR, mode, (((op1)->fld[0]).rtx), (((op0)->fld[0]).rtx)),
         gen_binary (IOR, mode, copy_rtx ((((op1)->fld[0]).rtx)), (((op0)->fld[1]).rtx))));
      break;

    case IOR:

      if (((enum rtx_code) (op1)->code) == CONST_INT
   && (mode_bitsize[(int) (mode)]) <= (8 * 8)
   && (nonzero_bits (op0, mode) & ~(((op1)->fld[0]).rtwint)) == 0)
 return op1;


      if (((enum rtx_code) (op0)->code) == AND
   && (rtx_equal_p ((((op0)->fld[0]).rtx), op1)
       || rtx_equal_p ((((op0)->fld[1]).rtx), op1))
   && ! side_effects_p ((((op0)->fld[0]).rtx))
   && ! side_effects_p ((((op0)->fld[1]).rtx)))
 return op1;




      if (((enum rtx_code) (op0)->code) == AND)
 {
   x = apply_distributive_law
     (gen_binary (AND, mode,
    gen_binary (IOR, mode, (((op0)->fld[0]).rtx), op1),
    gen_binary (IOR, mode, (((op0)->fld[1]).rtx),
         copy_rtx (op1))));

   if (((enum rtx_code) (x)->code) != IOR)
     return x;
 }

      if (((enum rtx_code) (op1)->code) == AND)
 {
   x = apply_distributive_law
     (gen_binary (AND, mode,
    gen_binary (IOR, mode, (((op1)->fld[0]).rtx), op0),
    gen_binary (IOR, mode, (((op1)->fld[1]).rtx),
         copy_rtx (op0))));

   if (((enum rtx_code) (x)->code) != IOR)
     return x;
 }




      if (((((enum rtx_code) (op0)->code) == ASHIFT && ((enum rtx_code) (op1)->code) == LSHIFTRT)
    || (((enum rtx_code) (op1)->code) == ASHIFT && ((enum rtx_code) (op0)->code) == LSHIFTRT))
   && rtx_equal_p ((((op0)->fld[0]).rtx), (((op1)->fld[0]).rtx))
   && ((enum rtx_code) ((((op0)->fld[1]).rtx))->code) == CONST_INT
   && ((enum rtx_code) ((((op1)->fld[1]).rtx))->code) == CONST_INT
   && (((((((op0)->fld[1]).rtx))->fld[0]).rtwint) + ((((((op1)->fld[1]).rtx))->fld[0]).rtwint)
       == (mode_bitsize[(int) (mode)])))
 return gen_rtx_fmt_ee (ROTATE, (mode), ((((op0)->fld[0]).rtx)), ((((enum rtx_code) (op0)->code) == ASHIFT ? (((op0)->fld[1]).rtx) : (((op1)->fld[1]).rtx))));
# 5439 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
      if (((enum rtx_code) (op1)->code) == CONST_INT && ((enum rtx_code) (op0)->code) == ASHIFTRT
   && ((enum rtx_code) ((((op0)->fld[0]).rtx))->code) == PLUS
   && ((enum rtx_code) (((((((op0)->fld[0]).rtx))->fld[1]).rtx))->code) == CONST_INT
   && ((enum rtx_code) ((((op0)->fld[1]).rtx))->code) == CONST_INT
   && ((((((op0)->fld[1]).rtx))->fld[0]).rtwint) < (8 * 8))
 {
   int count = ((((((op0)->fld[1]).rtx))->fld[0]).rtwint);
   long long mask = (((op1)->fld[0]).rtwint) << count;

   if (mask >> count == (((op1)->fld[0]).rtwint)
       && (mask & nonzero_bits ((((op0)->fld[0]).rtx), mode)) == 0)
     {
       do_SUBST(&(((((((op0)->fld[0]).rtx))->fld[1]).rtx)), (gen_rtx_CONST_INT (VOIDmode, (long long) ((((((((((op0)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint) | mask))));

       return op0;
     }
 }
      break;

    case XOR:




      if ((mode_bitsize[(int) (mode)]) <= (8 * 8)
   && (nonzero_bits (op0, mode)
       & nonzero_bits (op1, mode)) == 0)
 return (gen_binary (IOR, mode, op0, op1));




      {
 int num_negated = 0;

 if (((enum rtx_code) (op0)->code) == NOT)
   num_negated++, op0 = (((op0)->fld[0]).rtx);
 if (((enum rtx_code) (op1)->code) == NOT)
   num_negated++, op1 = (((op1)->fld[0]).rtx);

 if (num_negated == 2)
   {
     do_SUBST(&((((x)->fld[0]).rtx)), (op0));
     do_SUBST(&((((x)->fld[1]).rtx)), (op1));
   }
 else if (num_negated == 1)
   return
     simplify_gen_unary (NOT, mode, gen_binary (XOR, mode, op0, op1),
    mode);
      }





      if (((enum rtx_code) (op0)->code) == AND
   && rtx_equal_p ((((op0)->fld[1]).rtx), op1)
   && ! side_effects_p (op1))
 return gen_binary (AND, mode,
      simplify_gen_unary (NOT, mode, (((op0)->fld[0]).rtx), mode),
      op1);

      else if (((enum rtx_code) (op0)->code) == AND
        && rtx_equal_p ((((op0)->fld[0]).rtx), op1)
        && ! side_effects_p (op1))
 return gen_binary (AND, mode,
      simplify_gen_unary (NOT, mode, (((op0)->fld[1]).rtx), mode),
      op1);



      if (1 == 1
   && op1 == (const_int_rtx[64 +1])
   && (rtx_class[(int) (((enum rtx_code) (op0)->code))]) == '<'
   && (reversed = reversed_comparison (op0, mode, (((op0)->fld[0]).rtx),
           (((op0)->fld[1]).rtx))))
 return reversed;





      if (1 == 1
   && op1 == (const_int_rtx[64 +1])
   && ((enum rtx_code) (op0)->code) == LSHIFTRT
   && ((enum rtx_code) ((((op0)->fld[1]).rtx))->code) == CONST_INT
   && ((((((op0)->fld[1]).rtx))->fld[0]).rtwint) == (mode_bitsize[(int) (mode)]) - 1)
 return gen_rtx_fmt_ee (GE, (mode), ((((op0)->fld[0]).rtx)), ((const_int_rtx[64])));



      if ((mode_bitsize[(int) (mode)]) <= (8 * 8)
   && ((1 & mode_mask_array[(int) (mode)])
       == (unsigned long long) 1 << ((mode_bitsize[(int) (mode)]) - 1))
   && op1 == const_true_rtx
   && (rtx_class[(int) (((enum rtx_code) (op0)->code))]) == '<'
   && (reversed = reversed_comparison (op0, mode, (((op0)->fld[0]).rtx),
           (((op0)->fld[1]).rtx))))
 return reversed;

      break;

    default:
      fancy_abort ("/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c", 5542, __FUNCTION__);
    }

  return x;
}
# 5561 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static rtx
expand_compound_operation (x)
     rtx x;
{
  unsigned long long pos = 0, len;
  int unsignedp = 0;
  unsigned int modewidth;
  rtx tem;

  switch (((enum rtx_code) (x)->code))
    {
    case ZERO_EXTEND:
      unsignedp = 1;
    case SIGN_EXTEND:
# 5583 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == CONST_INT)
 return x;
# 5595 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
      if ((mode_size[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))]) > ((target_flags & 0x02000000) ? 8 : 4))
 return x;

      len = (mode_bitsize[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))]);



      if (len == 0)
 return x;

      break;

    case ZERO_EXTRACT:
      unsignedp = 1;
    case SIGN_EXTRACT:

      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == CLOBBER)
 return (((x)->fld[0]).rtx);

      if (((enum rtx_code) ((((x)->fld[1]).rtx))->code) != CONST_INT
   || ((enum rtx_code) ((((x)->fld[2]).rtx))->code) != CONST_INT
   || ((enum machine_mode) ((((x)->fld[0]).rtx))->mode) == VOIDmode)
 return x;

      len = ((((((x)->fld[1]).rtx))->fld[0]).rtwint);
      pos = ((((((x)->fld[2]).rtx))->fld[0]).rtwint);




      if (len + pos > (mode_bitsize[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))]))
 do_SUBST(&((((x)->fld[0]).rtx)), (gen_rtx_fmt_e (USE, (((enum machine_mode) (x)->mode)), ((((x)->fld[0]).rtx)))));

      if (0)
 pos = (mode_bitsize[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))]) - len - pos;

      break;

    default:
      return x;
    }



  if (((enum rtx_code) (x)->code) == SIGN_EXTEND
      && ((mode_bitsize[(int) (((enum machine_mode) (x)->mode))]) <= (8 * 8)
   && ((nonzero_bits ((((x)->fld[0]).rtx), ((enum machine_mode) ((((x)->fld[0]).rtx))->mode))
  & ~(((unsigned long long)
        mode_mask_array[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))])
       >> 1))
        == 0)))
    {
      rtx temp = gen_rtx_fmt_e (ZERO_EXTEND, (((enum machine_mode) (x)->mode)), ((((x)->fld[0]).rtx)));
      return expand_compound_operation (temp);
    }


  if (((enum rtx_code) (x)->code) == ZERO_EXTEND)
    {



      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == TRUNCATE
   && ((enum machine_mode) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->mode) == ((enum machine_mode) (x)->mode)
   && (mode_bitsize[(int) (((enum machine_mode) (x)->mode))]) <= (8 * 8)
   && (nonzero_bits (((((((x)->fld[0]).rtx))->fld[0]).rtx), ((enum machine_mode) (x)->mode))
       & ~mode_mask_array[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))]) == 0)
 return ((((((x)->fld[0]).rtx))->fld[0]).rtx);


      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == SUBREG
   && ((enum machine_mode) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->mode) == ((enum machine_mode) (x)->mode)
   && subreg_lowpart_p ((((x)->fld[0]).rtx))
   && (mode_bitsize[(int) (((enum machine_mode) (x)->mode))]) <= (8 * 8)
   && (nonzero_bits (((((((x)->fld[0]).rtx))->fld[0]).rtx), ((enum machine_mode) (x)->mode))
       & ~mode_mask_array[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))]) == 0)
 return ((((((x)->fld[0]).rtx))->fld[0]).rtx);





      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == TRUNCATE
   && ((enum machine_mode) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->mode) == ((enum machine_mode) (x)->mode)
   && (rtx_class[(int) (((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->code))]) == '<'
   && ((mode_bitsize[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))])
       <= (8 * 8))
   && ((long long) 1
       & ~mode_mask_array[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))]) == 0)
 return ((((((x)->fld[0]).rtx))->fld[0]).rtx);


      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == SUBREG
   && ((enum machine_mode) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->mode) == ((enum machine_mode) (x)->mode)
   && subreg_lowpart_p ((((x)->fld[0]).rtx))
   && (rtx_class[(int) (((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->code))]) == '<'
   && ((mode_bitsize[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))])
       <= (8 * 8))
   && ((long long) 1
       & ~mode_mask_array[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))]) == 0)
 return ((((((x)->fld[0]).rtx))->fld[0]).rtx);

    }
# 5714 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  modewidth = (mode_bitsize[(int) (((enum machine_mode) (x)->mode))]);
  if (modewidth + len >= pos)
    tem = simplify_shift_const ((rtx) 0, unsignedp ? LSHIFTRT : ASHIFTRT,
    ((enum machine_mode) (x)->mode),
    simplify_shift_const ((rtx) 0, ASHIFT,
            ((enum machine_mode) (x)->mode),
            (((x)->fld[0]).rtx),
            modewidth - pos - len),
    modewidth - len);

  else if (unsignedp && len < (8 * 8))
    tem = simplify_and_const_int ((rtx) 0, ((enum machine_mode) (x)->mode),
      simplify_shift_const ((rtx) 0, LSHIFTRT,
       ((enum machine_mode) (x)->mode),
       (((x)->fld[0]).rtx), pos),
      ((long long) 1 << len) - 1);
  else

    return x;



  if (((enum rtx_code) (tem)->code) == CLOBBER)
    return x;

  return tem;
}
# 5750 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static rtx
expand_field_assignment (x)
     rtx x;
{
  rtx inner;
  rtx pos;
  int len;
  rtx mask;
  enum machine_mode compute_mode;


  while (1)
    {
      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == STRICT_LOW_PART
   && ((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->code) == SUBREG)
 {
   inner = (((((((((x)->fld[0]).rtx))->fld[0]).rtx))->fld[0]).rtx);
   len = (mode_bitsize[(int) (((enum machine_mode) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->mode))]);
   pos = gen_rtx_CONST_INT (VOIDmode, (long long) (subreg_lsb (((((((x)->fld[0]).rtx))->fld[0]).rtx))));
 }
      else if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == ZERO_EXTRACT
        && ((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[1]).rtx))->code) == CONST_INT)
 {
   inner = ((((((x)->fld[0]).rtx))->fld[0]).rtx);
   len = (((((((((x)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint);
   pos = ((((((x)->fld[0]).rtx))->fld[2]).rtx);



   if (((enum rtx_code) (pos)->code) == CONST_INT
       && (((pos)->fld[0]).rtwint) + len > (mode_bitsize[(int) (((enum machine_mode) (inner)->mode))]))
     inner = gen_rtx_fmt_e (USE, (((enum machine_mode) ((((x)->fld[0]).rtx))->mode)), (inner));

   if (0)
     {
       if (((enum rtx_code) (pos)->code) == CONST_INT)
  pos = gen_rtx_CONST_INT (VOIDmode, (long long) ((mode_bitsize[(int) (((enum machine_mode) (inner)->mode))]) - len - (((pos)->fld[0]).rtwint)));

       else if (((enum rtx_code) (pos)->code) == MINUS
         && ((enum rtx_code) ((((pos)->fld[1]).rtx))->code) == CONST_INT
         && (((((((pos)->fld[1]).rtx))->fld[0]).rtwint)
      == (mode_bitsize[(int) (((enum machine_mode) (inner)->mode))]) - len))

  pos = (((pos)->fld[0]).rtx);
       else
  pos = gen_binary (MINUS, ((enum machine_mode) (pos)->mode),
      gen_rtx_CONST_INT (VOIDmode, (long long) ((mode_bitsize[(int) (((enum machine_mode) (inner)->mode))]) - len)),

      pos);
     }
 }



      else if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == SUBREG

        && nonzero_sign_valid
        && ((((mode_size[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))])
       + (((target_flags & 0x02000000) ? 8 : 4) - 1)) / ((target_flags & 0x02000000) ? 8 : 4))
     == (((mode_size[(int) (((enum machine_mode) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->mode))])
   + (((target_flags & 0x02000000) ? 8 : 4) - 1)) / ((target_flags & 0x02000000) ? 8 : 4))))
 {
   x = gen_rtx_fmt_ee (SET, (VOIDmode), (((((((x)->fld[0]).rtx))->fld[0]).rtx)), (gen_lowpart_for_combine (((enum machine_mode) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->mode), (((x)->fld[1]).rtx))));



   continue;
 }
      else
 break;

      while (((enum rtx_code) (inner)->code) == SUBREG && subreg_lowpart_p (inner))
 inner = (((inner)->fld[0]).rtx);

      compute_mode = ((enum machine_mode) (inner)->mode);


      if (! ((mode_class[(int) (compute_mode)]) == MODE_INT || (mode_class[(int) (compute_mode)]) == MODE_PARTIAL_INT || (mode_class[(int) (compute_mode)]) == MODE_COMPLEX_INT || (mode_class[(int) (compute_mode)]) == MODE_VECTOR_INT))
 {
   enum machine_mode imode;


   if (! ((mode_class[(int) (compute_mode)]) == MODE_FLOAT || (mode_class[(int) (compute_mode)]) == MODE_COMPLEX_FLOAT || (mode_class[(int) (compute_mode)]) == MODE_VECTOR_FLOAT))
     break;


   imode = mode_for_size ((mode_bitsize[(int) (compute_mode)]), MODE_INT, 0);
   if (imode == BLKmode)
     break;

   compute_mode = imode;
   inner = gen_lowpart_for_combine (imode, inner);
 }


      if (len < (8 * 8))
 mask = gen_rtx_CONST_INT (VOIDmode, (long long) (((long long) 1 << len) - 1));
      else
 break;




      x = gen_rtx_fmt_ee (SET, (VOIDmode), (copy_rtx (inner)), (gen_binary (IOR, compute_mode, gen_binary (AND, compute_mode, simplify_gen_unary (NOT, compute_mode, gen_binary (ASHIFT, compute_mode, mask, pos), compute_mode), inner), gen_binary (ASHIFT, compute_mode, gen_binary (AND, compute_mode, gen_lowpart_for_combine (compute_mode, (((x)->fld[1]).rtx)), mask), pos))));
# 5869 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
    }

  return x;
}
# 5901 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static rtx
make_extraction (mode, inner, pos, pos_rtx, len,
   unsignedp, in_dest, in_compare)
     enum machine_mode mode;
     rtx inner;
     long long pos;
     rtx pos_rtx;
     unsigned long long len;
     int unsignedp;
     int in_dest, in_compare;
{



  enum machine_mode is_mode = ((enum machine_mode) (inner)->mode);
  enum machine_mode inner_mode;
  enum machine_mode wanted_inner_mode = byte_mode;
  enum machine_mode wanted_inner_reg_mode = word_mode;
  enum machine_mode pos_mode = word_mode;
  enum machine_mode extraction_mode = word_mode;
  enum machine_mode tmode = mode_for_size (len, MODE_INT, 1);
  int spans_byte = 0;
  rtx new = 0;
  rtx orig_pos_rtx = pos_rtx;
  long long orig_pos;


  if (((enum rtx_code) (inner)->code) == USE)



    spans_byte = 1, inner = (((inner)->fld[0]).rtx);
  else if (((enum rtx_code) (inner)->code) == SUBREG && subreg_lowpart_p (inner))
    {





      if (((enum rtx_code) ((((inner)->fld[0]).rtx))->code) == MEM)
 is_mode = ((enum machine_mode) ((((inner)->fld[0]).rtx))->mode);
      inner = (((inner)->fld[0]).rtx);
    }

  inner_mode = ((enum machine_mode) (inner)->mode);

  if (pos_rtx && ((enum rtx_code) (pos_rtx)->code) == CONST_INT)
    pos = (((pos_rtx)->fld[0]).rtwint), pos_rtx = 0;
# 5963 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  if (tmode != BLKmode
      && ! (spans_byte && inner_mode != tmode)
      && ((pos_rtx == 0 && (pos % ((target_flags & 0x02000000) ? 64 : 32)) == 0
    && ((enum rtx_code) (inner)->code) != MEM
    && (! in_dest
        || (((enum rtx_code) (inner)->code) == REG
     && have_insn_for (STRICT_LOW_PART, tmode))))
   || (((enum rtx_code) (inner)->code) == MEM && pos_rtx == 0
       && (pos
    % (0 ? get_mode_alignment (tmode)
       : 8)) == 0


       && (mode_bitsize[(int) (inner_mode)]) >= (mode_bitsize[(int) (tmode)])
       && (inner_mode == tmode
    || (! mode_dependent_address_p ((((inner)->fld[0]).rtx))
        && ! ((inner)->volatil))))))
    {







      if (((enum rtx_code) (inner)->code) == MEM)
 {
   long long offset;


   if (0)
     offset = ((mode_bitsize[(int) (is_mode)]) - len - pos) / 8;
   else
     offset = pos / 8;

   new = adjust_address_1 (inner, tmode, offset, 0, 1);
 }
      else if (((enum rtx_code) (inner)->code) == REG)
 {


   if (tmode != inner_mode)
     {
       long long final_word = pos / ((target_flags & 0x02000000) ? 64 : 32);

       if (0
    && (mode_size[(int) (inner_mode)]) > ((target_flags & 0x02000000) ? 8 : 4))
  final_word = (((mode_size[(int) (inner_mode)])
          - (mode_size[(int) (tmode)]))
         / ((target_flags & 0x02000000) ? 8 : 4)) - final_word;

       final_word *= ((target_flags & 0x02000000) ? 8 : 4);
       if (0 &&
    (mode_size[(int) (inner_mode)]) > (mode_size[(int) (tmode)]))
  final_word += ((mode_size[(int) (inner_mode)])
          - (mode_size[(int) (tmode)])) % ((target_flags & 0x02000000) ? 8 : 4);

       new = gen_rtx_SUBREG (tmode, inner, final_word);
     }
   else
     new = inner;
 }
      else
 new = force_to_mode (inner, tmode,
        len >= (8 * 8)
        ? ~(unsigned long long) 0
        : ((unsigned long long) 1 << len) - 1,
        (rtx) 0, 0);




      if (in_dest)
 return (((enum rtx_code) (new)->code) == MEM ? new
  : (((enum rtx_code) (new)->code) != SUBREG
     ? gen_rtx_fmt_e (CLOBBER, (tmode), ((const_int_rtx[64])))
     : gen_rtx_fmt_e (STRICT_LOW_PART, (VOIDmode), (new))));

      if (mode == tmode)
 return new;

      if (((enum rtx_code) (new)->code) == CONST_INT)
 return gen_rtx_CONST_INT (VOIDmode, (long long) (trunc_int_for_mode ((((new)->fld[0]).rtwint), mode)));




      if (flag_expensive_optimizations
   && ((mode_bitsize[(int) (tmode)]) <= (8 * 8)
       && ((nonzero_bits (new, tmode)
     & ~(((unsigned long long)
   mode_mask_array[(int) (tmode)])
         >> 1))
    == 0)))
 {
   rtx temp = gen_rtx_fmt_e (ZERO_EXTEND, (mode), (new));
   rtx temp1 = gen_rtx_fmt_e (SIGN_EXTEND, (mode), (new));



   if (rtx_cost (temp, SET) <= rtx_cost (temp1, SET))
     return temp;
   return temp1;
 }




      return (gen_rtx_fmt_e (unsignedp ? ZERO_EXTEND : SIGN_EXTEND,
        mode, new));
    }




  if (pos_rtx == 0 && pos == 0 && ! in_dest
      && ! in_compare && ! spans_byte && unsignedp)
    return 0;






  if (! spans_byte && ((enum rtx_code) (inner)->code) == MEM
      && ((pos_rtx == 0 && pos + len > (mode_bitsize[(int) (is_mode)]))
   || (pos_rtx != 0 && len != 1)))
    return 0;



  if (in_dest && mode_for_extraction (EP_insv, -1) != MAX_MACHINE_MODE)
    {
      wanted_inner_reg_mode = mode_for_extraction (EP_insv, 0);
      pos_mode = mode_for_extraction (EP_insv, 2);
      extraction_mode = mode_for_extraction (EP_insv, 3);
    }

  if (! in_dest && unsignedp
      && mode_for_extraction (EP_extzv, -1) != MAX_MACHINE_MODE)
    {
      wanted_inner_reg_mode = mode_for_extraction (EP_extzv, 1);
      pos_mode = mode_for_extraction (EP_extzv, 3);
      extraction_mode = mode_for_extraction (EP_extzv, 0);
    }

  if (! in_dest && ! unsignedp
      && mode_for_extraction (EP_extv, -1) != MAX_MACHINE_MODE)
    {
      wanted_inner_reg_mode = mode_for_extraction (EP_extv, 1);
      pos_mode = mode_for_extraction (EP_extv, 3);
      extraction_mode = mode_for_extraction (EP_extv, 0);
    }



  if (mode != VOIDmode
      && (mode_size[(int) (extraction_mode)]) < (mode_size[(int) (mode)]))
    extraction_mode = mode;

  if (pos_rtx && ((enum machine_mode) (pos_rtx)->mode) != VOIDmode
      && (mode_size[(int) (pos_mode)]) < (mode_size[(int) (((enum machine_mode) (pos_rtx)->mode))]))
    pos_mode = ((enum machine_mode) (pos_rtx)->mode);




  if (((enum rtx_code) (inner)->code) != MEM)
    wanted_inner_mode = wanted_inner_reg_mode;
  else if (inner_mode != wanted_inner_mode
    && (mode_dependent_address_p ((((inner)->fld[0]).rtx))
        || ((inner)->volatil)))
    wanted_inner_mode = extraction_mode;

  orig_pos = pos;

  if (0)
    {







      int width = (((enum rtx_code) (inner)->code) == MEM
     ? (mode_bitsize[(int) (is_mode)])
     : (mode_bitsize[(int) (wanted_inner_mode)]));

      if (pos_rtx == 0)
 pos = width - len - pos;
      else
 pos_rtx
   = gen_rtx_fmt_ee (MINUS, (((enum machine_mode) (pos_rtx)->mode)), (gen_rtx_CONST_INT (VOIDmode, (long long) (width - len))), (pos_rtx));


    }




  if (wanted_inner_mode != VOIDmode
      && (mode_size[(int) (wanted_inner_mode)]) < (mode_size[(int) (is_mode)])
      && ((((enum rtx_code) (inner)->code) == MEM
    && (inner_mode == wanted_inner_mode
        || (! mode_dependent_address_p ((((inner)->fld[0]).rtx))
     && ! ((inner)->volatil))))))
    {
      int offset = 0;







      if (0
   && ! spans_byte
   && (mode_size[(int) (inner_mode)]) < (mode_size[(int) (is_mode)]))
 offset -= (mode_size[(int) (is_mode)]) - (mode_size[(int) (inner_mode)]);


      if (pos_rtx == 0)
 {
   offset += pos / 8;
   pos %= (mode_bitsize[(int) (wanted_inner_mode)]);
 }

      if (0 != 0
   && ! spans_byte
   && is_mode != wanted_inner_mode)
 offset = ((mode_size[(int) (is_mode)])
    - (mode_size[(int) (wanted_inner_mode)]) - offset);

      if (offset != 0 || inner_mode != wanted_inner_mode)
 inner = adjust_address_1 (inner, wanted_inner_mode, offset, 0, 1);
    }




  else if (((enum rtx_code) (inner)->code) != MEM)
    {
      if (((enum machine_mode) (inner)->mode) != wanted_inner_mode
   && (pos_rtx != 0
       || orig_pos + len > (mode_bitsize[(int) (wanted_inner_mode)])))
 return 0;

      inner = force_to_mode (inner, wanted_inner_mode,
        pos_rtx
        || len + orig_pos >= (8 * 8)
        ? ~(unsigned long long) 0
        : ((((unsigned long long) 1 << len) - 1)
    << orig_pos),
        (rtx) 0, 0);
    }



  if (pos_rtx != 0
      && (mode_size[(int) (pos_mode)]) > (mode_size[(int) (((enum machine_mode) (pos_rtx)->mode))]))
    {
      rtx temp = gen_rtx_fmt_e (ZERO_EXTEND, (pos_mode), (pos_rtx));





      if (flag_expensive_optimizations
   && ((mode_bitsize[(int) (((enum machine_mode) (pos_rtx)->mode))]) <= (8 * 8)
       && ((nonzero_bits (pos_rtx, ((enum machine_mode) (pos_rtx)->mode))
     & ~(((unsigned long long)
   mode_mask_array[(int) (((enum machine_mode) (pos_rtx)->mode))])
         >> 1))
    == 0)))
 {
   rtx temp1 = gen_rtx_fmt_e (SIGN_EXTEND, (pos_mode), (pos_rtx));



   if (rtx_cost (temp1, SET) < rtx_cost (temp, SET))
     temp = temp1;
 }
      pos_rtx = temp;
    }
  else if (pos_rtx != 0
    && (mode_size[(int) (pos_mode)]) < (mode_size[(int) (((enum machine_mode) (pos_rtx)->mode))]))
    pos_rtx = gen_lowpart_for_combine (pos_mode, pos_rtx);




  if (pos_rtx == 0 && orig_pos_rtx != 0 && (((orig_pos_rtx)->fld[0]).rtwint) == pos)
    pos_rtx = orig_pos_rtx;

  else if (pos_rtx == 0)
    pos_rtx = gen_rtx_CONST_INT (VOIDmode, (long long) (pos));


  new = gen_rtx_fmt_eee (unsignedp ? ZERO_EXTRACT : SIGN_EXTRACT,
    extraction_mode, inner, gen_rtx_CONST_INT (VOIDmode, (long long) (len)), pos_rtx);
  if (! in_dest)
    new = gen_lowpart_for_combine (mode, new);

  return new;
}




static rtx
extract_left_shift (x, count)
     rtx x;
     int count;
{
  enum rtx_code code = ((enum rtx_code) (x)->code);
  enum machine_mode mode = ((enum machine_mode) (x)->mode);
  rtx tem;

  switch (code)
    {
    case ASHIFT:



      if (((enum rtx_code) ((((x)->fld[1]).rtx))->code) == CONST_INT
   && ((((((x)->fld[1]).rtx))->fld[0]).rtwint) >= count)
 return simplify_shift_const ((rtx) 0, ASHIFT, mode, (((x)->fld[0]).rtx),
         ((((((x)->fld[1]).rtx))->fld[0]).rtwint) - count);
      break;

    case NEG: case NOT:
      if ((tem = extract_left_shift ((((x)->fld[0]).rtx), count)) != 0)
 return simplify_gen_unary (code, mode, tem, mode);

      break;

    case PLUS: case IOR: case XOR: case AND:


      if (((enum rtx_code) ((((x)->fld[1]).rtx))->code) == CONST_INT
   && (((((((x)->fld[1]).rtx))->fld[0]).rtwint) & ((((long long) 1 << count)) - 1)) == 0
   && (tem = extract_left_shift ((((x)->fld[0]).rtx), count)) != 0)
 return gen_binary (code, mode, tem,
      gen_rtx_CONST_INT (VOIDmode, (long long) (((((((x)->fld[1]).rtx))->fld[0]).rtwint) >> count)));

      break;

    default:
      break;
    }

  return 0;
}
# 6336 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static rtx
make_compound_operation (x, in_code)
     rtx x;
     enum rtx_code in_code;
{
  enum rtx_code code = ((enum rtx_code) (x)->code);
  enum machine_mode mode = ((enum machine_mode) (x)->mode);
  int mode_width = (mode_bitsize[(int) (mode)]);
  rtx rhs, lhs;
  enum rtx_code next_code;
  int i;
  rtx new = 0;
  rtx tem;
  const char *fmt;





  next_code = (code == MEM || code == PLUS || code == MINUS ? MEM
        : ((code == COMPARE || (rtx_class[(int) (code)]) == '<')
    && (((x)->fld[1]).rtx) == (const_int_rtx[64])) ? COMPARE
        : in_code == COMPARE ? SET : in_code);




  switch (code)
    {
    case ASHIFT:


      if (in_code == MEM && ((enum rtx_code) ((((x)->fld[1]).rtx))->code) == CONST_INT
   && ((((((x)->fld[1]).rtx))->fld[0]).rtwint) < (8 * 8)
   && ((((((x)->fld[1]).rtx))->fld[0]).rtwint) >= 0)
 {
   new = make_compound_operation ((((x)->fld[0]).rtx), next_code);
   new = gen_rtx_fmt_ee (MULT, (mode), (new), (gen_rtx_CONST_INT (VOIDmode, (long long) ((long long) 1 << ((((((x)->fld[1]).rtx))->fld[0]).rtwint)))));


 }
      break;

    case AND:


      if (((enum rtx_code) ((((x)->fld[1]).rtx))->code) != CONST_INT)
 break;



      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == LSHIFTRT
   && (i = exact_log2_wide ((unsigned long long) (((((((x)->fld[1]).rtx))->fld[0]).rtwint) + 1))) >= 0)
 {
   new = make_compound_operation (((((((x)->fld[0]).rtx))->fld[0]).rtx), next_code);
   new = make_extraction (mode, new, 0, ((((((x)->fld[0]).rtx))->fld[1]).rtx), i, 1,
     0, in_code == COMPARE);
 }


      else if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == SUBREG
        && subreg_lowpart_p ((((x)->fld[0]).rtx))
        && ((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->code) == LSHIFTRT
        && (i = exact_log2_wide ((unsigned long long) (((((((x)->fld[1]).rtx))->fld[0]).rtwint) + 1))) >= 0)
 {
   new = make_compound_operation ((((((((((x)->fld[0]).rtx))->fld[0]).rtx))->fld[0]).rtx),
      next_code);
   new = make_extraction (((enum machine_mode) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->mode), new, 0,
     (((((((((x)->fld[0]).rtx))->fld[0]).rtx))->fld[1]).rtx), i, 1,
     0, in_code == COMPARE);
 }

      else if ((((enum rtx_code) ((((x)->fld[0]).rtx))->code) == XOR
  || ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == IOR)
        && ((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[0]).rtx))->code) == LSHIFTRT
        && ((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[1]).rtx))->code) == LSHIFTRT
        && (i = exact_log2_wide ((unsigned long long) (((((((x)->fld[1]).rtx))->fld[0]).rtwint) + 1))) >= 0)
 {

   new = gen_rtx_fmt_ee (((enum rtx_code) ((((x)->fld[0]).rtx))->code), mode,
    gen_rtx_fmt_ee (AND, (mode), (((((((x)->fld[0]).rtx))->fld[0]).rtx)), ((((x)->fld[1]).rtx))),

    gen_rtx_fmt_ee (AND, (mode), (((((((x)->fld[0]).rtx))->fld[1]).rtx)), ((((x)->fld[1]).rtx))));

   new = make_compound_operation (new, in_code);
 }




      else if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == ROTATE
        && ((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[1]).rtx))->code) == CONST_INT
        && (i = exact_log2_wide ((unsigned long long) (((((((x)->fld[1]).rtx))->fld[0]).rtwint) + 1))) >= 0
        && i <= (((((((((x)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint))
 {
   new = make_compound_operation (((((((x)->fld[0]).rtx))->fld[0]).rtx), next_code);
   new = make_extraction (mode, new,
     ((mode_bitsize[(int) (mode)])
      - (((((((((x)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint)),
     (rtx) 0, i, 1, 0, in_code == COMPARE);
 }




      else if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == LSHIFTRT
        && !have_insn_for (LSHIFTRT, mode)
        && have_insn_for (ASHIFTRT, mode)
        && ((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[1]).rtx))->code) == CONST_INT
        && (((((((((x)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint) >= 0
        && (((((((((x)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint) < (8 * 8)
        && mode_width <= (8 * 8))
 {
   unsigned long long mask = mode_mask_array[(int) (mode)];

   mask >>= (((((((((x)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint);
   if ((((((((x)->fld[1]).rtx))->fld[0]).rtwint) & ~mask) == 0)
     do_SUBST(&((((x)->fld[0]).rtx)), (gen_rtx_fmt_ee (ASHIFTRT, (mode), (make_compound_operation (((((((x)->fld[0]).rtx))->fld[0]).rtx), next_code)), (((((((x)->fld[0]).rtx))->fld[1]).rtx)))));




 }





      else if ((i = exact_log2_wide ((unsigned long long) (((((((x)->fld[1]).rtx))->fld[0]).rtwint) + 1))) >= 0)
 new = make_extraction (mode,
          make_compound_operation ((((x)->fld[0]).rtx),
       next_code),
          0, (rtx) 0, i, 1, 0, in_code == COMPARE);



      else if (in_code == COMPARE
        && (i = exact_log2_wide ((unsigned long long) (((((((x)->fld[1]).rtx))->fld[0]).rtwint)))) >= 0)
 new = make_extraction (mode,
          make_compound_operation ((((x)->fld[0]).rtx),
       next_code),
          i, (rtx) 0, 1, 1, 0, 1);

      break;

    case LSHIFTRT:


      if (have_insn_for (ASHIFTRT, mode)
   && ! have_insn_for (LSHIFTRT, mode)
   && mode_width <= (8 * 8)
   && (nonzero_bits ((((x)->fld[0]).rtx), mode) & (1 << (mode_width - 1))) == 0)
 {
   new = gen_rtx_fmt_ee (ASHIFTRT, (mode), (make_compound_operation ((((x)->fld[0]).rtx), next_code)), ((((x)->fld[1]).rtx)));



   break;
 }



    case ASHIFTRT:
      lhs = (((x)->fld[0]).rtx);
      rhs = (((x)->fld[1]).rtx);



      if (((enum rtx_code) (rhs)->code) == CONST_INT
   && ((enum rtx_code) (lhs)->code) == ASHIFT
   && ((enum rtx_code) ((((lhs)->fld[1]).rtx))->code) == CONST_INT
   && (((rhs)->fld[0]).rtwint) >= ((((((lhs)->fld[1]).rtx))->fld[0]).rtwint))
 {
   new = make_compound_operation ((((lhs)->fld[0]).rtx), next_code);
   new = make_extraction (mode, new,
     (((rhs)->fld[0]).rtwint) - ((((((lhs)->fld[1]).rtx))->fld[0]).rtwint),
     (rtx) 0, mode_width - (((rhs)->fld[0]).rtwint),
     code == LSHIFTRT, 0, in_code == COMPARE);
   break;
 }






      if ((rtx_class[(int) (((enum rtx_code) (lhs)->code))]) != 'o'
   && ! (((enum rtx_code) (lhs)->code) == SUBREG
  && ((rtx_class[(int) (((enum rtx_code) ((((lhs)->fld[0]).rtx))->code))]) == 'o'))
   && ((enum rtx_code) (rhs)->code) == CONST_INT
   && (((rhs)->fld[0]).rtwint) < (8 * 8)
   && (new = extract_left_shift (lhs, (((rhs)->fld[0]).rtwint))) != 0)
 new = make_extraction (mode, make_compound_operation (new, next_code),
          0, (rtx) 0, mode_width - (((rhs)->fld[0]).rtwint),
          code == LSHIFTRT, 0, in_code == COMPARE);

      break;

    case SUBREG:




      tem = make_compound_operation ((((x)->fld[0]).rtx), in_code);
      if (((enum rtx_code) (tem)->code) != ((enum rtx_code) ((((x)->fld[0]).rtx))->code)
   && (mode_size[(int) (mode)]) < (mode_size[(int) (((enum machine_mode) (tem)->mode))])
   && subreg_lowpart_p (x))
 {
   rtx newer = force_to_mode (tem, mode, ~(long long) 0,
         (rtx) 0, 0);



   if (((enum rtx_code) (newer)->code) != SUBREG)
     newer = make_compound_operation (newer, in_code);

   return newer;
 }





      if ((((enum rtx_code) (tem)->code) == SIGN_EXTEND
    || ((enum rtx_code) (tem)->code) == ZERO_EXTEND)
   && subreg_lowpart_p (x))
 {
   if ((mode_size[(int) (mode)]) > (mode_size[(int) (((enum machine_mode) (tem)->mode))])
       || ((mode_size[(int) (mode)]) >
    (mode_size[(int) (((enum machine_mode) ((((tem)->fld[0]).rtx))->mode))])))
     tem = gen_rtx_fmt_e (((enum rtx_code) (tem)->code), mode, (((tem)->fld[0]).rtx));
   else
     tem = gen_lowpart_for_combine (mode, (((tem)->fld[0]).rtx));
   return tem;
 }
      break;

    default:
      break;
    }

  if (new)
    {
      x = gen_lowpart_for_combine (mode, new);
      code = ((enum rtx_code) (x)->code);
    }


  fmt = (rtx_format[(int) (code)]);
  for (i = 0; i < (rtx_length[(int) (code)]); i++)
    if (fmt[i] == 'e')
      {
 new = make_compound_operation ((((x)->fld[i]).rtx), next_code);
 do_SUBST(&((((x)->fld[i]).rtx)), (new));
      }

  return x;
}
# 6602 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static int
get_pos_from_mask (m, plen)
     unsigned long long m;
     unsigned long long *plen;
{

  int pos = exact_log2_wide ((unsigned long long) (m & -m));
  int len;

  if (pos < 0)
    return -1;



  len = exact_log2_wide ((unsigned long long) ((m >> pos) + 1));

  if (len <= 0)
    return -1;

  *plen = len;
  return pos;
}
# 6641 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static rtx
force_to_mode (x, mode, mask, reg, just_select)
     rtx x;
     enum machine_mode mode;
     unsigned long long mask;
     rtx reg;
     int just_select;
{
  enum rtx_code code = ((enum rtx_code) (x)->code);
  int next_select = just_select || code == XOR || code == NOT || code == NEG;
  enum machine_mode op_mode;
  unsigned long long fuller_mask, nonzero;
  rtx op0, op1, temp;







  if (code == CALL || code == ASM_OPERANDS || code == CLOBBER)
    return x;




  op_mode = (((mode_class[(int) (mode)]) == (mode_class[(int) (((enum machine_mode) (x)->mode))])
       && have_insn_for (code, mode))
      ? mode : ((enum machine_mode) (x)->mode));



  if ((code == LSHIFTRT || code == ASHIFTRT)
      && (mode_bitsize[(int) (mode)]) < (mode_bitsize[(int) (((enum machine_mode) (x)->mode))]))
    op_mode = ((enum machine_mode) (x)->mode);


  if (op_mode)
    mask &= mode_mask_array[(int) (op_mode)];




  if (op_mode)
    fuller_mask = ((mode_bitsize[(int) (op_mode)]) >= (8 * 8)
     ? mode_mask_array[(int) (op_mode)]
     : (((unsigned long long) 1 << (floor_log2_wide ((unsigned long long) (mask)) + 1))
        - 1));
  else
    fuller_mask = ~(long long) 0;


  nonzero = nonzero_bits (x, mode);


  if (! just_select && (nonzero & mask) == 0)
    return (const_int_rtx[64]);



  if (((enum rtx_code) (x)->code) == CONST_INT)
    return gen_int_mode ((((x)->fld[0]).rtwint) & mask, mode);



  if ((mode_size[(int) (((enum machine_mode) (x)->mode))]) < (mode_size[(int) (mode)])
      && (mode_mask_array[(int) (((enum machine_mode) (x)->mode))] & ~mask) == 0)
    return gen_lowpart_for_combine (mode, x);



  if (((enum machine_mode) (x)->mode) == mode && code != SUBREG && (~mask & nonzero) == 0)
    return x;

  switch (code)
    {
    case CLOBBER:


      return x;

    case USE:



      if (! 0
   && (mask & ~mode_mask_array[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))]) == 0)
 return force_to_mode ((((x)->fld[0]).rtx), mode, mask, reg, next_select);
      break;

    case SIGN_EXTEND:
    case ZERO_EXTEND:
    case ZERO_EXTRACT:
    case SIGN_EXTRACT:
      x = expand_compound_operation (x);
      if (((enum rtx_code) (x)->code) != code)
 return force_to_mode (x, mode, mask, reg, next_select);
      break;

    case REG:
      if (reg != 0 && (rtx_equal_p (get_last_value (reg), x)
         || rtx_equal_p (reg, get_last_value (x))))
 x = reg;
      break;

    case SUBREG:
      if (subreg_lowpart_p (x)



   && (((mode_size[(int) (((enum machine_mode) (x)->mode))])
        < (mode_size[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))]))
       || (0 == (mask
   & mode_mask_array[(int) (((enum machine_mode) (x)->mode))]
   & ~mode_mask_array[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))]))))
 return force_to_mode ((((x)->fld[0]).rtx), mode, mask, reg, next_select);
      break;

    case AND:




      if (((enum rtx_code) ((((x)->fld[1]).rtx))->code) == CONST_INT)
 {
   x = simplify_and_const_int (x, op_mode, (((x)->fld[0]).rtx),
          mask & ((((((x)->fld[1]).rtx))->fld[0]).rtwint));





   if (((enum rtx_code) (x)->code) == AND && ((enum rtx_code) ((((x)->fld[1]).rtx))->code) == CONST_INT
       && ((((((((x)->fld[1]).rtx))->fld[0]).rtwint) & mode_mask_array[(int) (((enum machine_mode) (x)->mode))])
    == (long long) mask))
     x = (((x)->fld[0]).rtx);






   if (((enum rtx_code) (x)->code) == AND && ((enum rtx_code) ((((x)->fld[1]).rtx))->code) == CONST_INT
       && mode_mask_array[(int) (((enum machine_mode) (x)->mode))] != mask
       && (mode_bitsize[(int) (((enum machine_mode) (x)->mode))]) <= (8 * 8))
     {
       long long cval = (((((((x)->fld[1]).rtx))->fld[0]).rtwint)
        | (mode_mask_array[(int) (((enum machine_mode) (x)->mode))] & ~mask));
       int width = (mode_bitsize[(int) (((enum machine_mode) (x)->mode))]);
       rtx y;



       if (width > 0 && width < (8 * 8)
    && (cval & ((long long) 1 << (width - 1))) != 0)
  cval |= (long long) -1 << width;

       y = gen_binary (AND, ((enum machine_mode) (x)->mode), (((x)->fld[0]).rtx), gen_rtx_CONST_INT (VOIDmode, (long long) (cval)));
       if (rtx_cost (y, SET) < rtx_cost (x, SET))
  x = y;
     }

   break;
 }

      goto binop;

    case PLUS:





      {
 unsigned int width = (mode_bitsize[(int) (mode)]);
 unsigned long long smask = mask;




 if (width < (8 * 8)
     && (smask & ((long long) 1 << (width - 1))) != 0)
   smask |= (long long) -1 << width;

 if (((enum rtx_code) ((((x)->fld[1]).rtx))->code) == CONST_INT
     && exact_log2_wide ((unsigned long long) (- smask)) >= 0
     && (nonzero_bits ((((x)->fld[0]).rtx), mode) & ~smask) == 0
     && (((((((x)->fld[1]).rtx))->fld[0]).rtwint) & ~smask) != 0)
   return force_to_mode (plus_constant_wide (((((x)->fld[0]).rtx)), (long long) ((((((((x)->fld[1]).rtx))->fld[0]).rtwint) & smask))),

    mode, smask, reg, next_select);
      }



    case MULT:



      mask = fuller_mask;
      goto binop;

    case MINUS:


      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == CONST_INT
   && (((unsigned long long) (((((((x)->fld[0]).rtx))->fld[0]).rtwint)
     & -((((((x)->fld[0]).rtx))->fld[0]).rtwint)))
       > mask))
 {
   x = simplify_gen_unary (NEG, ((enum machine_mode) (x)->mode), (((x)->fld[1]).rtx),
      ((enum machine_mode) (x)->mode));
   return force_to_mode (x, mode, mask, reg, next_select);
 }



      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == CONST_INT
   && ((((((((x)->fld[0]).rtx))->fld[0]).rtwint) | (long long) mask)
       == ((((((x)->fld[0]).rtx))->fld[0]).rtwint)))
 {
   x = simplify_gen_unary (NOT, ((enum machine_mode) (x)->mode),
      (((x)->fld[1]).rtx), ((enum machine_mode) (x)->mode));
   return force_to_mode (x, mode, mask, reg, next_select);
 }

      mask = fuller_mask;
      goto binop;

    case IOR:
    case XOR:





      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == LSHIFTRT
   && ((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[1]).rtx))->code) == CONST_INT
   && (((((((((x)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint) >= 0
   && (((((((((x)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint) < (8 * 8)
   && ((enum rtx_code) ((((x)->fld[1]).rtx))->code) == CONST_INT
   && (((((((((((x)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint)
        + floor_log2_wide ((unsigned long long) (((((((x)->fld[1]).rtx))->fld[0]).rtwint))))
       < (mode_bitsize[(int) (((enum machine_mode) (x)->mode))]))
   && (((((((x)->fld[1]).rtx))->fld[0]).rtwint)
       & ~nonzero_bits ((((x)->fld[0]).rtx), ((enum machine_mode) (x)->mode))) == 0)
 {
   temp = gen_rtx_CONST_INT (VOIDmode, (long long) ((((((((x)->fld[1]).rtx))->fld[0]).rtwint) & mask) << (((((((((x)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint)));

   temp = gen_binary (((enum rtx_code) (x)->code), ((enum machine_mode) (x)->mode),
        ((((((x)->fld[0]).rtx))->fld[0]).rtx), temp);
   x = gen_binary (LSHIFTRT, ((enum machine_mode) (x)->mode), temp,
     ((((((x)->fld[0]).rtx))->fld[1]).rtx));
   return force_to_mode (x, mode, mask, reg, next_select);
 }

    binop:



      op0 = gen_lowpart_for_combine (op_mode,
         force_to_mode ((((x)->fld[0]).rtx), mode, mask,
          reg, next_select));
      op1 = gen_lowpart_for_combine (op_mode,
         force_to_mode ((((x)->fld[1]).rtx), mode, mask,
          reg, next_select));

      if (op_mode != ((enum machine_mode) (x)->mode) || op0 != (((x)->fld[0]).rtx) || op1 != (((x)->fld[1]).rtx))
 x = gen_binary (code, op_mode, op0, op1);
      break;

    case ASHIFT:






      if (! (((enum rtx_code) ((((x)->fld[1]).rtx))->code) == CONST_INT
      && ((((((x)->fld[1]).rtx))->fld[0]).rtwint) >= 0
      && ((((((x)->fld[1]).rtx))->fld[0]).rtwint) < (mode_bitsize[(int) (mode)]))
   && ! (((enum machine_mode) ((((x)->fld[1]).rtx))->mode) != VOIDmode
  && (nonzero_bits ((((x)->fld[1]).rtx), ((enum machine_mode) ((((x)->fld[1]).rtx))->mode))
      < (unsigned long long) (mode_bitsize[(int) (mode)]))))
 break;




      if (((enum rtx_code) ((((x)->fld[1]).rtx))->code) == CONST_INT
   && ((((((x)->fld[1]).rtx))->fld[0]).rtwint) >= 0
   && ((((((x)->fld[1]).rtx))->fld[0]).rtwint) < (mode_bitsize[(int) (op_mode)])
   && (mode_bitsize[(int) (op_mode)]) <= (8 * 8))
 mask >>= ((((((x)->fld[1]).rtx))->fld[0]).rtwint);
      else
 mask = fuller_mask;

      op0 = gen_lowpart_for_combine (op_mode,
         force_to_mode ((((x)->fld[0]).rtx), op_mode,
          mask, reg, next_select));

      if (op_mode != ((enum machine_mode) (x)->mode) || op0 != (((x)->fld[0]).rtx))
 x = gen_binary (code, op_mode, op0, (((x)->fld[1]).rtx));
      break;

    case LSHIFTRT:




      if (((enum rtx_code) ((((x)->fld[1]).rtx))->code) == CONST_INT
   && ((((((x)->fld[1]).rtx))->fld[0]).rtwint) < (8 * 8)
   && (mode_bitsize[(int) (op_mode)]) <= (8 * 8))
 {
   rtx inner = (((x)->fld[0]).rtx);
   unsigned long long inner_mask;


   inner_mask = mask << ((((((x)->fld[1]).rtx))->fld[0]).rtwint);




   if ((mode_bitsize[(int) (op_mode)]) > (8 * 8)
       || (inner_mask & ~mode_mask_array[(int) (op_mode)]) != 0)
     op_mode = ((enum machine_mode) (x)->mode);

   inner = force_to_mode (inner, op_mode, inner_mask, reg, next_select);

   if (((enum machine_mode) (x)->mode) != op_mode || inner != (((x)->fld[0]).rtx))
     x = gen_binary (LSHIFTRT, op_mode, inner, (((x)->fld[1]).rtx));
 }





      if (((enum rtx_code) (x)->code) == LSHIFTRT
   && ((enum rtx_code) ((((x)->fld[1]).rtx))->code) == CONST_INT


   && ((((((((x)->fld[1]).rtx))->fld[0]).rtwint)
        + num_sign_bit_copies ((((x)->fld[0]).rtx), ((enum machine_mode) ((((x)->fld[0]).rtx))->mode)))
       >= (mode_bitsize[(int) (((enum machine_mode) (x)->mode))]))
   && exact_log2_wide ((unsigned long long) (mask + 1)) >= 0


   && ((((((((x)->fld[1]).rtx))->fld[0]).rtwint) + exact_log2_wide ((unsigned long long) (mask + 1)))
       <= (mode_bitsize[(int) (((enum machine_mode) (x)->mode))]))

   && ((int) num_sign_bit_copies ((((x)->fld[0]).rtx), ((enum machine_mode) ((((x)->fld[0]).rtx))->mode))
       >= exact_log2_wide ((unsigned long long) (mask + 1))))
 x = gen_binary (LSHIFTRT, ((enum machine_mode) (x)->mode), (((x)->fld[0]).rtx),
   gen_rtx_CONST_INT (VOIDmode, (long long) ((mode_bitsize[(int) (((enum machine_mode) (x)->mode))]) - exact_log2_wide ((unsigned long long) (mask + 1)))));


      goto shiftrt;

    case ASHIFTRT:


      if ((mode_bitsize[(int) (((enum machine_mode) (x)->mode))]) <= (8 * 8)
   && (mask == ((unsigned long long) 1
         << ((mode_bitsize[(int) (((enum machine_mode) (x)->mode))]) - 1))))
 return force_to_mode ((((x)->fld[0]).rtx), mode, mask, reg, next_select);
# 7014 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
      if (((enum rtx_code) ((((x)->fld[1]).rtx))->code) == CONST_INT && ((((((x)->fld[1]).rtx))->fld[0]).rtwint) >= 0
   && ((((((x)->fld[1]).rtx))->fld[0]).rtwint) < (8 * 8))
 {
   int i = -1;





   if ((mode_bitsize[(int) (((enum machine_mode) (x)->mode))]) > (8 * 8))
     {
       nonzero = ~(long long) 0;






       if ((mode_bitsize[(int) (((enum machine_mode) (x)->mode))]) - ((((((x)->fld[1]).rtx))->fld[0]).rtwint)
    < (8 * 8))
  nonzero >>= ((((((x)->fld[1]).rtx))->fld[0]).rtwint)
       + (8 * 8)
       - (mode_bitsize[(int) (((enum machine_mode) (x)->mode))]) ;
     }
   else
     {
       nonzero = mode_mask_array[(int) (((enum machine_mode) (x)->mode))];
       nonzero >>= ((((((x)->fld[1]).rtx))->fld[0]).rtwint);
     }

   if ((mask & ~nonzero) == 0
       || (i = exact_log2_wide ((unsigned long long) (mask))) >= 0)
     {
       x = simplify_shift_const
  (x, LSHIFTRT, ((enum machine_mode) (x)->mode), (((x)->fld[0]).rtx),
   i < 0 ? ((((((x)->fld[1]).rtx))->fld[0]).rtwint)
   : (mode_bitsize[(int) (((enum machine_mode) (x)->mode))]) - 1 - i);

       if (((enum rtx_code) (x)->code) != ASHIFTRT)
  return force_to_mode (x, mode, mask, reg, next_select);
     }
 }



      if (mask == 1)
 x = gen_binary (LSHIFTRT, ((enum machine_mode) (x)->mode), (((x)->fld[0]).rtx), (((x)->fld[1]).rtx));

    shiftrt:





      if ((((enum rtx_code) (x)->code) == LSHIFTRT || ((enum rtx_code) (x)->code) == ASHIFTRT)
   && ((enum rtx_code) ((((x)->fld[1]).rtx))->code) == CONST_INT
   && ((((((x)->fld[1]).rtx))->fld[0]).rtwint) >= 0
   && (((((((x)->fld[1]).rtx))->fld[0]).rtwint)
       <= (mode_bitsize[(int) (((enum machine_mode) (x)->mode))]) - (floor_log2_wide ((unsigned long long) (mask)) + 1))
   && ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == ASHIFT
   && ((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[1]).rtx))->code) == CONST_INT
   && (((((((((x)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint) == ((((((x)->fld[1]).rtx))->fld[0]).rtwint))
 return force_to_mode (((((((x)->fld[0]).rtx))->fld[0]).rtx), mode, mask,
         reg, next_select);

      break;

    case ROTATE:
    case ROTATERT:




      if (((enum rtx_code) ((((x)->fld[1]).rtx))->code) == CONST_INT
   && ((((((x)->fld[1]).rtx))->fld[0]).rtwint) >= 0)
 {
   temp = simplify_binary_operation (code == ROTATE ? ROTATERT : ROTATE,
         ((enum machine_mode) (x)->mode), gen_rtx_CONST_INT (VOIDmode, (long long) (mask)),
         (((x)->fld[1]).rtx));
   if (temp && ((enum rtx_code) (temp)->code) == CONST_INT)
     do_SUBST(&((((x)->fld[0]).rtx)), (force_to_mode ((((x)->fld[0]).rtx), ((enum machine_mode) (x)->mode), (((temp)->fld[0]).rtwint), reg, next_select)));


 }
      break;

    case NEG:


      if (mask == 1)
 return force_to_mode ((((x)->fld[0]).rtx), mode, mask, reg, just_select);




      mask = fuller_mask;
      goto unop;

    case NOT:




      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == LSHIFTRT
   && ((enum rtx_code) (((((((x)->fld[0]).rtx))->fld[1]).rtx))->code) == CONST_INT
   && (((((((((x)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint) >= 0
   && ((((((((((x)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint) + floor_log2_wide ((unsigned long long) (mask))
       < (mode_bitsize[(int) (((enum machine_mode) (x)->mode))]))
   && (((((((((x)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint) < (8 * 8))
 {
   temp = gen_rtx_CONST_INT (VOIDmode, (long long) (mask << (((((((((x)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint)));
   temp = gen_binary (XOR, ((enum machine_mode) (x)->mode), ((((((x)->fld[0]).rtx))->fld[0]).rtx), temp);
   x = gen_binary (LSHIFTRT, ((enum machine_mode) (x)->mode), temp, ((((((x)->fld[0]).rtx))->fld[1]).rtx));

   return force_to_mode (x, mode, mask, reg, next_select);
 }



      mask = fuller_mask;

    unop:
      op0 = gen_lowpart_for_combine (op_mode,
         force_to_mode ((((x)->fld[0]).rtx), mode, mask,
          reg, next_select));
      if (op_mode != ((enum machine_mode) (x)->mode) || op0 != (((x)->fld[0]).rtx))
 x = simplify_gen_unary (code, op_mode, op0, op_mode);
      break;

    case NE:



      if ((mask & ~1) == 0 && (((x)->fld[1]).rtx) == (const_int_rtx[64])
   && exact_log2_wide ((unsigned long long) (nonzero_bits ((((x)->fld[0]).rtx), mode))) >= 0
   && nonzero_bits ((((x)->fld[0]).rtx), mode) == 1)
 return force_to_mode ((((x)->fld[0]).rtx), mode, mask, reg, next_select);

      break;

    case IF_THEN_ELSE:



      do_SUBST(&((((x)->fld[1]).rtx)), (gen_lowpart_for_combine (((enum machine_mode) (x)->mode), force_to_mode ((((x)->fld[1]).rtx), mode, mask, reg, next_select))));



      do_SUBST(&((((x)->fld[2]).rtx)), (gen_lowpart_for_combine (((enum machine_mode) (x)->mode), force_to_mode ((((x)->fld[2]).rtx), mode, mask, reg,next_select))));



      break;

    default:
      break;
    }


  return gen_lowpart_for_combine (mode, x);
}
# 7183 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static rtx
if_then_else_cond (x, ptrue, pfalse)
     rtx x;
     rtx *ptrue, *pfalse;
{
  enum machine_mode mode = ((enum machine_mode) (x)->mode);
  enum rtx_code code = ((enum rtx_code) (x)->code);
  rtx cond0, cond1, true0, true1, false0, false1;
  unsigned long long nz;


  if ((code == NE || code == EQ)
      && ((enum rtx_code) ((((x)->fld[1]).rtx))->code) == CONST_INT && ((((((x)->fld[1]).rtx))->fld[0]).rtwint) == 0)
    {
      *ptrue = (code == NE) ? const_true_rtx : (const_int_rtx[64]);
      *pfalse = (code == NE) ? (const_int_rtx[64]) : const_true_rtx;
      return (((x)->fld[0]).rtx);
    }



  else if ((rtx_class[(int) (code)]) == '1'
    && (cond0 = if_then_else_cond ((((x)->fld[0]).rtx), &true0, &false0)) != 0)
    {
      *ptrue = simplify_gen_unary (code, mode, true0, ((enum machine_mode) ((((x)->fld[0]).rtx))->mode));
      *pfalse = simplify_gen_unary (code, mode, false0,
        ((enum machine_mode) ((((x)->fld[0]).rtx))->mode));
      return cond0;
    }



  else if (code == COMPARE)
    ;




  else if ((rtx_class[(int) (code)]) == 'c' || (rtx_class[(int) (code)]) == '2'
    || (rtx_class[(int) (code)]) == '<')
    {
      cond0 = if_then_else_cond ((((x)->fld[0]).rtx), &true0, &false0);
      cond1 = if_then_else_cond ((((x)->fld[1]).rtx), &true1, &false1);

      if ((cond0 != 0 || cond1 != 0)
   && ! (cond0 != 0 && cond1 != 0 && ! rtx_equal_p (cond0, cond1)))
 {



   if (cond0 == 0)
     true0 = copy_rtx (true0);
   else if (cond1 == 0)
     true1 = copy_rtx (true1);

   *ptrue = gen_binary (code, mode, true0, true1);
   *pfalse = gen_binary (code, mode, false0, false1);
   return cond0 ? cond0 : cond1;
 }





      if ((1 == 1 || 1 == -1)
   && (code == PLUS || code == IOR || code == XOR || code == MINUS
       || code == UMAX)
   && ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == MULT && ((enum rtx_code) ((((x)->fld[1]).rtx))->code) == MULT)
 {
   rtx op0 = ((((((x)->fld[0]).rtx))->fld[1]).rtx);
   rtx op1 = ((((((x)->fld[1]).rtx))->fld[1]).rtx);

   cond0 = ((((((x)->fld[0]).rtx))->fld[0]).rtx);
   cond1 = ((((((x)->fld[1]).rtx))->fld[0]).rtx);

   if ((rtx_class[(int) (((enum rtx_code) (cond0)->code))]) == '<'
       && (rtx_class[(int) (((enum rtx_code) (cond1)->code))]) == '<'
       && ((((enum rtx_code) (cond0)->code) == combine_reversed_comparison_code (cond1)
     && rtx_equal_p ((((cond0)->fld[0]).rtx), (((cond1)->fld[0]).rtx))
     && rtx_equal_p ((((cond0)->fld[1]).rtx), (((cond1)->fld[1]).rtx)))
    || ((swap_condition (((enum rtx_code) (cond0)->code))
         == combine_reversed_comparison_code (cond1))
        && rtx_equal_p ((((cond0)->fld[0]).rtx), (((cond1)->fld[1]).rtx))
        && rtx_equal_p ((((cond0)->fld[1]).rtx), (((cond1)->fld[0]).rtx))))
       && ! side_effects_p (x))
     {
       *ptrue = gen_binary (MULT, mode, op0, const_true_rtx);
       *pfalse = gen_binary (MULT, mode,
        (code == MINUS
         ? simplify_gen_unary (NEG, mode, op1,
          mode)
         : op1),
        const_true_rtx);
       return cond0;
     }
 }



      if ((1 == 1 || 1 == -1)
   && (code == MULT || code == AND || code == UMIN)
   && ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == MULT && ((enum rtx_code) ((((x)->fld[1]).rtx))->code) == MULT)
 {
   cond0 = ((((((x)->fld[0]).rtx))->fld[0]).rtx);
   cond1 = ((((((x)->fld[1]).rtx))->fld[0]).rtx);

   if ((rtx_class[(int) (((enum rtx_code) (cond0)->code))]) == '<'
       && (rtx_class[(int) (((enum rtx_code) (cond1)->code))]) == '<'
       && ((((enum rtx_code) (cond0)->code) == combine_reversed_comparison_code (cond1)
     && rtx_equal_p ((((cond0)->fld[0]).rtx), (((cond1)->fld[0]).rtx))
     && rtx_equal_p ((((cond0)->fld[1]).rtx), (((cond1)->fld[1]).rtx)))
    || ((swap_condition (((enum rtx_code) (cond0)->code))
         == combine_reversed_comparison_code (cond1))
        && rtx_equal_p ((((cond0)->fld[0]).rtx), (((cond1)->fld[1]).rtx))
        && rtx_equal_p ((((cond0)->fld[1]).rtx), (((cond1)->fld[0]).rtx))))
       && ! side_effects_p (x))
     {
       *ptrue = *pfalse = (const_int_rtx[64]);
       return cond0;
     }
 }
    }

  else if (code == IF_THEN_ELSE)
    {


      cond0 = (((x)->fld[0]).rtx);
      *ptrue = (((x)->fld[1]).rtx), *pfalse = (((x)->fld[2]).rtx);
      if (((enum rtx_code) (cond0)->code) == NE && (((cond0)->fld[1]).rtx) == (const_int_rtx[64]))
 return (((cond0)->fld[0]).rtx);
      else if (((enum rtx_code) (cond0)->code) == EQ && (((cond0)->fld[1]).rtx) == (const_int_rtx[64]))
 {
   *ptrue = (((x)->fld[2]).rtx), *pfalse = (((x)->fld[1]).rtx);
   return (((cond0)->fld[0]).rtx);
 }
      else
 return cond0;
    }



  else if (code == SUBREG
    && 0 != (cond0 = if_then_else_cond ((((x)->fld[0]).rtx),
            &true0, &false0)))
    {
      *ptrue = simplify_gen_subreg (mode, true0,
        ((enum machine_mode) ((((x)->fld[0]).rtx))->mode), (((x)->fld[1]).rtuint));
      *pfalse = simplify_gen_subreg (mode, false0,
         ((enum machine_mode) ((((x)->fld[0]).rtx))->mode), (((x)->fld[1]).rtuint));

      return cond0;
    }



  else if ((((enum rtx_code) (x)->code) == LABEL_REF || ((enum rtx_code) (x)->code) == SYMBOL_REF || ((enum rtx_code) (x)->code) == CONST_INT || ((enum rtx_code) (x)->code) == CONST_DOUBLE || ((enum rtx_code) (x)->code) == CONST || ((enum rtx_code) (x)->code) == HIGH || ((enum rtx_code) (x)->code) == CONST_VECTOR || ((enum rtx_code) (x)->code) == CONSTANT_P_RTX)
    || ((cond0 = get_last_value (x)) != 0 && (((enum rtx_code) (cond0)->code) == LABEL_REF || ((enum rtx_code) (cond0)->code) == SYMBOL_REF || ((enum rtx_code) (cond0)->code) == CONST_INT || ((enum rtx_code) (cond0)->code) == CONST_DOUBLE || ((enum rtx_code) (cond0)->code) == CONST || ((enum rtx_code) (cond0)->code) == HIGH || ((enum rtx_code) (cond0)->code) == CONST_VECTOR || ((enum rtx_code) (cond0)->code) == CONSTANT_P_RTX)))
    ;



  else if (mode == BImode)
    {
      *ptrue = gen_rtx_CONST_INT (VOIDmode, (long long) (1)), *pfalse = (const_int_rtx[64]);
      return x;
    }



  else if (x == (const_int_rtx[64 -1]) || x == (const_int_rtx[64])
    || (mode != VOIDmode
        && num_sign_bit_copies (x, mode) == (mode_bitsize[(int) (mode)])))
    {
      *ptrue = (const_int_rtx[64 -1]), *pfalse = (const_int_rtx[64]);
      return x;
    }


  else if (mode != VOIDmode
    && (mode_bitsize[(int) (mode)]) <= (8 * 8)
    && exact_log2_wide ((unsigned long long) (nz = nonzero_bits (x, mode))) >= 0)
    {
      *ptrue = gen_rtx_CONST_INT (VOIDmode, (long long) (trunc_int_for_mode (nz, mode))), *pfalse = (const_int_rtx[64]);
      return x;
    }


  *ptrue = *pfalse = x;
  return 0;
}
# 7383 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static rtx
known_cond (x, cond, reg, val)
     rtx x;
     enum rtx_code cond;
     rtx reg, val;
{
  enum rtx_code code = ((enum rtx_code) (x)->code);
  rtx temp;
  const char *fmt;
  int i, j;

  if (side_effects_p (x))
    return x;



  if (cond == EQ
      && rtx_equal_p (x, reg)
      && ! ((mode_class[(int) (((enum machine_mode) (x)->mode))]) == MODE_FLOAT || (mode_class[(int) (((enum machine_mode) (x)->mode))]) == MODE_COMPLEX_FLOAT || (mode_class[(int) (((enum machine_mode) (x)->mode))]) == MODE_VECTOR_FLOAT)
      && ! ((mode_class[(int) (((enum machine_mode) (val)->mode))]) == MODE_FLOAT || (mode_class[(int) (((enum machine_mode) (val)->mode))]) == MODE_COMPLEX_FLOAT || (mode_class[(int) (((enum machine_mode) (val)->mode))]) == MODE_VECTOR_FLOAT))
    return val;

  if (cond == UNEQ && rtx_equal_p (x, reg))
    return val;




  if (code == ABS && rtx_equal_p ((((x)->fld[0]).rtx), reg) && val == (const_int_rtx[64]))
    switch (cond)
      {
      case GE: case GT: case EQ:
 return (((x)->fld[0]).rtx);
      case LT: case LE:
 return simplify_gen_unary (NEG, ((enum machine_mode) ((((x)->fld[0]).rtx))->mode),
       (((x)->fld[0]).rtx),
       ((enum machine_mode) ((((x)->fld[0]).rtx))->mode));
      default:
 break;
      }




  else if ((rtx_class[(int) (code)]) == '<' || (rtx_class[(int) (code)]) == 'c')
    {
      if (rtx_equal_p ((((x)->fld[0]).rtx), val))
 cond = swap_condition (cond), temp = val, val = reg, reg = temp;

      if (rtx_equal_p ((((x)->fld[0]).rtx), reg) && rtx_equal_p ((((x)->fld[1]).rtx), val))
 {
   if ((rtx_class[(int) (code)]) == '<')
     {
       if (comparison_dominates_p (cond, code))
  return const_true_rtx;

       code = combine_reversed_comparison_code (x);
       if (code != UNKNOWN
    && comparison_dominates_p (cond, code))
  return (const_int_rtx[64]);
       else
  return x;
     }
   else if (code == SMAX || code == SMIN
     || code == UMIN || code == UMAX)
     {
       int unsignedp = (code == UMIN || code == UMAX);





       if ((code == SMAX || code == UMAX)
    && ! (cond == EQ || cond == NE))
  cond = reverse_condition (cond);

       switch (cond)
  {
  case GE: case GT:
    return unsignedp ? x : (((x)->fld[1]).rtx);
  case LE: case LT:
    return unsignedp ? x : (((x)->fld[0]).rtx);
  case GEU: case GTU:
    return unsignedp ? (((x)->fld[1]).rtx) : x;
  case LEU: case LTU:
    return unsignedp ? (((x)->fld[0]).rtx) : x;
  default:
    break;
  }
     }
 }
    }
  else if (code == SUBREG)
    {
      enum machine_mode inner_mode = ((enum machine_mode) ((((x)->fld[0]).rtx))->mode);
      rtx new, r = known_cond ((((x)->fld[0]).rtx), cond, reg, val);

      if ((((x)->fld[0]).rtx) != r)
 {


   new = simplify_subreg (((enum machine_mode) (x)->mode), r,
     inner_mode, (((x)->fld[1]).rtuint));
   if (new)
     return new;
   else
     do_SUBST(&((((x)->fld[0]).rtx)), (r));
 }

      return x;
    }






  else if (code == ZERO_EXTEND)
    {
      enum machine_mode inner_mode = ((enum machine_mode) ((((x)->fld[0]).rtx))->mode);
      rtx new, r = known_cond ((((x)->fld[0]).rtx), cond, reg, val);

      if ((((x)->fld[0]).rtx) != r)
 {


   new = simplify_unary_operation (ZERO_EXTEND, ((enum machine_mode) (x)->mode),
       r, inner_mode);
   if (new)
     return new;
   else
     do_SUBST(&((((x)->fld[0]).rtx)), (r));
 }

      return x;
    }

  fmt = (rtx_format[(int) (code)]);
  for (i = (rtx_length[(int) (code)]) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
 do_SUBST(&((((x)->fld[i]).rtx)), (known_cond ((((x)->fld[i]).rtx), cond, reg, val)));
      else if (fmt[i] == 'E')
 for (j = (((((x)->fld[i]).rtvec))->num_elem) - 1; j >= 0; j--)
   do_SUBST(&((((((x)->fld[i]).rtvec))->elem[j])), (known_cond ((((((x)->fld[i]).rtvec))->elem[j]), cond, reg, val)));

    }

  return x;
}




static int
rtx_equal_for_field_assignment_p (x, y)
     rtx x;
     rtx y;
{
  if (x == y || rtx_equal_p (x, y))
    return 1;

  if (x == 0 || y == 0 || ((enum machine_mode) (x)->mode) != ((enum machine_mode) (y)->mode))
    return 0;




  if (((enum rtx_code) (x)->code) == MEM && ((enum rtx_code) (y)->code) == SUBREG
      && ((enum rtx_code) ((((y)->fld[0]).rtx))->code) == MEM
      && rtx_equal_p ((((y)->fld[0]).rtx),
        gen_lowpart_for_combine (((enum machine_mode) ((((y)->fld[0]).rtx))->mode), x)))
    return 1;

  if (((enum rtx_code) (y)->code) == MEM && ((enum rtx_code) (x)->code) == SUBREG
      && ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == MEM
      && rtx_equal_p ((((x)->fld[0]).rtx),
        gen_lowpart_for_combine (((enum machine_mode) ((((x)->fld[0]).rtx))->mode), y)))
    return 1;






  return 0;
}






static rtx
make_field_assignment (x)
     rtx x;
{
  rtx dest = (((x)->fld[0]).rtx);
  rtx src = (((x)->fld[1]).rtx);
  rtx assign;
  rtx rhs, lhs;
  long long c1;
  long long pos;
  unsigned long long len = 0;
  rtx other;
  enum machine_mode mode;






  if (((enum rtx_code) (src)->code) == AND && ((enum rtx_code) ((((src)->fld[0]).rtx))->code) == ROTATE
      && ((enum rtx_code) (((((((src)->fld[0]).rtx))->fld[0]).rtx))->code) == CONST_INT
      && (((((((((src)->fld[0]).rtx))->fld[0]).rtx))->fld[0]).rtwint) == -2
      && rtx_equal_for_field_assignment_p (dest, (((src)->fld[1]).rtx)))
    {
      assign = make_extraction (VOIDmode, dest, 0, ((((((src)->fld[0]).rtx))->fld[1]).rtx),
    1, 1, 1, 0);
      if (assign != 0)
 return gen_rtx_fmt_ee (SET, (VOIDmode), (assign), ((const_int_rtx[64])));
      return x;
    }

  else if (((enum rtx_code) (src)->code) == AND && ((enum rtx_code) ((((src)->fld[0]).rtx))->code) == SUBREG
    && subreg_lowpart_p ((((src)->fld[0]).rtx))
    && ((mode_size[(int) (((enum machine_mode) ((((src)->fld[0]).rtx))->mode))])
        < (mode_size[(int) (((enum machine_mode) (((((((src)->fld[0]).rtx))->fld[0]).rtx))->mode))]))
    && ((enum rtx_code) (((((((src)->fld[0]).rtx))->fld[0]).rtx))->code) == ROTATE
    && ((((((((((((src)->fld[0]).rtx))->fld[0]).rtx))->fld[0]).rtx))->fld[0]).rtwint) == -2
    && rtx_equal_for_field_assignment_p (dest, (((src)->fld[1]).rtx)))
    {
      assign = make_extraction (VOIDmode, dest, 0,
    (((((((((src)->fld[0]).rtx))->fld[0]).rtx))->fld[1]).rtx),
    1, 1, 1, 0);
      if (assign != 0)
 return gen_rtx_fmt_ee (SET, (VOIDmode), (assign), ((const_int_rtx[64])));
      return x;
    }



  else if (((enum rtx_code) (src)->code) == IOR && ((enum rtx_code) ((((src)->fld[0]).rtx))->code) == ASHIFT
    && ((((((src)->fld[0]).rtx))->fld[0]).rtx) == (const_int_rtx[64 +1])
    && rtx_equal_for_field_assignment_p (dest, (((src)->fld[1]).rtx)))
    {
      assign = make_extraction (VOIDmode, dest, 0, ((((((src)->fld[0]).rtx))->fld[1]).rtx),
    1, 1, 1, 0);
      if (assign != 0)
 return gen_rtx_fmt_ee (SET, (VOIDmode), (assign), ((const_int_rtx[64 +1])));
      return x;
    }
# 7644 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  if (((enum rtx_code) (src)->code) != IOR && ((enum rtx_code) (src)->code) != XOR)
    return x;

  rhs = expand_compound_operation ((((src)->fld[0]).rtx));
  lhs = expand_compound_operation ((((src)->fld[1]).rtx));

  if (((enum rtx_code) (rhs)->code) == AND
      && ((enum rtx_code) ((((rhs)->fld[1]).rtx))->code) == CONST_INT
      && rtx_equal_for_field_assignment_p ((((rhs)->fld[0]).rtx), dest))
    c1 = ((((((rhs)->fld[1]).rtx))->fld[0]).rtwint), other = lhs;
  else if (((enum rtx_code) (lhs)->code) == AND
    && ((enum rtx_code) ((((lhs)->fld[1]).rtx))->code) == CONST_INT
    && rtx_equal_for_field_assignment_p ((((lhs)->fld[0]).rtx), dest))
    c1 = ((((((lhs)->fld[1]).rtx))->fld[0]).rtwint), other = rhs;
  else
    return x;

  pos = get_pos_from_mask ((~c1) & mode_mask_array[(int) (((enum machine_mode) (dest)->mode))], &len);
  if (pos < 0 || pos + len > (mode_bitsize[(int) (((enum machine_mode) (dest)->mode))])
      || (mode_bitsize[(int) (((enum machine_mode) (dest)->mode))]) > (8 * 8)
      || (c1 & nonzero_bits (other, ((enum machine_mode) (dest)->mode))) != 0)
    return x;

  assign = make_extraction (VOIDmode, dest, pos, (rtx) 0, len, 1, 1, 0);
  if (assign == 0)
    return x;



  mode = (((enum rtx_code) (assign)->code) == STRICT_LOW_PART
   ? ((enum machine_mode) ((((assign)->fld[0]).rtx))->mode) : ((enum machine_mode) (assign)->mode));




  src = force_to_mode (simplify_shift_const ((rtx) 0, LSHIFTRT,
          ((enum machine_mode) (src)->mode), other, pos),
         mode,
         (mode_bitsize[(int) (mode)]) >= (8 * 8)
         ? ~(unsigned long long) 0
         : ((unsigned long long) 1 << len) - 1,
         dest, 0);

  return gen_rtx_fmt_ee (SET, (VOIDmode), (assign), (src));
}




static rtx
apply_distributive_law (x)
     rtx x;
{
  enum rtx_code code = ((enum rtx_code) (x)->code);
  rtx lhs, rhs, other;
  rtx tem;
  enum rtx_code inner_code;




  if (((mode_class[(int) (((enum machine_mode) (x)->mode))]) == MODE_FLOAT || (mode_class[(int) (((enum machine_mode) (x)->mode))]) == MODE_COMPLEX_FLOAT || (mode_class[(int) (((enum machine_mode) (x)->mode))]) == MODE_VECTOR_FLOAT))
    return x;


  if (code != IOR && code != AND && code != XOR
      && code != PLUS && code != MINUS)
    return x;

  lhs = (((x)->fld[0]).rtx), rhs = (((x)->fld[1]).rtx);



  if ((rtx_class[(int) (((enum rtx_code) (lhs)->code))]) == 'o'
      || (rtx_class[(int) (((enum rtx_code) (rhs)->code))]) == 'o')
    return x;

  lhs = expand_compound_operation (lhs);
  rhs = expand_compound_operation (rhs);
  inner_code = ((enum rtx_code) (lhs)->code);
  if (inner_code != ((enum rtx_code) (rhs)->code))
    return x;


  switch (inner_code)
    {
    case LSHIFTRT:
    case ASHIFTRT:
    case AND:
    case IOR:

      if (code == PLUS || code == MINUS)
 return x;
      break;

    case MULT:
      if (code != PLUS && code != MINUS)
 return x;
      break;

    case ASHIFT:

      break;

    case SUBREG:
# 7760 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
      if (((enum machine_mode) ((((lhs)->fld[0]).rtx))->mode) != ((enum machine_mode) ((((rhs)->fld[0]).rtx))->mode)
   || (((lhs)->fld[1]).rtuint) != (((rhs)->fld[1]).rtuint)
   || ! subreg_lowpart_p (lhs)
   || ((mode_class[(int) (((enum machine_mode) (lhs)->mode))])
       != (mode_class[(int) (((enum machine_mode) ((((lhs)->fld[0]).rtx))->mode))]))
   || ((mode_size[(int) (((enum machine_mode) (lhs)->mode))])
       > (mode_size[(int) (((enum machine_mode) ((((lhs)->fld[0]).rtx))->mode))]))
   || (mode_size[(int) (((enum machine_mode) ((((lhs)->fld[0]).rtx))->mode))]) > ((target_flags & 0x02000000) ? 8 : 4))
 return x;

      tem = gen_binary (code, ((enum machine_mode) ((((lhs)->fld[0]).rtx))->mode),
   (((lhs)->fld[0]).rtx), (((rhs)->fld[0]).rtx));
      return gen_lowpart_for_combine (((enum machine_mode) (x)->mode), tem);

    default:
      return x;
    }





  if ((rtx_class[(int) (inner_code)]) == 'c'
      && rtx_equal_p ((((lhs)->fld[0]).rtx), (((rhs)->fld[0]).rtx)))
    other = (((lhs)->fld[0]).rtx), lhs = (((lhs)->fld[1]).rtx), rhs = (((rhs)->fld[1]).rtx);
  else if ((rtx_class[(int) (inner_code)]) == 'c'
    && rtx_equal_p ((((lhs)->fld[0]).rtx), (((rhs)->fld[1]).rtx)))
    other = (((lhs)->fld[0]).rtx), lhs = (((lhs)->fld[1]).rtx), rhs = (((rhs)->fld[0]).rtx);
  else if ((rtx_class[(int) (inner_code)]) == 'c'
    && rtx_equal_p ((((lhs)->fld[1]).rtx), (((rhs)->fld[0]).rtx)))
    other = (((lhs)->fld[1]).rtx), lhs = (((lhs)->fld[0]).rtx), rhs = (((rhs)->fld[1]).rtx);
  else if (rtx_equal_p ((((lhs)->fld[1]).rtx), (((rhs)->fld[1]).rtx)))
    other = (((lhs)->fld[1]).rtx), lhs = (((lhs)->fld[0]).rtx), rhs = (((rhs)->fld[0]).rtx);
  else
    return x;


  tem = gen_binary (code, ((enum machine_mode) (x)->mode), lhs, rhs);



  if (code == XOR && inner_code == IOR)
    {
      inner_code = AND;
      other = simplify_gen_unary (NOT, ((enum machine_mode) (x)->mode), other, ((enum machine_mode) (x)->mode));
    }




  return gen_binary (inner_code, ((enum machine_mode) (x)->mode),
       apply_distributive_law (tem), other);
}







static rtx
simplify_and_const_int (x, mode, varop, constop)
     rtx x;
     enum machine_mode mode;
     rtx varop;
     unsigned long long constop;
{
  unsigned long long nonzero;
  int i;
# 7837 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  varop = force_to_mode (varop, mode, constop, (rtx) 0, 0);


  if (((enum rtx_code) (varop)->code) == CLOBBER)
    return varop;



  if (((enum rtx_code) (varop)->code) == CONST_INT)
    return gen_rtx_CONST_INT (VOIDmode, (long long) (trunc_int_for_mode ((((varop)->fld[0]).rtwint) & constop, mode)));





  nonzero = nonzero_bits (varop, mode) & mode_mask_array[(int) (mode)];





  constop &= nonzero;


  if (constop == 0)
    return (const_int_rtx[64]);



  if (((enum rtx_code) (varop)->code) == NEG && nonzero_bits ((((varop)->fld[0]).rtx), mode) == 1
      && (i = exact_log2_wide ((unsigned long long) (constop))) >= 0)
    return simplify_shift_const ((rtx) 0, ASHIFT, mode, (((varop)->fld[0]).rtx), i);







  if (((enum rtx_code) (varop)->code) == IOR || ((enum rtx_code) (varop)->code) == XOR)
    return
      gen_lowpart_for_combine
 (mode,
  apply_distributive_law
  (gen_binary (((enum rtx_code) (varop)->code), ((enum machine_mode) (varop)->mode),
        simplify_and_const_int ((rtx) 0, ((enum machine_mode) (varop)->mode),
           (((varop)->fld[0]).rtx), constop),
        simplify_and_const_int ((rtx) 0, ((enum machine_mode) (varop)->mode),
           (((varop)->fld[1]).rtx), constop))));





  if (((enum rtx_code) (varop)->code) == PLUS
      && exact_log2_wide ((unsigned long long) (constop + 1)) >= 0)
    {
      rtx o0, o1;

      o0 = simplify_and_const_int ((rtx) 0, mode, (((varop)->fld[0]).rtx), constop);
      o1 = simplify_and_const_int ((rtx) 0, mode, (((varop)->fld[1]).rtx), constop);
      if (o0 == (const_int_rtx[64]))
 return o1;
      if (o1 == (const_int_rtx[64]))
 return o0;
    }



  if (x && ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == SUBREG
      && ((enum machine_mode) ((((x)->fld[0]).rtx))->mode) == mode
      && ((((((x)->fld[0]).rtx))->fld[0]).rtx) == varop)
    varop = (((x)->fld[0]).rtx);
  else
    varop = gen_lowpart_for_combine (mode, varop);


  if (((enum rtx_code) (varop)->code) == CLOBBER)
    return x ? x : varop;


  if (constop == nonzero)
    x = varop;
  else
    {

      constop = trunc_int_for_mode (constop, mode);

      if (x == 0 || ((enum rtx_code) (x)->code) != AND || ((enum machine_mode) (x)->mode) != mode)
 x = gen_binary (AND, mode, varop, gen_rtx_CONST_INT (VOIDmode, (long long) (constop)));

      else
 {
   if (((enum rtx_code) ((((x)->fld[1]).rtx))->code) != CONST_INT
       || (unsigned long long) ((((((x)->fld[1]).rtx))->fld[0]).rtwint) != constop)
     do_SUBST(&((((x)->fld[1]).rtx)), (gen_rtx_CONST_INT (VOIDmode, (long long) (constop))));

   do_SUBST(&((((x)->fld[0]).rtx)), (varop));
 }
    }

  return x;
}
# 7954 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static unsigned long long
nonzero_bits (x, mode)
     rtx x;
     enum machine_mode mode;
{
  unsigned long long nonzero = mode_mask_array[(int) (mode)];
  unsigned long long inner_nz;
  enum rtx_code code;
  unsigned int mode_width = (mode_bitsize[(int) (mode)]);
  rtx tem;


  if (((mode_class[(int) (((enum machine_mode) (x)->mode))]) == MODE_FLOAT || (mode_class[(int) (((enum machine_mode) (x)->mode))]) == MODE_COMPLEX_FLOAT || (mode_class[(int) (((enum machine_mode) (x)->mode))]) == MODE_VECTOR_FLOAT) || ((mode_class[(int) (mode)]) == MODE_FLOAT || (mode_class[(int) (mode)]) == MODE_COMPLEX_FLOAT || (mode_class[(int) (mode)]) == MODE_VECTOR_FLOAT))
    return nonzero;


  if ((mode_bitsize[(int) (((enum machine_mode) (x)->mode))]) > mode_width)
    {
      mode = ((enum machine_mode) (x)->mode);
      nonzero = mode_mask_array[(int) (mode)];
      mode_width = (mode_bitsize[(int) (mode)]);
    }

  if (mode_width > (8 * 8))


    return nonzero;
# 7990 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  if (((enum machine_mode) (x)->mode) != VOIDmode && ((enum machine_mode) (x)->mode) != mode
      && (mode_bitsize[(int) (((enum machine_mode) (x)->mode))]) <= ((target_flags & 0x02000000) ? 64 : 32)
      && (mode_bitsize[(int) (((enum machine_mode) (x)->mode))]) <= (8 * 8)
      && (mode_bitsize[(int) (mode)]) > (mode_bitsize[(int) (((enum machine_mode) (x)->mode))]))
    {
      nonzero &= nonzero_bits (x, ((enum machine_mode) (x)->mode));
      nonzero |= mode_mask_array[(int) (mode)] & ~mode_mask_array[(int) (((enum machine_mode) (x)->mode))];
      return nonzero;
    }


  code = ((enum rtx_code) (x)->code);
  switch (code)
    {
    case REG:
# 8017 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
      if ((x == (global_rtl[GR_STACK_POINTER])
    || x == (global_rtl[GR_FRAME_POINTER])
    || x == (global_rtl[GR_ARG_POINTER]))
   && (cfun->emit->regno_pointer_align[(((x)->fld[0]).rtuint)]))
 {
   unsigned long long alignment
     = (cfun->emit->regno_pointer_align[(((x)->fld[0]).rtuint)]) / 8;





   if (x == (global_rtl[GR_STACK_POINTER]) && ((!(target_flags & 0x00000800)) && !(target_flags & 0x00001000)))
     alignment = ((((target_flags & 0x02000000) ? (((1) + 7) & (-8)) : (((1) + 1) & (-2)))) < (alignment) ? (((target_flags & 0x02000000) ? (((1) + 7) & (-8)) : (((1) + 1) & (-2)))) : (alignment));


   nonzero &= ~(alignment - 1);
 }






      if (reg_last_set_value[(((x)->fld[0]).rtuint)] != 0
   && (reg_last_set_mode[(((x)->fld[0]).rtuint)] == mode
       || ((mode_class[(int) (reg_last_set_mode[(((x)->fld[0]).rtuint)])]) == MODE_INT
    && (mode_class[(int) (mode)]) == MODE_INT))
   && (reg_last_set_label[(((x)->fld[0]).rtuint)] == label_tick
       || ((((x)->fld[0]).rtuint) >= 53
    && (((reg_n_info)->data.reg[(((x)->fld[0]).rtuint)])->sets) == 1
    && ! bitmap_bit_p ((((basic_block_info)->data.bb[(0)]))->global_live_at_start, (((x)->fld[0]).rtuint))))

   && ((((reg_last_set[(((x)->fld[0]).rtuint)])->fld[0]).rtint) > max_uid_cuid ? insn_cuid (reg_last_set[(((x)->fld[0]).rtuint)]) : uid_cuid[(((reg_last_set[(((x)->fld[0]).rtuint)])->fld[0]).rtint)]) < subst_low_cuid)
 return reg_last_set_nonzero_bits[(((x)->fld[0]).rtuint)] & nonzero;

      tem = get_last_value (x);

      if (tem)
 {
# 8077 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
   return nonzero_bits (tem, mode) & nonzero;
 }
      else if (nonzero_sign_valid && reg_nonzero_bits[(((x)->fld[0]).rtuint)])
 {
   unsigned long long mask = reg_nonzero_bits[(((x)->fld[0]).rtuint)];

   if ((mode_bitsize[(int) (((enum machine_mode) (x)->mode))]) < mode_width)

     mask |= mode_mask_array[(int) (mode)] ^ mode_mask_array[(int) (((enum machine_mode) (x)->mode))];
   return nonzero & mask;
 }
      else
 return nonzero;

    case CONST_INT:







      return (((x)->fld[0]).rtwint);

    case MEM:







      break;

    case EQ: case NE:
    case UNEQ: case LTGT:
    case GT: case GTU: case UNGT:
    case LT: case LTU: case UNLT:
    case GE: case GEU: case UNGE:
    case LE: case LEU: case UNLE:
    case UNORDERED: case ORDERED:





      if ((mode_class[(int) (mode)]) == MODE_INT
   && mode_width <= (8 * 8))
 nonzero = 1;
      break;

    case NEG:
# 8137 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
      if ((mode_size[(int) (((enum machine_mode) (x)->mode))]) < mode_width)
 nonzero |= (mode_mask_array[(int) (mode)] & ~mode_mask_array[(int) (((enum machine_mode) (x)->mode))]);
      break;

    case ABS:







      break;

    case TRUNCATE:
      nonzero &= (nonzero_bits ((((x)->fld[0]).rtx), mode) & mode_mask_array[(int) (mode)]);
      break;

    case ZERO_EXTEND:
      nonzero &= nonzero_bits ((((x)->fld[0]).rtx), mode);
      if (((enum machine_mode) ((((x)->fld[0]).rtx))->mode) != VOIDmode)
 nonzero &= mode_mask_array[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))];
      break;

    case SIGN_EXTEND:



      inner_nz = nonzero_bits ((((x)->fld[0]).rtx), mode);
      if (((enum machine_mode) ((((x)->fld[0]).rtx))->mode) != VOIDmode)
 {
   inner_nz &= mode_mask_array[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))];
   if (inner_nz
       & (((long long) 1
    << ((mode_bitsize[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))]) - 1))))
     inner_nz |= (mode_mask_array[(int) (mode)]
    & ~mode_mask_array[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))]);
 }

      nonzero &= inner_nz;
      break;

    case AND:
      nonzero &= (nonzero_bits ((((x)->fld[0]).rtx), mode)
    & nonzero_bits ((((x)->fld[1]).rtx), mode));
      break;

    case XOR: case IOR:
    case UMIN: case UMAX: case SMIN: case SMAX:
      {
 unsigned long long nonzero0 = nonzero_bits ((((x)->fld[0]).rtx), mode);



 if ((nonzero & nonzero0) != nonzero)
   nonzero &= (nonzero0 | nonzero_bits ((((x)->fld[1]).rtx), mode));
      }
      break;

    case PLUS: case MINUS:
    case MULT:
    case DIV: case UDIV:
    case MOD: case UMOD:




      {
 unsigned long long nz0 = nonzero_bits ((((x)->fld[0]).rtx), mode);
 unsigned long long nz1 = nonzero_bits ((((x)->fld[1]).rtx), mode);
 int width0 = floor_log2_wide ((unsigned long long) (nz0)) + 1;
 int width1 = floor_log2_wide ((unsigned long long) (nz1)) + 1;
 int low0 = floor_log2_wide ((unsigned long long) (nz0 & -nz0));
 int low1 = floor_log2_wide ((unsigned long long) (nz1 & -nz1));
 long long op0_maybe_minusp
   = (nz0 & ((long long) 1 << (mode_width - 1)));
 long long op1_maybe_minusp
   = (nz1 & ((long long) 1 << (mode_width - 1)));
 unsigned int result_width = mode_width;
 int result_low = 0;

 switch (code)
   {
   case PLUS:
     result_width = ((width0) > (width1) ? (width0) : (width1)) + 1;
     result_low = ((low0) < (low1) ? (low0) : (low1));
     break;
   case MINUS:
     result_low = ((low0) < (low1) ? (low0) : (low1));
     break;
   case MULT:
     result_width = width0 + width1;
     result_low = low0 + low1;
     break;
   case DIV:
     if (width1 == 0)
       break;
     if (! op0_maybe_minusp && ! op1_maybe_minusp)
       result_width = width0;
     break;
   case UDIV:
     if (width1 == 0)
       break;
     result_width = width0;
     break;
   case MOD:
     if (width1 == 0)
       break;
     if (! op0_maybe_minusp && ! op1_maybe_minusp)
       result_width = ((width0) < (width1) ? (width0) : (width1));
     result_low = ((low0) < (low1) ? (low0) : (low1));
     break;
   case UMOD:
     if (width1 == 0)
       break;
     result_width = ((width0) < (width1) ? (width0) : (width1));
     result_low = ((low0) < (low1) ? (low0) : (low1));
     break;
   default:
     fancy_abort ("/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c", 8256, __FUNCTION__);
   }

 if (result_width < mode_width)
   nonzero &= ((long long) 1 << result_width) - 1;

 if (result_low > 0)
   nonzero &= ~(((long long) 1 << result_low) - 1);
# 8274 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
      }
      break;

    case ZERO_EXTRACT:
      if (((enum rtx_code) ((((x)->fld[1]).rtx))->code) == CONST_INT
   && ((((((x)->fld[1]).rtx))->fld[0]).rtwint) < (8 * 8))
 nonzero &= ((long long) 1 << ((((((x)->fld[1]).rtx))->fld[0]).rtwint)) - 1;
      break;

    case SUBREG:




      if (((x)->in_struct) && ((x)->unchanging))
 nonzero = (mode_mask_array[(int) (((enum machine_mode) (x)->mode))]
     & nonzero_bits ((((x)->fld[0]).rtx), ((enum machine_mode) (x)->mode)));




      if ((mode_bitsize[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))]) <= ((target_flags & 0x02000000) ? 64 : 32)
   && ((mode_bitsize[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))])
       <= (8 * 8)))
 {
   nonzero &= nonzero_bits ((((x)->fld[0]).rtx), mode);
# 8311 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
     {



       if ((mode_size[(int) (((enum machine_mode) (x)->mode))])
    > (mode_size[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))]))
  nonzero |= (mode_mask_array[(int) (((enum machine_mode) (x)->mode))]
       & ~mode_mask_array[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))]);
     }
 }
      break;

    case ASHIFTRT:
    case LSHIFTRT:
    case ASHIFT:
    case ROTATE:






      if (((enum rtx_code) ((((x)->fld[1]).rtx))->code) == CONST_INT
   && ((((((x)->fld[1]).rtx))->fld[0]).rtwint) >= 0
   && ((((((x)->fld[1]).rtx))->fld[0]).rtwint) < (8 * 8))
 {
   enum machine_mode inner_mode = ((enum machine_mode) (x)->mode);
   unsigned int width = (mode_bitsize[(int) (inner_mode)]);
   int count = ((((((x)->fld[1]).rtx))->fld[0]).rtwint);
   unsigned long long mode_mask = mode_mask_array[(int) (inner_mode)];
   unsigned long long op_nonzero = nonzero_bits ((((x)->fld[0]).rtx), mode);
   unsigned long long inner = op_nonzero & mode_mask;
   unsigned long long outer = 0;

   if (mode_width > width)
     outer = (op_nonzero & nonzero & ~mode_mask);

   if (code == LSHIFTRT)
     inner >>= count;
   else if (code == ASHIFTRT)
     {
       inner >>= count;




       if (inner & ((long long) 1 << (width - 1 - count)))
  inner |= (((long long) 1 << count) - 1) << (width - count);
     }
   else if (code == ASHIFT)
     inner <<= count;
   else
     inner = ((inner << (count % width)
        | (inner >> (width - (count % width)))) & mode_mask);

   nonzero &= (outer | inner);
 }
      break;

    case FFS:

      nonzero = ((long long) 1 << (floor_log2_wide ((unsigned long long) (mode_width)) + 1)) - 1;
      break;

    case IF_THEN_ELSE:
      nonzero &= (nonzero_bits ((((x)->fld[1]).rtx), mode)
    | nonzero_bits ((((x)->fld[2]).rtx), mode));
      break;

    default:
      break;
    }

  return nonzero;
}
# 8395 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static unsigned int
num_sign_bit_copies (x, mode)
     rtx x;
     enum machine_mode mode;
{
  enum rtx_code code = ((enum rtx_code) (x)->code);
  unsigned int bitwidth;
  int num0, num1, result;
  unsigned long long nonzero;
  rtx tem;





  if (mode == VOIDmode)
    mode = ((enum machine_mode) (x)->mode);

  if (mode == VOIDmode || ((mode_class[(int) (mode)]) == MODE_FLOAT || (mode_class[(int) (mode)]) == MODE_COMPLEX_FLOAT || (mode_class[(int) (mode)]) == MODE_VECTOR_FLOAT) || ((mode_class[(int) (((enum machine_mode) (x)->mode))]) == MODE_FLOAT || (mode_class[(int) (((enum machine_mode) (x)->mode))]) == MODE_COMPLEX_FLOAT || (mode_class[(int) (((enum machine_mode) (x)->mode))]) == MODE_VECTOR_FLOAT))
    return 1;

  bitwidth = (mode_bitsize[(int) (mode)]);


  if (bitwidth < (mode_bitsize[(int) (((enum machine_mode) (x)->mode))]))
    {
      num0 = num_sign_bit_copies (x, ((enum machine_mode) (x)->mode));
      return ((1) > (num0 - (int) ((mode_bitsize[(int) (((enum machine_mode) (x)->mode))]) - bitwidth)) ? (1) : (num0 - (int) ((mode_bitsize[(int) (((enum machine_mode) (x)->mode))]) - bitwidth)));

    }

  if (((enum machine_mode) (x)->mode) != VOIDmode && bitwidth > (mode_bitsize[(int) (((enum machine_mode) (x)->mode))]))
    {




      return 1;
# 8444 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
    }

  switch (code)
    {
    case REG:
# 8458 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
      if (reg_last_set_value[(((x)->fld[0]).rtuint)] != 0
   && reg_last_set_mode[(((x)->fld[0]).rtuint)] == mode
   && (reg_last_set_label[(((x)->fld[0]).rtuint)] == label_tick
       || ((((x)->fld[0]).rtuint) >= 53
    && (((reg_n_info)->data.reg[(((x)->fld[0]).rtuint)])->sets) == 1
    && ! bitmap_bit_p ((((basic_block_info)->data.bb[(0)]))->global_live_at_start, (((x)->fld[0]).rtuint))))

   && ((((reg_last_set[(((x)->fld[0]).rtuint)])->fld[0]).rtint) > max_uid_cuid ? insn_cuid (reg_last_set[(((x)->fld[0]).rtuint)]) : uid_cuid[(((reg_last_set[(((x)->fld[0]).rtuint)])->fld[0]).rtint)]) < subst_low_cuid)
 return reg_last_set_sign_bit_copies[(((x)->fld[0]).rtuint)];

      tem = get_last_value (x);
      if (tem != 0)
 return num_sign_bit_copies (tem, mode);

      if (nonzero_sign_valid && reg_sign_bit_copies[(((x)->fld[0]).rtuint)] != 0
   && (mode_bitsize[(int) (((enum machine_mode) (x)->mode))]) == bitwidth)
 return reg_sign_bit_copies[(((x)->fld[0]).rtuint)];
      break;

    case MEM:






      break;

    case CONST_INT:


      nonzero = (((x)->fld[0]).rtwint) & mode_mask_array[(int) (mode)];
      if (bitwidth <= (8 * 8)
   && (nonzero & ((long long) 1 << (bitwidth - 1))) != 0)
 nonzero = (~nonzero) & mode_mask_array[(int) (mode)];

      return (nonzero == 0 ? bitwidth : bitwidth - floor_log2_wide ((unsigned long long) (nonzero)) - 1);

    case SUBREG:




      if (((x)->in_struct) && ! ((x)->unchanging))
 {
   num0 = num_sign_bit_copies ((((x)->fld[0]).rtx), mode);
   return (((int) bitwidth - (int) (mode_bitsize[(int) (((enum machine_mode) (x)->mode))]) + 1) > (num0) ? ((int) bitwidth - (int) (mode_bitsize[(int) (((enum machine_mode) (x)->mode))]) + 1) : (num0));


 }


      if (bitwidth <= (mode_bitsize[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))]))
 {
   num0 = num_sign_bit_copies ((((x)->fld[0]).rtx), VOIDmode);
   return ((1) > ((num0 - (int) ((mode_bitsize[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))]) - bitwidth))) ? (1) : ((num0 - (int) ((mode_bitsize[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))]) - bitwidth))));


 }
# 8536 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
      break;

    case SIGN_EXTRACT:
      if (((enum rtx_code) ((((x)->fld[1]).rtx))->code) == CONST_INT)
 return ((1) > ((int) bitwidth - ((((((x)->fld[1]).rtx))->fld[0]).rtwint)) ? (1) : ((int) bitwidth - ((((((x)->fld[1]).rtx))->fld[0]).rtwint)));
      break;

    case SIGN_EXTEND:
      return (bitwidth - (mode_bitsize[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))])
       + num_sign_bit_copies ((((x)->fld[0]).rtx), VOIDmode));

    case TRUNCATE:

      num0 = num_sign_bit_copies ((((x)->fld[0]).rtx), VOIDmode);
      return ((1) > ((num0 - (int) ((mode_bitsize[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))]) - bitwidth))) ? (1) : ((num0 - (int) ((mode_bitsize[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))]) - bitwidth))));


    case NOT:
      return num_sign_bit_copies ((((x)->fld[0]).rtx), mode);

    case ROTATE: case ROTATERT:



      if (((enum rtx_code) ((((x)->fld[1]).rtx))->code) == CONST_INT
   && ((((((x)->fld[1]).rtx))->fld[0]).rtwint) >= 0
   && ((((((x)->fld[1]).rtx))->fld[0]).rtwint) < (int) bitwidth)
 {
   num0 = num_sign_bit_copies ((((x)->fld[0]).rtx), mode);
   return ((1) > (num0 - (code == ROTATE ? ((((((x)->fld[1]).rtx))->fld[0]).rtwint) : (int) bitwidth - ((((((x)->fld[1]).rtx))->fld[0]).rtwint))) ? (1) : (num0 - (code == ROTATE ? ((((((x)->fld[1]).rtx))->fld[0]).rtwint) : (int) bitwidth - ((((((x)->fld[1]).rtx))->fld[0]).rtwint))));

 }
      break;

    case NEG:




      num0 = num_sign_bit_copies ((((x)->fld[0]).rtx), mode);
      if (bitwidth > (8 * 8))
 return num0 > 1 ? num0 - 1 : 1;

      nonzero = nonzero_bits ((((x)->fld[0]).rtx), mode);
      if (nonzero == 1)
 return bitwidth;

      if (num0 > 1
   && (((long long) 1 << (bitwidth - 1)) & nonzero))
 num0--;

      return num0;

    case IOR: case AND: case XOR:
    case SMIN: case SMAX: case UMIN: case UMAX:


      num0 = num_sign_bit_copies ((((x)->fld[0]).rtx), mode);
      num1 = num_sign_bit_copies ((((x)->fld[1]).rtx), mode);
      return ((num0) < (num1) ? (num0) : (num1));

    case PLUS: case MINUS:





      if (code == PLUS && (((x)->fld[1]).rtx) == (const_int_rtx[64 -1])
   && bitwidth <= (8 * 8))
 {
   nonzero = nonzero_bits ((((x)->fld[0]).rtx), mode);
   if ((((long long) 1 << (bitwidth - 1)) & nonzero) == 0)
     return (nonzero == 1 || nonzero == 0 ? bitwidth
      : bitwidth - floor_log2_wide ((unsigned long long) (nonzero)) - 1);
 }

      num0 = num_sign_bit_copies ((((x)->fld[0]).rtx), mode);
      num1 = num_sign_bit_copies ((((x)->fld[1]).rtx), mode);
      result = ((1) > (((num0) < (num1) ? (num0) : (num1)) - 1) ? (1) : (((num0) < (num1) ? (num0) : (num1)) - 1));
# 8627 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
      return result;

    case MULT:





      num0 = num_sign_bit_copies ((((x)->fld[0]).rtx), mode);
      num1 = num_sign_bit_copies ((((x)->fld[1]).rtx), mode);

      result = bitwidth - (bitwidth - num0) - (bitwidth - num1);
      if (result > 0
   && (bitwidth > (8 * 8)
       || (((nonzero_bits ((((x)->fld[0]).rtx), mode)
      & ((long long) 1 << (bitwidth - 1))) != 0)
    && ((nonzero_bits ((((x)->fld[1]).rtx), mode)
         & ((long long) 1 << (bitwidth - 1))) != 0))))
 result--;

      return ((1) > (result) ? (1) : (result));

    case UDIV:



      if (bitwidth > (8 * 8))
 return 1;
      else if ((nonzero_bits ((((x)->fld[0]).rtx), mode)
  & ((long long) 1 << (bitwidth - 1))) != 0)
 return 1;
      else
 return num_sign_bit_copies ((((x)->fld[0]).rtx), mode);

    case UMOD:

      return num_sign_bit_copies ((((x)->fld[1]).rtx), mode);

    case DIV:



      result = num_sign_bit_copies ((((x)->fld[0]).rtx), mode);
      if (result > 1
   && (bitwidth > (8 * 8)
       || (nonzero_bits ((((x)->fld[1]).rtx), mode)
    & ((long long) 1 << (bitwidth - 1))) != 0))
 result--;

      return result;

    case MOD:
      result = num_sign_bit_copies ((((x)->fld[1]).rtx), mode);
      if (result > 1
   && (bitwidth > (8 * 8)
       || (nonzero_bits ((((x)->fld[1]).rtx), mode)
    & ((long long) 1 << (bitwidth - 1))) != 0))
 result--;

      return result;

    case ASHIFTRT:


      num0 = num_sign_bit_copies ((((x)->fld[0]).rtx), mode);
      if (((enum rtx_code) ((((x)->fld[1]).rtx))->code) == CONST_INT
   && ((((((x)->fld[1]).rtx))->fld[0]).rtwint) > 0)
 num0 = (((int) bitwidth) < (num0 + ((((((x)->fld[1]).rtx))->fld[0]).rtwint)) ? ((int) bitwidth) : (num0 + ((((((x)->fld[1]).rtx))->fld[0]).rtwint)));

      return num0;

    case ASHIFT:

      if (((enum rtx_code) ((((x)->fld[1]).rtx))->code) != CONST_INT
   || ((((((x)->fld[1]).rtx))->fld[0]).rtwint) < 0
   || ((((((x)->fld[1]).rtx))->fld[0]).rtwint) >= (int) bitwidth)
 return 1;

      num0 = num_sign_bit_copies ((((x)->fld[0]).rtx), mode);
      return ((1) > (num0 - ((((((x)->fld[1]).rtx))->fld[0]).rtwint)) ? (1) : (num0 - ((((((x)->fld[1]).rtx))->fld[0]).rtwint)));

    case IF_THEN_ELSE:
      num0 = num_sign_bit_copies ((((x)->fld[1]).rtx), mode);
      num1 = num_sign_bit_copies ((((x)->fld[2]).rtx), mode);
      return ((num0) < (num1) ? (num0) : (num1));

    case EQ: case NE: case GE: case GT: case LE: case LT:
    case UNEQ: case LTGT: case UNGE: case UNGT: case UNLE: case UNLT:
    case GEU: case GTU: case LEU: case LTU:
    case UNORDERED: case ORDERED:


      nonzero = 1;
      if (bitwidth <= (8 * 8)
   && (nonzero & ((long long) 1 << (bitwidth - 1))) != 0)
 nonzero = (~nonzero) & mode_mask_array[(int) (mode)];

      return (nonzero == 0 ? bitwidth : bitwidth - floor_log2_wide ((unsigned long long) (nonzero)) - 1);
      break;

    default:
      break;
    }






  if (bitwidth > (8 * 8))
    return 1;

  nonzero = nonzero_bits (x, mode);
  return (nonzero & ((long long) 1 << (bitwidth - 1))
   ? 1 : bitwidth - floor_log2_wide ((unsigned long long) (nonzero)) - 1);
}
# 8755 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
unsigned int
extended_count (x, mode, unsignedp)
     rtx x;
     enum machine_mode mode;
     int unsignedp;
{
  if (nonzero_sign_valid == 0)
    return 0;

  return (unsignedp
   ? ((mode_bitsize[(int) (mode)]) <= (8 * 8)
      ? ((mode_bitsize[(int) (mode)]) - 1
  - floor_log2_wide ((unsigned long long) (nonzero_bits (x, mode))))
      : 0)
   : num_sign_bit_copies (x, mode) - 1);
}
# 8793 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static int
merge_outer_ops (pop0, pconst0, op1, const1, mode, pcomp_p)
     enum rtx_code *pop0;
     long long *pconst0;
     enum rtx_code op1;
     long long const1;
     enum machine_mode mode;
     int *pcomp_p;
{
  enum rtx_code op0 = *pop0;
  long long const0 = *pconst0;

  const0 &= mode_mask_array[(int) (mode)];
  const1 &= mode_mask_array[(int) (mode)];


  if (op0 == AND)
    const1 &= const0;




  if (op1 == NIL || op0 == SET)
    return 1;

  else if (op0 == NIL)
    op0 = op1, const0 = const1;

  else if (op0 == op1)
    {
      switch (op0)
 {
 case AND:
   const0 &= const1;
   break;
 case IOR:
   const0 |= const1;
   break;
 case XOR:
   const0 ^= const1;
   break;
 case PLUS:
   const0 += const1;
   break;
 case NEG:
   op0 = NIL;
   break;
 default:
   break;
 }
    }


  else if (op0 == PLUS || op1 == PLUS || op0 == NEG || op1 == NEG)
    return 0;



  else if (const0 != const1)
    return 0;

  else
    switch (op0)
      {
      case IOR:
 if (op1 == AND)

   op0 = SET;
 else

   {;}
 break;

      case XOR:
 if (op1 == AND)

   op0 = AND, *pcomp_p = 1;
 else

   op0 = AND, *pconst0 = ~const0;
 break;

      case AND:
 if (op1 == IOR)

 op0 = SET;
 else

   *pcomp_p = 1;
 break;
      default:
 break;
      }


  const0 &= mode_mask_array[(int) (mode)];
  if (const0 == 0
      && (op0 == IOR || op0 == XOR || op0 == PLUS))
    op0 = NIL;
  else if (const0 == 0 && op0 == AND)
    op0 = SET;
  else if ((unsigned long long) const0 == mode_mask_array[(int) (mode)]
    && op0 == AND)
    op0 = NIL;




  const0 = trunc_int_for_mode (const0, mode);

  *pop0 = op0;
  *pconst0 = const0;

  return 1;
}
# 8917 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static rtx
simplify_shift_const (x, code, result_mode, varop, orig_count)
     rtx x;
     enum rtx_code code;
     enum machine_mode result_mode;
     rtx varop;
     int orig_count;
{
  enum rtx_code orig_code = code;
  unsigned int count;
  int signed_count;
  enum machine_mode mode = result_mode;
  enum machine_mode shift_mode, tmode;
  unsigned int mode_words
    = ((mode_size[(int) (mode)]) + (((target_flags & 0x02000000) ? 8 : 4) - 1)) / ((target_flags & 0x02000000) ? 8 : 4);

  enum rtx_code outer_op = NIL;
  long long outer_const = 0;
  rtx const_rtx;
  int complement_p = 0;
  rtx new;
# 8950 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  if (orig_count < 0 || orig_count >= (int) (mode_bitsize[(int) (mode)]))
    {
      if (x)
 return x;

      return gen_rtx_fmt_ee (code, mode, varop, gen_rtx_CONST_INT (VOIDmode, (long long) (orig_count)));
    }

  count = orig_count;




  while (count != 0)
    {


      if (((enum rtx_code) (varop)->code) == CLOBBER)
 return varop;



      if (complement_p)
 break;


      if (code == ROTATERT)
 code = ROTATE, count = (mode_bitsize[(int) (result_mode)]) - count;





      shift_mode
 = (code == ASHIFTRT || code == LSHIFTRT || code == ROTATE
    ? result_mode : mode);
# 8997 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
      if (count > (mode_bitsize[(int) (shift_mode)]) - 1)
 {
   if (code == ASHIFTRT)
     count = (mode_bitsize[(int) (shift_mode)]) - 1;
   else if (code == ROTATE || code == ROTATERT)
     count %= (mode_bitsize[(int) (shift_mode)]);
   else
     {


       varop = (const_int_rtx[64]);
       count = 0;
       break;
     }
 }



      if (code == ASHIFTRT
   && (num_sign_bit_copies (varop, shift_mode)
       == (mode_bitsize[(int) (shift_mode)])))
 {
   count = 0;
   break;
 }






      if (code == ASHIFTRT
   && (count + num_sign_bit_copies (varop, shift_mode)
       >= (mode_bitsize[(int) (shift_mode)])))
 count = (mode_bitsize[(int) (shift_mode)]) - 1;





      if ((mode_bitsize[(int) (shift_mode)]) <= (8 * 8)
   && code == ASHIFTRT
   && ((nonzero_bits (varop, shift_mode)
        & ((long long) 1 << ((mode_bitsize[(int) (shift_mode)]) - 1)))
       == 0))
 code = LSHIFTRT;

      switch (((enum rtx_code) (varop)->code))
 {
 case SIGN_EXTEND:
 case ZERO_EXTEND:
 case SIGN_EXTRACT:
 case ZERO_EXTRACT:
   new = expand_compound_operation (varop);
   if (new != varop)
     {
       varop = new;
       continue;
     }
   break;

 case MEM:



   if ((code == ASHIFTRT || code == LSHIFTRT)
       && ! mode_dependent_address_p ((((varop)->fld[0]).rtx))
       && ! ((varop)->volatil)
       && (tmode = mode_for_size ((mode_bitsize[(int) (mode)]) - count,
      MODE_INT, 1)) != BLKmode)
     {
       new = adjust_address_1 (varop, tmode, 0 ? 0 : count / 8, 0, 1);



       varop = gen_rtx_fmt_e (code == ASHIFTRT ? SIGN_EXTEND
         : ZERO_EXTEND, mode, new);
       count = 0;
       continue;
     }
   break;

 case USE:




   if ((code == ASHIFTRT || code == LSHIFTRT)
       && (tmode = mode_for_size ((mode_bitsize[(int) (mode)]) - count,
      MODE_INT, 1)) != BLKmode
       && tmode == ((enum machine_mode) ((((varop)->fld[0]).rtx))->mode))
     {
       if (0)
  new = (((varop)->fld[0]).rtx);
       else
  {
    new = copy_rtx ((((varop)->fld[0]).rtx));
    do_SUBST(&((((new)->fld[0]).rtx)), (plus_constant_wide (((((new)->fld[0]).rtx)), (long long) (count / 8))));


  }

       varop = gen_rtx_fmt_e (code == ASHIFTRT ? SIGN_EXTEND
         : ZERO_EXTEND, mode, new);
       count = 0;
       continue;
     }
   break;

 case SUBREG:



   if (subreg_lowpart_p (varop)
       && ((mode_size[(int) (((enum machine_mode) ((((varop)->fld[0]).rtx))->mode))])
    > (mode_size[(int) (((enum machine_mode) (varop)->mode))]))
       && ((((mode_size[(int) (((enum machine_mode) ((((varop)->fld[0]).rtx))->mode))])
      + (((target_flags & 0x02000000) ? 8 : 4) - 1)) / ((target_flags & 0x02000000) ? 8 : 4))
    == mode_words))
     {
       varop = (((varop)->fld[0]).rtx);
       if ((mode_size[(int) (((enum machine_mode) (varop)->mode))]) > (mode_size[(int) (mode)]))
  mode = ((enum machine_mode) (varop)->mode);
       continue;
     }
   break;

 case MULT:



   if (((enum rtx_code) ((((varop)->fld[1]).rtx))->code) == CONST_INT
       && exact_log2_wide ((unsigned long long) (((((((varop)->fld[1]).rtx))->fld[0]).rtwint))) >= 0)
     {
       varop
  = gen_binary (ASHIFT, ((enum machine_mode) (varop)->mode), (((varop)->fld[0]).rtx),
         gen_rtx_CONST_INT (VOIDmode, (long long) (exact_log2_wide ((unsigned long long) (((((((varop)->fld[1]).rtx))->fld[0]).rtwint))))));
       continue;
     }
   break;

 case UDIV:

   if (((enum rtx_code) ((((varop)->fld[1]).rtx))->code) == CONST_INT
       && exact_log2_wide ((unsigned long long) (((((((varop)->fld[1]).rtx))->fld[0]).rtwint))) >= 0)
     {
       varop
  = gen_binary (LSHIFTRT, ((enum machine_mode) (varop)->mode), (((varop)->fld[0]).rtx),
         gen_rtx_CONST_INT (VOIDmode, (long long) (exact_log2_wide ((unsigned long long) (((((((varop)->fld[1]).rtx))->fld[0]).rtwint))))));
       continue;
     }
   break;

 case ASHIFTRT:





   if (code == LSHIFTRT && count == (mode_bitsize[(int) (result_mode)]) - 1
       && ((mode_bitsize[(int) (result_mode)])
    >= (mode_bitsize[(int) (((enum machine_mode) (varop)->mode))])))
     {
       varop = (((varop)->fld[0]).rtx);
       continue;
     }



 case LSHIFTRT:
 case ASHIFT:
 case ROTATE:


   if (((enum rtx_code) ((((varop)->fld[1]).rtx))->code) == CONST_INT
       && ((((((varop)->fld[1]).rtx))->fld[0]).rtwint) >= 0
       && ((((((varop)->fld[1]).rtx))->fld[0]).rtwint) < (mode_bitsize[(int) (((enum machine_mode) (varop)->mode))])
       && (mode_bitsize[(int) (result_mode)]) <= (8 * 8)
       && (mode_bitsize[(int) (mode)]) <= (8 * 8))
     {
       enum rtx_code first_code = ((enum rtx_code) (varop)->code);
       unsigned int first_count = ((((((varop)->fld[1]).rtx))->fld[0]).rtwint);
       unsigned long long mask;
       rtx mask_rtx;
# 9189 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
       if (code == ASHIFT && first_code == ASHIFTRT
    && ((mode_bitsize[(int) (result_mode)])
        - (mode_bitsize[(int) (((enum machine_mode) (varop)->mode))])) == count)
  {


    mask = (mode_mask_array[(int) (mode)]
     & ~(((long long) 1 << first_count) - 1));

    varop = simplify_and_const_int ((rtx) 0, result_mode,
        (((varop)->fld[0]).rtx), mask);
    varop = simplify_shift_const ((rtx) 0, ASHIFT, result_mode,
      varop, count);
    count = first_count;
    code = ASHIFTRT;
    continue;
  }
# 9214 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
       if (code == ASHIFTRT && first_code == ASHIFT
    && ((enum machine_mode) (varop)->mode) == shift_mode
    && (num_sign_bit_copies ((((varop)->fld[0]).rtx), shift_mode)
        > first_count))
  {
    varop = (((varop)->fld[0]).rtx);

    signed_count = count - first_count;
    if (signed_count < 0)
      count = -signed_count, code = ASHIFT;
    else
      count = signed_count;

    continue;
  }
# 9245 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
       if (code == first_code)
  {
    if (((enum machine_mode) (varop)->mode) != result_mode
        && (code == ASHIFTRT || code == LSHIFTRT
     || code == ROTATE))
      break;

    count += first_count;
    varop = (((varop)->fld[0]).rtx);
    continue;
  }

       if (code == ASHIFTRT
    || (code == ROTATE && first_code == ASHIFTRT)
    || (mode_bitsize[(int) (mode)]) > (8 * 8)
    || (((enum machine_mode) (varop)->mode) != result_mode
        && (first_code == ASHIFTRT || first_code == LSHIFTRT
     || first_code == ROTATE
     || code == ROTATE)))
  break;





       mask_rtx = gen_rtx_CONST_INT (VOIDmode, (long long) (nonzero_bits (varop, ((enum machine_mode) (varop)->mode))));

       mask_rtx
  = simplify_binary_operation (code, result_mode, mask_rtx,
          gen_rtx_CONST_INT (VOIDmode, (long long) (count)));


       if (mask_rtx == 0
    || ((enum rtx_code) (mask_rtx)->code) != CONST_INT
    || ! merge_outer_ops (&outer_op, &outer_const, AND,
     (((mask_rtx)->fld[0]).rtwint),
     result_mode, &complement_p))
  break;



       signed_count = count;
       if ((code == ASHIFTRT || code == LSHIFTRT)
    == (first_code == ASHIFTRT || first_code == LSHIFTRT))
  signed_count += first_count;
       else
  signed_count -= first_count;





       if (signed_count > 0
    && ((first_code == ROTATE && code == ASHIFT)
        || (first_code == ASHIFTRT && code == LSHIFTRT)))
  code = first_code, count = signed_count;
       else if (signed_count < 0)
  code = first_code, count = -signed_count;
       else
  count = signed_count;

       varop = (((varop)->fld[0]).rtx);
       continue;
     }





   else if (((enum rtx_code) (varop)->code) == code
     && ((enum rtx_code) ((((varop)->fld[1]).rtx))->code) != CONST_INT
     && 0 != (new
       = simplify_binary_operation (code, mode,
        (((varop)->fld[0]).rtx),
        gen_rtx_CONST_INT (VOIDmode, (long long) (count)))))
     {
       varop = gen_rtx_fmt_ee (code, mode, new, (((varop)->fld[1]).rtx));
       count = 0;
       continue;
     }
   break;

 case NOT:

   varop = gen_rtx_fmt_ee (XOR, (mode), ((((varop)->fld[0]).rtx)), (gen_rtx_CONST_INT (VOIDmode, (long long) (mode_mask_array[(int) (mode)]))));

   continue;

 case IOR:
 case AND:
 case XOR:







   if (((enum rtx_code) (varop)->code) == IOR && ((enum rtx_code) ((((varop)->fld[0]).rtx))->code) == PLUS
       && ((((((varop)->fld[0]).rtx))->fld[1]).rtx) == (const_int_rtx[64 -1])
       && (1 == 1 || 1 == -1)
       && (code == LSHIFTRT || code == ASHIFTRT)
       && count == (mode_bitsize[(int) (((enum machine_mode) (varop)->mode))]) - 1
       && rtx_equal_p (((((((varop)->fld[0]).rtx))->fld[0]).rtx), (((varop)->fld[1]).rtx)))
     {
       count = 0;
       varop = gen_rtx_fmt_ee (LE, (((enum machine_mode) (varop)->mode)), ((((varop)->fld[1]).rtx)), ((const_int_rtx[64])));


       if (1 == 1 ? code == ASHIFTRT : code == LSHIFTRT)
  varop = gen_rtx_fmt_e (NEG, (((enum machine_mode) (varop)->mode)), (varop));

       continue;
     }







   if (((enum rtx_code) ((((varop)->fld[1]).rtx))->code) == CONST_INT
       && (new = simplify_binary_operation (code, result_mode,
         (((varop)->fld[1]).rtx),
         gen_rtx_CONST_INT (VOIDmode, (long long) (count)))) != 0
       && ((enum rtx_code) (new)->code) == CONST_INT
       && merge_outer_ops (&outer_op, &outer_const, ((enum rtx_code) (varop)->code),
      (((new)->fld[0]).rtwint), result_mode, &complement_p))
     {
       varop = (((varop)->fld[0]).rtx);
       continue;
     }




   {
     rtx lhs = simplify_shift_const ((rtx) 0, code, shift_mode,
         (((varop)->fld[0]).rtx), count);
     rtx rhs = simplify_shift_const ((rtx) 0, code, shift_mode,
         (((varop)->fld[1]).rtx), count);

     varop = gen_binary (((enum rtx_code) (varop)->code), shift_mode, lhs, rhs);
     varop = apply_distributive_law (varop);

     count = 0;
   }
   break;

 case EQ:




   if (code == LSHIFTRT
       && (((varop)->fld[1]).rtx) == (const_int_rtx[64])
       && ((enum machine_mode) ((((varop)->fld[0]).rtx))->mode) == result_mode
       && count == (mode_bitsize[(int) (result_mode)]) - 1
       && (mode_bitsize[(int) (result_mode)]) <= (8 * 8)
       && ((1
     & ((long long) 1
        < ((mode_bitsize[(int) (result_mode)]) - 1))))
       && nonzero_bits ((((varop)->fld[0]).rtx), result_mode) == 1
       && merge_outer_ops (&outer_op, &outer_const, XOR,
      (long long) 1, result_mode,
      &complement_p))
     {
       varop = (((varop)->fld[0]).rtx);
       count = 0;
       continue;
     }
   break;

 case NEG:


   if (code == LSHIFTRT && count == (mode_bitsize[(int) (result_mode)]) - 1
       && nonzero_bits ((((varop)->fld[0]).rtx), result_mode) == 1)
     {
       varop = (((varop)->fld[0]).rtx);
       count = 0;
       continue;
     }



   if (code == ASHIFT
       && merge_outer_ops (&outer_op, &outer_const, NEG,
      (long long) 0, result_mode,
      &complement_p))
     {
       varop = (((varop)->fld[0]).rtx);
       continue;
     }
   break;

 case PLUS:



   if (code == LSHIFTRT && count == (mode_bitsize[(int) (result_mode)]) - 1
       && (((varop)->fld[1]).rtx) == (const_int_rtx[64 -1])
       && nonzero_bits ((((varop)->fld[0]).rtx), result_mode) == 1
       && merge_outer_ops (&outer_op, &outer_const, XOR,
      (long long) 1, result_mode,
      &complement_p))
     {
       count = 0;
       varop = (((varop)->fld[0]).rtx);
       continue;
     }







   if ((code == ASHIFTRT || code == LSHIFTRT)
       && count < (8 * 8)
       && nonzero_bits ((((varop)->fld[1]).rtx), result_mode) >> count == 0
       && (nonzero_bits ((((varop)->fld[1]).rtx), result_mode)
    & nonzero_bits ((((varop)->fld[0]).rtx), result_mode)) == 0)
     {
       varop = (((varop)->fld[0]).rtx);
       continue;
     }
   else if ((code == ASHIFTRT || code == LSHIFTRT)
     && count < (8 * 8)
     && (mode_bitsize[(int) (result_mode)]) <= (8 * 8)
     && 0 == (nonzero_bits ((((varop)->fld[0]).rtx), result_mode)
       >> count)
     && 0 == (nonzero_bits ((((varop)->fld[0]).rtx), result_mode)
       & nonzero_bits ((((varop)->fld[1]).rtx),
       result_mode)))
     {
       varop = (((varop)->fld[1]).rtx);
       continue;
     }


   if (code == ASHIFT
       && ((enum rtx_code) ((((varop)->fld[1]).rtx))->code) == CONST_INT
       && (new = simplify_binary_operation (ASHIFT, result_mode,
         (((varop)->fld[1]).rtx),
         gen_rtx_CONST_INT (VOIDmode, (long long) (count)))) != 0
       && ((enum rtx_code) (new)->code) == CONST_INT
       && merge_outer_ops (&outer_op, &outer_const, PLUS,
      (((new)->fld[0]).rtwint), result_mode, &complement_p))
     {
       varop = (((varop)->fld[0]).rtx);
       continue;
     }
   break;

 case MINUS:







   if ((1 == 1 || 1 == -1)
       && ((enum rtx_code) ((((varop)->fld[0]).rtx))->code) == ASHIFTRT
       && count == (mode_bitsize[(int) (((enum machine_mode) (varop)->mode))]) - 1
       && (code == LSHIFTRT || code == ASHIFTRT)
       && ((enum rtx_code) (((((((varop)->fld[0]).rtx))->fld[1]).rtx))->code) == CONST_INT
       && (((((((((varop)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint) == count
       && rtx_equal_p (((((((varop)->fld[0]).rtx))->fld[0]).rtx), (((varop)->fld[1]).rtx)))
     {
       count = 0;
       varop = gen_rtx_fmt_ee (GT, (((enum machine_mode) (varop)->mode)), ((((varop)->fld[1]).rtx)), ((const_int_rtx[64])));


       if (1 == 1 ? code == ASHIFTRT : code == LSHIFTRT)
  varop = gen_rtx_fmt_e (NEG, (((enum machine_mode) (varop)->mode)), (varop));

       continue;
     }
   break;

 case TRUNCATE:


   if (code == LSHIFTRT
       && ((enum rtx_code) ((((varop)->fld[0]).rtx))->code) == LSHIFTRT
       && ((enum rtx_code) (((((((varop)->fld[0]).rtx))->fld[1]).rtx))->code) == CONST_INT
       && ((((((((((varop)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint)
    >= ((mode_bitsize[(int) (((enum machine_mode) ((((varop)->fld[0]).rtx))->mode))])
        - (mode_bitsize[(int) (((enum machine_mode) (varop)->mode))]))))
     {
       rtx varop_inner = (((varop)->fld[0]).rtx);

       varop_inner
  = gen_rtx_fmt_ee (LSHIFTRT, (((enum machine_mode) (varop_inner)->mode)), ((((varop_inner)->fld[0]).rtx)), (gen_rtx_CONST_INT (VOIDmode, (long long) (count + ((((((varop_inner)->fld[1]).rtx))->fld[0]).rtwint)))));



       varop = gen_rtx_fmt_e (TRUNCATE, (((enum machine_mode) (varop)->mode)), (varop_inner));
       count = 0;
       continue;
     }
   break;

 default:
   break;
 }

      break;
    }






  shift_mode
    = (code == ASHIFTRT || code == LSHIFTRT || code == ROTATE
       ? result_mode : mode);
# 9575 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  if (x && (rtx_class[(int) (((enum rtx_code) (x)->code))]) == '2'
      && ((enum rtx_code) ((((x)->fld[1]).rtx))->code) == CONST_INT
      && ((((((x)->fld[1]).rtx))->fld[0]).rtwint) == count)
    const_rtx = (((x)->fld[1]).rtx);
  else
    const_rtx = gen_rtx_CONST_INT (VOIDmode, (long long) (count));

  if (x && ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == SUBREG
      && ((enum machine_mode) ((((x)->fld[0]).rtx))->mode) == shift_mode
      && ((((((x)->fld[0]).rtx))->fld[0]).rtx) == varop)
    varop = (((x)->fld[0]).rtx);
  else if (((enum machine_mode) (varop)->mode) != shift_mode)
    varop = gen_lowpart_for_combine (shift_mode, varop);


  if (((enum rtx_code) (varop)->code) == CLOBBER)
    return x ? x : varop;

  new = simplify_binary_operation (code, shift_mode, varop, const_rtx);
  if (new != 0)
    x = new;
  else
    x = gen_rtx_fmt_ee (code, shift_mode, varop, const_rtx);






  if (outer_op != NIL && ((enum rtx_code) (x)->code) == code
      && ((enum rtx_code) ((((x)->fld[1]).rtx))->code) == CONST_INT)
    x = simplify_shift_const (x, code, shift_mode, (((x)->fld[0]).rtx),
         ((((((x)->fld[1]).rtx))->fld[0]).rtwint));



  if (orig_code == LSHIFTRT && result_mode != shift_mode)
    x = simplify_and_const_int ((rtx) 0, shift_mode, x,
    mode_mask_array[(int) (result_mode)] >> orig_count);


  x = gen_lowpart_for_combine (result_mode, x);



  if (complement_p)
    x =simplify_gen_unary (NOT, result_mode, x, result_mode);

  if (outer_op != NIL)
    {
      if ((mode_bitsize[(int) (result_mode)]) < (8 * 8))
 outer_const = trunc_int_for_mode (outer_const, result_mode);

      if (outer_op == AND)
 x = simplify_and_const_int ((rtx) 0, result_mode, x, outer_const);
      else if (outer_op == SET)


 x = gen_rtx_CONST_INT (VOIDmode, (long long) (outer_const));
      else if ((rtx_class[(int) (outer_op)]) == '1')
 x = simplify_gen_unary (outer_op, result_mode, x, result_mode);
      else
 x = gen_binary (outer_op, result_mode, x, gen_rtx_CONST_INT (VOIDmode, (long long) (outer_const)));
    }

  return x;
}
# 9656 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static int
recog_for_combine (pnewpat, insn, pnotes)
     rtx *pnewpat;
     rtx insn;
     rtx *pnotes;
{
  rtx pat = *pnewpat;
  int insn_code_number;
  int num_clobbers_to_add = 0;
  int i;
  rtx notes = 0;
  rtx dummy_insn;




  if (((enum rtx_code) (pat)->code) == PARALLEL)
    for (i = (((((pat)->fld[0]).rtvec))->num_elem) - 1; i >= 0; i--)
      if (((enum rtx_code) ((((((pat)->fld[0]).rtvec))->elem[i]))->code) == CLOBBER
   && ((((((((pat)->fld[0]).rtvec))->elem[i]))->fld[0]).rtx) == (const_int_rtx[64]))
 return -1;



  dummy_insn = shallow_copy_rtx (insn);
  (((dummy_insn)->fld[3]).rtx) = pat;
  (((dummy_insn)->fld[6]).rtx) = 0;

  insn_code_number = recog (pat, dummy_insn, &num_clobbers_to_add);






  if (insn_code_number < 0 && ! check_asm_operands (pat)
      && ((enum rtx_code) (pat)->code) == PARALLEL)
    {
      int pos;

      for (pos = 0, i = 0; i < (((((pat)->fld[0]).rtvec))->num_elem); i++)
 if (((enum rtx_code) ((((((pat)->fld[0]).rtvec))->elem[i]))->code) != CLOBBER)
   {
     if (i != pos)
       do_SUBST(&((((((pat)->fld[0]).rtvec))->elem[pos])), ((((((pat)->fld[0]).rtvec))->elem[i])));
     pos++;
   }

      do_SUBST_INT((unsigned int *)&((((((pat)->fld[0]).rtvec))->num_elem)), (pos));

      if (pos == 1)
 pat = (((((pat)->fld[0]).rtvec))->elem[0]);

      (((dummy_insn)->fld[3]).rtx) = pat;
      insn_code_number = recog (pat, dummy_insn, &num_clobbers_to_add);
    }


  if (insn_code_number < 0 && ((enum rtx_code) (pat)->code) == SET && set_noop_p (pat))
    insn_code_number = 2147483647, num_clobbers_to_add = 0;



  if (num_clobbers_to_add)
    {
      rtx newpat = gen_rtx_fmt_E (PARALLEL, (VOIDmode), (rtvec_alloc (((enum rtx_code) (pat)->code) == PARALLEL ? ((((((pat)->fld[0]).rtvec))->num_elem) + num_clobbers_to_add) : num_clobbers_to_add + 1)));





      if (((enum rtx_code) (pat)->code) == PARALLEL)
 for (i = 0; i < (((((pat)->fld[0]).rtvec))->num_elem); i++)
   (((((newpat)->fld[0]).rtvec))->elem[i]) = (((((pat)->fld[0]).rtvec))->elem[i]);
      else
 (((((newpat)->fld[0]).rtvec))->elem[0]) = pat;

      add_clobbers (newpat, insn_code_number);

      for (i = (((((newpat)->fld[0]).rtvec))->num_elem) - num_clobbers_to_add;
    i < (((((newpat)->fld[0]).rtvec))->num_elem); i++)
 {
   if (((enum rtx_code) (((((((((newpat)->fld[0]).rtvec))->elem[i]))->fld[0]).rtx))->code) == REG
       && ! reg_dead_at_p (((((((((newpat)->fld[0]).rtvec))->elem[i]))->fld[0]).rtx), insn))
     return -1;
   notes = gen_rtx_fmt_ee (EXPR_LIST, (REG_UNUSED), (((((((((newpat)->fld[0]).rtvec))->elem[i]))->fld[0]).rtx)), (notes));

 }
      pat = newpat;
    }

  *pnewpat = pat;
  *pnotes = notes;

  return insn_code_number;
}
# 9764 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static rtx
gen_lowpart_for_combine (mode, x)
     enum machine_mode mode;
     rtx x;
{
  rtx result;

  if (((enum machine_mode) (x)->mode) == mode)
    return x;




  if ((mode_size[(int) (mode)]) > ((target_flags & 0x02000000) ? 8 : 4)
      && ! ((((enum machine_mode) (x)->mode) == VOIDmode
      && (((enum rtx_code) (x)->code) == CONST_INT
   || ((enum rtx_code) (x)->code) == CONST_DOUBLE))
     || (mode_size[(int) (((enum machine_mode) (x)->mode))]) == (mode_size[(int) (mode)])))
    return gen_rtx_fmt_e (CLOBBER, (((enum machine_mode) (x)->mode)), ((const_int_rtx[64])));




  if (((enum rtx_code) (x)->code) == SUBREG && ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == MEM)
    {
      x = (((x)->fld[0]).rtx);
      if (((enum machine_mode) (x)->mode) == mode)
 return x;
    }

  result = gen_lowpart_common (mode, x);
# 9805 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  if (result)
    return result;

  if (((enum rtx_code) (x)->code) == MEM)
    {
      int offset = 0;



      if (((x)->volatil) || mode_dependent_address_p ((((x)->fld[0]).rtx)))
 return gen_rtx_fmt_e (CLOBBER, (((enum machine_mode) (x)->mode)), ((const_int_rtx[64])));




      if ((mode_size[(int) (((enum machine_mode) (x)->mode))]) < (mode_size[(int) (mode)]))
 return gen_rtx_SUBREG (mode, x, 0);

      if (0)
 offset = ((((mode_size[(int) (((enum machine_mode) (x)->mode))])) > (((target_flags & 0x02000000) ? 8 : 4)) ? ((mode_size[(int) (((enum machine_mode) (x)->mode))])) : (((target_flags & 0x02000000) ? 8 : 4)))
    - (((mode_size[(int) (mode)])) > (((target_flags & 0x02000000) ? 8 : 4)) ? ((mode_size[(int) (mode)])) : (((target_flags & 0x02000000) ? 8 : 4))));

      if (0)
 {


   offset -= (((((target_flags & 0x02000000) ? 8 : 4)) < ((mode_size[(int) (mode)])) ? (((target_flags & 0x02000000) ? 8 : 4)) : ((mode_size[(int) (mode)])))
       - ((((target_flags & 0x02000000) ? 8 : 4)) < ((mode_size[(int) (((enum machine_mode) (x)->mode))])) ? (((target_flags & 0x02000000) ? 8 : 4)) : ((mode_size[(int) (((enum machine_mode) (x)->mode))]))));
 }

      return adjust_address_1 (x, mode, offset, 0, 1);
    }



  else if ((rtx_class[(int) (((enum rtx_code) (x)->code))]) == '<')
    return gen_rtx_fmt_ee (((enum rtx_code) (x)->code), mode, (((x)->fld[0]).rtx), (((x)->fld[1]).rtx));




  else
    {
      int offset = 0;
      rtx res;




      if (((enum machine_mode) (x)->mode) == VOIDmode)
 return gen_rtx_fmt_e (CLOBBER, (((enum machine_mode) (x)->mode)), ((const_int_rtx[64])));

      offset = subreg_lowpart_offset (mode, ((enum machine_mode) (x)->mode));
      res = simplify_gen_subreg (mode, x, ((enum machine_mode) (x)->mode), offset);
      if (res)
 return res;
      return gen_rtx_fmt_e (CLOBBER, (((enum machine_mode) (x)->mode)), ((const_int_rtx[64])));
    }
}




static rtx
gen_binary (code, mode, op0, op1)
     enum rtx_code code;
     enum machine_mode mode;
     rtx op0, op1;
{
  rtx result;
  rtx tem;

  if ((rtx_class[(int) (code)]) == 'c'
      && swap_commutative_operands_p (op0, op1))
    tem = op0, op0 = op1, op1 = tem;

  if ((rtx_class[(int) (code)]) == '<')
    {
      enum machine_mode op_mode = ((enum machine_mode) (op0)->mode);



      if (((enum rtx_code) (op0)->code) == COMPARE && op1 == (const_int_rtx[64]))
 {
   op1 = (((op0)->fld[1]).rtx);
   op0 = (((op0)->fld[0]).rtx);
   op_mode = ((enum machine_mode) (op0)->mode);
 }

      if (op_mode == VOIDmode)
 op_mode = ((enum machine_mode) (op1)->mode);
      result = simplify_relational_operation (code, op_mode, op0, op1);
    }
  else
    result = simplify_binary_operation (code, mode, op0, op1);

  if (result)
    return result;


  if ((rtx_class[(int) (code)]) == 'c'
      && swap_commutative_operands_p (op0, op1))
    return gen_rtx_fmt_ee (code, mode, op1, op0);



  else if (code == AND && ((enum rtx_code) (op1)->code) == CONST_INT
    && (mode_bitsize[(int) (mode)]) <= (8 * 8)
    && (nonzero_bits (op0, mode) & ~(((op1)->fld[0]).rtwint)) == 0)
    return op0;

  return gen_rtx_fmt_ee (code, mode, op0, op1);
}
# 9930 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static enum rtx_code
simplify_comparison (code, pop0, pop1)
     enum rtx_code code;
     rtx *pop0;
     rtx *pop1;
{
  rtx op0 = *pop0;
  rtx op1 = *pop1;
  rtx tem, tem1;
  int i;
  enum machine_mode mode, tmode;


  while (1)
    {



      if (code != GTU && code != GEU && code != LTU && code != LEU
   && ((enum rtx_code) (op0)->code) == ASHIFTRT && ((enum rtx_code) (op1)->code) == ASHIFTRT
   && ((enum rtx_code) ((((op0)->fld[0]).rtx))->code) == ASHIFT
   && ((enum rtx_code) ((((op1)->fld[0]).rtx))->code) == ASHIFT
   && ((enum rtx_code) (((((((op0)->fld[0]).rtx))->fld[0]).rtx))->code) == SUBREG
   && ((enum rtx_code) (((((((op1)->fld[0]).rtx))->fld[0]).rtx))->code) == SUBREG
   && (((enum machine_mode) ((((((((((op0)->fld[0]).rtx))->fld[0]).rtx))->fld[0]).rtx))->mode)
       == ((enum machine_mode) ((((((((((op1)->fld[0]).rtx))->fld[0]).rtx))->fld[0]).rtx))->mode))
   && ((enum rtx_code) ((((op0)->fld[1]).rtx))->code) == CONST_INT
   && ((enum rtx_code) ((((op1)->fld[1]).rtx))->code) == CONST_INT
   && ((enum rtx_code) (((((((op0)->fld[0]).rtx))->fld[1]).rtx))->code) == CONST_INT
   && ((enum rtx_code) (((((((op1)->fld[0]).rtx))->fld[1]).rtx))->code) == CONST_INT
   && ((((((op0)->fld[1]).rtx))->fld[0]).rtwint) == ((((((op1)->fld[1]).rtx))->fld[0]).rtwint)
   && ((((((op0)->fld[1]).rtx))->fld[0]).rtwint) == (((((((((op0)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint)
   && ((((((op0)->fld[1]).rtx))->fld[0]).rtwint) == (((((((((op1)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint)
   && (((((((op0)->fld[1]).rtx))->fld[0]).rtwint)
       == ((mode_bitsize[(int) (((enum machine_mode) (op0)->mode))])
    - ((mode_bitsize[(int) (((enum machine_mode) ((((((((((op0)->fld[0]).rtx))->fld[0]).rtx))->fld[0]).rtx))->mode))])))))

 {
   op0 = (((((((((op0)->fld[0]).rtx))->fld[0]).rtx))->fld[0]).rtx);
   op1 = (((((((((op1)->fld[0]).rtx))->fld[0]).rtx))->fld[0]).rtx);
 }






      if (((enum rtx_code) (op0)->code) == ((enum rtx_code) (op1)->code)
   && (mode_bitsize[(int) (((enum machine_mode) (op0)->mode))]) <= (8 * 8)
   && ((((enum rtx_code) (op0)->code) == ROTATE && (code == NE || code == EQ))
       || ((((enum rtx_code) (op0)->code) == LSHIFTRT || ((enum rtx_code) (op0)->code) == ASHIFT)
    && (code != GT && code != LT && code != GE && code != LE))
       || (((enum rtx_code) (op0)->code) == ASHIFTRT
    && (code != GTU && code != LTU
        && code != GEU && code != LEU)))
   && ((enum rtx_code) ((((op0)->fld[1]).rtx))->code) == CONST_INT
   && ((((((op0)->fld[1]).rtx))->fld[0]).rtwint) >= 0
   && ((((((op0)->fld[1]).rtx))->fld[0]).rtwint) < (8 * 8)
   && (((op0)->fld[1]).rtx) == (((op1)->fld[1]).rtx))
 {
   enum machine_mode mode = ((enum machine_mode) (op0)->mode);
   unsigned long long mask = mode_mask_array[(int) (mode)];
   int shift_count = ((((((op0)->fld[1]).rtx))->fld[0]).rtwint);

   if (((enum rtx_code) (op0)->code) == LSHIFTRT || ((enum rtx_code) (op0)->code) == ASHIFTRT)
     mask &= (mask >> shift_count) << shift_count;
   else if (((enum rtx_code) (op0)->code) == ASHIFT)
     mask = (mask & (mask << shift_count)) >> shift_count;

   if ((nonzero_bits ((((op0)->fld[0]).rtx), mode) & ~mask) == 0
       && (nonzero_bits ((((op1)->fld[0]).rtx), mode) & ~mask) == 0)
     op0 = (((op0)->fld[0]).rtx), op1 = (((op1)->fld[0]).rtx);
   else
     break;
 }
# 10019 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
      else if (((enum rtx_code) (op0)->code) == AND && ((enum rtx_code) (op1)->code) == AND
        && ((enum rtx_code) ((((op0)->fld[1]).rtx))->code) == CONST_INT
        && ((enum rtx_code) ((((op1)->fld[1]).rtx))->code) == CONST_INT)
 {
   rtx inner_op0 = (((op0)->fld[0]).rtx);
   rtx inner_op1 = (((op1)->fld[0]).rtx);
   long long c0 = ((((((op0)->fld[1]).rtx))->fld[0]).rtwint);
   long long c1 = ((((((op1)->fld[1]).rtx))->fld[0]).rtwint);
   int changed = 0;

   if (((enum rtx_code) (inner_op0)->code) == SUBREG && ((enum rtx_code) (inner_op1)->code) == SUBREG
       && ((mode_size[(int) (((enum machine_mode) (inner_op0)->mode))])
    > (mode_size[(int) (((enum machine_mode) ((((inner_op0)->fld[0]).rtx))->mode))]))
       && (((enum machine_mode) ((((inner_op0)->fld[0]).rtx))->mode)
    == ((enum machine_mode) ((((inner_op1)->fld[0]).rtx))->mode))
       && ((mode_bitsize[(int) (((enum machine_mode) ((((inner_op0)->fld[0]).rtx))->mode))])
    <= (8 * 8))
       && (0 == ((~c0) & nonzero_bits ((((inner_op0)->fld[0]).rtx),
          ((enum machine_mode) ((((inner_op0)->fld[0]).rtx))->mode))))
       && (0 == ((~c1) & nonzero_bits ((((inner_op1)->fld[0]).rtx),
          ((enum machine_mode) ((((inner_op1)->fld[0]).rtx))->mode)))))
     {
       op0 = (((inner_op0)->fld[0]).rtx);
       op1 = (((inner_op1)->fld[0]).rtx);



       code = unsigned_condition (code);

       changed = 1;
     }

   else if (c0 == c1)
     for (tmode = class_narrowest_mode[(int) ((mode_class[(int) (((enum machine_mode) (op0)->mode))]))];

   tmode != ((enum machine_mode) (op0)->mode); tmode = ((enum machine_mode)mode_wider_mode[(int) (tmode)]))
       if ((unsigned long long) c0 == mode_mask_array[(int) (tmode)])
  {
    op0 = gen_lowpart_for_combine (tmode, inner_op0);
    op1 = gen_lowpart_for_combine (tmode, inner_op1);
    code = unsigned_condition (code);
    changed = 1;
    break;
  }

   if (! changed)
     break;
 }




      else if ((((enum rtx_code) (op0)->code) == NOT && ((enum rtx_code) (op1)->code) == NOT)
        || (((enum rtx_code) (op0)->code) == NEG && ((enum rtx_code) (op1)->code) == NEG
     && (code == EQ || code == NE)))
 op0 = (((op0)->fld[0]).rtx), op1 = (((op1)->fld[0]).rtx), code = swap_condition (code);

      else
 break;
    }




  if (swap_commutative_operands_p (op0, op1))
    {
      tem = op0, op0 = op1, op1 = tem;
      code = swap_condition (code);
    }






  while (((enum rtx_code) (op1)->code) == CONST_INT)
    {
      enum machine_mode mode = ((enum machine_mode) (op0)->mode);
      unsigned int mode_width = (mode_bitsize[(int) (mode)]);
      unsigned long long mask = mode_mask_array[(int) (mode)];
      int equality_comparison_p;
      int sign_bit_comparison_p;
      int unsigned_comparison_p;
      long long const_op;






      if ((mode_class[(int) (mode)]) != MODE_INT
   && ! (mode == VOIDmode
  && (((enum rtx_code) (op0)->code) == COMPARE
      || (rtx_class[(int) (((enum rtx_code) (op0)->code))]) == '<')))
 break;



      const_op = trunc_int_for_mode ((((op1)->fld[0]).rtwint), mode);
      op1 = gen_rtx_CONST_INT (VOIDmode, (long long) (const_op));





      if (const_op
   && (code == EQ || code == NE || code == GE || code == GEU
       || code == LT || code == LTU)
   && mode_width <= (8 * 8)
   && exact_log2_wide ((unsigned long long) (const_op)) >= 0
   && nonzero_bits (op0, mode) == (unsigned long long) const_op)
 {
   code = (code == EQ || code == GE || code == GEU ? NE : EQ);
   op1 = (const_int_rtx[64]), const_op = 0;
 }




      if (const_op == -1
   && (code == EQ || code == NE || code == GT || code == LE
       || code == GEU || code == LTU)
   && num_sign_bit_copies (op0, mode) == mode_width)
 {
   code = (code == EQ || code == LE || code == GEU ? NE : EQ);
   op1 = (const_int_rtx[64]), const_op = 0;
 }





      switch (code)
 {
 case LT:

   if (const_op > 0)
     {
       const_op -= 1;
       op1 = gen_rtx_CONST_INT (VOIDmode, (long long) (const_op));
       code = LE;

     }
   else
     break;

 case LE:

   if (const_op < 0)
     {
       const_op += 1;
       op1 = gen_rtx_CONST_INT (VOIDmode, (long long) (const_op));
       code = LT;
     }



   else if (const_op == 0
     && mode_width <= (8 * 8)
     && (nonzero_bits (op0, mode)
         & ((long long) 1 << (mode_width - 1))) == 0)
     code = EQ;
   break;

 case GE:

   if (const_op > 0)
     {
       const_op -= 1;
       op1 = gen_rtx_CONST_INT (VOIDmode, (long long) (const_op));
       code = GT;

     }
   else
     break;

 case GT:

   if (const_op < 0)
     {
       const_op += 1;
       op1 = gen_rtx_CONST_INT (VOIDmode, (long long) (const_op));
       code = GE;
     }



   else if (const_op == 0
     && mode_width <= (8 * 8)
     && (nonzero_bits (op0, mode)
         & ((long long) 1 << (mode_width - 1))) == 0)
     code = NE;
   break;

 case LTU:

   if (const_op > 0)
     {
       const_op -= 1;
       op1 = gen_rtx_CONST_INT (VOIDmode, (long long) (const_op));
       code = LEU;

     }


   else if ((mode_width <= (8 * 8))
     && (const_op == (long long) 1 << (mode_width - 1)))
     {
       const_op = 0, op1 = (const_int_rtx[64]);
       code = GE;
       break;
     }
   else
     break;

 case LEU:

   if (const_op == 0)
     code = EQ;


   else if ((mode_width <= (8 * 8))
     && (const_op == ((long long) 1 << (mode_width - 1)) - 1))
     {
       const_op = 0, op1 = (const_int_rtx[64]);
       code = GE;
     }
   break;

 case GEU:

   if (const_op > 1)
     {
       const_op -= 1;
       op1 = gen_rtx_CONST_INT (VOIDmode, (long long) (const_op));
       code = GTU;

     }


   else if ((mode_width <= (8 * 8))
     && (const_op == (long long) 1 << (mode_width - 1)))
     {
       const_op = 0, op1 = (const_int_rtx[64]);
       code = LT;
       break;
     }
   else
     break;

 case GTU:

   if (const_op == 0)
     code = NE;


   else if ((mode_width <= (8 * 8))
      && (const_op == ((long long) 1 << (mode_width - 1)) - 1))
     {
       const_op = 0, op1 = (const_int_rtx[64]);
       code = LT;
     }
   break;

 default:
   break;
 }



      equality_comparison_p = (code == EQ || code == NE);
      sign_bit_comparison_p = ((code == LT || code == GE) && const_op == 0);
      unsigned_comparison_p = (code == LTU || code == LEU || code == GTU
          || code == GEU);



      if (sign_bit_comparison_p
   && (mode_bitsize[(int) (mode)]) <= (8 * 8))
 op0 = force_to_mode (op0, mode,
        ((long long) 1
         << ((mode_bitsize[(int) (mode)]) - 1)),
        (rtx) 0, 0);





      switch (((enum rtx_code) (op0)->code))
 {
 case ZERO_EXTRACT:





   if (((enum rtx_code) ((((op0)->fld[0]).rtx))->code) == CONST_INT
       && (((op0)->fld[1]).rtx) == (const_int_rtx[64 +1])
       && equality_comparison_p && const_op == 0
       && (i = exact_log2_wide ((unsigned long long) (((((((op0)->fld[0]).rtx))->fld[0]).rtwint)))) >= 0)
     {
       if (0)
  {
    enum machine_mode new_mode
      = mode_for_extraction (EP_extzv, 1);
    if (new_mode == MAX_MACHINE_MODE)
      i = ((target_flags & 0x02000000) ? 64 : 32) - 1 - i;
    else
      {
        mode = new_mode;
        i = ((mode_bitsize[(int) (mode)]) - 1 - i);
      }
  }

       op0 = (((op0)->fld[2]).rtx);
       op1 = gen_rtx_CONST_INT (VOIDmode, (long long) (i));
       const_op = i;


       code = reverse_condition (code);
       continue;
     }



 case SIGN_EXTRACT:
   tem = expand_compound_operation (op0);
   if (tem != op0)
     {
       op0 = tem;
       continue;
     }
   break;

 case NOT:

   if (equality_comparison_p
       && (tem = simplify_unary_operation (NOT, mode, op1, mode)) != 0)
     {
       op0 = (((op0)->fld[0]).rtx);
       op1 = tem;
       continue;
     }



   if (sign_bit_comparison_p)
     {
       op0 = (((op0)->fld[0]).rtx);
       code = (code == GE ? LT : GE);
       continue;
     }
   break;

 case NEG:

   if (equality_comparison_p
       && (tem = simplify_unary_operation (NEG, mode, op1, mode)) != 0)
     {
       op0 = (((op0)->fld[0]).rtx);
       op1 = tem;
       continue;
     }


   if (const_op != 0)
     break;




   if (sign_bit_comparison_p
       && (((enum rtx_code) ((((op0)->fld[0]).rtx))->code) == ABS
    || (mode_width <= (8 * 8)
        && (nonzero_bits ((((op0)->fld[0]).rtx), mode)
     & ((long long) 1 << (mode_width - 1))) == 0)))
     {
       op0 = (((op0)->fld[0]).rtx);
       code = (code == LT ? NE : EQ);
       continue;
     }



   if (num_sign_bit_copies (op0, mode) >= 2)
     {
       op0 = (((op0)->fld[0]).rtx);
       code = swap_condition (code);
       continue;
     }
   break;

 case ROTATE:


   if (equality_comparison_p && ((enum rtx_code) ((((op0)->fld[1]).rtx))->code) == CONST_INT
       && (tem = simplify_binary_operation (ROTATERT, mode,
         op1, (((op0)->fld[1]).rtx))) != 0)
     {
       op0 = (((op0)->fld[0]).rtx);
       op1 = tem;
       continue;
     }




   if (const_op == 0 && sign_bit_comparison_p
       && ((enum rtx_code) ((((op0)->fld[1]).rtx))->code) == CONST_INT
       && mode_width <= (8 * 8))
     {
       op0 = simplify_and_const_int ((rtx) 0, mode, (((op0)->fld[0]).rtx),
         ((long long) 1
          << (mode_width - 1
       - ((((((op0)->fld[1]).rtx))->fld[0]).rtwint))));
       code = (code == LT ? NE : EQ);
       continue;
     }



 case ABS:

   if (const_op == 0 && equality_comparison_p)
     {
       op0 = (((op0)->fld[0]).rtx);
       continue;
     }
   break;

 case SIGN_EXTEND:




   if (! unsigned_comparison_p
       && ((mode_bitsize[(int) (((enum machine_mode) ((((op0)->fld[0]).rtx))->mode))])
    <= (8 * 8))
       && ((unsigned long long) const_op
    < (((unsigned long long) 1
        << ((mode_bitsize[(int) (((enum machine_mode) ((((op0)->fld[0]).rtx))->mode))]) - 1)))))
     {
       op0 = (((op0)->fld[0]).rtx);
       continue;
     }
   break;

 case SUBREG:
# 10476 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
   if (mode_width <= (8 * 8)
       && subreg_lowpart_p (op0)
       && ((enum rtx_code) ((((op0)->fld[0]).rtx))->code) == PLUS
       && ((enum rtx_code) (((((((op0)->fld[0]).rtx))->fld[1]).rtx))->code) == CONST_INT
       && (((((((((op0)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint) < 0
       && (-(((((((((op0)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint)
    < (long long) (mode_mask_array[(int) (mode)] / 2))
       && (unsigned long long) const_op < mode_mask_array[(int) (mode)] / 2
       && (0 == (nonzero_bits (((((((op0)->fld[0]).rtx))->fld[0]).rtx),
          ((enum machine_mode) ((((op0)->fld[0]).rtx))->mode))
   & ~mode_mask_array[(int) (mode)])
    || (num_sign_bit_copies (((((((op0)->fld[0]).rtx))->fld[0]).rtx),
        ((enum machine_mode) ((((op0)->fld[0]).rtx))->mode))
        > ((mode_bitsize[(int) (((enum machine_mode) ((((op0)->fld[0]).rtx))->mode))])
    - (mode_bitsize[(int) (mode)])))))
     {
       op0 = (((op0)->fld[0]).rtx);
       continue;
     }



   if (subreg_lowpart_p (op0)
       && (mode_bitsize[(int) (((enum machine_mode) ((((op0)->fld[0]).rtx))->mode))]) < mode_width)
                        ;
   else
     break;



 case ZERO_EXTEND:
   if ((unsigned_comparison_p || equality_comparison_p)
       && ((mode_bitsize[(int) (((enum machine_mode) ((((op0)->fld[0]).rtx))->mode))])
    <= (8 * 8))
       && ((unsigned long long) const_op
    < mode_mask_array[(int) (((enum machine_mode) ((((op0)->fld[0]).rtx))->mode))]))
     {
       op0 = (((op0)->fld[0]).rtx);
       continue;
     }
   break;

 case PLUS:



   if (equality_comparison_p
       && 0 != (tem = simplify_binary_operation (MINUS, mode,
       op1, (((op0)->fld[1]).rtx))))
     {
       op0 = (((op0)->fld[0]).rtx);
       op1 = tem;
       continue;
     }


   if (const_op == 0 && (((op0)->fld[1]).rtx) == (const_int_rtx[64 -1])
       && ((enum rtx_code) ((((op0)->fld[0]).rtx))->code) == ABS && sign_bit_comparison_p)
     {
       op0 = ((((((op0)->fld[0]).rtx))->fld[0]).rtx);
       code = (code == LT ? EQ : NE);
       continue;
     }
   break;

 case MINUS:
# 10551 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
   if (equality_comparison_p
       && 0 != (tem = simplify_binary_operation (PLUS, mode,
       (((op0)->fld[1]).rtx), op1)))
     {
       op0 = (((op0)->fld[0]).rtx);
       op1 = tem;
       continue;
     }

   if (equality_comparison_p
       && 0 != (tem = simplify_binary_operation (MINUS, mode,
       (((op0)->fld[0]).rtx), op1)))
     {
       op0 = (((op0)->fld[1]).rtx);
       op1 = tem;
       continue;
     }



   if (sign_bit_comparison_p && ((enum rtx_code) ((((op0)->fld[0]).rtx))->code) == ASHIFTRT
       && ((enum rtx_code) (((((((op0)->fld[0]).rtx))->fld[1]).rtx))->code) == CONST_INT
       && (((((((((op0)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint) == mode_width - 1
       && rtx_equal_p (((((((op0)->fld[0]).rtx))->fld[0]).rtx), (((op0)->fld[1]).rtx)))
     {
       op0 = (((op0)->fld[1]).rtx);
       code = (code == GE ? LE : GT);
       continue;
     }
   break;

 case XOR:


   if (equality_comparison_p
       && 0 != (tem = simplify_binary_operation (XOR, mode,
       (((op0)->fld[1]).rtx), op1)))
     {
       op0 = (((op0)->fld[0]).rtx);
       op1 = tem;
       continue;
     }
   break;

 case EQ: case NE:
 case UNEQ: case LTGT:
 case LT: case LTU: case UNLT: case LE: case LEU: case UNLE:
 case GT: case GTU: case UNGT: case GE: case GEU: case UNGE:
        case UNORDERED: case ORDERED:


   if (const_op != 0



       || (mode_class[(int) (((enum machine_mode) ((((op0)->fld[0]).rtx))->mode))]) == MODE_CC)
     break;


   if (((enum rtx_code) ((((op0)->fld[0]).rtx))->code) == COMPARE)
     tem = ((((((op0)->fld[0]).rtx))->fld[0]).rtx), tem1 = ((((((op0)->fld[0]).rtx))->fld[1]).rtx);
   else
     tem = (((op0)->fld[0]).rtx), tem1 = (((op0)->fld[1]).rtx);



   if (code == NE || code == EQ
       || ((mode_bitsize[(int) (((enum machine_mode) (op0)->mode))]) <= (8 * 8)
    && (mode_class[(int) (((enum machine_mode) (op0)->mode))]) == MODE_INT
    && (1
        & (((long long) 1
     << ((mode_bitsize[(int) (((enum machine_mode) (op0)->mode))]) - 1))))
    && (code == LT || code == GE)))
     {
       enum rtx_code new_code;
       if (code == LT || code == NE)
  new_code = ((enum rtx_code) (op0)->code);
       else
  new_code = combine_reversed_comparison_code (op0);

       if (new_code != UNKNOWN)
  {
    code = new_code;
    op0 = tem;
    op1 = tem1;
    continue;
  }
     }
   break;

 case IOR:


   if (sign_bit_comparison_p && ((enum rtx_code) ((((op0)->fld[0]).rtx))->code) == PLUS
       && ((((((op0)->fld[0]).rtx))->fld[1]).rtx) == (const_int_rtx[64 -1])
       && rtx_equal_p (((((((op0)->fld[0]).rtx))->fld[0]).rtx), (((op0)->fld[1]).rtx)))
     {
       op0 = (((op0)->fld[1]).rtx);
       code = (code == GE ? GT : LE);
       continue;
     }
   break;

 case AND:


   if (const_op == 0 && equality_comparison_p
       && ((enum rtx_code) ((((op0)->fld[0]).rtx))->code) == ASHIFT
       && ((((((op0)->fld[0]).rtx))->fld[0]).rtx) == (const_int_rtx[64 +1]))
     {
       op0 = simplify_and_const_int
  (op0, mode, gen_rtx_fmt_ee (LSHIFTRT, (mode), ((((op0)->fld[1]).rtx)), (((((((op0)->fld[0]).rtx))->fld[1]).rtx))),


   (long long) 1);
       continue;
     }




   if (const_op == 0 && equality_comparison_p
       && mode_width <= (8 * 8)
       && ((enum rtx_code) ((((op0)->fld[1]).rtx))->code) == CONST_INT
       && ((enum rtx_code) ((((op0)->fld[0]).rtx))->code) == LSHIFTRT
       && ((enum rtx_code) (((((((op0)->fld[0]).rtx))->fld[1]).rtx))->code) == CONST_INT
       && (((((((((op0)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint) >= 0
       && (((((((((op0)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint) < (8 * 8))
     {
       mask = ((((((((op0)->fld[1]).rtx))->fld[0]).rtwint) & mode_mask_array[(int) (mode)])
        << (((((((((op0)->fld[0]).rtx))->fld[1]).rtx))->fld[0]).rtwint));
       if ((~1 & mask) == 0
    && ((rtx_class[(int) (((enum rtx_code) (((((((op0)->fld[0]).rtx))->fld[0]).rtx))->code))]) == '<'
        || ((tem = get_last_value (((((((op0)->fld[0]).rtx))->fld[0]).rtx))) != 0
     && (rtx_class[(int) (((enum rtx_code) (tem)->code))]) == '<')))
  {
    op0 = ((((((op0)->fld[0]).rtx))->fld[0]).rtx);
    continue;
  }
     }




   if (equality_comparison_p
       && const_op == 0
       && ((enum rtx_code) ((((op0)->fld[1]).rtx))->code) == CONST_INT
       && mode_width <= (8 * 8)
       && ((((((((op0)->fld[1]).rtx))->fld[0]).rtwint) & mode_mask_array[(int) (mode)])
    == (unsigned long long) 1 << (mode_width - 1)))
     {
       op0 = (((op0)->fld[0]).rtx);
       code = (code == EQ ? GE : LT);
       continue;
     }





   if ((equality_comparison_p || unsigned_comparison_p)
       && ((enum rtx_code) ((((op0)->fld[1]).rtx))->code) == CONST_INT
       && (i = exact_log2_wide ((unsigned long long) ((((((((op0)->fld[1]).rtx))->fld[0]).rtwint) & mode_mask_array[(int) (mode)]) + 1))) >= 0


       && const_op >> i == 0
       && (tmode = mode_for_size (i, MODE_INT, 1)) != BLKmode)
     {
       op0 = gen_lowpart_for_combine (tmode, (((op0)->fld[0]).rtx));
       continue;
     }





   if (((enum rtx_code) ((((op0)->fld[0]).rtx))->code) == SUBREG
       && (0






    || ((mode_width
         <= ((mode_bitsize[(int) (((enum machine_mode) (((((((op0)->fld[0]).rtx))->fld[0]).rtx))->mode))])))

        && subreg_lowpart_p ((((op0)->fld[0]).rtx))))







       && ((mode_size[(int) (((enum machine_mode) ((((op0)->fld[0]).rtx))->mode))])
    <= (mode_size[(int) (((enum machine_mode) (((((((op0)->fld[0]).rtx))->fld[0]).rtx))->mode))]))

       && ((enum rtx_code) ((((op0)->fld[1]).rtx))->code) == CONST_INT
       && mode_width <= (8 * 8)
       && ((mode_bitsize[(int) (((enum machine_mode) (((((((op0)->fld[0]).rtx))->fld[0]).rtx))->mode))])
    <= (8 * 8))
       && (((((((op0)->fld[1]).rtx))->fld[0]).rtwint) & ~mask) == 0
       && 0 == (~mode_mask_array[(int) (((enum machine_mode) (((((((op0)->fld[0]).rtx))->fld[0]).rtx))->mode))]
         & ((((((op0)->fld[1]).rtx))->fld[0]).rtwint))
       && (unsigned long long) ((((((op0)->fld[1]).rtx))->fld[0]).rtwint) != mask
       && ((unsigned long long) ((((((op0)->fld[1]).rtx))->fld[0]).rtwint)
    != mode_mask_array[(int) (((enum machine_mode) (((((((op0)->fld[0]).rtx))->fld[0]).rtx))->mode))]))

     {
       op0
  = gen_lowpart_for_combine
    (mode,
     gen_binary (AND, ((enum machine_mode) (((((((op0)->fld[0]).rtx))->fld[0]).rtx))->mode),
          ((((((op0)->fld[0]).rtx))->fld[0]).rtx), (((op0)->fld[1]).rtx)));
       continue;
     }



   if (const_op == 0 && equality_comparison_p
       && (((op0)->fld[1]).rtx) == (const_int_rtx[64 +1])
       && ((enum rtx_code) ((((op0)->fld[0]).rtx))->code) == LSHIFTRT
       && ((enum rtx_code) (((((((op0)->fld[0]).rtx))->fld[0]).rtx))->code) == NOT)
     {
       op0 = simplify_and_const_int
  (op0, mode,
   gen_rtx_fmt_ee (LSHIFTRT, (mode), ((((((((((op0)->fld[0]).rtx))->fld[0]).rtx))->fld[0]).rtx)), (((((((op0)->fld[0]).rtx))->fld[1]).rtx))),

   (long long) 1);
       code = (code == NE ? EQ : NE);
       continue;
     }
   break;

 case ASHIFT:





   if (((enum rtx_code) ((((op0)->fld[1]).rtx))->code) == CONST_INT
       && ((((((op0)->fld[1]).rtx))->fld[0]).rtwint) >= 0
       && ((((((((op0)->fld[1]).rtx))->fld[0]).rtwint) + ! equality_comparison_p)
    < (8 * 8))
       && ((const_op
     & (((long long) 1 << ((((((op0)->fld[1]).rtx))->fld[0]).rtwint)) - 1)) == 0)
       && mode_width <= (8 * 8)
       && (nonzero_bits ((((op0)->fld[0]).rtx), mode)
    & ~(mask >> (((((((op0)->fld[1]).rtx))->fld[0]).rtwint)
          + ! equality_comparison_p))) == 0)
     {


       unsigned long long temp = const_op & mode_mask_array[(int) (mode)];

       temp >>= ((((((op0)->fld[1]).rtx))->fld[0]).rtwint);
       op1 = gen_rtx_CONST_INT (VOIDmode, (long long) (trunc_int_for_mode (temp, mode)));
       op0 = (((op0)->fld[0]).rtx);
       continue;
     }



   if (sign_bit_comparison_p && ((enum rtx_code) ((((op0)->fld[1]).rtx))->code) == CONST_INT
       && mode_width <= (8 * 8))
     {
       op0 = simplify_and_const_int ((rtx) 0, mode, (((op0)->fld[0]).rtx),
         ((long long) 1
          << (mode_width - 1
       - ((((((op0)->fld[1]).rtx))->fld[0]).rtwint))));
       code = (code == LT ? NE : EQ);
       continue;
     }




   if (const_op == 0 && equality_comparison_p
       && ((enum rtx_code) ((((op0)->fld[1]).rtx))->code) == CONST_INT
       && ((((((op0)->fld[1]).rtx))->fld[0]).rtwint) == mode_width - 1)
     {
       op0 = simplify_and_const_int ((rtx) 0, mode, (((op0)->fld[0]).rtx),
         (long long) 1);
       continue;
     }
   break;

 case ASHIFTRT:


   if (equality_comparison_p && const_op == 0
       && ((enum rtx_code) ((((op0)->fld[1]).rtx))->code) == CONST_INT)
     {
       op0 = simplify_shift_const ((rtx) 0, LSHIFTRT, mode,
       (((op0)->fld[0]).rtx),
       ((((((op0)->fld[1]).rtx))->fld[0]).rtwint));
       continue;
     }



   if (! unsigned_comparison_p
       && ((enum rtx_code) ((((op0)->fld[1]).rtx))->code) == CONST_INT
       && ((enum rtx_code) ((((op0)->fld[0]).rtx))->code) == ASHIFT
       && (((op0)->fld[1]).rtx) == ((((((op0)->fld[0]).rtx))->fld[1]).rtx)
       && (tmode = mode_for_size (mode_width - ((((((op0)->fld[1]).rtx))->fld[0]).rtwint),
      MODE_INT, 1)) != BLKmode
       && (((unsigned long long) const_op
     + (mode_mask_array[(int) (tmode)] >> 1) + 1)
    <= mode_mask_array[(int) (tmode)]))
     {
       op0 = gen_lowpart_for_combine (tmode, ((((((op0)->fld[0]).rtx))->fld[0]).rtx));
       continue;
     }




   if (! unsigned_comparison_p
       && ((enum rtx_code) ((((op0)->fld[1]).rtx))->code) == CONST_INT
       && ((enum rtx_code) ((((op0)->fld[0]).rtx))->code) == PLUS
       && ((enum rtx_code) (((((((op0)->fld[0]).rtx))->fld[1]).rtx))->code) == CONST_INT
       && ((enum rtx_code) (((((((op0)->fld[0]).rtx))->fld[0]).rtx))->code) == ASHIFT
       && (((op0)->fld[1]).rtx) == (((((((((op0)->fld[0]).rtx))->fld[0]).rtx))->fld[1]).rtx)
       && (tmode = mode_for_size (mode_width - ((((((op0)->fld[1]).rtx))->fld[0]).rtwint),
      MODE_INT, 1)) != BLKmode
       && (((unsigned long long) const_op
     + (mode_mask_array[(int) (tmode)] >> 1) + 1)
    <= mode_mask_array[(int) (tmode)]))
     {
       rtx inner = (((((((((op0)->fld[0]).rtx))->fld[0]).rtx))->fld[0]).rtx);
       rtx add_const = ((((((op0)->fld[0]).rtx))->fld[1]).rtx);
       rtx new_const = gen_binary (ASHIFTRT, ((enum machine_mode) (op0)->mode), add_const,
       (((op0)->fld[1]).rtx));

       op0 = gen_binary (PLUS, tmode,
    gen_lowpart_for_combine (tmode, inner),
    new_const);
       continue;
     }


 case LSHIFTRT:




   if (((enum rtx_code) ((((op0)->fld[1]).rtx))->code) == CONST_INT
       && ((((((op0)->fld[1]).rtx))->fld[0]).rtwint) >= 0
       && ((((((op0)->fld[1]).rtx))->fld[0]).rtwint) < (8 * 8)
       && mode_width <= (8 * 8)
       && (nonzero_bits ((((op0)->fld[0]).rtx), mode)
    & (((long long) 1 << ((((((op0)->fld[1]).rtx))->fld[0]).rtwint)) - 1)) == 0
       && (((unsigned long long) const_op
     + (((enum rtx_code) (op0)->code) != LSHIFTRT
        ? ((mode_mask_array[(int) (mode)] >> ((((((op0)->fld[1]).rtx))->fld[0]).rtwint) >> 1)
    + 1)
        : 0))
    <= mode_mask_array[(int) (mode)] >> ((((((op0)->fld[1]).rtx))->fld[0]).rtwint)))
     {


       if (((enum rtx_code) (op0)->code) == LSHIFTRT)
  code = unsigned_condition (code);

       const_op <<= ((((((op0)->fld[1]).rtx))->fld[0]).rtwint);
       op1 = gen_rtx_CONST_INT (VOIDmode, (long long) (const_op));
       op0 = (((op0)->fld[0]).rtx);
       continue;
     }



   if (const_op == 0
       && (equality_comparison_p || sign_bit_comparison_p)
       && ((enum rtx_code) ((((op0)->fld[1]).rtx))->code) == CONST_INT
       && ((((((op0)->fld[1]).rtx))->fld[0]).rtwint) == mode_width - 1)
     {
       op0 = (((op0)->fld[0]).rtx);
       code = (code == NE || code == GT ? LT : GE);
       continue;
     }
   break;

 default:
   break;
 }

      break;
    }
# 10965 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  op0 = make_compound_operation (op0, op1 == (const_int_rtx[64]) ? COMPARE : SET);
  op1 = make_compound_operation (op1, SET);

  if (((enum rtx_code) (op0)->code) == SUBREG && subreg_lowpart_p (op0)


      && ((enum rtx_code) ((((op0)->fld[0]).rtx))->code) == REG
      && (mode_class[(int) (((enum machine_mode) (op0)->mode))]) == MODE_INT
      && (mode_class[(int) (((enum machine_mode) ((((op0)->fld[0]).rtx))->mode))]) == MODE_INT
      && (code == NE || code == EQ))
    {
      if ((mode_size[(int) (((enum machine_mode) (op0)->mode))])
   > (mode_size[(int) (((enum machine_mode) ((((op0)->fld[0]).rtx))->mode))]))
 {
   op0 = (((op0)->fld[0]).rtx);
   op1 = gen_lowpart_for_combine (((enum machine_mode) (op0)->mode), op1);
 }
      else if (((mode_bitsize[(int) (((enum machine_mode) ((((op0)->fld[0]).rtx))->mode))])
  <= (8 * 8))
        && (nonzero_bits ((((op0)->fld[0]).rtx),
     ((enum machine_mode) ((((op0)->fld[0]).rtx))->mode))
     & ~mode_mask_array[(int) (((enum machine_mode) (op0)->mode))]) == 0)
 {
   tem = gen_lowpart_for_combine (((enum machine_mode) ((((op0)->fld[0]).rtx))->mode), op1);

   if ((nonzero_bits (tem, ((enum machine_mode) ((((op0)->fld[0]).rtx))->mode))
        & ~mode_mask_array[(int) (((enum machine_mode) (op0)->mode))]) == 0)
     op0 = (((op0)->fld[0]).rtx), op1 = tem;
 }
    }







  mode = ((enum machine_mode) (op0)->mode);
  if (mode != VOIDmode && (mode_class[(int) (mode)]) == MODE_INT
      && (mode_size[(int) (mode)]) < ((target_flags & 0x02000000) ? 8 : 4)
      && ! have_insn_for (COMPARE, mode))
    for (tmode = ((enum machine_mode)mode_wider_mode[(int) (mode)]);
  (tmode != VOIDmode
   && (mode_bitsize[(int) (tmode)]) <= (8 * 8));
  tmode = ((enum machine_mode)mode_wider_mode[(int) (tmode)]))
      if (have_insn_for (COMPARE, tmode))
 {
   int zero_extended;





   zero_extended = ((code == EQ || code == NE
       || code == GEU || code == GTU
       || code == LEU || code == LTU)
      && (nonzero_bits (op0, tmode)
          & ~mode_mask_array[(int) (mode)]) == 0
      && ((((enum rtx_code) (op1)->code) == CONST_INT
    || (nonzero_bits (op1, tmode)
        & ~mode_mask_array[(int) (mode)]) == 0)));

   if (zero_extended
       || ((num_sign_bit_copies (op0, tmode)
     > (mode_bitsize[(int) (tmode)]) - (mode_bitsize[(int) (mode)]))
    && (num_sign_bit_copies (op1, tmode)
        > (mode_bitsize[(int) (tmode)]) - (mode_bitsize[(int) (mode)]))))
     {


       if (((enum rtx_code) (op0)->code) == AND
    && !have_insn_for (AND, mode))
  op0 = gen_binary (AND, tmode,
      gen_lowpart_for_combine (tmode,
          (((op0)->fld[0]).rtx)),
      gen_lowpart_for_combine (tmode,
          (((op0)->fld[1]).rtx)));

       op0 = gen_lowpart_for_combine (tmode, op0);
       if (zero_extended && ((enum rtx_code) (op1)->code) == CONST_INT)
  op1 = gen_rtx_CONST_INT (VOIDmode, (long long) ((((op1)->fld[0]).rtwint) & mode_mask_array[(int) (mode)]));
       op1 = gen_lowpart_for_combine (tmode, op1);
       break;
     }




   if (op1 == (const_int_rtx[64]) && (code == LT || code == GE)
       && (mode_bitsize[(int) (mode)]) <= (8 * 8))
     {
       op0 = gen_binary (AND, tmode,
    gen_lowpart_for_combine (tmode, op0),
    gen_rtx_CONST_INT (VOIDmode, (long long) ((long long) 1 << ((mode_bitsize[(int) (mode)]) - 1))));

       code = (code == LT) ? NE : EQ;
       break;
     }
 }







  *pop0 = op0;
  *pop1 = op1;

  return code;
}



static enum rtx_code
combine_reversed_comparison_code (exp)
     rtx exp;
{
  enum rtx_code code1 = reversed_comparison_code (exp, ((void*)0));
  rtx x;

  if (code1 != UNKNOWN
      || (mode_class[(int) (((enum machine_mode) ((((exp)->fld[0]).rtx))->mode))]) != MODE_CC)
    return code1;


  x = get_last_value ((((exp)->fld[0]).rtx));
  if (!x || ((enum rtx_code) (x)->code) != COMPARE)
    return UNKNOWN;
  return reversed_comparison_code_parts (((enum rtx_code) (exp)->code),
      (((x)->fld[0]).rtx), (((x)->fld[1]).rtx), ((void*)0));
}


static rtx
reversed_comparison (exp, mode, op0, op1)
     rtx exp, op0, op1;
     enum machine_mode mode;
{
  enum rtx_code reversed_code = combine_reversed_comparison_code (exp);
  if (reversed_code == UNKNOWN)
    return (rtx) 0;
  else
    return gen_binary (reversed_code, mode, op0, op1);
}





static void
update_table_tick (x)
     rtx x;
{
  enum rtx_code code = ((enum rtx_code) (x)->code);
  const char *fmt = (rtx_format[(int) (code)]);
  int i;

  if (code == REG)
    {
      unsigned int regno = (((x)->fld[0]).rtuint);
      unsigned int endregno
 = regno + (regno < 53
     ? (((regno) >= 8 && (regno) <= (8 + 7)) || (((regno) >= (20 + 1) && (regno) <= ((20 + 1) + 7)) || ((regno) >= (((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) && (regno) <= ((((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) + 7))) || ((regno) >= (((20 + 1) + 7) + 1) && (regno) <= ((((20 + 1) + 7) + 1) + 7)) ? (((mode_class[(int) (((enum machine_mode) (x)->mode))]) == MODE_COMPLEX_INT || (mode_class[(int) (((enum machine_mode) (x)->mode))]) == MODE_COMPLEX_FLOAT) ? 2 : 1) : ((((enum machine_mode) (x)->mode)) == TFmode ? ((target_flags & 0x02000000) ? 2 : 3) : (((enum machine_mode) (x)->mode)) == TCmode ? ((target_flags & 0x02000000) ? 4 : 6) : (((mode_size[(int) (((enum machine_mode) (x)->mode))]) + ((target_flags & 0x02000000) ? 8 : 4) - 1) / ((target_flags & 0x02000000) ? 8 : 4)))) : 1);
      unsigned int r;

      for (r = regno; r < endregno; r++)
 reg_last_set_table_tick[r] = label_tick;

      return;
    }

  for (i = (rtx_length[(int) (code)]) - 1; i >= 0; i--)


    if (fmt[i] == 'e')
      update_table_tick ((((x)->fld[i]).rtx));
}






static void
record_value_for_reg (reg, insn, value)
     rtx reg;
     rtx insn;
     rtx value;
{
  unsigned int regno = (((reg)->fld[0]).rtuint);
  unsigned int endregno
    = regno + (regno < 53
        ? (((regno) >= 8 && (regno) <= (8 + 7)) || (((regno) >= (20 + 1) && (regno) <= ((20 + 1) + 7)) || ((regno) >= (((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) && (regno) <= ((((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) + 7))) || ((regno) >= (((20 + 1) + 7) + 1) && (regno) <= ((((20 + 1) + 7) + 1) + 7)) ? (((mode_class[(int) (((enum machine_mode) (reg)->mode))]) == MODE_COMPLEX_INT || (mode_class[(int) (((enum machine_mode) (reg)->mode))]) == MODE_COMPLEX_FLOAT) ? 2 : 1) : ((((enum machine_mode) (reg)->mode)) == TFmode ? ((target_flags & 0x02000000) ? 2 : 3) : (((enum machine_mode) (reg)->mode)) == TCmode ? ((target_flags & 0x02000000) ? 4 : 6) : (((mode_size[(int) (((enum machine_mode) (reg)->mode))]) + ((target_flags & 0x02000000) ? 8 : 4) - 1) / ((target_flags & 0x02000000) ? 8 : 4)))) : 1);
  unsigned int i;



  if (value && insn && reg_overlap_mentioned_p (reg, value))
    {
      rtx tem;



      subst_low_cuid = ((((insn)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (insn) : uid_cuid[(((insn)->fld[0]).rtint)]);
      tem = get_last_value (reg);





      if (tem)
 {
   if (((rtx_class[(int) (((enum rtx_code) (tem)->code))]) == '2'
        || (rtx_class[(int) (((enum rtx_code) (tem)->code))]) == 'c')
       && ((enum rtx_code) ((((tem)->fld[0]).rtx))->code) == CLOBBER
       && ((enum rtx_code) ((((tem)->fld[1]).rtx))->code) == CLOBBER)
     tem = (((tem)->fld[0]).rtx);

   value = replace_rtx (copy_rtx (value), reg, tem);
 }
    }





  for (i = regno; i < endregno; i++)
    {
      if (insn)
 reg_last_set[i] = insn;

      reg_last_set_value[i] = 0;
      reg_last_set_mode[i] = 0;
      reg_last_set_nonzero_bits[i] = 0;
      reg_last_set_sign_bit_copies[i] = 0;
      reg_last_death[i] = 0;
    }


  if (value)
    update_table_tick (value);
# 11215 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  for (i = regno; i < endregno; i++)
    {
      reg_last_set_label[i] = label_tick;
      if (value && reg_last_set_table_tick[i] == label_tick)
 reg_last_set_invalid[i] = 1;
      else
 reg_last_set_invalid[i] = 0;
    }




  if (value && ! get_last_value_validate (&value, insn,
       reg_last_set_label[regno], 0))
    {
      value = copy_rtx (value);
      if (! get_last_value_validate (&value, insn,
         reg_last_set_label[regno], 1))
 value = 0;
    }




  reg_last_set_value[regno] = value;

  if (value)
    {
      enum machine_mode mode = ((enum machine_mode) (reg)->mode);
      subst_low_cuid = ((((insn)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (insn) : uid_cuid[(((insn)->fld[0]).rtint)]);
      reg_last_set_mode[regno] = mode;
      if ((mode_class[(int) (mode)]) == MODE_INT
   && (mode_bitsize[(int) (mode)]) <= (8 * 8))
 mode = nonzero_bits_mode;
      reg_last_set_nonzero_bits[regno] = nonzero_bits (value, mode);
      reg_last_set_sign_bit_copies[regno]
 = num_sign_bit_copies (value, ((enum machine_mode) (reg)->mode));
    }
}





static void
record_dead_and_set_regs_1 (dest, setter, data)
     rtx dest, setter;
     void *data;
{
  rtx record_dead_insn = (rtx) data;

  if (((enum rtx_code) (dest)->code) == SUBREG)
    dest = (((dest)->fld[0]).rtx);

  if (((enum rtx_code) (dest)->code) == REG)
    {



      if (((enum rtx_code) (setter)->code) == SET && dest == (((setter)->fld[0]).rtx))
 record_value_for_reg (dest, record_dead_insn, (((setter)->fld[1]).rtx));
      else if (((enum rtx_code) (setter)->code) == SET
        && ((enum rtx_code) ((((setter)->fld[0]).rtx))->code) == SUBREG
        && ((((((setter)->fld[0]).rtx))->fld[0]).rtx) == dest
        && (mode_bitsize[(int) (((enum machine_mode) (dest)->mode))]) <= ((target_flags & 0x02000000) ? 64 : 32)
        && subreg_lowpart_p ((((setter)->fld[0]).rtx)))
 record_value_for_reg (dest, record_dead_insn,
         gen_lowpart_for_combine (((enum machine_mode) (dest)->mode),
             (((setter)->fld[1]).rtx)));
      else
 record_value_for_reg (dest, record_dead_insn, (rtx) 0);
    }
  else if (((enum rtx_code) (dest)->code) == MEM

    && ! push_operand (dest, ((enum machine_mode) (dest)->mode)))
    mem_last_set = ((((record_dead_insn)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (record_dead_insn) : uid_cuid[(((record_dead_insn)->fld[0]).rtint)]);
}
# 11303 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static void
record_dead_and_set_regs (insn)
     rtx insn;
{
  rtx link;
  unsigned int i;

  for (link = (((insn)->fld[6]).rtx); link; link = (((link)->fld[1]).rtx))
    {
      if (((enum reg_note) ((enum machine_mode) (link)->mode)) == REG_DEAD
   && ((enum rtx_code) ((((link)->fld[0]).rtx))->code) == REG)
 {
   unsigned int regno = ((((((link)->fld[0]).rtx))->fld[0]).rtuint);
   unsigned int endregno
     = regno + (regno < 53
         ? (((regno) >= 8 && (regno) <= (8 + 7)) || (((regno) >= (20 + 1) && (regno) <= ((20 + 1) + 7)) || ((regno) >= (((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) && (regno) <= ((((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) + 7))) || ((regno) >= (((20 + 1) + 7) + 1) && (regno) <= ((((20 + 1) + 7) + 1) + 7)) ? (((mode_class[(int) (((enum machine_mode) ((((link)->fld[0]).rtx))->mode))]) == MODE_COMPLEX_INT || (mode_class[(int) (((enum machine_mode) ((((link)->fld[0]).rtx))->mode))]) == MODE_COMPLEX_FLOAT) ? 2 : 1) : ((((enum machine_mode) ((((link)->fld[0]).rtx))->mode)) == TFmode ? ((target_flags & 0x02000000) ? 2 : 3) : (((enum machine_mode) ((((link)->fld[0]).rtx))->mode)) == TCmode ? ((target_flags & 0x02000000) ? 4 : 6) : (((mode_size[(int) (((enum machine_mode) ((((link)->fld[0]).rtx))->mode))]) + ((target_flags & 0x02000000) ? 8 : 4) - 1) / ((target_flags & 0x02000000) ? 8 : 4))))
         : 1);

   for (i = regno; i < endregno; i++)
     reg_last_death[i] = insn;
 }
      else if (((enum reg_note) ((enum machine_mode) (link)->mode)) == REG_INC)
 record_value_for_reg ((((link)->fld[0]).rtx), insn, (rtx) 0);
    }

  if (((enum rtx_code) (insn)->code) == CALL_INSN)
    {
      for (i = 0; i < 53; i++)
 if (((regs_invalidated_by_call) & (((HARD_REG_ELT_TYPE) (1)) << (i))))
   {
     reg_last_set_value[i] = 0;
     reg_last_set_mode[i] = 0;
     reg_last_set_nonzero_bits[i] = 0;
     reg_last_set_sign_bit_copies[i] = 0;
     reg_last_death[i] = 0;
   }

      last_call_cuid = mem_last_set = ((((insn)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (insn) : uid_cuid[(((insn)->fld[0]).rtint)]);






      return;
    }

  note_stores ((((insn)->fld[3]).rtx), record_dead_and_set_regs_1, insn);
}
# 11362 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static void
record_promoted_value (insn, subreg)
     rtx insn;
     rtx subreg;
{
  rtx links, set;
  unsigned int regno = ((((((subreg)->fld[0]).rtx))->fld[0]).rtuint);
  enum machine_mode mode = ((enum machine_mode) (subreg)->mode);

  if ((mode_bitsize[(int) (mode)]) > (8 * 8))
    return;

  for (links = (((insn)->fld[5]).rtx); links;)
    {
      insn = (((links)->fld[0]).rtx);
      set = (((rtx_class[(int) (((enum rtx_code) (insn)->code))]) == 'i') ? (((enum rtx_code) ((((insn)->fld[3]).rtx))->code) == SET ? (((insn)->fld[3]).rtx) : single_set_2 (insn, (((insn)->fld[3]).rtx))) : (rtx) 0);

      if (! set || ((enum rtx_code) ((((set)->fld[0]).rtx))->code) != REG
   || ((((((set)->fld[0]).rtx))->fld[0]).rtuint) != regno
   || ((enum machine_mode) ((((set)->fld[0]).rtx))->mode) != ((enum machine_mode) ((((subreg)->fld[0]).rtx))->mode))
 {
   links = (((links)->fld[1]).rtx);
   continue;
 }

      if (reg_last_set[regno] == insn)
 {
   if (((subreg)->unchanging))
     reg_last_set_nonzero_bits[regno] &= mode_mask_array[(int) (mode)];
 }

      if (((enum rtx_code) ((((set)->fld[1]).rtx))->code) == REG)
 {
   regno = ((((((set)->fld[1]).rtx))->fld[0]).rtuint);
   links = (((insn)->fld[5]).rtx);
 }
      else
 break;
    }
}




static void
check_promoted_subreg (insn, x)
     rtx insn;
     rtx x;
{
  if (((enum rtx_code) (x)->code) == SUBREG && ((x)->in_struct)
      && ((enum rtx_code) ((((x)->fld[0]).rtx))->code) == REG)
    record_promoted_value (insn, x);
  else
    {
      const char *format = (rtx_format[(int) (((enum rtx_code) (x)->code))]);
      int i, j;

      for (i = 0; i < (rtx_length[(int) (((enum rtx_code) (x)->code))]); i++)
 switch (format[i])
   {
   case 'e':
     check_promoted_subreg (insn, (((x)->fld[i]).rtx));
     break;
   case 'V':
   case 'E':
     if ((((x)->fld[i]).rtvec) != 0)
       for (j = 0; j < (((((x)->fld[i]).rtvec))->num_elem); j++)
  check_promoted_subreg (insn, (((((x)->fld[i]).rtvec))->elem[j]));
     break;
   }
    }
}
# 11445 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static int
get_last_value_validate (loc, insn, tick, replace)
     rtx *loc;
     rtx insn;
     int tick;
     int replace;
{
  rtx x = *loc;
  const char *fmt = (rtx_format[(int) (((enum rtx_code) (x)->code))]);
  int len = (rtx_length[(int) (((enum rtx_code) (x)->code))]);
  int i;

  if (((enum rtx_code) (x)->code) == REG)
    {
      unsigned int regno = (((x)->fld[0]).rtuint);
      unsigned int endregno
 = regno + (regno < 53
     ? (((regno) >= 8 && (regno) <= (8 + 7)) || (((regno) >= (20 + 1) && (regno) <= ((20 + 1) + 7)) || ((regno) >= (((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) && (regno) <= ((((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) + 7))) || ((regno) >= (((20 + 1) + 7) + 1) && (regno) <= ((((20 + 1) + 7) + 1) + 7)) ? (((mode_class[(int) (((enum machine_mode) (x)->mode))]) == MODE_COMPLEX_INT || (mode_class[(int) (((enum machine_mode) (x)->mode))]) == MODE_COMPLEX_FLOAT) ? 2 : 1) : ((((enum machine_mode) (x)->mode)) == TFmode ? ((target_flags & 0x02000000) ? 2 : 3) : (((enum machine_mode) (x)->mode)) == TCmode ? ((target_flags & 0x02000000) ? 4 : 6) : (((mode_size[(int) (((enum machine_mode) (x)->mode))]) + ((target_flags & 0x02000000) ? 8 : 4) - 1) / ((target_flags & 0x02000000) ? 8 : 4)))) : 1);
      unsigned int j;

      for (j = regno; j < endregno; j++)
 if (reg_last_set_invalid[j]


     || (! (regno >= 53
     && (((reg_n_info)->data.reg[regno])->sets) == 1
     && (! bitmap_bit_p ((((basic_block_info)->data.bb[(0)]))->global_live_at_start, regno)))

  && reg_last_set_label[j] > tick))
   {
     if (replace)
       *loc = gen_rtx_fmt_e (CLOBBER, (((enum machine_mode) (x)->mode)), ((const_int_rtx[64])));
     return replace;
   }

      return 1;
    }



  else if (((enum rtx_code) (x)->code) == MEM && ! ((x)->unchanging)
    && ((((insn)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (insn) : uid_cuid[(((insn)->fld[0]).rtint)]) <= mem_last_set)
    {
      if (replace)
 *loc = gen_rtx_fmt_e (CLOBBER, (((enum machine_mode) (x)->mode)), ((const_int_rtx[64])));
      return replace;
    }

  for (i = 0; i < len; i++)
    if ((fmt[i] == 'e'
  && get_last_value_validate (&(((x)->fld[i]).rtx), insn, tick, replace) == 0)

 || fmt[i] == 'E')
      return 0;


  return 1;
}





static rtx
get_last_value (x)
     rtx x;
{
  unsigned int regno;
  rtx value;




  if (((enum rtx_code) (x)->code) == SUBREG
      && subreg_lowpart_p (x)
      && ((mode_size[(int) (((enum machine_mode) (x)->mode))])
   <= (mode_size[(int) (((enum machine_mode) ((((x)->fld[0]).rtx))->mode))]))
      && (value = get_last_value ((((x)->fld[0]).rtx))) != 0)
    return gen_lowpart_for_combine (((enum machine_mode) (x)->mode), value);

  if (((enum rtx_code) (x)->code) != REG)
    return 0;

  regno = (((x)->fld[0]).rtuint);
  value = reg_last_set_value[regno];
# 11541 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
  if (value == 0
      || (reg_last_set_label[regno] != label_tick
   && (regno < 53
       || (((reg_n_info)->data.reg[regno])->sets) != 1
       || (bitmap_bit_p ((((basic_block_info)->data.bb[(0)]))->global_live_at_start, regno)))))

    return 0;



  if (((((reg_last_set[regno])->fld[0]).rtint) > max_uid_cuid ? insn_cuid (reg_last_set[regno]) : uid_cuid[(((reg_last_set[regno])->fld[0]).rtint)]) >= subst_low_cuid)
    return 0;


  if (get_last_value_validate (&value, reg_last_set[regno],
          reg_last_set_label[regno], 0))
    return value;




  value = copy_rtx (value);
  if (get_last_value_validate (&value, reg_last_set[regno],
          reg_last_set_label[regno], 1))
    return value;

  return 0;
}




static int
use_crosses_set_p (x, from_cuid)
     rtx x;
     int from_cuid;
{
  const char *fmt;
  int i;
  enum rtx_code code = ((enum rtx_code) (x)->code);

  if (code == REG)
    {
      unsigned int regno = (((x)->fld[0]).rtuint);
      unsigned endreg = regno + (regno < 53
     ? (((regno) >= 8 && (regno) <= (8 + 7)) || (((regno) >= (20 + 1) && (regno) <= ((20 + 1) + 7)) || ((regno) >= (((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) && (regno) <= ((((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) + 7))) || ((regno) >= (((20 + 1) + 7) + 1) && (regno) <= ((((20 + 1) + 7) + 1) + 7)) ? (((mode_class[(int) (((enum machine_mode) (x)->mode))]) == MODE_COMPLEX_INT || (mode_class[(int) (((enum machine_mode) (x)->mode))]) == MODE_COMPLEX_FLOAT) ? 2 : 1) : ((((enum machine_mode) (x)->mode)) == TFmode ? ((target_flags & 0x02000000) ? 2 : 3) : (((enum machine_mode) (x)->mode)) == TCmode ? ((target_flags & 0x02000000) ? 4 : 6) : (((mode_size[(int) (((enum machine_mode) (x)->mode))]) + ((target_flags & 0x02000000) ? 8 : 4) - 1) / ((target_flags & 0x02000000) ? 8 : 4)))) : 1);




      if (regno == 7 && ((!(target_flags & 0x00000800)) && !(target_flags & 0x00001000)))
 return 1;

      for (; regno < endreg; regno++)
 if (reg_last_set[regno]
     && ((((reg_last_set[regno])->fld[0]).rtint) > max_uid_cuid ? insn_cuid (reg_last_set[regno]) : uid_cuid[(((reg_last_set[regno])->fld[0]).rtint)]) > from_cuid)
   return 1;
      return 0;
    }

  if (code == MEM && mem_last_set > from_cuid)
    return 1;

  fmt = (rtx_format[(int) (code)]);

  for (i = (rtx_length[(int) (code)]) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'E')
 {
   int j;
   for (j = (((((x)->fld[i]).rtvec))->num_elem) - 1; j >= 0; j--)
     if (use_crosses_set_p ((((((x)->fld[i]).rtvec))->elem[j]), from_cuid))
       return 1;
 }
      else if (fmt[i] == 'e'
        && use_crosses_set_p ((((x)->fld[i]).rtx), from_cuid))
 return 1;
    }
  return 0;
}




static unsigned int reg_dead_regno, reg_dead_endregno;
static int reg_dead_flag;






static void
reg_dead_at_p_1 (dest, x, data)
     rtx dest;
     rtx x;
     void *data __attribute__ ((__unused__));
{
  unsigned int regno, endregno;

  if (((enum rtx_code) (dest)->code) != REG)
    return;

  regno = (((dest)->fld[0]).rtuint);
  endregno = regno + (regno < 53
        ? (((regno) >= 8 && (regno) <= (8 + 7)) || (((regno) >= (20 + 1) && (regno) <= ((20 + 1) + 7)) || ((regno) >= (((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) && (regno) <= ((((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) + 7))) || ((regno) >= (((20 + 1) + 7) + 1) && (regno) <= ((((20 + 1) + 7) + 1) + 7)) ? (((mode_class[(int) (((enum machine_mode) (dest)->mode))]) == MODE_COMPLEX_INT || (mode_class[(int) (((enum machine_mode) (dest)->mode))]) == MODE_COMPLEX_FLOAT) ? 2 : 1) : ((((enum machine_mode) (dest)->mode)) == TFmode ? ((target_flags & 0x02000000) ? 2 : 3) : (((enum machine_mode) (dest)->mode)) == TCmode ? ((target_flags & 0x02000000) ? 4 : 6) : (((mode_size[(int) (((enum machine_mode) (dest)->mode))]) + ((target_flags & 0x02000000) ? 8 : 4) - 1) / ((target_flags & 0x02000000) ? 8 : 4)))) : 1);

  if (reg_dead_endregno > regno && reg_dead_regno < endregno)
    reg_dead_flag = (((enum rtx_code) (x)->code) == CLOBBER) ? 1 : -1;
}
# 11660 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static int
reg_dead_at_p (reg, insn)
     rtx reg;
     rtx insn;
{
  int block;
  unsigned int i;


  reg_dead_regno = (((reg)->fld[0]).rtuint);
  reg_dead_endregno = reg_dead_regno + (reg_dead_regno < 53
     ? (((reg_dead_regno) >= 8 && (reg_dead_regno) <= (8 + 7)) || (((reg_dead_regno) >= (20 + 1) && (reg_dead_regno) <= ((20 + 1) + 7)) || ((reg_dead_regno) >= (((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) && (reg_dead_regno) <= ((((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) + 7))) || ((reg_dead_regno) >= (((20 + 1) + 7) + 1) && (reg_dead_regno) <= ((((20 + 1) + 7) + 1) + 7)) ? (((mode_class[(int) (((enum machine_mode) (reg)->mode))]) == MODE_COMPLEX_INT || (mode_class[(int) (((enum machine_mode) (reg)->mode))]) == MODE_COMPLEX_FLOAT) ? 2 : 1) : ((((enum machine_mode) (reg)->mode)) == TFmode ? ((target_flags & 0x02000000) ? 2 : 3) : (((enum machine_mode) (reg)->mode)) == TCmode ? ((target_flags & 0x02000000) ? 4 : 6) : (((mode_size[(int) (((enum machine_mode) (reg)->mode))]) + ((target_flags & 0x02000000) ? 8 : 4) - 1) / ((target_flags & 0x02000000) ? 8 : 4))))

     : 1);

  reg_dead_flag = 0;


  if (reg_dead_regno < 53)
    {
      for (i = reg_dead_regno; i < reg_dead_endregno; i++)
 if (((newpat_used_regs) & (((HARD_REG_ELT_TYPE) (1)) << (i))))
   return 0;
    }



  for (; insn && ((enum rtx_code) (insn)->code) != CODE_LABEL && ((enum rtx_code) (insn)->code) != BARRIER;
       insn = prev_nonnote_insn (insn))
    {
      note_stores ((((insn)->fld[3]).rtx), reg_dead_at_p_1, ((void*)0));
      if (reg_dead_flag)
 return reg_dead_flag == 1 ? 1 : 0;

      if (find_regno_note (insn, REG_DEAD, reg_dead_regno))
 return 1;
    }


  if (insn == 0)
    block = 0;
  else
    {
      for (block = 0; block < n_basic_blocks; block++)
 if (insn == ((((basic_block_info)->data.bb[(block)]))->head))
   break;

      if (block == n_basic_blocks)
 return 0;
    }

  for (i = reg_dead_regno; i < reg_dead_endregno; i++)
    if (bitmap_bit_p ((((basic_block_info)->data.bb[(block)]))->global_live_at_start, i))
      return 0;

  return 1;
}




static void
mark_used_regs_combine (x)
     rtx x;
{
  enum rtx_code code = ((enum rtx_code) (x)->code);
  unsigned int regno;
  int i;

  switch (code)
    {
    case LABEL_REF:
    case SYMBOL_REF:
    case CONST_INT:
    case CONST:
    case CONST_DOUBLE:
    case CONST_VECTOR:
    case PC:
    case ADDR_VEC:
    case ADDR_DIFF_VEC:
    case ASM_INPUT:





      return;

    case CLOBBER:


      if (((enum rtx_code) ((((x)->fld[0]).rtx))->code) == MEM)
 mark_used_regs_combine (((((((x)->fld[0]).rtx))->fld[0]).rtx));
      return;

    case REG:
      regno = (((x)->fld[0]).rtuint);


      if (regno < 53)
 {
   unsigned int endregno, r;


   if (regno == 7

       || regno == 6


       || (regno == 16 && fixed_regs[regno])

       || regno == 20)
     return;

   endregno = regno + (((regno) >= 8 && (regno) <= (8 + 7)) || (((regno) >= (20 + 1) && (regno) <= ((20 + 1) + 7)) || ((regno) >= (((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) && (regno) <= ((((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) + 7))) || ((regno) >= (((20 + 1) + 7) + 1) && (regno) <= ((((20 + 1) + 7) + 1) + 7)) ? (((mode_class[(int) (((enum machine_mode) (x)->mode))]) == MODE_COMPLEX_INT || (mode_class[(int) (((enum machine_mode) (x)->mode))]) == MODE_COMPLEX_FLOAT) ? 2 : 1) : ((((enum machine_mode) (x)->mode)) == TFmode ? ((target_flags & 0x02000000) ? 2 : 3) : (((enum machine_mode) (x)->mode)) == TCmode ? ((target_flags & 0x02000000) ? 4 : 6) : (((mode_size[(int) (((enum machine_mode) (x)->mode))]) + ((target_flags & 0x02000000) ? 8 : 4) - 1) / ((target_flags & 0x02000000) ? 8 : 4))));
   for (r = regno; r < endregno; r++)
     ((newpat_used_regs) |= ((HARD_REG_ELT_TYPE) (1)) << (r));
 }
      return;

    case SET:
      {


 rtx testreg = (((x)->fld[0]).rtx);

 while (((enum rtx_code) (testreg)->code) == SUBREG
        || ((enum rtx_code) (testreg)->code) == ZERO_EXTRACT
        || ((enum rtx_code) (testreg)->code) == SIGN_EXTRACT
        || ((enum rtx_code) (testreg)->code) == STRICT_LOW_PART)
   testreg = (((testreg)->fld[0]).rtx);

 if (((enum rtx_code) (testreg)->code) == MEM)
   mark_used_regs_combine ((((testreg)->fld[0]).rtx));

 mark_used_regs_combine ((((x)->fld[1]).rtx));
      }
      return;

    default:
      break;
    }



  {
    const char *fmt = (rtx_format[(int) (code)]);

    for (i = (rtx_length[(int) (code)]) - 1; i >= 0; i--)
      {
 if (fmt[i] == 'e')
   mark_used_regs_combine ((((x)->fld[i]).rtx));
 else if (fmt[i] == 'E')
   {
     int j;

     for (j = 0; j < (((((x)->fld[i]).rtvec))->num_elem); j++)
       mark_used_regs_combine ((((((x)->fld[i]).rtvec))->elem[j]));
   }
      }
  }
}





rtx
remove_death (regno, insn)
     unsigned int regno;
     rtx insn;
{
  rtx note = find_regno_note (insn, REG_DEAD, regno);

  if (note)
    {
      (((reg_n_info)->data.reg[regno])->deaths)--;
      remove_note (insn, note);
    }

  return note;
}
# 11853 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static void
move_deaths (x, maybe_kill_insn, from_cuid, to_insn, pnotes)
     rtx x;
     rtx maybe_kill_insn;
     int from_cuid;
     rtx to_insn;
     rtx *pnotes;
{
  const char *fmt;
  int len, i;
  enum rtx_code code = ((enum rtx_code) (x)->code);

  if (code == REG)
    {
      unsigned int regno = (((x)->fld[0]).rtuint);
      rtx where_dead = reg_last_death[regno];
      rtx before_dead, after_dead;


      if (maybe_kill_insn && reg_set_p (x, maybe_kill_insn)
   && ! reg_referenced_p (x, maybe_kill_insn))
 return;



      before_dead = where_dead;
      while (before_dead && (((before_dead)->fld[0]).rtint) > max_uid_cuid)
 before_dead = (((before_dead)->fld[1]).rtx);

      after_dead = where_dead;
      while (after_dead && (((after_dead)->fld[0]).rtint) > max_uid_cuid)
 after_dead = (((after_dead)->fld[2]).rtx);

      if (before_dead && after_dead
   && ((((before_dead)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (before_dead) : uid_cuid[(((before_dead)->fld[0]).rtint)]) >= from_cuid
   && (((((after_dead)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (after_dead) : uid_cuid[(((after_dead)->fld[0]).rtint)]) < ((((to_insn)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (to_insn) : uid_cuid[(((to_insn)->fld[0]).rtint)])
       || (where_dead != after_dead
    && ((((after_dead)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (after_dead) : uid_cuid[(((after_dead)->fld[0]).rtint)]) == ((((to_insn)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (to_insn) : uid_cuid[(((to_insn)->fld[0]).rtint)]))))
 {
   rtx note = remove_death (regno, where_dead);
# 11903 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
   if (note != 0 && regno < 53
       && ((mode_size[(int) (((enum machine_mode) ((((note)->fld[0]).rtx))->mode))])
    > (mode_size[(int) (((enum machine_mode) (x)->mode))])))
     {
       unsigned int deadregno = ((((((note)->fld[0]).rtx))->fld[0]).rtuint);
       unsigned int deadend
  = (deadregno + (((deadregno) >= 8 && (deadregno) <= (8 + 7)) || (((deadregno) >= (20 + 1) && (deadregno) <= ((20 + 1) + 7)) || ((deadregno) >= (((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) && (deadregno) <= ((((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) + 7))) || ((deadregno) >= (((20 + 1) + 7) + 1) && (deadregno) <= ((((20 + 1) + 7) + 1) + 7)) ? (((mode_class[(int) (((enum machine_mode) ((((note)->fld[0]).rtx))->mode))]) == MODE_COMPLEX_INT || (mode_class[(int) (((enum machine_mode) ((((note)->fld[0]).rtx))->mode))]) == MODE_COMPLEX_FLOAT) ? 2 : 1) : ((((enum machine_mode) ((((note)->fld[0]).rtx))->mode)) == TFmode ? ((target_flags & 0x02000000) ? 2 : 3) : (((enum machine_mode) ((((note)->fld[0]).rtx))->mode)) == TCmode ? ((target_flags & 0x02000000) ? 4 : 6) : (((mode_size[(int) (((enum machine_mode) ((((note)->fld[0]).rtx))->mode))]) + ((target_flags & 0x02000000) ? 8 : 4) - 1) / ((target_flags & 0x02000000) ? 8 : 4)))));

       unsigned int ourend
  = regno + (((regno) >= 8 && (regno) <= (8 + 7)) || (((regno) >= (20 + 1) && (regno) <= ((20 + 1) + 7)) || ((regno) >= (((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) && (regno) <= ((((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) + 7))) || ((regno) >= (((20 + 1) + 7) + 1) && (regno) <= ((((20 + 1) + 7) + 1) + 7)) ? (((mode_class[(int) (((enum machine_mode) (x)->mode))]) == MODE_COMPLEX_INT || (mode_class[(int) (((enum machine_mode) (x)->mode))]) == MODE_COMPLEX_FLOAT) ? 2 : 1) : ((((enum machine_mode) (x)->mode)) == TFmode ? ((target_flags & 0x02000000) ? 2 : 3) : (((enum machine_mode) (x)->mode)) == TCmode ? ((target_flags & 0x02000000) ? 4 : 6) : (((mode_size[(int) (((enum machine_mode) (x)->mode))]) + ((target_flags & 0x02000000) ? 8 : 4) - 1) / ((target_flags & 0x02000000) ? 8 : 4))));
       unsigned int i;

       for (i = deadregno; i < deadend; i++)
  if (i < regno || i >= ourend)
    (((where_dead)->fld[6]).rtx)
      = gen_rtx_fmt_ee (EXPR_LIST, (REG_DEAD), (gen_rtx_REG (reg_raw_mode[i], i)), ((((where_dead)->fld[6]).rtx)));


     }






   else if ((note == 0
      || (note != 0
   && ((mode_size[(int) (((enum machine_mode) ((((note)->fld[0]).rtx))->mode))])
       < (mode_size[(int) (((enum machine_mode) (x)->mode))]))))
     && regno < 53
     && (((regno) >= 8 && (regno) <= (8 + 7)) || (((regno) >= (20 + 1) && (regno) <= ((20 + 1) + 7)) || ((regno) >= (((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) && (regno) <= ((((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) + 7))) || ((regno) >= (((20 + 1) + 7) + 1) && (regno) <= ((((20 + 1) + 7) + 1) + 7)) ? (((mode_class[(int) (((enum machine_mode) (x)->mode))]) == MODE_COMPLEX_INT || (mode_class[(int) (((enum machine_mode) (x)->mode))]) == MODE_COMPLEX_FLOAT) ? 2 : 1) : ((((enum machine_mode) (x)->mode)) == TFmode ? ((target_flags & 0x02000000) ? 2 : 3) : (((enum machine_mode) (x)->mode)) == TCmode ? ((target_flags & 0x02000000) ? 4 : 6) : (((mode_size[(int) (((enum machine_mode) (x)->mode))]) + ((target_flags & 0x02000000) ? 8 : 4) - 1) / ((target_flags & 0x02000000) ? 8 : 4)))) > 1)
     {
       unsigned int ourend
  = regno + (((regno) >= 8 && (regno) <= (8 + 7)) || (((regno) >= (20 + 1) && (regno) <= ((20 + 1) + 7)) || ((regno) >= (((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) && (regno) <= ((((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) + 7))) || ((regno) >= (((20 + 1) + 7) + 1) && (regno) <= ((((20 + 1) + 7) + 1) + 7)) ? (((mode_class[(int) (((enum machine_mode) (x)->mode))]) == MODE_COMPLEX_INT || (mode_class[(int) (((enum machine_mode) (x)->mode))]) == MODE_COMPLEX_FLOAT) ? 2 : 1) : ((((enum machine_mode) (x)->mode)) == TFmode ? ((target_flags & 0x02000000) ? 2 : 3) : (((enum machine_mode) (x)->mode)) == TCmode ? ((target_flags & 0x02000000) ? 4 : 6) : (((mode_size[(int) (((enum machine_mode) (x)->mode))]) + ((target_flags & 0x02000000) ? 8 : 4) - 1) / ((target_flags & 0x02000000) ? 8 : 4))));
       unsigned int i, offset;
       rtx oldnotes = 0;

       if (note)
  offset = (((regno) >= 8 && (regno) <= (8 + 7)) || (((regno) >= (20 + 1) && (regno) <= ((20 + 1) + 7)) || ((regno) >= (((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) && (regno) <= ((((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) + 7))) || ((regno) >= (((20 + 1) + 7) + 1) && (regno) <= ((((20 + 1) + 7) + 1) + 7)) ? (((mode_class[(int) (((enum machine_mode) ((((note)->fld[0]).rtx))->mode))]) == MODE_COMPLEX_INT || (mode_class[(int) (((enum machine_mode) ((((note)->fld[0]).rtx))->mode))]) == MODE_COMPLEX_FLOAT) ? 2 : 1) : ((((enum machine_mode) ((((note)->fld[0]).rtx))->mode)) == TFmode ? ((target_flags & 0x02000000) ? 2 : 3) : (((enum machine_mode) ((((note)->fld[0]).rtx))->mode)) == TCmode ? ((target_flags & 0x02000000) ? 4 : 6) : (((mode_size[(int) (((enum machine_mode) ((((note)->fld[0]).rtx))->mode))]) + ((target_flags & 0x02000000) ? 8 : 4) - 1) / ((target_flags & 0x02000000) ? 8 : 4))));
       else
  offset = 1;

       for (i = regno + offset; i < ourend; i++)
  move_deaths (gen_rtx_REG (reg_raw_mode[i], i),
        maybe_kill_insn, from_cuid, to_insn, &oldnotes);
     }

   if (note != 0 && ((enum machine_mode) ((((note)->fld[0]).rtx))->mode) == ((enum machine_mode) (x)->mode))
     {
       (((note)->fld[1]).rtx) = *pnotes;
       *pnotes = note;
     }
   else
     *pnotes = gen_rtx_fmt_ee (EXPR_LIST, (REG_DEAD), (x), (*pnotes));

   (((reg_n_info)->data.reg[regno])->deaths)++;
 }

      return;
    }

  else if (((enum rtx_code) (x)->code) == SET)
    {
      rtx dest = (((x)->fld[0]).rtx);

      move_deaths ((((x)->fld[1]).rtx), maybe_kill_insn, from_cuid, to_insn, pnotes);







      if (((enum rtx_code) (dest)->code) == ZERO_EXTRACT
   || ((enum rtx_code) (dest)->code) == STRICT_LOW_PART
   || (((enum rtx_code) (dest)->code) == SUBREG
       && ((((mode_size[(int) (((enum machine_mode) (dest)->mode))])
      + ((target_flags & 0x02000000) ? 8 : 4) - 1) / ((target_flags & 0x02000000) ? 8 : 4))
    == (((mode_size[(int) (((enum machine_mode) ((((dest)->fld[0]).rtx))->mode))])
         + ((target_flags & 0x02000000) ? 8 : 4) - 1) / ((target_flags & 0x02000000) ? 8 : 4)))))
 {
   move_deaths (dest, maybe_kill_insn, from_cuid, to_insn, pnotes);
   return;
 }



      if (((enum rtx_code) (dest)->code) == SUBREG)
 dest = (((dest)->fld[0]).rtx);





      if (((enum rtx_code) (dest)->code) == MEM)
 move_deaths ((((dest)->fld[0]).rtx), maybe_kill_insn, from_cuid,
       to_insn, pnotes);
      return;
    }

  else if (((enum rtx_code) (x)->code) == CLOBBER)
    return;

  len = (rtx_length[(int) (code)]);
  fmt = (rtx_format[(int) (code)]);

  for (i = 0; i < len; i++)
    {
      if (fmt[i] == 'E')
 {
   int j;
   for (j = (((((x)->fld[i]).rtvec))->num_elem) - 1; j >= 0; j--)
     move_deaths ((((((x)->fld[i]).rtvec))->elem[j]), maybe_kill_insn, from_cuid,
    to_insn, pnotes);
 }
      else if (fmt[i] == 'e')
 move_deaths ((((x)->fld[i]).rtx), maybe_kill_insn, from_cuid, to_insn, pnotes);
    }
}




static int
reg_bitfield_target_p (x, body)
     rtx x;
     rtx body;
{
  int i;

  if (((enum rtx_code) (body)->code) == SET)
    {
      rtx dest = (((body)->fld[0]).rtx);
      rtx target;
      unsigned int regno, tregno, endregno, endtregno;

      if (((enum rtx_code) (dest)->code) == ZERO_EXTRACT)
 target = (((dest)->fld[0]).rtx);
      else if (((enum rtx_code) (dest)->code) == STRICT_LOW_PART)
 target = ((((((dest)->fld[0]).rtx))->fld[0]).rtx);
      else
 return 0;

      if (((enum rtx_code) (target)->code) == SUBREG)
 target = (((target)->fld[0]).rtx);

      if (((enum rtx_code) (target)->code) != REG)
 return 0;

      tregno = (((target)->fld[0]).rtuint), regno = (((x)->fld[0]).rtuint);
      if (tregno >= 53 || regno >= 53)
 return target == x;

      endtregno = tregno + (((tregno) >= 8 && (tregno) <= (8 + 7)) || (((tregno) >= (20 + 1) && (tregno) <= ((20 + 1) + 7)) || ((tregno) >= (((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) && (tregno) <= ((((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) + 7))) || ((tregno) >= (((20 + 1) + 7) + 1) && (tregno) <= ((((20 + 1) + 7) + 1) + 7)) ? (((mode_class[(int) (((enum machine_mode) (target)->mode))]) == MODE_COMPLEX_INT || (mode_class[(int) (((enum machine_mode) (target)->mode))]) == MODE_COMPLEX_FLOAT) ? 2 : 1) : ((((enum machine_mode) (target)->mode)) == TFmode ? ((target_flags & 0x02000000) ? 2 : 3) : (((enum machine_mode) (target)->mode)) == TCmode ? ((target_flags & 0x02000000) ? 4 : 6) : (((mode_size[(int) (((enum machine_mode) (target)->mode))]) + ((target_flags & 0x02000000) ? 8 : 4) - 1) / ((target_flags & 0x02000000) ? 8 : 4))));
      endregno = regno + (((regno) >= 8 && (regno) <= (8 + 7)) || (((regno) >= (20 + 1) && (regno) <= ((20 + 1) + 7)) || ((regno) >= (((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) && (regno) <= ((((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) + 7))) || ((regno) >= (((20 + 1) + 7) + 1) && (regno) <= ((((20 + 1) + 7) + 1) + 7)) ? (((mode_class[(int) (((enum machine_mode) (x)->mode))]) == MODE_COMPLEX_INT || (mode_class[(int) (((enum machine_mode) (x)->mode))]) == MODE_COMPLEX_FLOAT) ? 2 : 1) : ((((enum machine_mode) (x)->mode)) == TFmode ? ((target_flags & 0x02000000) ? 2 : 3) : (((enum machine_mode) (x)->mode)) == TCmode ? ((target_flags & 0x02000000) ? 4 : 6) : (((mode_size[(int) (((enum machine_mode) (x)->mode))]) + ((target_flags & 0x02000000) ? 8 : 4) - 1) / ((target_flags & 0x02000000) ? 8 : 4))));

      return endregno > tregno && regno < endtregno;
    }

  else if (((enum rtx_code) (body)->code) == PARALLEL)
    for (i = (((((body)->fld[0]).rtvec))->num_elem) - 1; i >= 0; i--)
      if (reg_bitfield_target_p (x, (((((body)->fld[0]).rtvec))->elem[i])))
 return 1;

  return 0;
}
# 12081 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
static void
distribute_notes (notes, from_insn, i3, i2, elim_i2, elim_i1)
     rtx notes;
     rtx from_insn;
     rtx i3, i2;
     rtx elim_i2, elim_i1;
{
  rtx note, next_note;
  rtx tem;

  for (note = notes; note; note = next_note)
    {
      rtx place = 0, place2 = 0;



      if ((((note)->fld[0]).rtx) && ((enum rtx_code) ((((note)->fld[0]).rtx))->code) == REG
   && ((((((note)->fld[0]).rtx))->fld[0]).rtuint) >= 53)
 (((note)->fld[0]).rtx) = (cfun->emit->x_regno_reg_rtx)[((((((note)->fld[0]).rtx))->fld[0]).rtuint)];

      next_note = (((note)->fld[1]).rtx);
      switch (((enum reg_note) ((enum machine_mode) (note)->mode)))
 {
 case REG_BR_PROB:
 case REG_BR_PRED:
 case REG_EXEC_COUNT:



   place = i3;
   break;

 case REG_VTABLE_REF:


   place = i3;
   break;

 case REG_NON_LOCAL_GOTO:
   if (((enum rtx_code) (i3)->code) == JUMP_INSN)
     place = i3;
   else if (i2 && ((enum rtx_code) (i2)->code) == JUMP_INSN)
     place = i2;
   else
     fancy_abort ("/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c", 12125, __FUNCTION__);
   break;

 case REG_EH_REGION:

   if (((enum rtx_code) (i3)->code) == CALL_INSN)
     place = i3;
   else if (i2 && ((enum rtx_code) (i2)->code) == CALL_INSN)
     place = i2;
   else if (flag_non_call_exceptions)
     {
       if (may_trap_p (i3))
  place = i3;
       else if (i2 && may_trap_p (i2))
  place = i2;



     }
   else
     fancy_abort ("/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c", 12145, __FUNCTION__);
   break;

 case REG_NORETURN:
 case REG_SETJMP:


   if (((enum rtx_code) (i3)->code) == CALL_INSN)
     place = i3;
   else if (i2 && ((enum rtx_code) (i2)->code) == CALL_INSN)
     place = i2;
   else
     fancy_abort ("/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c", 12157, __FUNCTION__);
   break;

 case REG_UNUSED:
# 12178 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
   if (reg_set_p ((((note)->fld[0]).rtx), (((i3)->fld[3]).rtx)))
     {
       if (from_insn != i3)
  break;

       if (! (((enum rtx_code) ((((note)->fld[0]).rtx))->code) == REG
       ? find_regno_note (i3, REG_UNUSED, ((((((note)->fld[0]).rtx))->fld[0]).rtuint))
       : find_reg_note (i3, REG_UNUSED, (((note)->fld[0]).rtx))))
  place = i3;
     }



   else if (reg_referenced_p ((((note)->fld[0]).rtx), (((i3)->fld[3]).rtx))
     && ! (((enum rtx_code) ((((note)->fld[0]).rtx))->code) == REG
    ? find_regno_note (i3, REG_DEAD,
         ((((((note)->fld[0]).rtx))->fld[0]).rtuint))
    : find_reg_note (i3, REG_DEAD, (((note)->fld[0]).rtx))))
     {
       ((note)->mode = (enum machine_mode) ((enum machine_mode) (REG_DEAD)));
       place = i3;
     }
   break;

 case REG_EQUAL:
 case REG_EQUIV:
 case REG_NOALIAS:
# 12216 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
   if (from_insn == i3
       && ((((note)->fld[0]).rtx) == 0 || (((enum rtx_code) ((((note)->fld[0]).rtx))->code) == LABEL_REF || ((enum rtx_code) ((((note)->fld[0]).rtx))->code) == SYMBOL_REF || ((enum rtx_code) ((((note)->fld[0]).rtx))->code) == CONST_INT || ((enum rtx_code) ((((note)->fld[0]).rtx))->code) == CONST_DOUBLE || ((enum rtx_code) ((((note)->fld[0]).rtx))->code) == CONST || ((enum rtx_code) ((((note)->fld[0]).rtx))->code) == HIGH || ((enum rtx_code) ((((note)->fld[0]).rtx))->code) == CONST_VECTOR || ((enum rtx_code) ((((note)->fld[0]).rtx))->code) == CONSTANT_P_RTX)))
     place = i3;
   break;

 case REG_INC:
 case REG_NO_CONFLICT:


   if (reg_mentioned_p ((((note)->fld[0]).rtx), (((i3)->fld[3]).rtx)))
     place = i3;

   if (i2 && reg_mentioned_p ((((note)->fld[0]).rtx), (((i2)->fld[3]).rtx)))
     {
       if (place)
  place2 = i2;
       else
  place = i2;
     }
   break;

 case REG_LABEL:




   if (reg_mentioned_p ((((note)->fld[0]).rtx), (((i3)->fld[3]).rtx))
       || ((tem = find_reg_note (i3, REG_EQUAL, (rtx) 0))
    && ((enum rtx_code) ((((tem)->fld[0]).rtx))->code) == LABEL_REF
    && ((((((tem)->fld[0]).rtx))->fld[0]).rtx) == (((note)->fld[0]).rtx)))
     place = i3;

   if (i2
       && (reg_mentioned_p ((((note)->fld[0]).rtx), (((i2)->fld[3]).rtx))
    || ((tem = find_reg_note (i2, REG_EQUAL, (rtx) 0))
        && ((enum rtx_code) ((((tem)->fld[0]).rtx))->code) == LABEL_REF
        && ((((((tem)->fld[0]).rtx))->fld[0]).rtx) == (((note)->fld[0]).rtx))))
     {
       if (place)
  place2 = i2;
       else
  place = i2;
     }



   if (place && ((enum rtx_code) (place)->code) == JUMP_INSN && (((place)->fld[7]).rtx))
     {
       if ((((place)->fld[7]).rtx) != (((note)->fld[0]).rtx))
  fancy_abort ("/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c", 12265, __FUNCTION__);
       if (((enum rtx_code) ((((place)->fld[7]).rtx))->code) == CODE_LABEL)
  ((((((place)->fld[7]).rtx))->fld[3]).rtint)--;
       place = 0;
     }
   if (place2 && ((enum rtx_code) (place2)->code) == JUMP_INSN && (((place2)->fld[7]).rtx))
     {
       if ((((place2)->fld[7]).rtx) != (((note)->fld[0]).rtx))
  fancy_abort ("/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c", 12273, __FUNCTION__);
       if (((enum rtx_code) ((((place2)->fld[7]).rtx))->code) == CODE_LABEL)
  ((((((place2)->fld[7]).rtx))->fld[3]).rtint)--;
       place2 = 0;
     }
   break;

 case REG_NONNEG:
 case REG_WAS_0:




   break;

 case REG_RETVAL:



   if (((enum rtx_code) (from_insn)->code) != NOTE)
     place = from_insn;
   else
     {
       tem = find_reg_note ((((note)->fld[0]).rtx), REG_LIBCALL, (rtx) 0);
       place = prev_real_insn (from_insn);
       if (tem && place)
  (((tem)->fld[0]).rtx) = place;


       else if ((((note)->fld[0]).rtx) == from_insn)
  tem = place = 0;
     }
   break;

 case REG_LIBCALL:

   if (((enum rtx_code) (from_insn)->code) != NOTE)
     place = from_insn;
   else
     {
       tem = find_reg_note ((((note)->fld[0]).rtx), REG_RETVAL, (rtx) 0);
       place = next_real_insn (from_insn);
       if (tem && place)
  (((tem)->fld[0]).rtx) = place;


       else if ((((note)->fld[0]).rtx) == from_insn)
  tem = place = 0;
     }
   break;

 case REG_DEAD:
# 12339 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
   if (from_insn
       && ((enum rtx_code) (from_insn)->code) == CALL_INSN
       && find_reg_fusage (from_insn, USE, (((note)->fld[0]).rtx)))
     place = from_insn;
   else if (reg_referenced_p ((((note)->fld[0]).rtx), (((i3)->fld[3]).rtx)))
     place = i3;
   else if (i2 != 0 && next_nonnote_insn (i2) == i3
     && reg_referenced_p ((((note)->fld[0]).rtx), (((i2)->fld[3]).rtx)))
     place = i2;

   if (rtx_equal_p ((((note)->fld[0]).rtx), elim_i2)
       || rtx_equal_p ((((note)->fld[0]).rtx), elim_i1))
     break;

   if (place == 0)
     {
       basic_block bb = (((basic_block_info)->data.bb[(this_basic_block)]));

       for (tem = (((i3)->fld[1]).rtx); place == 0; tem = (((tem)->fld[1]).rtx))
  {
    if (! ((rtx_class[(int) (((enum rtx_code) (tem)->code))]) == 'i'))
      {
        if (tem == bb->head)
   break;
        continue;
      }




    if (reg_set_p ((((note)->fld[0]).rtx), (((tem)->fld[3]).rtx)))
      {
        rtx set = (((rtx_class[(int) (((enum rtx_code) (tem)->code))]) == 'i') ? (((enum rtx_code) ((((tem)->fld[3]).rtx))->code) == SET ? (((tem)->fld[3]).rtx) : single_set_2 (tem, (((tem)->fld[3]).rtx))) : (rtx) 0);
        rtx inner_dest = 0;




        if (set != 0)
   for (inner_dest = (((set)->fld[0]).rtx);
        (((enum rtx_code) (inner_dest)->code) == STRICT_LOW_PART
         || ((enum rtx_code) (inner_dest)->code) == SUBREG
         || ((enum rtx_code) (inner_dest)->code) == ZERO_EXTRACT);
        inner_dest = (((inner_dest)->fld[0]).rtx))
     ;
# 12393 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
        if (set != 0 && ! side_effects_p ((((set)->fld[1]).rtx))
     && rtx_equal_p ((((note)->fld[0]).rtx), inner_dest)





     )
   {





     (((tem)->fld[3]).rtx) = (global_rtl[GR_PC]);

     distribute_notes ((((tem)->fld[6]).rtx), tem, tem,
         (rtx) 0, (rtx) 0, (rtx) 0);
     distribute_links ((((tem)->fld[5]).rtx));

     ((tem)->code = (enum rtx_code) (NOTE));
     (((tem)->fld[4]).rtint) = NOTE_INSN_DELETED;
     (((tem)->fld[3]).rtstr) = 0;
# 12434 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
   }



        else if (reg_referenced_p ((((note)->fld[0]).rtx),
       (((tem)->fld[3]).rtx)))
   {
     place = tem;

     if (! find_regno_note (tem, REG_UNUSED,
       ((((((note)->fld[0]).rtx))->fld[0]).rtuint)))
       (((tem)->fld[6]).rtx)
         = gen_rtx_fmt_ee (EXPR_LIST, (REG_UNUSED), ((((note)->fld[0]).rtx)), ((((tem)->fld[6]).rtx)));

   }
        else
   {
     ((note)->mode = (enum machine_mode) ((enum machine_mode) (REG_UNUSED)));



     if (! find_regno_note (tem, REG_UNUSED,
       ((((((note)->fld[0]).rtx))->fld[0]).rtuint)))
       place = tem;
     break;
   }
      }
    else if (reg_referenced_p ((((note)->fld[0]).rtx), (((tem)->fld[3]).rtx))
      || (((enum rtx_code) (tem)->code) == CALL_INSN
          && find_reg_fusage (tem, USE, (((note)->fld[0]).rtx))))
      {
        place = tem;







        if (i2 && (((place)->fld[0]).rtint) <= max_uid_cuid
     && ((((place)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (place) : uid_cuid[(((place)->fld[0]).rtint)]) > ((((i2)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (i2) : uid_cuid[(((i2)->fld[0]).rtint)])
     && from_insn
     && ((((from_insn)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (from_insn) : uid_cuid[(((from_insn)->fld[0]).rtint)]) > ((((i2)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (i2) : uid_cuid[(((i2)->fld[0]).rtint)])
     && reg_referenced_p ((((note)->fld[0]).rtx), (((i2)->fld[3]).rtx)))
   {
     rtx links = (((place)->fld[5]).rtx);
     (((place)->fld[5]).rtx) = 0;
     distribute_links (links);
   }
        break;
      }

    if (tem == bb->head)
      break;
  }






       if (((enum reg_note) ((enum machine_mode) (note)->mode)) == REG_DEAD && place == 0
    && bitmap_bit_p (bb->global_live_at_start, ((((((note)->fld[0]).rtx))->fld[0]).rtuint)))

  {
    ((refresh_blocks)->elms [(this_basic_block) / ((unsigned) (8 * 8))] |= (unsigned long long) 1 << (this_basic_block) % ((unsigned) (8 * 8)));
    need_refresh = 1;
  }
     }







   if (place && ((enum reg_note) ((enum machine_mode) (note)->mode)) == REG_DEAD)
     {
       unsigned int regno = ((((((note)->fld[0]).rtx))->fld[0]).rtuint);




       if (noop_move_p (place))
  {
    ((refresh_blocks)->elms [(this_basic_block) / ((unsigned) (8 * 8))] |= (unsigned long long) 1 << (this_basic_block) % ((unsigned) (8 * 8)));
    need_refresh = 1;
  }

       if (dead_or_set_p (place, (((note)->fld[0]).rtx))
    || reg_bitfield_target_p ((((note)->fld[0]).rtx), (((place)->fld[3]).rtx)))
  {



    if (reg_last_death[regno] != place)
      reg_last_death[regno] = 0;
    place = 0;
  }
       else
  reg_last_death[regno] = place;
# 12545 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
       if (place && regno < 53
    && (((regno) >= 8 && (regno) <= (8 + 7)) || (((regno) >= (20 + 1) && (regno) <= ((20 + 1) + 7)) || ((regno) >= (((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) && (regno) <= ((((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) + 7))) || ((regno) >= (((20 + 1) + 7) + 1) && (regno) <= ((((20 + 1) + 7) + 1) + 7)) ? (((mode_class[(int) (((enum machine_mode) ((((note)->fld[0]).rtx))->mode))]) == MODE_COMPLEX_INT || (mode_class[(int) (((enum machine_mode) ((((note)->fld[0]).rtx))->mode))]) == MODE_COMPLEX_FLOAT) ? 2 : 1) : ((((enum machine_mode) ((((note)->fld[0]).rtx))->mode)) == TFmode ? ((target_flags & 0x02000000) ? 2 : 3) : (((enum machine_mode) ((((note)->fld[0]).rtx))->mode)) == TCmode ? ((target_flags & 0x02000000) ? 4 : 6) : (((mode_size[(int) (((enum machine_mode) ((((note)->fld[0]).rtx))->mode))]) + ((target_flags & 0x02000000) ? 8 : 4) - 1) / ((target_flags & 0x02000000) ? 8 : 4)))) > 1)
  {
    unsigned int endregno
      = regno + (((regno) >= 8 && (regno) <= (8 + 7)) || (((regno) >= (20 + 1) && (regno) <= ((20 + 1) + 7)) || ((regno) >= (((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) && (regno) <= ((((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) + 7))) || ((regno) >= (((20 + 1) + 7) + 1) && (regno) <= ((((20 + 1) + 7) + 1) + 7)) ? (((mode_class[(int) (((enum machine_mode) ((((note)->fld[0]).rtx))->mode))]) == MODE_COMPLEX_INT || (mode_class[(int) (((enum machine_mode) ((((note)->fld[0]).rtx))->mode))]) == MODE_COMPLEX_FLOAT) ? 2 : 1) : ((((enum machine_mode) ((((note)->fld[0]).rtx))->mode)) == TFmode ? ((target_flags & 0x02000000) ? 2 : 3) : (((enum machine_mode) ((((note)->fld[0]).rtx))->mode)) == TCmode ? ((target_flags & 0x02000000) ? 4 : 6) : (((mode_size[(int) (((enum machine_mode) ((((note)->fld[0]).rtx))->mode))]) + ((target_flags & 0x02000000) ? 8 : 4) - 1) / ((target_flags & 0x02000000) ? 8 : 4))));

    int all_used = 1;
    unsigned int i;

    for (i = regno; i < endregno; i++)
      if ((! refers_to_regno_p (i, i + 1, (((place)->fld[3]).rtx), 0)
    && ! find_regno_fusage (place, USE, i))
   || dead_or_set_regno_p (place, i))
        all_used = 0;

    if (! all_used)
      {



        for (i = regno; i < endregno;
      i += (((i) >= 8 && (i) <= (8 + 7)) || (((i) >= (20 + 1) && (i) <= ((20 + 1) + 7)) || ((i) >= (((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) && (i) <= ((((((((20 + 1) + 7) + 1) + 7) + 1) + 7) + 1) + 7))) || ((i) >= (((20 + 1) + 7) + 1) && (i) <= ((((20 + 1) + 7) + 1) + 7)) ? (((mode_class[(int) (reg_raw_mode[i])]) == MODE_COMPLEX_INT || (mode_class[(int) (reg_raw_mode[i])]) == MODE_COMPLEX_FLOAT) ? 2 : 1) : ((reg_raw_mode[i]) == TFmode ? ((target_flags & 0x02000000) ? 2 : 3) : (reg_raw_mode[i]) == TCmode ? ((target_flags & 0x02000000) ? 4 : 6) : (((mode_size[(int) (reg_raw_mode[i])]) + ((target_flags & 0x02000000) ? 8 : 4) - 1) / ((target_flags & 0x02000000) ? 8 : 4)))))
   {
     rtx piece = gen_rtx_REG (reg_raw_mode[i], i);
     basic_block bb = (((basic_block_info)->data.bb[(this_basic_block)]));

     if (! dead_or_set_p (place, piece)
         && ! reg_bitfield_target_p (piece,
         (((place)->fld[3]).rtx)))
       {
         rtx new_note
    = gen_rtx_fmt_ee (EXPR_LIST, (REG_DEAD), (piece), ((rtx) 0));

         distribute_notes (new_note, place, place,
      (rtx) 0, (rtx) 0, (rtx) 0);
       }
     else if (! refers_to_regno_p (i, i + 1,
       (((place)->fld[3]).rtx), 0)
       && ! find_regno_fusage (place, USE, i))
       for (tem = (((place)->fld[1]).rtx); ;
     tem = (((tem)->fld[1]).rtx))
         {
    if (! ((rtx_class[(int) (((enum rtx_code) (tem)->code))]) == 'i'))
      {
        if (tem == bb->head)
          {
     ((refresh_blocks)->elms [(this_basic_block) / ((unsigned) (8 * 8))] |= (unsigned long long) 1 << (this_basic_block) % ((unsigned) (8 * 8)));

     need_refresh = 1;
     break;
          }
        continue;
      }
    if (dead_or_set_p (tem, piece)
        || reg_bitfield_target_p (piece,
             (((tem)->fld[3]).rtx)))
      {
        (((tem)->fld[6]).rtx)
          = gen_rtx_fmt_ee (EXPR_LIST, (REG_UNUSED), (piece), ((((tem)->fld[6]).rtx)));

        break;
      }
         }

   }

        place = 0;
      }
  }
     }
   break;

 default:


   fancy_abort ("/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c", 12620, __FUNCTION__);
 }

      if (place)
 {
   (((note)->fld[1]).rtx) = (((place)->fld[6]).rtx);
   (((place)->fld[6]).rtx) = note;
 }
      else if ((((enum reg_note) ((enum machine_mode) (note)->mode)) == REG_DEAD
  || ((enum reg_note) ((enum machine_mode) (note)->mode)) == REG_UNUSED)
        && ((enum rtx_code) ((((note)->fld[0]).rtx))->code) == REG)
 (((reg_n_info)->data.reg[((((((note)->fld[0]).rtx))->fld[0]).rtuint)])->deaths)--;

      if (place2)
 {
   if ((((enum reg_note) ((enum machine_mode) (note)->mode)) == REG_DEAD
        || ((enum reg_note) ((enum machine_mode) (note)->mode)) == REG_UNUSED)
       && ((enum rtx_code) ((((note)->fld[0]).rtx))->code) == REG)
     (((reg_n_info)->data.reg[((((((note)->fld[0]).rtx))->fld[0]).rtuint)])->deaths)++;

   (((place2)->fld[6]).rtx) = gen_rtx_fmt_ee (((enum rtx_code) (note)->code),
            ((enum reg_note) ((enum machine_mode) (note)->mode)),
            (((note)->fld[0]).rtx),
            (((place2)->fld[6]).rtx));
 }
    }
}





static void
distribute_links (links)
     rtx links;
{
  rtx link, next_link;

  for (link = links; link; link = next_link)
    {
      rtx place = 0;
      rtx insn;
      rtx set, reg;

      next_link = (((link)->fld[1]).rtx);
# 12677 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
      if (((enum rtx_code) ((((link)->fld[0]).rtx))->code) == NOTE
   || (set = (((rtx_class[(int) (((enum rtx_code) ((((link)->fld[0]).rtx))->code))]) == 'i') ? (((enum rtx_code) (((((((link)->fld[0]).rtx))->fld[3]).rtx))->code) == SET ? ((((((link)->fld[0]).rtx))->fld[3]).rtx) : single_set_2 ((((link)->fld[0]).rtx), ((((((link)->fld[0]).rtx))->fld[3]).rtx))) : (rtx) 0)) == 0)
 continue;

      reg = (((set)->fld[0]).rtx);
      while (((enum rtx_code) (reg)->code) == SUBREG || ((enum rtx_code) (reg)->code) == ZERO_EXTRACT
      || ((enum rtx_code) (reg)->code) == SIGN_EXTRACT
      || ((enum rtx_code) (reg)->code) == STRICT_LOW_PART)
 reg = (((reg)->fld[0]).rtx);
# 12696 "/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c"
      for (insn = ((((((link)->fld[0]).rtx))->fld[2]).rtx);
    (insn && (this_basic_block == n_basic_blocks - 1
       || ((((basic_block_info)->data.bb[(this_basic_block + 1)]))->head) != insn));
    insn = (((insn)->fld[2]).rtx))
 if (((rtx_class[(int) (((enum rtx_code) (insn)->code))]) == 'i') && reg_overlap_mentioned_p (reg, (((insn)->fld[3]).rtx)))
   {
     if (reg_referenced_p (reg, (((insn)->fld[3]).rtx)))
       place = insn;
     break;
   }
 else if (((enum rtx_code) (insn)->code) == CALL_INSN
   && find_reg_fusage (insn, USE, reg))
   {
     place = insn;
     break;
   }




      if (place)
 {
   rtx link2;

   for (link2 = (((place)->fld[5]).rtx); link2; link2 = (((link2)->fld[1]).rtx))
     if ((((link2)->fld[0]).rtx) == (((link)->fld[0]).rtx))
       break;

   if (link2 == 0)
     {
       (((link)->fld[1]).rtx) = (((place)->fld[5]).rtx);
       (((place)->fld[5]).rtx) = link;



       if (added_links_insn == 0
    || ((((added_links_insn)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (added_links_insn) : uid_cuid[(((added_links_insn)->fld[0]).rtint)]) > ((((place)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (place) : uid_cuid[(((place)->fld[0]).rtint)]))
  added_links_insn = place;
     }
 }
    }
}



static int
insn_cuid (insn)
     rtx insn;
{
  while (insn != 0 && (((insn)->fld[0]).rtint) > max_uid_cuid
  && ((enum rtx_code) (insn)->code) == INSN && ((enum rtx_code) ((((insn)->fld[3]).rtx))->code) == USE)
    insn = (((insn)->fld[2]).rtx);

  if ((((insn)->fld[0]).rtint) > max_uid_cuid)
    fancy_abort ("/Users/ddunbar/compiler-group/test-suite-externals/trunk/lnt-compile-suite-src/403.gcc/combine.c", 12750, __FUNCTION__);

  return ((((insn)->fld[0]).rtint) > max_uid_cuid ? insn_cuid (insn) : uid_cuid[(((insn)->fld[0]).rtint)]);
}

void
dump_combine_stats (file)
     FILE *file;
{
  fnotice
    (file,
     ";; Combiner statistics: %d attempts, %d substitutions (%d requiring new space),\n;; %d successes.\n\n",
     combine_attempts, combine_merges, combine_extras, combine_successes);
}

void
dump_combine_total_stats (file)
     FILE *file;
{
  fnotice
    (file,
     "\n;; Combiner totals: %d attempts, %d substitutions (%d requiring new space),\n;; %d successes.\n",
     total_attempts, total_merges, total_extras, total_successes);
}
