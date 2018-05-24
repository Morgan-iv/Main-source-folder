program justrow;
var a: array[1..100] of integer;
    b: array[1..100] of boolean;
n, m, i, j, k: integer;
begin
read(n, m);
for i:=1 to n do
begin
     b[i]:=true;
     for j:=1 to m do
     begin
          read(a[j]);
          for k:=1 to j-1 do
              b[i]:=b[i] and (a[k]<>a[j])
     end;
end;
for i:=1 to n do if b[i] then write(0, ' ') else write(1, ' ');
end.