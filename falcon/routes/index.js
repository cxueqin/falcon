/*!
 * WEB 首页UI层处理定义文件
 *
 * 版权所有 2017 陈学芹 联系方式: 7219530@qq.com
 *
 * 本软件在GNU LGPL-V2.1协议下发布，欢迎使用.
 */
var express = require('express');
var router = express.Router();
require('./common');

function restrict(req, res, next) {
  if (req.session.user) {
    next();
  } else {
    req.session.error = 'Access denied!';
    res.render('login', loginOptions);
  }
}

function restrictAPI(req, res, next) {
  if (req.session.user) {
    next();
  } else {
    req.status(500).end();
  }
}

/* GET home page. */
// TODO: post 请求没做检查会存在漏洞隐患
router.get('*', restrict);
router.all('/api/*', restrictAPI);

router.get('/', function(req, res){
  res.render('index', indexOptions);
});

router.get('/restricted', restrict, function(req, res){
  res.send('Wahoo! restricted area, click to <a href="/logout">logout</a>');
});

module.exports = router;
