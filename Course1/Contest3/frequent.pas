program frq;
const MAXN = 10000;
type word = array[1..MAXN] of char;
var W: word; n, i: integer;
function frequent(var w: word; n: integer): char;
var arr: array['a'..'z'] of integer;
i: integer; c: char;
begin
for c:='a' to 'z' do
    arr[c]:=0;
for i:=1 to n do inc(arr[w[i]]);
i:=0;
for c:='a' to 'z' do
    if i<arr[c] then begin i:=arr[c]; frequent:=c end;
end;
begin
readln(n);
for i:=1 to n do read(w[i]);
writeln(frequent(w, n));
end.