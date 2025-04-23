declare i32 @printf(ptr noundef, ...)
declare i32 @scanf(ptr noundef, ...)
@read_int = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@write_int = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

@x = global i32 0
@y = global i32 0

define i32 @modulo(i32 %0, i32 %1) {
    %a = alloca i32 
    store i32 %0, ptr %a
    %b = alloca i32
    store i32 %1, ptr %b
    %modulo = alloca i32
    %2 = load i32, ptr %a
    %3 = load i32, ptr %b
    %4 = srem i32 %2, %3
    store i32 %4, ptr %modulo
    %5 = load i32, ptr %modulo
    ret i32 %5
}

define i32 @mdc(i32 %0, i32 %1) {
entry:
    %a = alloca i32
    store i32 %0, ptr %a
    %b = alloca i32
    store i32 %1, ptr %b
    %2 = add i32 0, 0
    %3 = load i32, ptr %b
    %4 = icmp eq %3, %2
    br i1 %4, label %then, label %else

then:
    %mdc = alloca i32
    %5 = load i32, ptr %a
    store i32 %5, ptr %mdc
    %6 = load i32, ptr %mdc
    ret i32 %6

else:
    %mdc = alloca i32
    %7 = load i32, ptr %a
    %8 = load i32, ptr %b
    %modulo = alloca i32
    %9 = call i32 @modulo(i32 %7, %8)
    store i32 %9, ptr %modulo
    %10 = load i32, ptr %modulo
    %11 = load i32, ptr %b
    %12 = call i32 @mdc(i32 %11, i32 %10)
    store i32 %12, ptr %mdc
    %13 = load i32, ptr %mdc
    ret i32 %13
}

define i32 @main(){
    %0 = call i32 (ptr, ...) @scanf(ptr @read_int, ptr @x)
    %1 = call i32 (ptr, ...) @scanf(ptr @read_int, ptr @y)
    %2 = load i32, ptr @x
    %3 = load i32, ptr @y
    %4 = call i32 @mdc(i32 %2, i32 %3)
    %5 = call i32 (ptr, ...) @printf(ptr @write_int, i32 %4)
    ret i32 0
}