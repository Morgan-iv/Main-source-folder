program length;
var c, d: char; l: integer;
begin
read(c);
l:=0;
d:='a';
while c<>'.' do
begin
     if c in ['0'..'9'] then
        if d='a' then d:=c
                 else begin writeln('NO'); exit end;
     l:=l+1;
     read(c);
end;
if l = ord(d)-ord('0') then writeln('YES')
                       else writeln('NO')
end.
