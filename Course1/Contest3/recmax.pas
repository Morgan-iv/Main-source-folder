program recmax;
function rec(a: integer): integer;
var b: integer;
begin
     read(b);
     if b=0 then rec:=a
     else if a>b then rec:=rec(a)
                 else rec:=rec(b);
end;
begin
writeln(rec(0));
end.
