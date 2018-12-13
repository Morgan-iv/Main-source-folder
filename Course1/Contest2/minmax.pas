program minmax;
var a: array [1..100] of real;
n, i, min, max: integer; r: real;
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
r:=a[min];
a[min]:=a[max];
a[max]:=r;
for i:=1 to n do write(a[i], ' ');
end.
