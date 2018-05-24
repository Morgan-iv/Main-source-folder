program sign;
var a, s: integer;
    b: boolean;
begin
read(a);
b:=a>0;
s:=0;
while a<>0 do
begin
     if not (a>0=b) then s:=s+1;
     b:=a>0;
     read(a)
end;
writeln(s);
end.
