program doubles;
var a: array[1..1000] of char;
    b: array[char] of boolean;
n, i, k: integer; c: char;
begin
readln(n);
k:=1;
for c:=chr(0) to chr(255) do b[c]:=false;
for i:=1 to n do
begin
     read(a[k]);
     if b[a[k]] then continue;
     b[a[k]]:=true;
     k:=k+1;
end;
for i:=1 to k-1 do write(a[i]);
end.
