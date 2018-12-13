program summ;
type list = ^node;
     node = record
          elem: integer;
          next: list;
     end;
function sum(l: list): integer;
var a, b: list; t: boolean; res: integer;
begin
     a:=l;
     b:=l;
     while l<>nil do
     begin
          if l^.elem>a^.elem then begin a:=l; t:=true; end;
          if l^.elem<b^.elem then begin b:=l; t:=false; end;
          l:=l^.next;
     end;
     if t then begin l:=a; a:=b; b:=l; end;
     res:=b^.elem;
     while a^.elem<>b^.elem do
     begin
          res:=res+a^.elem;
          a:=a^.next;
     end;
     sum:=res;
end;
var l, p: list; n, i: integer;
begin
read(n);
new(l);
p:=l;
p^.next:=nil;
for i:=1 to n do
begin
     new(p^.next);
     p:=p^.next;
     p^.next:=nil;
     read(p^.elem);
end;
p:=l;
l:=l^.next;
dispose(p);
writeln(sum(l));
end.
