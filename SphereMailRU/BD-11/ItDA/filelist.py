import os
import re

def file_tree(file_filter = None, path = '.', level = 0):
	if os.path.isdir(path):
		print(path)
		flist = os.listdir(path)
		for i in flist:
			file_tree(file_filter, path + '/' + i, level + 1)
	elif file_filter and re.match('.*' + file_filter + '$', path):
		print (path)
file_tree()