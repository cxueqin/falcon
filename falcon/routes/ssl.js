'use strict'

/*!
 * WEB UI层端口扫描处理定义文件
 *
 * 版权所有 2017 陈学芹 联系方式: 7219530@qq.com
 *
 * 本软件在GNU LGPL-V2.1协议下发布，欢迎使用.
 */
var express = require('express');
var router = express.Router();
require('./common');
var execOptions = {title:"SSL检测" };
var logger = require('../lib/logger');
const util = require('util');
var sslCertficate = require('../ssl/cert');
const assert = require('assert');
/*TODO: support multi session concurrent*/
var ssl_state = undefined;
var ssl_error = 0;
var ssl_cert = undefined;

router.get('/ui/ssl/exec', function(req, res){
  res.render('ssl_exec', execOptions);
});

/* Note current only support one ssl detect session */
router.post('/api/ssl', function(req, res){
  //TODO:处理参数异常情况
  if (ssl_state == 'running') {
    logger.err("ssl detect already running");
    res.status(500).end();
    return;
  }
  var host = req.body.host;
  var port = req.body.port || 443;
  if (!host) {
    logger.err("Lack of host parameter");
    res.status(500).end();
    return;
  }
  ssl_state = 'running';
  logger.debug("start to check ssl certificate %s:%s", host, port);
  sslCertficate.get(host, port).then((cert)=> {
    ssl_state = 'done';
    ssl_cert = cert;
    ssl_error = 0;
    logger.debug("ssl done");
  })
  .catch((e)=>{
    ssl_error = 400;
    ssl_state = 'done';
  })
  res.status(200).end();
});

router.get('/api/ssl', function(req, res){
  if (typeof ssl_state === 'undefined') {
    /* ssl never been started */
    res.status(500).end();
    return;
  }

  if (ssl_state == 'running') {
    res.status(100).end();
    return;
  }
  assert(ssl_state == 'done');
  var result = {};
  result.errcode = ssl_error;
  if (ssl_cert) {
    result.cert = ssl_cert;
  }
  res.status(200).json(result);
});

router.get('/ui/ssl/history', function(req, res){
  //TODO: add history
  res.status(500).end();
});

module.exports = router;
