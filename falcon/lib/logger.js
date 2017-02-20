/*!
 * 系统日志类
 *
 * 版权所有 2017 陈学芹 联系方式: 7219530@qq.com
 *
 * 本软件在GNU LGPL-V2.1协议下发布，欢迎使用.
 */

'use strict';

var syslog = require('modern-syslog');
var ConfigManager = require('./config');

function Logger() {
  var ident = "falcon";
  var option = syslog.LOG_PID | syslog.LOG_ODELAY ;
  if (ConfigManager.ifLogConsole()) {
    option |= syslog.LOG_PERROR
  }
  var facility = syslog.LOG_LOCAL0;

  syslog.open(ident, option, facility);
  syslog.upto(ConfigManager.getLoglevel());
  this.init();
}

// Encapsulation in JavaScript 封装，继承，多态CRUD就属于多态一种
// The one disadvantage of overwriting the prototype is that
// the constructor property no longer points to the prototype,
// so we have to set it manually.
Logger.prototype = {
  constructor: Logger,

  //这样的现代写法真是绝妙
  wrap: function (name) {

    this[name] = syslog[name];

  },

  init: function () {
    this.wrap('emerg');
    this.wrap('alert');
    this.wrap('emerg');
    this.wrap('crit');
    this.wrap('err');
    this.wrap('warn');
    this.wrap('warning');
    this.wrap('note');
    this.wrap('notice');
    this.wrap('info');
    this.wrap('debug');
    //这是直观的方法
    //this.emerg = syslog.emerg;
    //this.notice = syslog.notice;
  }
};

exports = module.exports = new Logger();
