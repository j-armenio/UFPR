program exemplo (input,output);
var x, y: integer; 
var y: real;
function modulo (a,b: integer): integer;
var i,k: real; {vei praq serve i e k}
begin
    modulo := a mod b
end;
function mdc (a,b: integer; var c:real): integer; {vei praq c}
var l,m: real;  {vei praq serve l, m e n}
var n: integer;
begin
    if b = 0 then 
        mdc := a
    else 
        mdc := mdc (b, modulo(a, b) )
end;

begin
    read (x,y);
    write ( mdc(x,y) )
end.