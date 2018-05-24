program nodesc;
var a, b, n: integer;
begin
read(n);
read(a);
n:=n-1;
while n>0 do
begin
     read(b);
     if b<a then begin writeln('NO'); exit end;
     a:=b;
     n:=n-1;
end;
writeln('YES')
end.