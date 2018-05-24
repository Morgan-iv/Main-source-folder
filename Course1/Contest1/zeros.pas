program zeros;
var c, n, m, r: integer;
begin
read(n);
c:=0;
m:=0;
while n>0 do
begin
     read(r);
     if r=0 then c:=c+1
     else begin if m<c then m:=c; c:=0 end;
     n:=n-1;
end;
if c>m then m:=c;
write(m)
end.