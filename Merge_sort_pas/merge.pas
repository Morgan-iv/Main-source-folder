program merge;
type list = ^node;
     node = record
     elem: integer;
     next: list;
     end;
     stack = ^st_node;
     st_node = record
     ls: list;
     next: stack;
     end;

procedure mrg(var st: stack);
var p: list; t: stack;
begin
     if st^.ls^.elem<>st^.next^.ls^.elem then exit;
     new(t);
     new(t^.ls);
     t^.ls^.elem:=st^.ls^.elem*2;
     p:=t^.ls;
     while (st^.ls^.next<>nil) and (st^.next^.ls^.next<>nil) do
     begin
          if st^.ls^.next^.elem>st^.next^.ls^.next^.elem then
          begin
               p^.next:=st^.next^.ls^.next;
               st^.next^.ls^.next:=st^.next^.ls^.next^.next;
               p:=p^.next;
               p^.next:=nil;
          end else
          begin
               p^.next:=st^.ls^.next;
               st^.ls^.next:=st^.ls^.next^.next;
               p:=p^.next;
               p^.next:=nil;
          end;
     end;
     if st^.ls^.next=nil then
     begin
          p^.next:=st^.next^.ls^.next;
          st^.next^.ls^.next:=nil;
     end else
     begin
          p^.next:=st^.ls^.next;
          st^.ls^.next:=nil;
     end;
     t^.next:=st;
     st:=t;
     t:=st^.next;
     st^.next:=st^.next^.next;
     dispose(t^.ls);
     dispose(t);
     t:=st^.next;
     st^.next:=st^.next^.next;
     dispose(t^.ls);
     dispose(t);
     mrg(st);
end;
     
procedure merge_sort(filein, fileout: string);
var f, g: text; st, t: stack; p: list;
begin
assign(f, filein);
assign(g, fileout);
reset(f);
rewrite(g);
new(st);
st^.next:=nil;
new(st^.ls);
st^.ls^.next:=nil;
st^.ls^.elem:=0;
while not seekeof(f) do
begin
     new(t);
     t^.next:=st;
     st:=t;
     new(st^.ls);
     st^.ls^.elem:=1;
     new(st^.ls^.next);
     read(f, st^.ls^.next^.elem);
     st^.ls^.next^.next:=nil;
     mrg(st);
end;
while st^.next^.ls^.elem<>0 do begin
new(t);
new(t^.ls);
t^.ls^.elem:=st^.ls^.elem*2;
p:=t^.ls;
while (st^.ls^.next<>nil) and (st^.next^.ls^.next<>nil) do
begin
     if st^.ls^.next^.elem>st^.next^.ls^.next^.elem then
     begin
          p^.next:=st^.next^.ls^.next;
          st^.next^.ls^.next:=st^.next^.ls^.next^.next;
          p:=p^.next;
          p^.next:=nil;
     end else
     begin
          p^.next:=st^.ls^.next;
          st^.ls^.next:=st^.ls^.next^.next;
          p:=p^.next;
          p^.next:=nil;
     end;
end;
if st^.ls^.next=nil then
begin
     p^.next:=st^.next^.ls^.next;
     st^.next^.ls^.next:=nil;
end else
begin
     p^.next:=st^.ls^.next;
     st^.ls^.next:=nil;
end;
t^.next:=st;
st:=t;
t:=st^.next;
st^.next:=st^.next^.next;
dispose(t^.ls);
dispose(t);
t:=st^.next;
st^.next:=st^.next^.next;
dispose(t^.ls);
dispose(t);
end;
p:=st^.ls^.next;
while p<>nil do
begin
     writeln(g, p^.elem);
     p:=p^.next;
end;
close(f);
close(g);
end;

var ipt, opt: string;
begin
read(ipt);
read(opt);
merge_sort(ipt, opt);
end.
