program sumlength;
var c: char; l, s: integer;
begin
read(c);
l:=0;
s:=0;
while c<>'.' do
begin
     if c in ['0'..'9'] then s:=s+ord(c)-ord('0');
     l:=l+1;
     read(c);
end;
if l = s then writeln('YES')
         else writeln('NO')
end.
