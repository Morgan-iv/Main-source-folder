program equality;
type list = ^node;
     node = record
          elem: integer;
          next: list;
     end;
function equal(l, m: list): boolean;
begin
     while (l<>nil) and (m<>nil) do
     begin
          if (l^.elem<>m^.elem) then begin equal:=false; exit end;
          l:=l^.next;
          m:=m^.next;
     end;
     if l<>m then equal:=false
     else equal:=true;
end;
var l, k, p: list; i, n, m: integer;
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
if equal(l, k) then writeln('YES') else writeln('NO');
end.