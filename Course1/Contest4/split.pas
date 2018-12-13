program spl;
type list = ^node;
     node = record
          elem: integer;
          next: list;
     end;
procedure split(l: list; var a, b: list);
var p: list;
begin
new(a);
new(b);
p:=b;
p^.next:=nil;
a^.next:=l;
l:=a;
while l^.next<>nil do
begin
     if l^.next^.elem<=0 then
     begin
          p^.next:=l^.next;
          l^.next:=l^.next^.next;
          p:=p^.next;
          p^.next:=nil;
     end
     else l:=l^.next;
end;
l:=a;
a:=a^.next;
dispose(l);
p:=b;
b:=b^.next;
dispose(p);
end;
var l, l1, l2: list; n, i: integer;
begin
read(n);
new(l);
l1:=l;
l1^.next:=nil;
for i:=1 to n do
begin
     new(l1^.next);
     l1:=l1^.next;
     l1^.next:=nil;
     read(l1^.elem);
end;
l1:=l;
l:=l^.next;
dispose(l1);
split(l, l1, l2);
write('L1: ');
while l1<>nil do
begin
     write(l1^.elem, ' ');
     l1:=l1^.next;
end;
writeln;
write('L2: ');
while l2<>nil do
begin
     write(l2^.elem, ' ');
     l2:=l2^.next;
end;
end.
