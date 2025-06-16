declare i32 @printf(ptr noundef, ...)
declare i32 @scanf(ptr noundef, ...)
@read_int = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@write_int = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

@x = global i32 0
@y = global i32 0

define void @plus(i32 %0, i32 %1) {
entry:
    %a = alloca i32
    store i32 %0, ptr %a
    %b = alloca i32
    store i32 %1, ptr %b
    %c = alloca i32

    %2 = load i32, ptr %a
    %3 = load i32, ptr %b
    %4 = add i32 %2, %3
    store i32 %4, ptr %c
    %5 = load i32, ptr %c
    %6 = call i32 (ptr, ...) @printf(ptr @write_int, i32 %5)
}

define i32 @main(){
entry:
    %0 = call i32 (ptr, ...) @scanf(ptr @red_int, i32 ptr @x)
    %1 = call i32 (ptr, ...) @scanf(ptr @red_int, i32 ptr @y)
    %2 = load i32, ptr @x
    %3 = load i32, ptr @y
    %4 = call i32 @plus(i32 %2, i32 %3)
    ret i32 0
}
