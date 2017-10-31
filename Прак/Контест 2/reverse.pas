program reverse;
var a: array [1..100] of real;
n, i: integer; r: real;
begin
read(n);
for i:=1 to n do read(a[i]);
for i:=1 to n div 2 do
begin
     r:=a[i];
     a[i]:=a[n-i+1];
     a[n-i+1]:=r;
end;
for i:=1 to n do write(a[i], ' ');
end.