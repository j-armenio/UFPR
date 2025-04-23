declare i32 @printf(ptr noundef, ...)
declare i32 @scanf(ptr noundef, ...)
@read_int = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@write_int = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

@i = global i32 0
@num = global i32 0
@maior = global i32 0

define i32 @main(){
entry:
    %0 = call i32 (ptr, ...) @scanf(ptr @read_int, ptr @maior)
    %1 = add i32 0, 1
    store i32 %1, ptr @i
    br label %test_while

test_while:
    %2 = load i32, ptr @i
    %3 = add i32 0, 10
    %4 = icmp slt i32 %2, %3
    br i1 %4, label %while, label %end_while

while:
    %5 = call i32 (ptr, ...) @scanf(ptr @read_int, ptr @num)
    %6 = load i32, ptr @num
    %7 = load i32, ptr @maior
    %8 = icmp sgt i32 %6, %7
    br i1 %8, label %then, label %else

then:
    %9 = load i32, ptr @num
    store i32 %9, ptr @maior
    br label %end_if

else:
    %10 = load i32, ptr @maior
    store i32 %10, ptr @maior
    br label %end_if

end_if:
    %11 = load i32, ptr @i
    %12 = add i32 0, 1
    %13 = add i32 %11, %12
    store i32 %13, ptr @i
    br label %test_while

end_while:
    %14 = load i32, ptr @maior
    %15 = call i32 (ptr, ...) @printf(ptr @write_int, i32 %14)
    ret i32 0
}
