// RUN: %clang -Wnon-modular-include-in-framework-module -Wno-trigraphs -Wno-missing-field-initializers -Wno-missing-prototypes -Wreturn-type -Wunreachable-code -Wdeprecated-objc-isa-usage -Wobjc-root-class -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wconditional-uninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wshorten-64-to-32 -Wno-newline-eof -Wdeprecated-declarations -Wno-sign-conversion -Wno-implicit-atomic-properties -Wno-receiver-is-weak -Wno-arc-repeated-use-of-weak -Wduplicate-method-match -Wno-selector -Wno-strict-selector-match -Wundeclared-selector -Wno-deprecated-implementations -Wprotocol -c %s -O0 -g -include %test_root/Cocoa_Prefix_Precompiled.h 
// Assumes prefix file is used.

@interface A : NSString
-(void) f0;
@end

@implementation A
-(void) f0 {}
@end
