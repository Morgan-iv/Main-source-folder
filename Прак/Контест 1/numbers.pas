program numbers;
var a, b, c: integer;
begin
for a:=1 to 9 do
    for b:=0 to 9 do
    begin
         if b=a then continue;
         for c:=0 to 9 do
         begin
              if c=a then continue;
              if c=b then continue;
              writeln(a, b, c);
         end
    end
end.
         