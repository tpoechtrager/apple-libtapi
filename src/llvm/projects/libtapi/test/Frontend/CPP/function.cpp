// RUN: %tapi-frontend -target i386-apple-macos10.12 %s 2>&1 | FileCheck %s
// RUN: %tapi-frontend -target x86_64-apple-macos10.12 %s 2>&1 | FileCheck %s

// CHECK-LABEL: functions:

// CHECK-NEXT: - name: __Z3fooi
// CHECK-NEXT:   loc:
// CHECK-NEXT:   availability: i:0 o:0 u:0
// CHECK-NEXT:   isWeakDefined: false
// CHECK-NEXT:   inlined: true
inline int foo(int x) { return x + 1; }

// CHECK-NEXT:  - name: __Z3bari
// CHECK-NEXT:    loc:
// CHECK-NEXT:    availability: i:0 o:0 u:0
// CHECK-NEXT:    isWeakDefined: false
// CHECK-NEXT:   inlined: false
extern int bar(int x) { return x + 1; }

// CHECK-NEXT: - name: __Z3bazi
// CHECK-NEXT:   loc:
// CHECK-NEXT:   availability: i:0 o:0 u:0
// CHECK-NEXT:   isWeakDefined: false
// CHECK-NEXT:   inlined: true
inline int baz(int x) {
  static const int a[] = {1, 2, 3};
  return a[x];
}
