// RUN: %tapi-frontend -target i386-apple-macos10.12 %s 2>&1 | FileCheck %s
// RUN: %tapi-frontend -target x86_64-apple-macos10.12 %s 2>&1 | FileCheck %s

// CHECK:      - name: _sincos
// CHECK-NEXT:   loc:
// CHECK-NEXT:   availability: i:0 o:0 u:0
// CHECK-NEXT:   isWeakDefined: false
// CHECK-NEXT:   inlined: false
inline __attribute__((always_inline)) void sincos();
inline __attribute__((always_inline)) void sincos() {}
extern inline __attribute__((always_inline)) void sincos();
