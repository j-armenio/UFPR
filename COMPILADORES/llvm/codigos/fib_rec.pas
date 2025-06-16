Program fibonacciRecursivo();
var res:integer;
function Fibo(x:integer):integer;
begin
    if (x = 0) then
        Fibo := 0
    else if (x = 1) then
        Fibo := 1
    else
        Fibo := Fibo(x-1) + Fibo(x-2);
end;
begin
    res := Fibo(5)
end.
