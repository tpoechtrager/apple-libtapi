// RUN: %clang_cc1 -triple x86_64-apple-macosx10.11.0 -Wdeprecated-objc-isa-usage -Werror=deprecated-objc-isa-usage -emit-obj -mrelax-all -disable-free -disable-llvm-verifier -main-file-name trivial-input.cpp -mrelocation-model pic -pic-level 2 -mthread-model posix -mdisable-fp-elim -masm-verbose -munwind-tables -target-cpu core2 -target-linker-version 253.9 -gdwarf-2 -dwarf-column-info -stdlib=libc++ -O0 -Wnon-modular-include-in-framework-module -Wno-trigraphs -Wno-missing-field-initializers -Wno-missing-prototypes -Wreturn-type -Wunreachable-code -Wdeprecated-objc-isa-usage -Wobjc-root-class -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wconditional-uninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wshorten-64-to-32 -Wno-newline-eof -Wdeprecated-declarations -Wno-sign-conversion -Wno-non-virtual-dtor -Wno-overloaded-virtual -Wno-exit-time-destructors -Wno-c++11-extensions -Winvalid-offsetof -fdeprecated-macro -ferror-limit 19 -fmessage-length 0 -stack-protector 1 -mstackrealign -fblocks -fobjc-runtime=macosx-10.11.0 -fencode-extended-block-signature -fcxx-exceptions -fexceptions -fmax-type-align=16 -fdiagnostics-show-option -x c++ %s

template <typename T>
class C {
  T data;

public:
  virtual T f0() { return data; }
};

int f0(C<int> x) {
  return x.f0();
}
