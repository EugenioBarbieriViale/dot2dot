require('dotenv').config();
const { Telegraf } = require('telegraf');

const bot = new Telegraf(process.env.TOKEN);

bot.command('start', (ctx) => {
  ctx.reply('Welcome to Dot2Dot Game!', {
    reply_markup: {
      inline_keyboard: [
        [{
          text: 'Play dot2dot',
          web_app: { url: "https://eugeniobarbieriviale.github.io/dot2dot-telegram/" }
        }]
      ]
    }
  });
});

bot.launch();
