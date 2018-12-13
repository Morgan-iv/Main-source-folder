program ccat;
type list = ^node;
     node = record
          elem: integer;
          next: list;
     end;
procedure concat(var a: list; b: list);
var p, q: list;
begin
if (a=nil) and (b=nil) then exit;
if a=nil then
begin
     new(a);
     a^.elem:=b^.elem;
     a^.next:=nil;
     b:=b^.next;
end;
p:=a;
while p^.next<>nil do p:=p^.next;
while b<>nil do
begin
     new(q);
     q^.elem:=b^.elem;
     q^.next:=nil;
     p^.next:=q;
     p:=p^.next;
     b:=b^.next;
end;
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
concat(l, k);
write('L1: ');
while l<>nil do
begin
     write(l^.elem, ' ');
     l:=l^.next;
end;
writeln;
write('L2: ');
while k<>nil do
begin
     write(k^.elem, ' ');
     k:=k^.next;
end;
end.
