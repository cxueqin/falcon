/*!
 * DOS测试控制类
 *
 * 版权所有 2017 陈学芹 联系方式: 7219530@qq.com
 *
 * 本软件在GNU LGPL-V2.1协议下发布，欢迎使用.
 */
var exec = require('child_process').exec;
var sprintf = require('sprintf-js').sprintf;
var logger = require('../lib/logger');
var _ = require('lodash');

FloodController = function() {
	this.type = '';
  this.state = 'idle';
	this.target = '';
}

FloodController.prototype = {
  constructor: FloodController,
  init: function () {
    this.agents = new Array();
  },

  getState: function () {
    return {
      state: this.state,
      type: this.type
    };
  },

	addAgent: function (agent) {
    this.agents.push(agent);
	},

  getAgents: function () {
    return _.clone(this.agents);
	},

  removeAgent: function (agent) {
    var removedAgent = _.remove(this.agents, function (item){
      return agent.host == item.host;
    });
    if  (removedAgent.length == 1) {
      return 0;
    } else {
      return -1;
    }
	},

	findAgent: function (selector) {
    return _.find(this.agents, selector);
	},

  startFlood: function (target, option) {
		if (this.state != 'idle') {
			throw new Error(`invalid state ${this.state}`);
		}
		_.forEach(this.agents, function (agent) {
			agent.start(target, option);
		});
		this.state = 'started';
	},

  stopFlood: function () {
		if (this.state != 'started') {
			throw new Error(`invalid state ${this.state}`);
		}

		_.forEach(this.agents, function (agent) {
			agent.stop();
		});
    this.state = 'idle';
  },

  getTrafficStat: function () {
		var packetSent = 0;
		var byteSent = 0;

		_.forEach(this.agents, function (agent){
			var stat = agent.getTraffic();

			packetSent += stat.packetSent;
			byteSent += stat.byteSent;
			logger.debug("agent %s packetSent %d byteSent %d", agent.host,
				stat.packetSent, stat.byteSent);
		});
		logger.debug("packetSent = %d byteSent = %d", packetSent, byteSent);
		return {
			packetSent: packetSent,
			byteSent: byteSent
		};
  },
}

module.exports = new FloodController();
