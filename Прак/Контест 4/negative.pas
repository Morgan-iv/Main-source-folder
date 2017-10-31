program negative;
type list = ^node;
     node = record
          elem: integer;
          next: list;
     end;
procedure delete(var l: list);
var p, q: list;
begin
new(p);
p^.next:=l;
l:=p;
while l^.next<>nil do
begin
     if l^.next^.elem<0 then
     begin
          q:=l^.next;
          l^.next:=l^.next^.next;
          dispose(q);
     end
     else l:=l^.next;
end;
l:=p^.next;
dispose(p);
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
delete(l);
while l<>nil do
begin
     write(l^.elem, ' ');
     l:=l^.next;
end;
end.