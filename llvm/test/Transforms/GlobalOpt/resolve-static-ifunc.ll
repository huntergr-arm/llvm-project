; NOTE: Assertions have been autogenerated by utils/update_test_checks.py UTC_ARGS: --function caller --check-globals all --version 4
; RUN: opt --passes=globalopt -o - -S < %s | FileCheck %s --implicit-check-not=trivial\.ifunc --implicit-check-not=dead_ifunc

target datalayout = "e-m:e-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128"
target triple = "aarch64-unknown-linux-gnu"

@trivial.ifunc = internal ifunc void (), ptr @trivial.resolver
;.
; CHECK: @unknown_condition = external local_unnamed_addr global i1
; CHECK: @alias_decl = weak_odr alias void (), ptr @aliased_decl.ifunc
; CHECK: @alias_def = weak_odr alias void (), ptr @aliased_def._Msimd
; CHECK: @external_ifunc.ifunc = dso_local ifunc void (), ptr @external_ifunc.resolver
; CHECK: @complex.ifunc = internal ifunc void (), ptr @complex.resolver
; CHECK: @sideeffects.ifunc = internal ifunc void (), ptr @sideeffects.resolver
; CHECK: @interposable_ifunc.ifunc = internal ifunc void (), ptr @interposable_ifunc.resolver
; CHECK: @interposable_resolver.ifunc = weak ifunc void (), ptr @interposable_resolver.resolver
; CHECK: @aliased_decl.ifunc = weak_odr ifunc void (), ptr @aliased_decl.resolver
; CHECK: @aliased_def.ifunc = weak_odr ifunc void (), ptr @aliased_def.resolver
;.
define ptr @trivial.resolver() {
  ret ptr @trivial._Msimd
}
define void @trivial._Msimd() {
  ret void
}
define void @trivial.default() {
  ret void
}


@dead_ifunc.ifunc = internal ifunc void (), ptr @trivial.resolver

@external_ifunc.ifunc = dso_local ifunc void (), ptr @external_ifunc.resolver
define ptr @external_ifunc.resolver() {
  ret ptr @external_ifunc._Msimd
}
define void @external_ifunc._Msimd() {
  ret void
}
define void @external_ifunc.default() {
  ret void
}

@unknown_condition = external global i1
@complex.ifunc = internal ifunc void (), ptr @complex.resolver
define ptr @complex.resolver() {
entry:
  %v = load i1, ptr @unknown_condition
  br i1 %v, label %fast, label %slow
fast:
  ret ptr @complex._Msimd
slow:
  ret ptr @complex._Msimd
}
define void @complex._Msimd() {
  ret void
}
define void @complex.default() {
  ret void
}

@sideeffects.ifunc = internal ifunc void (), ptr @sideeffects.resolver
define ptr @sideeffects.resolver() {
  store i1 0, ptr @unknown_condition
  ret ptr @sideeffects.default
}
define void @sideeffects._Msimd() {
  ret void
}
define void @sideeffects.default() {
  ret void
}

@interposable_ifunc.ifunc = internal ifunc void (), ptr @interposable_ifunc.resolver
define weak ptr @interposable_ifunc.resolver() {
  ret ptr @interposable_ifunc.resolver
}
define void @interposable_ifunc._Msimd() {
  ret void
}
define void @interposable_ifunc.default() {
  ret void
}

@interposable_resolver.ifunc = weak ifunc void (), ptr @interposable_resolver.resolver
define ptr @interposable_resolver.resolver() {
  ret ptr @interposable_resolver.resolver
}
define void @interposable_resolver._Msimd() {
  ret void
}
define void @interposable_resolver.default() {
  ret void
}

@alias_decl = weak_odr alias void (), ptr @aliased_decl.ifunc
@aliased_decl.ifunc = weak_odr ifunc void (), ptr @aliased_decl.resolver
declare void @aliased_decl._Msimd()
define ptr @aliased_decl.resolver() {
  ret ptr @aliased_decl._Msimd
}

@alias_def = weak_odr alias void (), ptr @aliased_def.ifunc
@aliased_def.ifunc = weak_odr ifunc void (), ptr @aliased_def.resolver
define void @aliased_def._Msimd() { ret void }
define ptr @aliased_def.resolver() {
  ret ptr @aliased_def._Msimd
}

define void @caller() {
; CHECK-LABEL: define void @caller() local_unnamed_addr {
; CHECK-NEXT:    call void @trivial._Msimd()
; CHECK-NEXT:    call void @external_ifunc._Msimd()
; CHECK-NEXT:    call void @complex.ifunc()
; CHECK-NEXT:    call void @sideeffects.ifunc()
; CHECK-NEXT:    call void @interposable_ifunc.ifunc()
; CHECK-NEXT:    call void @interposable_resolver.ifunc()
; CHECK-NEXT:    call void @aliased_decl.ifunc()
; CHECK-NEXT:    call void @aliased_def._Msimd()
; CHECK-NEXT:    ret void
;
  call void @trivial.ifunc()
  call void @external_ifunc.ifunc()
  call void @complex.ifunc()
  call void @sideeffects.ifunc()
  call void @interposable_ifunc.ifunc()
  call void @interposable_resolver.ifunc()
  call void @aliased_decl.ifunc()
  call void @aliased_def.ifunc()
  ret void
}
