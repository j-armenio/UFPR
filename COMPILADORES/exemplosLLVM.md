# Exemplos de conversão Pascal -> LLVM

## Ex1

```pascal
Program func(input, output);
var x,y,z: integer;

function soma(a, b: integer): integer;
begin
    soma := a+b;
end;

begin
    read(x);
    read(y);
    z := soma(x,y);
    write(z);
end.
```

```llvm
declare i32 @printf(ptr noundef, ...)
declare i32 @scanf(ptr noundef, ...)
@read_int = private ... c"%d\00"
@write_int = private ... c"%d\0A\00"

@x = global i32 0
@y = global i32 0
@z = global i32 0

define i32 @soma(i32 %0, i32 %1){
entry:
    %a = alloca i32
    store i32 %0, ptr %a
    %b = alloca i32
    store i32 %1, ptr %b
    %soma = alloca i32 
    %2 = load i32, ptr %a
    %3 = load i32, ptr %b
    %4 = add i32, %2, %3
    store i32 %4, ptr %soma
    %5 = load i32, ptr %soma
    ret i32 %5
}

define i32 @main(){
entry:
    %0 = call i32 (ptr, ...) @scanf(ptr @read_int, ptr @x)
    %1 = call i32 (ptr, ...) @scanf(ptr @read_int, ptr @y)
    %2 = load i32 @x
    %3 = load i32 @y
    %4 = call i32 @soma(i32 %2, i32 %3)
    store i32 %4, ptr @z
    %5 = load i32, ptr @z
    %6 = call i32 (ptr, ...) @printf(ptr @write_int, i32 %5)
    ret i32 0
}
```

## Ex2

```pascal

```

```llvm

```