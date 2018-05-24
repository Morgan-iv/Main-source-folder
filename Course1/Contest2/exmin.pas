program exmin;
var a, b: array [1..100] of integer;
n, m, i, j, min: integer; k: boolean;
begin
read(n, m);
min:=maxint-2;
for i:=1 to n do read(a[i]);
for i:=1 to m do read(b[i]);
for i:=1 to n do
begin
     if a[i]>min then continue;
     k:=true;
     for j:=1 to m do if a[i]=b[j] then begin k:=false; break end;
     if k then min:=a[i];
end;
write(min);
end.
