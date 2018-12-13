program max3;
var a, b, c, r: integer;
begin
a:=0;
b:=0;
c:=0;
read(r);
while r<>0 do
begin
     if r>=a then begin
                  c:=b;
                  b:=a;
                  a:=r
                  end
else if r>=b then begin
                  c:=b;
                  b:=r
                  end
else if r>c then c:=r;
     read(r);
end;
writeln(a, ' ', b, ' ', c)
end.
