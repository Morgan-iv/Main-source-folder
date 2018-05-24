program digits;
var a, c: char; d: integer;
begin
read(a);
if not (a in ['0'..'9']) then begin writeln('NO'); exit end;
read(c);
if not (c in ['0'..'9']) then begin writeln('NO'); exit end;
d:=ord(c)-ord(a);
while c<>'.' do
begin
     if not (c in ['0'..'9']) then begin writeln('NO'); exit end;
     if ord(c)-ord(a)<>d then begin writeln('NO'); exit end;
     a:=c;
     read(c);
end;
writeln('YES')
end.