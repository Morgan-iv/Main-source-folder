program sum;
var a: array [1..100] of integer;
n, i, min, max: integer;
begin
read(n);
min:=1;
max:=1;
for i:=1 to n do
begin
     read(a[i]);
     if a[i] < a[min] then min:=i;
     if a[i] > a[max] then max:=i;
end;
n:=0;
if min > max then begin i:=min; min:=max; max:=i end;
for i:=min to max do n:=n+a[i];
write(n);
end.