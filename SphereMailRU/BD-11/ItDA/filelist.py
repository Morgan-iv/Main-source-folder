import os
import re

def filelist(name = '', cdir = '.', level = 0):
	if os.path.isdir(cdir):
		print(cdir)
		flist = os.listdir(cdir)
		for i in flist:
			filelist(name, cdir + '/' + i, level + 1)
	elif re.match('.*' + name + '$', cdir):
		print (cdir)
filelist()