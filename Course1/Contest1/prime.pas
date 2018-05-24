program prime;
var n, a, s: longint;
begin
read(n);
if n=1 then begin writeln('NO'); exit end;
if n<4 then begin writeln('YES'); exit end;
s:=trunc(sqrt(n))+1;
a:=2;
while a<=s do
begin
     if n mod a = 0 then begin writeln('NO'); exit end;
     a:=a+1;
end;
writeln('YES')
end.