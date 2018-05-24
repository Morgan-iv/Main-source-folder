program mergeand;
type list = ^node;
     node = record
          elem: integer;
          next: list;
     end;
procedure merge_and(a, b: list; var l:list);
var p, q: list;
begin
if (a=nil) or (b=nil) then begin l:=nil; exit end;
new(l);
p:=l;
p^.next:=nil;
while a<>nil do
begin
     p:=b;
     while (p^.elem<>a^.elem) and (p^.next<>nil) do p:=p^.next;
     if p^.elem<>a^.elem then begin a:=a^.next; continue end;
     p:=l;
     while (p^.elem<>a^.elem) and (p^.next<>nil) do p:=p^.next;
     if (p^.elem<>a^.elem) and (p^.next=nil) then
     begin
          p^.next:=a;
          a:=a^.next;
          p:=p^.next;
          p^.next:=nil;
     end
     else a:=a^.next;
end;
p:=l;
l:=l^.next;
dispose(p);
end;
var l, k, p: list; n, m, i: integer;
begin
read(n, m);
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
new(k);
p:=k;
p^.next:=nil;
for i:=1 to m do
begin
     new(p^.next);
     p:=p^.next;
     p^.next:=nil;
     read(p^.elem);
end;
p:=k;
k:=k^.next;
dispose(p);
merge_and(l, k, p);
while p<>nil do
begin
     write(p^.elem, ' ');
     p:=p^.next;
end;
end.