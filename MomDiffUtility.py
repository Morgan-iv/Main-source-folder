import docx2txt
import difflib
import os

from tkinter import *
from tkinter import filedialog

def delspace(text):
    while(text.find('\r') > -1):
        text = text.replace('\r', '')
    while(text.find('\t') > -1):
        text = text.replace('\t', ' ')
    while(text.find('  ') > -1):
        text = text.replace('  ', ' ')
    while(text.find(' \n') > -1):
        text = text.replace(' \n', '\n')
    while(text.find('\n\n') > -1):
        text = text.replace('\n\n', '\n')
    return text

def load_docx(name):
    return delspace(docx2txt.process(name))

def mydiff(a, b):
    if (len(a) > len(b)):
        a, b = b, a
    lres = list(difflib.Differ().compare(a, b))
    return len(lres)

def mymet(a, b):
    l1 = len(a)
    l2 = len(b)
    d = mydiff(a, b)
    dlen = d / ((l1 + l2) * 0.5)
    return ((dlen ** 4) - 1) * (100 / 15.0)

def findfiles(directory):
    flist = []
    for root, dirs, files in os.walk(directory):
        for name in files:
            if name.endswith('.docx') and not name.startswith('~$'):
                flist.append(os.path.join(root, name))
    return flist

def process(filename, flist):
    report = ''
    try:
        testfile = load_docx(filename)
    except Exception as e:
        report += 'Не удалось считать файл для сравнения: {}\n'.format(filename)
        report += 'Аварийное завершение проверки\n'
        yield report
    if report == '':
        count = 0
        sz = len(flist)
        for name in flist:
            count = count + 1
            try:
                curf = load_docx(name)
            except Exception as e:
                yield '{} из {}: Не удалось считать файл {}\n'.format(count, sz, name)
                continue
            res = mymet(curf, testfile)
            yield '{} из {}: Результат: {} Имя файла: {}\n'.format(count, sz, res, name)

def Run(ev):
    if root.my_dname == '' or root.my_fname == '':
        textbox['state'] = 'normal'
        textbox.delete(1.0, END)
        textbox.insert(END, 'Выберите файл и папку для проверки!')
        textbox['state'] = 'disabled'
        return
    textbox['state'] = 'normal'
    textbox.insert(END, 'Проверяемый файл: {}\n'.format(root.my_fname))
    textbox['state'] = 'disabled'
    for rep in process(root.my_fname, root.my_flist):
        textbox['state'] = 'normal'
        textbox.insert(END, rep)
        textbox['state'] = 'disabled'
        textbox.update()

def OpenDir(ev): 
    root.my_flist = []
    root.my_dname = filedialog.askdirectory()
    if root.my_dname != '':
        root.my_flist = findfiles(root.my_dname)
        res = 'Список файлов для проверки:\n'
        for f in root.my_flist:
            res += f + '\n'
        textbox['state'] = 'normal'
        textbox.delete(1.0, END)
        textbox.insert(END, res)
        textbox['state'] = 'disabled'

def OpenFile(ev):
    root.my_fname = filedialog.askopenfilename(filetypes = (('Документы MS Word 2007+', '*.docx'), ('Все файлы', '*.*')))
    textbox['state'] = 'normal'
    textbox.insert(END, 'Проверяемый файл: {}\n'.format(root.my_fname))
    textbox['state'] = 'disabled'

def SaveFile(ev):
    fn = filedialog.asksaveasfilename(filetypes = (('Текстовые файлы', '*.txt'), ('Все файлы', '*.*')))
    if fn == '':
        return
    if not fn.endswith('.txt'):
        fn += '.txt'
    textbox['state'] = 'normal'
    open(fn, 'wt').write(textbox.get(1.0, END))
    textbox['state'] = 'disabled'

root = Tk()
root.my_dname = ''
root.my_fname = ''
root.my_flist = []

panelFrame = Frame(root, height = 60, bg = 'gray')
textFrame = Frame(root, height = 340, width = 600)

panelFrame.pack(side = 'top', fill = 'x')
textFrame.pack(side = 'bottom', fill = 'both', expand = 1)

textbox = Text(textFrame)
scrollbar = Scrollbar(textFrame)

scrollbar['command'] = textbox.yview
textbox['yscrollcommand'] = scrollbar.set
textbox['state'] = 'disabled'

textbox.pack(side = 'left', fill = 'both', expand = 1)
scrollbar.pack(side = 'right', fill = 'y')

DOpenBtn = Button(panelFrame, text = 'Dir')
FOpenBtn = Button(panelFrame, text = 'File')
RunBtn = Button(panelFrame, text = 'Run')
SaveBtn = Button(panelFrame, text = 'Save')

DOpenBtn.bind('<Button-1>', OpenDir)
FOpenBtn.bind('<Button-1>', OpenFile)
RunBtn.bind('<Button-1>', Run)
SaveBtn.bind('<Button-1>', SaveFile)

DOpenBtn.place(x = 10, y = 10, width = 40, height = 40)
FOpenBtn.place(x = 60, y = 10, width = 40, height = 40)
RunBtn.place(x = 110, y = 10, width = 40, height = 40)
SaveBtn.place(x = 160, y = 10, width = 40, height = 40)

root.mainloop()
