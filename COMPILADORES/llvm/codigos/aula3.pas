Program Rec(input, output);
var x, y: integer;
function mdc(a, b: integer):integer;
begin
    if b=0 then mdc := a
    else mdc :=(b, a mod b)
end;
begin
    read(x,y);
    write(mdc(x,y));
end.