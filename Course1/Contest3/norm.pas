program norm;
var b: array[1..500, 1..500] of real;
n, m, i, j, k: integer; p, q, t: real;
begin
read(n, m);
for i:=1 to n do
    for j:=1 to m do
        b[i, j]:=0;
for k:=1 to 3 do
begin
     for i:=1 to n do
     begin
          read(p);
          b[i, 1]:=b[i, 1]+p;
          p:=abs(p);
          for j:=2 to m do
          begin
               read(q);
               b[i, j]:=b[i, j]+q;
               if abs(q) > p then p:=abs(q);
          end;
          t:=t+p;
     end;
end;
q:=0;
for i:=1 to n do
begin
     p:=abs(b[i, 1]);
     for j:=2 to m do
         if abs(b[i, j]) > p then p:=abs(b[i, j]);
     q:=q+p;
end;
writeln(t/q);
end.
     