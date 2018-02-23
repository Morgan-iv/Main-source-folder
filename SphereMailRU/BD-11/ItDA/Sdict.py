import random
import json
import csv

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
	def __iadd__(self, another):
		for i in another.items():
			self[i[0]] = i[1]
		return self
	def __add__(self, another):
		ndict = Sdict(self)
		ndict.__iadd__(another)
		return ndict
	def getkeystartsfrom(self, s):
		res = []
		for i in self.keys():
			if i.startswith(s):
				res.append(i)
		return res
	def updfromcsv(self, fname):
		with open(fname) as fread:
			for i, x in enumerate(csv.reader(fread)):
				if len(x) == 2:
					self[x[0]] = x[1]
				else:
					print ('string ' + str(i) + ' is invalid')
	def getfromcsv(self, fname):
		self.clear()
		self.updfromcsv(fname)
	def updfromjson(self, fname):
		with open(fname) as fread:
			self.update(json.load(fread))
	def getfromjson(self, fname):
		self.clear()
		self.updfromjson(fname)
	def to_json(self, fname):
		with open(fname, 'w') as fwrite:
			json.dump(self, fwrite)
	def to_csv(self, fname):
		with open(fname, 'w') as fwrite:
			csv.writer(fwrite).writerows(self.items())
	def __init__(self, fname):
		if isinstance(fname, str):
			super().__init__()
			if fname.endswith('.csv'):
				self.updfromcsv(fname)
			if fname.endswith('.json'):
				self.updfromjson(fname)
		else:
			super().__init__(fname)
			
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
print(m.getkeystartsfrom('c'))