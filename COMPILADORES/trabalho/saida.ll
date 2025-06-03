declare i32 @printf(ptr noundef, ...)
declare i32 @scanf(ptr noundef, ...)
@read_int = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@read_float = private unnamed_addr constant [4 x i8] c"%f\00", align 1
@write_int = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@write_float = private unnamed_addr constant [4 x i8] c"%f\00", align 1

@i = global i32 0
@num = global i32 0
@maior = global i32 0

define i32 @main() {
entry:
    %0 = call i32 (ptr, ...) @scanf(ptr @read_int, ptr @maior)
    store i32 1, ptr @i
    br label %l0

l0:
    %1 = load i32, ptr @i
    %2 = icmp slt i32 %1, 10
    br i1 %2, label %l1, label %l2

l1:
    %3 = call i32 (ptr, ...) @scanf(ptr @read_int, ptr @num)
    %4 = load i32, ptr @num
    %5 = load i32, ptr %maior
    %6 = icmp sgt i32 %4, %5
    br i1 %6, label %l3, label %l4

l3:
    store i32 (null), ptr @maior
    br label %l5

l4:
    store i32 (null), ptr @maior
    br label %l5

l5:
    %7 = load i32, ptr @i
    %8 = add i32 %7, 1
    store i32 %8, ptr @i
    br label %l0

l2:
    %9 = load i32, ptr @maior
    %10 = call i32 (ptr, ...) @printf(ptr @write_int, i32 %9)
    ret i32 0
}
