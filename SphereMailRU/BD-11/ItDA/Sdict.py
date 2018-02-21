import random

class Sdict(dict):
	def iteritems(self):
		return self.items().__iter__()
	def iterkeys(self):
		return self.keys().__iter__()
	def itervalues(self):
		return self.values().__iter__()
	def getrandomkey(self):
		return list(self.keys())[random.sample(range(len(self)), 1)[0]]
	def keymaxlen(self):
		m = 0
		for i in self.keys():
			if len(i) > m:
				m = len(i)
		return m
	def pe(self, another):
		for i in another.items():
			self[i[0]] = i[1]
		return self
	def __add__(self, another):
		ndict = Sdict(self)
		ndict.pe(another)
		return ndict
	def getkeysfrom(self, str):
		res = []
		for i in self.keys():
			if i.startswith(str):
				res.append(i)
		return res
		
a = {'a':1, 'b':2, 'cc':3}
print(a)
b = Sdict(a)
c = Sdict(a)
c['c'] = 4
c['b'] = 6
b['k'] = 5
print(b)
print(b.iteritems())
print(b.getrandomkey())
print(b.keymaxlen())
m = b + c
print(b)
print(c)
print(m.getkeysfrom('c'))