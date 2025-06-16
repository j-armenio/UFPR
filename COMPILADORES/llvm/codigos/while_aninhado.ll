declare i32 @printf(ptr noundef, ...)
declare i32 @scanf(ptr noundef, ...)
@read_int = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@write_int = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

@a = global i32 0
@b = global i32 0

define i32 @main() {
entry:
    %0 = add i32 0, 10
    store i32 %0, ptr @a
    br label %teste_while_1

teste_while_1:
    %1 = load i32, ptr @a
    %2 = add i32 0, 0
    %3 = icmp sgt i32 %1, %2
    br i1 %3, label %while_1, label %end_while_1

while_1:
    %4 = load i32, ptr @a
    %5 = call i32 (ptr, ...) @printf(ptr @write_int, i32 %4)
    %6 = load i32, ptr @a
    %7 = add i32 0, 1
    %8 = sub i32 %6, %7
    store i32 %8, ptr @a
    %9 = add i32 0, 5
    store i32 %9, ptr @b
    br label %teste_while_2

teste_while_2:
    %10 = load i32, ptr @b
    %11 = add i32 0, 0
    %12 = icmp sgt i32 %10, %11
    br i1 %12, label %while_2, label %end_while_2

while_2:
    %13 = load i32, ptr @b
    %14 = call i32 (ptr, ...) @printf(ptr @write_int, i32 %13)
    %15 = load i32, ptr @b
    %16 = add i32 0, 1
    %17 = sub i32 %15, %16
    store i32 %17, ptr @b
    br label %teste_while_2

end_while_2:
    br label %teste_while_1

end_while_1:
    ret i32 0
}
