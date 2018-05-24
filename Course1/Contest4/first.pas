program frst;
type list = ^node;
     node = record
          elem: integer;
          next: list;
     end;
procedure first(l: list);
var p, q: list;
begin
if l=nil then exit;
while l^.next<>nil do
begin
     p:=l;
     while p^.next<>nil do
     begin
          if p^.next^.elem=l^.elem then
          begin
               q:=p^.next;
               p^.next:=p^.next^.next;
               dispose(q);
          end
          else p:=p^.next;
     end;
     if l^.next<>nil then l:=l^.next;
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
first(l);
while l<>nil do
begin
     write(l^.elem, ' ');
     l:=l^.next;
end;
end.