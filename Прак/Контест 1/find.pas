program find;
var n, a, m, s: integer;
begin
read(n, a);
for m:=1 to n do
begin
     read(s);
     if s=a then begin writeln(m); exit end;
end;
writeln(0);
end.