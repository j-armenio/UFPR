declare i32 @printf(ptr noundef, ...)
declare i32 @scanf(ptr noundef, ...)
@read_int = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@write_int = private unnamed_addr constant [4 x i8] c"%d\A0\00", align 1

@i = global i32 0
@num = global i32 0
@maior = global i32 0


