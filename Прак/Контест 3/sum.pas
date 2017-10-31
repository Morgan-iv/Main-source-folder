program sm;
const MAXN = 1000;
type matrix = array[1..MAXN, 1..MAXN] of real;
var A: matrix; i, j, n, m: integer;
function sum(var a: matrix; n, m: integer): real;
var i, j: integer; r, x, y: real;
begin
r:=0;
for i:=1 to n div 2 do
begin
     x:=a[i, 1];
     y:=a[n-i+1, 1];
     for j:=2 to m do
     begin
          if a[i, j]>x then x:=a[i, j];
          if a[n-i+1, j]>y then y:=a[n-i+1, j];
     end;
     r:=r+x*y*2;
end;
if n mod 2 = 1 then
begin
     x:=a[n div 2 + 1, 1];
     for j:=2 to m do
         if a[n div 2 + 1, j]>x then x:=a[n div 2 + 1, j];
     r:=r+x*x;
end;
sum:=r;
end;
begin
read(n, m);
for i:=1 to n do
    for j:=1 to m do
        read(a[i, j]);
writeln(sum(a, n, m));
end.