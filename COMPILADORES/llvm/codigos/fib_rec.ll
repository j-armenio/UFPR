declare i32 @printf(ptr noundef, ...)
declare i32 @scanf(ptr noundef, ...)
@read_int = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@write_int = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

@res = global i32 0

define i32 @Fibo(i32 %0) {
entry:
    %x = alloca i32
    %Fibo = alloca i32
    %1 = load i32, ptr %x
    %2 = add i32 0, 0
    %3 = icmp eq i32 %1, %2
    br i1 %3, label %then_1, label %else_if

then_1:
    %4 = add i32 0, 0
    store i32 %4, ptr %Fibo
    br label %end

else_if:
    %5 = add i32 0, 1
    %6 = load i32, ptr %x
    %7 = icmp eq i32 %6, %5
    br i1 %7, label %then_2, label %else

then_2:
    %8 = add i32 0, 1
    store i32 %8, ptr %Fibo
    br label %end

else:
    %9 = load i32, ptr %x
    %10 = add i32 0, 1
    %11 = sub i32 %9, %10  ; x-1
    %12 = call i32 @Fibo(i32 %11)
    %13 = load i32, ptr %x
    %14 = add i32 0, 2
    %15 = sub i32 %13, %14 ; x-2
    %16 = call i32 @Fibo(i32 %15)
    %17 = add i32 %12, %16
    store i32 %17, ptr %Fibo
    br label %end

end:
    %18 = load i32, ptr @Fibo
    ret i32 %18
}

define i32 @main() {
entry:
    %0 = add i32 0, 5
    %1 = call i32 @Fibo(i32 %0)
    store i32 %1, ptr @res
    %3 = load i32, ptr @res
    %4 = call i32 (ptr, ...) @printf(ptr @write_int, i32 %3)
    ret i32 0
}
