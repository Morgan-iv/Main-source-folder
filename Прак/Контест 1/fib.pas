program fib;
var a, b, c, n: longint;
begin
read(n);
a:=1;
b:=1;
c:=1;
while c<n do
begin
     a:=b;
     b:=c;
     c:=a+b;
end;
if c=n then writeln('YES') else writeln('NO')
end.