program ls;
var a, b: integer;
function less(a, b: integer): boolean;
var n, m, i, r: integer;
begin
n:=0;
m:=0;
r:=round(sqrt(a));
if r*r>a then r:=r-1;
for i:=1 to r do
    if a mod i = 0 then n:=n+1;
n:=n*2;
if round(sqrt(a))*round(sqrt(a)) = a then n:=n-1;
r:=round(sqrt(b));
if r*r>b then r:=r-1;
for i:=1 to r do
    if b mod i = 0 then m:=m+1;
m:=m*2;
if round(sqrt(b))*round(sqrt(b)) = b then m:=m-1;
less:=n<m
end;
begin
read(a, b);
if less(a, b) then writeln('YES') else writeln('NO');
end.
