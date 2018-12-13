program group;
type list = ^node;
     node = record
          elem: integer;
          next: list;
     end;
procedure groups(l: list);
var p: list;
begin
if l=nil then exit;
while l^.next<>nil do
begin
     if l^.next^.elem=l^.elem then
     begin
          p:=l^.next;
          l^.next:=l^.next^.next;
          dispose(p);
     end
     else l:=l^.next;
end;
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
groups(l);
while l<>nil do
begin
     write(l^.elem, ' ');
     l:=l^.next;
end;
end.
