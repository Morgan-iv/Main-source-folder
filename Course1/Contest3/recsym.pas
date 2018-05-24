program recsym;
type pstring = array [1..1000] of char;
var s: pstring;
i, n, j: integer;
function sym(var s: pstring; i, j: integer): boolean;
begin
     if i>=j then sym:=true else
     if s[i]=s[j] then sym:=sym(s, i+1, j-1)
                  else sym:=false;
end;
begin
readln(n);
for i:=1 to n do
    read(s[i]);
read(i, j);
if sym(s, i, j) then writeln('YES') else writeln('NO');
end.