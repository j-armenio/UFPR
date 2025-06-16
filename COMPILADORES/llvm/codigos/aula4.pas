Program Ref(input, output);
var x,y,ma,me: integer;
Procedure MaiorMenor(a,b:integer; var maior,menor: integer);
begin
    if (a > b) then
    begin
        maior := a;
        menor := b;
    end;
    else
    begin
        maior := b;
        menor := a;
    end;
end;
begin
    read(x,y);
    MaiorMenor(x,y,ma,me);
    write(ma,me);
end.