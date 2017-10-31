program mean;
var a: array [1..100] of real;
n, i, s: integer; r, d: real;
begin
read(n);
r:=0;
for i:=1 to n do
begin
     read(a[i]);
     r:=r+a[i];
end;
r:=r/n;
s:=1;
d:=abs(a[1]-r);
for i:=2 to n do if (abs(a[i]-r)<d) then begin s:=i; d:=abs(a[i]-r) end;
write(s);
end.