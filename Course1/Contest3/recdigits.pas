program recdigits;
function rec(a: integer): integer;
var b: char;
begin
     read(b);
     if b='.' then rec:=a else
     if b in ['0'..'9'] then rec:=rec(a+1)
                        else rec:=rec(a);
end;
begin
writeln(rec(0));
end.
