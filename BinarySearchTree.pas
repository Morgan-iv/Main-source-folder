program BinTree;
const n = 11;
type tree = ^node;
     node = record
          elem: integer;
          left, right: tree;
     end;
     tf = file of integer;
     ta = array[1..n] of integer;
function SubTree(e: integer; t: tree): tree;
begin
     if t=nil then begin subtree:=nil; exit end;
     if t^.elem>e then subtree:=subtree(e, t^.left);
     if t^.elem=e then subtree:=t^.left;
     if t^.elem<e then begin
        t^.right:=subtree(e, t^.right);
        subtree:=t;
     end;
end;
procedure InsertElemToTree(e: integer; var t: tree);
begin
     if t=nil then begin
        new(t);
        t^.elem:=e;
        t^.left:=nil;
        t^.right:=nil;
        exit
     end;
     if t^.elem>e then InsertElemToTree(e, t^.left)
                  else InsertElemToTree(e, t^.right)
end;
procedure ReadTree(var t: tree);
var i, e: integer;
begin
     for i:=1 to n do
     begin
          read(e);
          InsertElemToTree(e, t);
     end;
end;
procedure WriteTree(t: tree; k:integer);
var i: integer;
begin
     if t=nil then exit;
     for i:=1 to k do write('~');
     writeln(t^.elem);
     WriteTree(t^.left, k+1);
     WriteTree(t^.right, k+1);
end;
var t: tree;
begin
ReadTree(t);       // 10 11 4 2 1 3 8 9 6 5 7
WriteTree(t, 0);
t:=SubTree(6, t);
WriteTree(t, 0);
end.
