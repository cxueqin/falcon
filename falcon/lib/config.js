/*!
 * 系统配置辅助类
 *
 * 版权所有 2017 陈学芹 联系方式: 7219530@qq.com
 *
 * 本软件在GNU LGPL-V2.1协议下发布，欢迎使用.
 */
'use strict';

var CONFIG_FILE = '/etc/falcon.conf';
var IniConfigParser = require('ini-config-parser');
var fs = require('fs');
var DEFAULT_PORT = 8000;
var _ = require('lodash');

function ConfigManager(path) {
  //参数必须赋值，否则抛出异常
  if (!path || !fs.existsSync(path)) {
    throw new Error(`lack of falcon config file ${path}`)
  }

  var data = fs.readFileSync(path).toString();
  this.config = IniConfigParser.parse(data);

}

// Encapsulation in JavaScript 封装，继承，多态CRUD就属于多态一种
// The one disadvantage of overwriting the prototype is that
// the constructor property no longer points to the prototype,
// so we have to set it manually.
ConfigManager.prototype = {
  constructor: ConfigManager,

  getNmapStorageDir: function () {
    return this.config.daemon.datadir+"/nmap";
  },

  getSSL: function () {
    var port = this.config.daemon.port_ssl;

    if ( !port ) {
      throw new Error(`missing port_ssl config`);
    }
    var portInt = parseInt(port, 10);
    if (isNaN(portInt)) {
      throw new Error(`port_ssl ${port} invalid`);
    }

    if (portInt < 1024 || portInt > 65535) {
      throw new Error(`port_ssl ${port} range invalid`);
    }

    var key = this.config.daemon.key;
    var cert = this.config.daemon.cert;
    if (!key || !fs.existsSync(key)) {
      throw new Error(`missing private key file ${key}`);
    }

    if (!cert || !fs.existsSync(cert)) {
      throw new Error(`missing cerificate key file ${cert}`);
    }

    return {
      port: port,
      key: fs.readFileSync(key),
      cert: fs.readFileSync(cert)
    };
  },

  getPort: function () {
    var port = this.config.daemon.port;

    if ( !port )
      return DEFAULT_PORT;
    var portInt = parseInt(port, 10);
    if (isNaN(portInt))
      return DEFAULT_PORT;
    if (portInt < 1024 || portInt > 65535)
      return DEFAULT_PORT;
    return portInt;
  },

  getLoglevel: function () {
    var map = {
      emerg:    'LOG_EMERG',
      alert:    'LOG_ALERT',
      crit:     'LOG_CRIT',
      err:      'LOG_ERR',
      warning:  'LOG_WARNING',
      notice:   'LOG_NOTICE',
      info:     'LOG_INFO',
      debug:    'LOG_DEBUG'
    };
    var level = this.config.daemon.loglevel;
    if ( !level )
      return 'LOG_INFO';
    if ( map[level] ) {
      return map[level];
    } else {
      return 'LOG_INFO';
    }
  },

  ifLogConsole: function () {
    var flag = this.config.daemon.log_console;
    var trueArray = ["true", "yes", "1"];

    if ( typeof flag === 'undefined' ) {
      return false;
    }
    var flagStr = new String(flag);
    var flagLC = flagStr.toLowerCase();
    for (var i =0; i< trueArray.length; i++) {
      if (flagLC == trueArray[i]) {
        return true;
      }
    }
    return false;
  },

  getDdosAgents: function () {
    var strAgents = this.config.ddos.agents;

    return _.split(strAgents, ',');
  }
};

exports = module.exports = new ConfigManager(CONFIG_FILE);
