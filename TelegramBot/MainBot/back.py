import bot_token as tk
import config_server as cfg_s
import cherrypy
import telebot

bot = telebot.TeleBot(tk.token)

class WebhookServer(object):
    @cherrypy.expose
    def index(self):
        if 'content-length' in cherrypy.request.headers and \
                        'content-type' in cherrypy.request.headers and \
                        cherrypy.request.headers['content-type'] == 'application/json':
            length = int(cherrypy.request.headers['content-length'])
            json_string = cherrypy.request.body.read(length).decode("utf-8")
            update = telebot.types.Update.de_json(json_string)
            # Эта функция обеспечивает проверку входящего сообщения
            bot.process_new_updates([update])
            return ''
        else:
            raise cherrypy.HTTPError(403)

# Снимаем вебхук перед повторной установкой (избавляет от некоторых проблем)
bot.remove_webhook()

 # Ставим заново вебхук
bot.set_webhook(url=cfg_s.WEBHOOK_URL_BASE + cfg_s.WEBHOOK_URL_PATH,
                certificate=open(cfg_s.WEBHOOK_SSL_CERT, 'r'))

# Указываем настройки сервера CherryPy
cherrypy.config.update({
    'server.socket_host': cfg_s.WEBHOOK_LISTEN,
    'server.socket_port': cfg_s.WEBHOOK_PORT,
    'server.ssl_module': 'builtin',
    'server.ssl_certificate': cfg_s.WEBHOOK_SSL_CERT,
    'server.ssl_private_key': cfg_s.WEBHOOK_SSL_PRIV
})

 # Собственно, запуск!
def runbot():
	cherrypy.quickstart(WebhookServer(), cfg_s.WEBHOOK_URL_PATH, {'/': {}})

