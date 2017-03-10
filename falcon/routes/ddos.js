/*!
 * DOS WEB UI层处理定义文件
 *
 * 版权所有 2017 陈学芹 联系方式: 7219530@qq.com
 *
 * 本软件在GNU LGPL-V2.1协议下发布，欢迎使用.
 */
var express = require('express');
var router = express.Router();
require('./common');
var ddosOptions = {title:"DDOS测试" };
var ConfigManager = require('../lib/config');
var _ = require('lodash');
var logger = require('../lib/logger');
const util = require('util');
var  controller = require('../ddos/controller');
var FloodAgent = require('../ddos/agent');
var  FloodListener = require('../ddos/listener');

controller.init();
var listener = new FloodListener( ConfigManager.getPort(), controller);
listener.start();

var agents = ConfigManager.getDdosAgents();
_.forEach(agents, function (agent) {
  controller.addAgent(new FloodAgent(agent));
});

router.get('/ui/ddos', function(req, res){
  res.render('ddos', ddosOptions);
});

router.post('/api/ddos', function(req, res){
  //TODO:处理参数异常情况,target为非法地址
  var hcode = 200;
  logger.debug("ddos type %s target %s", req.body.type, req.body.target);
  var result;
  try {
    controller.startFlood(req.body.target, {type: req.body.type});
    result = {};
  } catch (e) {
    result = e;
    hcode = 500;
  } finally {
    res.status(hcode).json(result);
  }
});

router.delete('/api/ddos', function(req, res){
  //TODO:处理参数异常情况,target为非法地址
  var hcode = 200;
  logger.debug("stop flood");
  var result;
  try {
    controller.stopFlood();
    result = {};
  } catch (e) {
    result = e;
    hcode = 500;
  } finally {
    res.status(hcode).json(result);
  }
});

router.get('/api/ddos', function(req, res){
  var result = controller.getState();
  res.status(200).json(result);
});

router.get('/api/ddos/traffic', function(req, res){
  var result = controller.getTrafficStat();
  res.status(200).json(result);
});


module.exports = router;
