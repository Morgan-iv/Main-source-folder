program order;
type list = ^node;
     node = record
          elem: integer;
          next: list;
     end;
var l, p, q: list; n, i, a: integer;
begin
read(n);
if n=0 then exit;
new(l);
p:=l;
p^.elem:=maxint-2;
p^.next:=nil;
for i:=1 to n do
begin
     read(a);
     p:=l;
     while p^.elem<a do p:=p^.next;
     new(q);
     q^.elem:=p^.elem;
     q^.next:=p^.next;
     p^.elem:=a;
     p^.next:=q;
end;
while l^.next<>nil do
begin
     write(l^.elem, ' ');
     l:=l^.next;
end;
end.
