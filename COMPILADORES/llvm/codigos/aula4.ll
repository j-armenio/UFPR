declare i32 @printf(ptr noundef, ...)
declare i32 @scanf(ptr noundef, ...)
@read_int = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@write_int = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

@x = global i32 0
@y = global i32 0
@ma = global i32 0
@me = global i32 0

define void @MaiorMenor(i32 %0, i32 %1, ptr %maior, ptr %menor) {
entry:
    %a = alloca i32
    %b = alloca i32
    store i32 %0, ptr %a
    store i32 %1, ptr %b
    %2 = load i32, ptr %a
    %3 = load i32, ptr %b
    %4 = icmp sgt i32 %2, %3
    br i1 %4, label %then, label %else

then:
    %5 = load i32, ptr %a
    store i32 %5, ptr %maior
    %6 = load i32, ptr %b
    store i32 %6, ptr %menor
    br label %end

else:
    %7 = load i32, ptr %b
    store i32 %7, ptr %maior
    %8 = load i32, ptr %a
    store i32 %8, ptr %menor
    br label %end

end:
}

define i32 @main(){
entry:
    %0 = call i32 (ptr, ...) @scanf(ptr @read_int, ptr @x)
    %1 = call i32 (ptr, ...) @scanf(ptr @read_int, ptr @y)
    %2 = load i32, ptr @x
    %3 = load i32, ptr @y
    call @MaiorMenor(i32 %2, i32 %3, ptr @ma, ptr @me)
    %4 = load i32, ptr @ma
    %5 = call i32 (ptr, ...) @printf(ptr @write_int, i32 %4)
    %6 = load i32, ptr @me
    %7 = call i32 (ptr, ...) @printf(ptr @write_int, i32 %6)
    ret i32 0
}