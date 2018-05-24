program maxrow;
type row = array [1..101] of real;
var matrix: array[1..100] of row;
    index:  array[1..100] of integer;
e, s: row; n, m, i, j: integer; x: real;

procedure qsort(a, b: integer);
var e: real; i, j, k: integer;
begin
     if a=b then exit;
     e:=(s[a]+s[b])/2;
     i:=a;
     j:=b;
     while s[i]<e do i:=i+1;
     while s[j]>e do j:=j-1;
     while i<j do
     begin
          x:=s[i];
          s[i]:=s[j];
          s[j]:=x;
          index[i]:=index[i]+index[j];
          index[j]:=index[i]-index[j];
          index[i]:=index[i]-index[j];
          if i>=j then break;
          while s[i]<e do i:=i+1;
          while s[j]>e do j:=j-1;
     end;
     if j=b then qsort(a, b-1)
     else if i=j then begin qsort(a, j); qsort(i+1, b) end
                 else begin qsort(a, j); qsort(i, b) end
end;

begin
read(n, m);
for i:=1 to n do
begin
     read(matrix[i][1]);
     matrix[i][101]:=matrix[i][1];
     for j:=2 to m do
     begin
          read(matrix[i][j]);
          if matrix[i][j] > matrix[i][101] then matrix[i][101]:=matrix[i][j];
     end;
     s[i]:=matrix[i][101];
     index[i]:=i;
end;
qsort(1, n);
for i:=1 to n do begin
    for j:=1 to m do write(matrix[index[i]][j], ' ');
    writeln();
end;
end.
    
