program maxmin;
type arr = array [1..100] of real;
var a, b, c: arr;
n, i: integer;
function max(var m: arr): real;
var result: real;
begin
     result:=m[1];
     for i:=2 to n do if result<m[i] then result:=m[i];
     max:=result;
end;
function max(var m, k: arr): real;
var result: real;
begin
     result:=m[1]+k[1];
     for i:=2 to n do if result<m[i]+k[i] then result:=m[i]+k[i];
     max:=result;
end;
function min(var m: arr): real;
var result: real;
begin
     result:=m[1];
     for i:=2 to n do if result>m[i] then result:=m[i];
     min:=result;
end;
function min(var m, k: arr): real;
var result: real;
begin
     result:=m[1]+k[1];
     for i:=2 to n do if result>m[i]+k[i] then result:=m[i]+k[i];
     min:=result;
end;
begin
read(n);
for i:=1 to n do read(a[i]);
for i:=1 to n do read(b[i]);
for i:=1 to n do read(c[i]);
if min(a)>=max(b) then writeln(max(b, c)+min(c))
   else writeln(min(b)/max(a)+max(c)/min(b, c));
end.
