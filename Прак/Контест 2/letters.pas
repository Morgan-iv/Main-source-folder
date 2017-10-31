program letters;
var a: array['a'..'z'] of integer;
c: char;
begin
for c:='a' to 'z' do a[c]:=0;
read(c);
while c <> '.' do
begin
     a[c]:=a[c]+1;
     read(c);
end;
for c:='a' to 'z' do if a[c]=1 then write(c, ' ');
end.