program count;
var b: array[char] of boolean;
n, i, k: integer; c: char;
begin
readln(n);
k:=0;
for c:=chr(0) to chr(255) do b[c]:=false;
for i:=1 to n do
begin
     read(c);
     if b[c] then continue;
     b[c]:=true;
     k:=k+1;
end;
write(k);
end.