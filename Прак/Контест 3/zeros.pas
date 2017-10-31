program zeros;
var arr: array[1..1500, 1..500] of integer;
    x: array[0..2] of integer;
    y: array[0..2] of integer;
n, m, i, j, k: integer; p: boolean;
begin
read(n, m);
for k:=0 to 2 do begin y[k]:=k; x[k]:=0;
for i:=1 to n do
begin
     p:=true;
     for j:=1 to m do
     begin
          read(arr[i+n*k, j]);
          if arr[i+n*k, j]<>0 then p:=false;
     end;
     if p then x[k]:=x[k]+1;
end;
end;
if x[1]<x[0] then begin k:=x[1]; x[1]:=x[0]; x[0]:=k; k:=y[1]; y[1]:=y[0]; y[0]:=k end;
if x[2]<x[1] then begin k:=x[2]; x[2]:=x[1]; x[1]:=k; k:=y[2]; y[2]:=y[1]; y[1]:=k end;
if x[1]<x[0] then begin k:=x[1]; x[1]:=x[0]; x[0]:=k; k:=y[1]; y[1]:=y[0]; y[0]:=k end;
if x[2]<x[1] then begin k:=x[2]; x[2]:=x[1]; x[1]:=k; k:=y[2]; y[2]:=y[1]; y[1]:=k end;
for k:=0 to 2 do begin if x[k]<x[2] then continue;
for i:=1 to n do
    for j:=1 to m do
        write(arr[i+y[k]*n, j], ' ');
end;
end.