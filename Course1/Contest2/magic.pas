program magic;
var matrix: array[1..100, 1..100] of integer;
n, i, j, k, s1, s2: integer; b: boolean;
begin
read(n);
b:=true;
if n=1 then begin writeln('YES'); exit end;
for i:=1 to n do
    for j:=1 to n do
        read(matrix[i, j]);
k:=0;
for i:=1 to n do k:=k+matrix[1, i];
for i:=1 to n do
begin
     s1:=0;
     s2:=0;
     for j:=1 to n do begin s1:=s1+matrix[i, j]; s2:=s2+matrix[j, i] end;
     b:=b and (s1=k) and (s2=k);
end;
if b then writeln('YES') else writeln('NO')
end.
