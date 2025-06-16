declare i32 @printf(ptr noundef, ...)
declare i32 @scanf(ptr noundef, ...)
@read_int = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@write_int = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

@x = global i32 0
@y = global i32 0
@z = global double 0.0

define i32 @plus(i32 %0, i32 %1) {
entry:
    %a = alloca i32
    %b = alloca i32 
    %plus = alloca i32
    store i32 %0, ptr %a
    store i32 %1, ptr %b
    %2 = load i32, ptr %a
    %3 = load i32, ptr %b
    %4 = add i32 %2, %3
    store i32 %4, ptr %plus
    %5 = load i32, ptr %plus
    ret i32 %5
}

define i32 @main() {
entry:
    %0 = call i32 (ptr, ...) @scanf(ptr @read_int, ptr @x)
    %1 = add i32 0, 3
    store i32 %1, ptr @y          ; y=3
    %2 = fadd double 0.0, 4.0
    store double %2, ptr @z       ; z=4
    %3 = load i32, ptr %x
    %4 = add i32 0, 1
    %5 = add i32 %3, %4
    store i32 %5, ptr @x          ; x=x+1
    %6 = load i32, ptr %y
    %7 = load i32, ptr %x
    %8 = mul i32 %6, %7
    store i32 %8, ptr @x          ; x=y*x
    %9 = fadd double, 0.0, 2.0
    %10 = load double, ptr @z
    %11 = fmul double %9, %10
    store double %11, ptr @z      ; z=2*z
    %12 = load i32, ptr @x
    %13 = load i32, ptr @y
    %14 = call i32 @plus(i32 %12, i32 %13)
    %15 = load i32, ptr @x
    %16 = call i32 (ptr, ...) @printf(ptr @write_int, ptr @x)
    %17 = load i32, ptr @y
    %18 = call i32 (ptr, ...) @printf(ptr @write_int, ptr @y)
    %19 = load double, ptr @z
    %20 = call i32 (ptr, ...) @printf(ptr @write_float, ptr @z)
    ret i32 0
}
