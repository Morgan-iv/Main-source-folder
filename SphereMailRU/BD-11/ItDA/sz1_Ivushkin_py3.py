import numpy as np

#шахматная доска
x = 5 #x - размерность
y = 5 #y - размерность
t = 1 #начало с 0 или с 1
A = np.ones((x, y), dtype = np.int32)
A.ravel()[t::2] = 0
print (A)

#убрать 3 наибольших
x = 10 #размерность
A = np.random.rand(x)
A[np.argsort(a)[-3:]] = 0
print (A)

#диагональная матрица квадратов
x = 5 #размерность
t = np.arange(1, x + 1)
A = np.diag(t * t)
print (A)

#змейка
x = 5 #x - размерность
y = 5 #y - размерность
print (np.matrix(np.arange(1, x * y + 1).reshape((x, y))).T)

#евклидово расстояние
x = 5 #x - размерность
y = 5 #y - размерность
A = np.random.rand(x, y) #матрица
b = np.random.rand(y) #вектор
A = A - b
A = A * A
print (A.sum(axis=1))

#косинусная мера матрицы
import math

x = 5 #x - размерность
y = 5 #y - размерность
A = np.random.rand(x, y) #матрица
b = np.random.rand(y) #вектор
print ( (A * b).sum(axis = 1) / ( np.sqrt((A * A).sum(axis = 1)) * math.sqrt((b * b).sum()) ) )

#косинусная мера векторов

x = 5 #x - размерность
a = np.random.rand(x) #вектор 1
b = np.random.rand(x) #вектор 2
print ( (a * b).sum() / ( math.sqrt((a * a).sum()) * math.sqrt((b * b).sum()) ) )
