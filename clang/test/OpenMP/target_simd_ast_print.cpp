// RUN: %clang_cc1 -verify -fopenmp -fopenmp-version=45 -ast-print %s -Wno-openmp-mapping | FileCheck %s --check-prefix=CHECK --check-prefix=OMP45
// RUN: %clang_cc1 -fopenmp -fopenmp-version=45 -x c++ -std=c++11 -emit-pch -o %t %s -Wno-openmp-mapping
// RUN: %clang_cc1 -fopenmp -fopenmp-version=45 -std=c++11 -include-pch %t -verify %s -ast-print -Wno-openmp-mapping | FileCheck %s --check-prefix=CHECK --check-prefix=OMP45
// RUN: %clang_cc1 -verify -fopenmp -fopenmp-version=50 -DOMP5 -ast-print %s -Wno-openmp-mapping | FileCheck %s --check-prefix=CHECK --check-prefix=OMP50
// RUN: %clang_cc1 -fopenmp -fopenmp-version=50 -DOMP5 -x c++ -std=c++11 -emit-pch -o %t %s -Wno-openmp-mapping
// RUN: %clang_cc1 -fopenmp -fopenmp-version=50 -DOMP5 -std=c++11 -include-pch %t -verify %s -ast-print -Wno-openmp-mapping | FileCheck %s --check-prefix=CHECK --check-prefix=OMP50
// RUN: %clang_cc1 -verify -fopenmp -fopenmp-version=51 -ast-print %s -Wno-openmp-mapping -DOMP51 | FileCheck %s --check-prefix=CHECK --check-prefix=OMP51
// RUN: %clang_cc1 -fopenmp -fopenmp-version=51 -x c++ -std=c++11 -emit-pch -o %t %s -Wno-openmp-mapping -DOMP51
// RUN: %clang_cc1 -fopenmp -fopenmp-version=51 -std=c++11 -include-pch %t -verify %s -ast-print -Wno-openmp-mapping -DOMP51 | FileCheck %s --check-prefix=CHECK --check-prefix=OMP51
// RUN: %clang_cc1 -verify -fopenmp -fopenmp-version=52 -ast-print %s -Wno-openmp-mapping -DOMP52 | FileCheck %s --check-prefix=CHECK --check-prefix=OMP52
// RUN: %clang_cc1 -fopenmp -fopenmp-version=52 -x c++ -std=c++11 -emit-pch -o %t %s -Wno-openmp-mapping -DOMP52
// RUN: %clang_cc1 -fopenmp -fopenmp-version=52 -std=c++11 -include-pch %t -verify %s -ast-print -Wno-openmp-mapping -DOMP52 | FileCheck %s --check-prefix=CHECK --check-prefix=OMP52

// RUN: %clang_cc1 -verify -fopenmp-simd -fopenmp-version=45 -ast-print %s -Wno-openmp-mapping | FileCheck %s --check-prefix=CHECK --check-prefix=OMP45
// RUN: %clang_cc1 -fopenmp-simd -fopenmp-version=45 -x c++ -std=c++11 -emit-pch -o %t %s -Wno-openmp-mapping
// RUN: %clang_cc1 -fopenmp-simd -fopenmp-version=45 -std=c++11 -include-pch %t -verify %s -ast-print -Wno-openmp-mapping | FileCheck %s --check-prefix=CHECK --check-prefix=OMP45
// RUN: %clang_cc1 -verify -fopenmp-simd -fopenmp-version=50 -DOMP5 -ast-print %s -Wno-openmp-mapping | FileCheck %s --check-prefix=CHECK --check-prefix=OMP50
// RUN: %clang_cc1 -fopenmp-simd -fopenmp-version=50 -DOMP5 -x c++ -std=c++11 -emit-pch -o %t %s -Wno-openmp-mapping
// RUN: %clang_cc1 -fopenmp-simd -fopenmp-version=50 -DOMP5 -std=c++11 -include-pch %t -verify %s -ast-print -Wno-openmp-mapping | FileCheck %s --check-prefix=CHECK --check-prefix=OMP50
// RUN: %clang_cc1 -verify -fopenmp-simd -fopenmp-version=51 -ast-print %s -Wno-openmp-mapping -DOMP51 | FileCheck %s --check-prefix=CHECK --check-prefix=OMP51
// RUN: %clang_cc1 -fopenmp-simd -fopenmp-version=51 -x c++ -std=c++11 -emit-pch -o %t %s -Wno-openmp-mapping -DOMP51
// RUN: %clang_cc1 -fopenmp-simd -fopenmp-version=51 -std=c++11 -include-pch %t -verify %s -ast-print -Wno-openmp-mapping -DOMP51 | FileCheck %s --check-prefix=CHECK --check-prefix=OMP51
// expected-no-diagnostics
// RUN: %clang_cc1 -verify -fopenmp-simd -fopenmp-version=52 -ast-print %s -Wno-openmp-mapping -DOMP52 | FileCheck %s --check-prefix=CHECK --check-prefix=OMP52
// RUN: %clang_cc1 -fopenmp-simd -fopenmp-version=52 -x c++ -std=c++11 -emit-pch -o %t %s -Wno-openmp-mapping -DOMP52
// RUN: %clang_cc1 -fopenmp-simd -fopenmp-version=52 -std=c++11 -include-pch %t -verify %s -ast-print -Wno-openmp-mapping -DOMP52 | FileCheck %s --check-prefix=CHECK --check-prefix=OMP52
// expected-no-diagnostics

#ifndef HEADER
#define HEADER

typedef void **omp_allocator_handle_t;
extern const omp_allocator_handle_t omp_null_allocator;
extern const omp_allocator_handle_t omp_default_mem_alloc;
extern const omp_allocator_handle_t omp_large_cap_mem_alloc;
extern const omp_allocator_handle_t omp_const_mem_alloc;
extern const omp_allocator_handle_t omp_high_bw_mem_alloc;
extern const omp_allocator_handle_t omp_low_lat_mem_alloc;
extern const omp_allocator_handle_t omp_cgroup_mem_alloc;
extern const omp_allocator_handle_t omp_pteam_mem_alloc;
extern const omp_allocator_handle_t omp_thread_mem_alloc;

void foo() {}

struct S {
  S(): a(0) {}
  S(int v) : a(v) {}
  int a;
  typedef int type;
};

template <typename T>
class S7 : public T {
protected:
  T a;
  S7() : a(0) {}

public:
  S7(typename T::type v) : a(v) {
#pragma omp target simd private(a) private(this->a) private(T::a)
    for (int k = 0; k < a.a; ++k)
      ++this->a.a;
  }
  S7 &operator=(S7 &s) {
#pragma omp target simd private(a) private(this->a)
    for (int k = 0; k < s.a.a; ++k)
      ++s.a.a;
    return *this;
  }
};

// CHECK: #pragma omp target simd private(this->a) private(this->a) private(T::a){{$}}
// CHECK: #pragma omp target simd private(this->a) private(this->a)
// CHECK: #pragma omp target simd private(this->a) private(this->a) private(this->S::a)

class S8 : public S7<S> {
  S8() {}

public:
  S8(int v) : S7<S>(v){
#pragma omp target simd private(a) private(this->a) private(S7<S>::a)
    for (int k = 0; k < a.a; ++k)
      ++this->a.a;
  }
  S8 &operator=(S8 &s) {
#pragma omp target simd private(a) private(this->a)
    for (int k = 0; k < s.a.a; ++k)
      ++s.a.a;
    return *this;
  }
};

// CHECK: #pragma omp target simd private(this->a) private(this->a) private(this->S7<S>::a)
// CHECK: #pragma omp target simd private(this->a) private(this->a)

template <class T, int N>
T tmain(T argc, T *argv) {
  T b = argc, c, d, e, f, h;
  T arr[N][10], arr1[N];
  T i, j;
  T s;
  static T a;
// CHECK: static T a;
  static T g;
  const T clen = 5;
// CHECK: T clen = 5;
  T *p;
#pragma omp threadprivate(g)
#pragma omp target simd linear(a) allocate(a)
  // CHECK: #pragma omp target simd linear(a) allocate(a)
  for (T i = 0; i < 2; ++i)
    a = 2;
// CHECK-NEXT: for (T i = 0; i < 2; ++i)
// CHECK-NEXT: a = 2;
#pragma omp target simd allocate(f) private(argc, b), firstprivate(c, d), lastprivate(d, f) collapse(N) if (target :argc) reduction(+ : h)
  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 2; ++j)
      for (int j = 0; j < 2; ++j)
        for (int j = 0; j < 2; ++j)
          for (int j = 0; j < 2; ++j)
            foo();
  // CHECK-NEXT: #pragma omp target simd allocate(f) private(argc,b) firstprivate(c,d) lastprivate(d,f) collapse(N) if(target: argc) reduction(+: h)
  // CHECK-NEXT: for (int i = 0; i < 2; ++i)
  // CHECK-NEXT: for (int j = 0; j < 2; ++j)
  // CHECK-NEXT: for (int j = 0; j < 2; ++j)
  // CHECK-NEXT: for (int j = 0; j < 2; ++j)
  // CHECK-NEXT: for (int j = 0; j < 2; ++j)
  // CHECK-NEXT: foo();

#pragma omp target simd private(argc,b) firstprivate(argv) if(target:argc > 0) reduction(+:c, arr1[argc]) reduction(max:e, arr[:N][0:10])
  for (T i = 0; i < 2; ++i) {}
  // CHECK: #pragma omp target simd private(argc,b) firstprivate(argv) if(target: argc > 0) reduction(+: c,arr1[argc]) reduction(max: e,arr[:N][0:10])
  // CHECK-NEXT: for (T i = 0; i < 2; ++i) {
  // CHECK-NEXT: }

#pragma omp target simd if(N) reduction(^:e, f, arr[0:N][:argc]) reduction(&& : h)
  for (T i = 0; i < 2; ++i) {}
  // CHECK: #pragma omp target simd if(N) reduction(^: e,f,arr[0:N][:argc]) reduction(&&: h)
  // CHECK-NEXT: for (T i = 0; i < 2; ++i) {
  // CHECK-NEXT: }

#ifdef OMP5
#pragma omp target simd if(target:argc > 0) if (simd:argc) allocate(omp_default_mem_alloc:f) private(f) uses_allocators(omp_default_mem_alloc)
#else
#pragma omp target simd if(target:argc > 0)
#endif // OMP5
  for (T i = 0; i < 2; ++i) {}
  // OMP45: #pragma omp target simd if(target: argc > 0)
  // OMP50: #pragma omp target simd if(target: argc > 0) if(simd: argc) allocate(omp_default_mem_alloc: f) private(f) uses_allocators(omp_default_mem_alloc)
  // OMP51: #pragma omp target simd if(target: argc > 0)
  // OMP52: #pragma omp target simd if(target: argc > 0)
  // CHECK-NEXT: for (T i = 0; i < 2; ++i) {
  // CHECK-NEXT: }

#pragma omp target simd if(N)
  for (T i = 0; i < 2; ++i) {}
  // CHECK: #pragma omp target simd if(N)
  // CHECK-NEXT: for (T i = 0; i < 2; ++i) {
  // CHECK-NEXT: }

#pragma omp target simd map(i)
  for (T i = 0; i < 2; ++i) {}
  // CHECK: #pragma omp target simd map(tofrom: i)
  // CHECK-NEXT: for (T i = 0; i < 2; ++i) {
  // CHECK-NEXT: }

#pragma omp target simd map(arr1[0:10], i)
  for (T i = 0; i < 2; ++i) {}
  // CHECK: #pragma omp target simd map(tofrom: arr1[0:10],i)
  // CHECK-NEXT: for (T i = 0; i < 2; ++i) {
  // CHECK-NEXT: }

#pragma omp target simd map(to: i) map(from: j)
  for (T i = 0; i < 2; ++i) {}
  // CHECK: #pragma omp target simd map(to: i) map(from: j)
  // CHECK-NEXT: for (T i = 0; i < 2; ++i) {
  // CHECK-NEXT: }

#pragma omp target simd map(always,alloc: i)
  for (T i = 0; i < 2; ++i) {}
  // CHECK: #pragma omp target simd map(always,alloc: i)
  // CHECK-NEXT: for (T i = 0; i < 2; ++i) {
  // CHECK-NEXT: }

#pragma omp target simd nowait
  for (T i = 0; i < 2; ++i) {}
  // CHECK: #pragma omp target simd nowait
  // CHECK-NEXT: for (T i = 0; i < 2; ++i) {
  // CHECK-NEXT: }

#pragma omp target simd depend(in : argc, arr[i:argc], arr1[:])
  for (T i = 0; i < 2; ++i) {}
  // CHECK: #pragma omp target simd depend(in : argc,arr[i:argc],arr1[:])
  // CHECK-NEXT: for (T i = 0; i < 2; ++i) {
  // CHECK-NEXT: }

#pragma omp target simd defaultmap(tofrom: scalar)
  for (T i = 0; i < 2; ++i) {}
  // CHECK: #pragma omp target simd defaultmap(tofrom: scalar)
  // CHECK-NEXT: for (T i = 0; i < 2; ++i) {
  // CHECK-NEXT: }

#pragma omp target simd safelen(clen-1)
  for (T i = 0; i < 2; ++i) {}
  // CHECK: #pragma omp target simd safelen(clen - 1)
  // CHECK-NEXT: for (T i = 0; i < 2; ++i) {
  // CHECK-NEXT: }

#pragma omp target simd simdlen(clen-1)
  for (T i = 0; i < 2; ++i) {}
  // CHECK: #pragma omp target simd simdlen(clen - 1)
  // CHECK-NEXT: for (T i = 0; i < 2; ++i) {
  // CHECK-NEXT: }

#pragma omp target simd aligned(arr1:N-1)
  for (T i = 0; i < N; ++i) {}
  // CHECK: #pragma omp target simd aligned(arr1: N - 1)
  // CHECK-NEXT: for (T i = 0; i < N; ++i) {
  // CHECK-NEXT: }

#pragma omp target simd is_device_ptr(p)
  for (T i = 0; i < N; ++i) {}
  // CHECK: #pragma omp target simd is_device_ptr(p)
  // CHECK-NEXT: for (T i = 0; i < N; ++i) {
  // CHECK-NEXT: }

  return T();
}

int main(int argc, char **argv) {
  int b = argc, c, d, e, f, h;
  int arr[5][10], arr1[5];
  int i, j;
  int s;
  static int a;
// CHECK: static int a;
  const int clen = 5;
// CHECK: int clen = 5;
  static float g;
#pragma omp threadprivate(g)
  int *p;
#pragma omp target simd linear(a)
  // CHECK: #pragma omp target simd linear(a)
  for (int i = 0; i < 2; ++i)
    a = 2;
// CHECK-NEXT: for (int i = 0; i < 2; ++i)
// CHECK-NEXT: a = 2;

#ifdef OMP52
#pragma omp target simd private(argc, b), firstprivate(argv, c), lastprivate(d, f) collapse(2) if (target: argc) reduction(+ : h) linear(a: step(-5))
#else
#pragma omp target simd private(argc, b), firstprivate(argv, c), lastprivate(d, f) collapse(2) if (target: argc) reduction(+ : h) linear(a:-5)
#endif
  for (int i = 0; i < 10; ++i)
    for (int j = 0; j < 10; ++j)
      foo();
  // CHECK: #pragma omp target simd private(argc,b) firstprivate(argv,c) lastprivate(d,f) collapse(2) if(target: argc) reduction(+: h) linear(a: step(-5))
  // CHECK-NEXT: for (int i = 0; i < 10; ++i)
  // CHECK-NEXT: for (int j = 0; j < 10; ++j)
  // CHECK-NEXT: foo();

#pragma omp target simd private(argc,b) firstprivate(argv) if (argc > 0) reduction(+:c, arr1[argc]) reduction(max:e, arr[:5][0:10])
  for (int i = 0; i < 2; ++i) {}
  // CHECK: #pragma omp target simd private(argc,b) firstprivate(argv) if(argc > 0) reduction(+: c,arr1[argc]) reduction(max: e,arr[:5][0:10])
  // CHECK-NEXT: for (int i = 0; i < 2; ++i) {
  // CHECK-NEXT: }

#pragma omp target simd if (5) reduction(^:e, f, arr[0:5][:argc]) reduction(&& : h)
  for (int i = 0; i < 2; ++i) {}
  // CHECK: #pragma omp target simd if(5) reduction(^: e,f,arr[0:5][:argc]) reduction(&&: h)
  // CHECK-NEXT: for (int i = 0; i < 2; ++i) {
  // CHECK-NEXT: }

#if defined(OMP51) || defined(OMP52)
#pragma omp target simd if (target:argc > 0) if(simd:argc) nontemporal(argc, c, d) lastprivate(conditional: d, f) order(reproducible:concurrent)
#elif OMP5
#pragma omp target simd if (target:argc > 0) if(simd:argc) nontemporal(argc, c, d) lastprivate(conditional: d, f) order(concurrent)
#else
#pragma omp target simd if (target:argc > 0)
#endif // OMP51
  for (int i = 0; i < 2; ++i) {}
  // OMP45: #pragma omp target simd if(target: argc > 0)
  // OMP50: #pragma omp target simd if(target: argc > 0) if(simd: argc) nontemporal(argc,c,d) lastprivate(conditional: d,f) order(concurrent)
  // OMP51: #pragma omp target simd if(target: argc > 0) if(simd: argc) nontemporal(argc,c,d) lastprivate(conditional: d,f) order(reproducible: concurrent)
  // OMP52: #pragma omp target simd if(target: argc > 0) if(simd: argc) nontemporal(argc,c,d) lastprivate(conditional: d,f) order(reproducible: concurrent)
  // CHECK-NEXT: for (int i = 0; i < 2; ++i) {
  // CHECK-NEXT: }

#pragma omp target simd if (5)
  for (int i = 0; i < 2; ++i) {}
  // CHECK: #pragma omp target simd if(5)
  // CHECK-NEXT: for (int i = 0; i < 2; ++i) {
  // CHECK-NEXT: }

#pragma omp target simd map(i)
  for (int i = 0; i < 2; ++i) {}
  // CHECK: #pragma omp target simd  map(tofrom: i)
  // CHECK-NEXT: for (int i = 0; i < 2; ++i) {
  // CHECK-NEXT: }

#pragma omp target simd map(arr1[0:10], i)
  for (int i = 0; i < 2; ++i) {}
  // CHECK: #pragma omp target simd map(tofrom: arr1[0:10],i)
  // CHECK-NEXT: for (int i = 0; i < 2; ++i) {
  // CHECK-NEXT: }

#pragma omp target simd map(to: i) map(from: j)
  for (int i = 0; i < 2; ++i) {}
  // CHECK: #pragma omp target simd map(to: i) map(from: j)
  // CHECK-NEXT: for (int i = 0; i < 2; ++i) {
  // CHECK-NEXT: }

#pragma omp target simd map(always,alloc: i)
  for (int i = 0; i < 2; ++i) {}
  // CHECK: #pragma omp target simd map(always,alloc: i)
  // CHECK-NEXT: for (int i = 0; i < 2; ++i) {
  // CHECK-NEXT: }

#pragma omp target simd nowait
  for (int i = 0; i < 2; ++i) {}
  // CHECK: #pragma omp target simd nowait
  // CHECK-NEXT: for (int i = 0; i < 2; ++i) {
  // CHECK-NEXT: }

#pragma omp target simd depend(in : argc, arr[i:argc], arr1[:])
  for (int i = 0; i < 2; ++i) {}
  // CHECK: #pragma omp target simd depend(in : argc,arr[i:argc],arr1[:])
  // CHECK-NEXT: for (int i = 0; i < 2; ++i) {
  // CHECK-NEXT: }

#pragma omp target simd defaultmap(tofrom: scalar)
  for (int i = 0; i < 2; ++i) {}
  // CHECK: #pragma omp target simd defaultmap(tofrom: scalar)
  // CHECK-NEXT: for (int i = 0; i < 2; ++i) {
  // CHECK-NEXT: }

#pragma omp target simd safelen(clen-1)
  for (int i = 0; i < 2; ++i) {}
  // CHECK: #pragma omp target simd safelen(clen - 1)
  // CHECK-NEXT: for (int i = 0; i < 2; ++i) {
  // CHECK-NEXT: }

#pragma omp target simd simdlen(clen-1)
  for (int i = 0; i < 2; ++i) {}
  // CHECK: #pragma omp target simd simdlen(clen - 1)
  // CHECK-NEXT: for (int i = 0; i < 2; ++i) {
  // CHECK-NEXT: }

#pragma omp target simd aligned(arr1:4)
  for (int i = 0; i < 2; ++i) {}
  // CHECK: #pragma omp target simd aligned(arr1: 4)
  // CHECK-NEXT: for (int i = 0; i < 2; ++i) {
  // CHECK-NEXT: }

#pragma omp target simd is_device_ptr(p)
  for (int i = 0; i < 2; ++i) {}
  // CHECK: #pragma omp target simd is_device_ptr(p)
  // CHECK-NEXT: for (int i = 0; i < 2; ++i) {
  // CHECK-NEXT: }

  return (tmain<int, 5>(argc, &argc));
}

#endif
