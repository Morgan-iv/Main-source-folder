program various;
var a, r, s, n: integer;
begin
read(n);
read(a);
n:=n-1;
s:=1;
while n>0 do
begin
     read(r);
     if r>a then begin a:=r; s:=s+1 end;
     n:=n-1;
end;
writeln(s)
end.
