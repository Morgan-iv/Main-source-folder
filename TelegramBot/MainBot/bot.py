import telebot
import back
import config as cfg
import bot_token as tk
import requests
from urllib.request import urlopen
#from lxml import html
#from lxml import etree
from bs4 import BeautifulSoup
import random

maxquote = 1000

def getmaxquote():
	QURL = 'https://bash.im/'
	resp = requests.get(QURL, allow_redirects=False)
	bs = BeautifulSoup(resp.text, 'lxml')
	sel = bs.select('html#godtier body div#body div.quote div.actions a.id')
	return int(sel[0].text[1:])

def getquote(num):
	QURL = 'https://bash.im/quote/%d' % num
	resp = requests.get(QURL, allow_redirects=False)
	if (resp.status_code >= 300):
		return None
	bs = BeautifulSoup(resp.text, 'lxml')
	for br in bs.find_all("br"):
		br.replace_with("\n")
	return bs.select('.text')[0].text

@back.bot.message_handler(commands=['start', 'help'])
def handle_start_help(message):
    back.bot.send_message(message.chat.id, cfg.help_msg)
	
@back.bot.message_handler(commands=['quote'])
def handle_quote(message):
	s = message.text[message.entities[0].length:].strip()
	try:
		i = int(s)
		q = getquote(i)
		if q == None:
			back.bot.send_message(message.chat.id, cfg.noquote_msg % i)
		else:
			back.bot.send_message(message.chat.id, ('quote %d:\n' % i) + q)
	except ValueError:
		i = random.randint(0, maxquote)
		q = getquote(i)
		while q == None:
			i = random.randint(0, maxquote)
			q = getquote(i)
		back.bot.send_message(message.chat.id, ('quote %d:\n' % i) + q)
		
@back.bot.message_handler(commands=['update'])
def update_quote(message):
	global maxquote
	maxquote = getmaxquote();
	back.bot.send_message(message.chat.id, 'Update!')

@back.bot.message_handler(func=lambda message: True, content_types=['text'])
def echo_message(message):
    back.bot.reply_to(message, message.text)
	



back.runbot()

