declare i32 @printf(ptr noundef, ...)
declare i32 @scanf(ptr noundef, ...)
@read_int = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@write_int = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

@x = global i32 0
@y = global i32 0

define i32 @mdc(i32 %0, i32 %1) {
entry:
    %a = alloca i32
    store i32 %0, ptr %a
    %b = alloca i32
    store i32 %1, ptr %b
    %mdc = alloca i32

    %2 = add i32 0, 0
    %3 = load i32, ptr %b
    %4 = icmp eq i32 %2, %3
    br i1 %4, label %then, label %else

then:
    %5 = load i32, ptr %a
    store i32 %5, ptr %mdc
    br label %end

else:
    %6 = load i32, ptr %b ; b
    %7 = load i32, ptr %a
    %8 = load i32, ptr %b
    %9 = srem i32 %7, %8  ; a mod b
    %10 = call i32 @mdc(i32 %6, i32 %9)
    store i32 %10, ptr %mdc
    br label %end

end:
    %11 = load i32, ptr %mdc
    ret i32 %11
}

define i32 @main(){
entry:
    %0 = call i32 (ptr, ...) @scanf(ptr @read_int, ptr @x)
    %1 = call i32 (ptr, ...) @scanf(ptr @read_int, ptr @y)
    %2 = load i32, ptr @x 
    %3 = load i32, ptr @y
    %4 = call i32 @mdc(i32 %2, i32 %3)
    %5 = call i32 (ptr, ...) @printf(ptr @write_int, i32 %4)
    ret i32 0
}
