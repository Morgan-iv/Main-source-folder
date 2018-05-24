program matr;
const MAXN = 1000;
type matrix = array[1..MAXN, 1..MAXN] of real;
var A, B: matrix; n, m, p, q, i, j: integer;
procedure swap(var a, b: matrix; n, m, p, q: integer);
var i, j, x, y, f, g: integer; r: real;
begin
x:=1;
y:=1;
f:=1;
g:=1;
for i:=1 to n do
    for j:=1 to m do
        if a[x, y]<a[i, j] then begin x:=i; y:=j end;
for i:=1 to p do
    for j:=1 to q do
        if b[f, g]<b[i, j] then begin f:=i; g:=j end;
r:=a[x, y];
a[x, y]:=b[f, g];
b[f, g]:=r;
end;
begin
read(n, m, p, q);
for i:=1 to n do
    for j:=1 to m do
        read(a[i, j]);
for i:=1 to p do
    for j:=1 to q do
        read(b[i, j]);
swap(a, b, n, m, p, q);
for i:=1 to n do
    for j:=1 to m do
        write(a[i, j], ' ');
for i:=1 to p do
    for j:=1 to q do
        write(b[i, j], ' ');
end.