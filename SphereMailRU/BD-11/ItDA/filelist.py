import os
import re

"""
Выводит не через таб, а с предыдущими папками + /
Можно и через таб, но тогда придется вырезать отдельно имя файла, а лишние строковые операции - беее
Ну и хранить к этому еще уровень вложенности
"""

def file_tree(file_filter = None, path = '.'):
	if os.path.isdir(path):
		print(path)
		flist = os.listdir(path)
		for i in flist:
			file_tree(file_filter, path + '/' + i)
	elif not file_filter or re.match('.*' + file_filter + '$', path):
		print (path)
file_tree()
