/*!
 * DOS测试事件监听类
 *
 * 版权所有 2017 陈学芹 联系方式: 7219530@qq.com
 *
 * 本软件在GNU LGPL-V2.1协议下发布，欢迎使用.
 */
var exec = require('child_process').exec;
var sprintf = require('sprintf-js').sprintf;
var logger = require('../lib/logger');
var dgram = require('dgram');
var qs  = require('querystring');

FloodListener = function(port, controller) {
  if ( !port ) {
    throw new Error('missing port or controller parameter');
  }
  this.port = port;
  this.controller = controller;
  this.stat = { packetSent: 0, byteSent: 0};
}

FloodListener.prototype = {
  constructor: FloodListener,

  start: function () {
    var controller = this.controller;
    if (this.socket) {
      return;
    }
    var thiz = this;
    this.socket = dgram.createSocket('udp4');
    this.socket.bind(this.port, function () {
      logger.debug(`bind port ${thiz.port} success`);
    });

    this.socket.on('message', function(msg, rinfo) {
      logger.debug(`got: ${msg} from ${rinfo.address}:${rinfo.port}`);
      var str = msg.toString('utf8', 0, msg.length);
      if (!/^packets=\d+&bytes=\d+$/.test(str)) {
        logger.warn('invalid incoming data (%s)', str);
        return;
      }

      var trafficData = qs.parse(str);
      var nPacket = parseInt(trafficData.packets);
      var nByte = parseInt(trafficData.bytes);
      thiz.stat.packetSent += nPacket;
      thiz.stat.byteSent += nByte;
      if (!controller) {
        logger.warn('no controller to handle ');
        return;
      }
      var agent = controller.findAgent({host: rinfo.address});
      if (!agent ) {
        logger.warn('unknown udp packet from %s', rinfo.address);
        return;
      }
    	agent.processTraffic({packet:nPacket, byte:nByte});
    });

  },

  stop: function () {
    if (!this.socket) {
      return;
    }
    var thiz = this;
    this.socket.close(()=> {
      thiz.socket = null;
    });
  },

  getState: function () {
    if (!this.socket) {
      return 'idle';
    } else {
      return 'started';
    }
  },

  getTraffic: function () {
    return this.stat;
  }
}

module.exports = FloodListener;
