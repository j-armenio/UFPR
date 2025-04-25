Program func(input, output);
var x, y, z: integer;
function soma(a, b: integer): integer;
begin
    soma := a + b;
end;

begin 
    read(x);
    read(y);
    z := soma(x, y);
    write(z);
end.