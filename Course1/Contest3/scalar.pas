program scalar;
var mat: array[1..3, 1..101] of real;
i, n, m, j: integer;
function scl(a, b: integer): real;
var r: real;
begin
     r:=0;
     for i:=1 to n do
         r:=r+mat[a, i]*mat[b, i];
     scl:=r;
end;
begin
readln(n);
for j:=1 to 3 do
begin
    read(mat[j, 1]);
    mat[j, 101]:=mat[j, 1];
    for i:=2 to n do
    begin
        read(mat[j, i]);
        if mat[j, i]<mat[j, 101] then mat[j, 101]:=mat[j, i]
    end;
    readln;
end;
m:=1;
for i:=2 to 3 do if mat[i, 101]>mat[m, 101] then m:=i;
if m=3 then writeln((scl(m, m)-scl(1, 2)):0:5) else writeln((scl(m, m)-scl(3-m, 3)):0:5);
end.
