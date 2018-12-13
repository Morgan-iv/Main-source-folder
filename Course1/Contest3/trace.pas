program trace;
var m: array[1..200, 1..100] of real;
i, j, p, n, t, k: integer; x, y: real;
begin
read(n);
for i:=1 to 2*n do
    for j:=1 to n do
        read(m[i, j]);
x:=0;
y:=0;
for i:=1 to n do
begin
     x:=x+m[i, i];
     y:=y+m[n+i, i];
end;
if x>y then begin t:=1; k:=0 end else begin t:=0; k:=1 end;
for i:=1 to n do
    for j:=1 to n do
    begin
         m[i+n*k, j]:=0;
         for p:=1 to n do
             m[i+n*k, j]:=m[i+n*k, j]+m[i+n*t, p]*m[p+n*t, j];
    end;
for i:=1 to n do
begin
     for j:=1 to n do
         write(m[i+n*k, j], ' ');
     writeln;
end;
end.
