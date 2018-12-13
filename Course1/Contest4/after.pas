program aft;
type list = ^node;
     node = record
          elem: integer;
          next: list;
     end;
procedure after(l: list; e, a: integer);
var p: list;
begin
while l<>nil do
begin
     if l^.elem=e then
     begin
          new(p);
          p^.elem:=a;
          p^.next:=l^.next;
          l^.next:=p;
          l:=l^.next;
     end;
     l:=l^.next;
end;
end;
var l, p: list; n, e, e1, i: integer;
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
read(e, e1);
after(l, e, e1);
while l<>nil do
begin
     write(l^.elem, ' ');
     l:=l^.next;
end;
end.
