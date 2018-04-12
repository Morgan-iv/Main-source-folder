import telebot
import back
import config as cfg
import bot_token as tk


@back.bot.message_handler(commands=['start', 'help'])
def handle_start_help(message):
    back.bot.send_message(message.chat.id, cfg.help_msg)

@back.bot.message_handler(func=lambda message: True, content_types=['text'])
def echo_message(message):
    back.bot.reply_to(message, message.text)


back.runbot()
