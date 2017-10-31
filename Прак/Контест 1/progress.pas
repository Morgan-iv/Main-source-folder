program progress;
var a, b, c, n: integer;
begin
read(n);
if n<3 then begin writeln('YES'); exit end;
read(a);
read(b);
n:=n-2;
while n>0 do
begin
     read(c);
     if a+c<>2*b then begin writeln('NO'); exit end;
     a:=b;
     b:=c;
     n:=n-1;
end;
writeln('YES')
end.