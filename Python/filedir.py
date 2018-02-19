import os
import re

def filelist(name='', cdir='.', level=0):
	flist = os.listdir(cdir)
	for i in flist:
		for j in range(level):
			print('    ', end=' ')
		if os.path.isdir(cdir + '/' + i):
			print(i)
			filelist(name, cdir + '/' + i, level + 1)
		elif re.match(r'^[^\/]*' + name + '$', i):
			print(i)
filelist()