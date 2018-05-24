program symetrie;
var a: array[1..100] of integer;
    b: array[1..100] of boolean;
n, m, i, j: integer;
begin
read(n, m);
for i:=1 to n do
begin
     b[i]:=true;
     for j:=1 to m do
     begin
          read(a[j]);
          b[i]:=b[i] and not ((j > (m + 1) div 2) and (a[j] <> a[m - j + 1]))
     end;
end;
for i:=1 to n do if b[i] then write(1, ' ') else write(0, ' ');
end.