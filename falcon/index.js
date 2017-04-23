/*!
 * 猎鹰网络安全评估系统主程序
 *
 * 版权所有 2017 陈学芹 联系方式: 7219530@qq.com
 *
 * 本软件在GNU LGPL-V2.1协议下发布，欢迎使用.
 */

var express = require('express');
var bodyParser = require('body-parser');
var session = require('express-session');
var paginate = require('express-paginate');

var logger = require('./lib/logger');
var ConfigManager = require('./lib/config');
var env = require('./lib/env');
// 检查运行环境是否满足要求
env.validate();

var app = module.exports = express();

const PAGE_LIMIT = 10;
const PAGE_MAX_LIMIT = 50;
app.use(paginate.middleware(PAGE_LIMIT, PAGE_MAX_LIMIT));

// config static resource
app.use(express.static(__dirname+'/public'));

app.set('view engine', 'ejs');
app.set('views', __dirname + '/views');

// middleware
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
// follow http://expressjs.com/en/advanced/best-practice-security.html
app.disable('x-powered-by');
app.use(session({
  resave: false, // don't save session if unmodified
  saveUninitialized: false, // don't create session until something stored
  secret: 'shhhh, very secret',
  name: 'fsid'
}));

// Session-persisted message middleware

app.use(function(req, res, next){
  var sess = req.session;
  /* session test,just counter */
  if (sess.views) {
    sess.views++;
  }else {
    sess.views = 1;
  }
  next();
});


var index = require('./routes/index');
var login = require('./routes/login');
var logout = require('./routes/logout');
var nmap = require('./routes/nmap');
var ddos = require('./routes/ddos');
var ssl = require('./routes/ssl');
var error = require('./routes/error');

app.use(index);
app.use(login);
app.use(logout);
app.use(nmap);
app.use(ddos);
app.use(ssl);
app.use(error);

/* istanbul ignore next */
if (!module.parent) {
  var port = ConfigManager.getPort();
  app.listen(port);
  logger.info('Falcon service started on http port %s', port);

  try {
    var options = ConfigManager.getSSL();
    var https = require('https');

    https.createServer(options, app).listen(options.port, () => {
      logger.info('Falcon service started on https port %s', options.port);
    });

  } catch (e) {
    logger.debug('Falcon https disabled (%s)', e.message);
  } finally {
  }
}
