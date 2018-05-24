program double;
type list = ^node;
     node = record
          elem: integer;
          next: list;
     end;
function doubles(l: list): boolean;
var q: list;
begin
     doubles:=false;
     if l=nil then exit;
     while l^.next<>nil do
     begin
          q:=l^.next;
          while q<>nil do
          begin
               if q^.elem=l^.elem then begin doubles:=true; exit end;
               q:=q^.next;
          end;
          l:=l^.next;
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
if doubles(l) then writeln('YES') else writeln('NO');
end.