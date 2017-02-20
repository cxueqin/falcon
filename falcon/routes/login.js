/*!
 * WEB 登陆认证处理定义文件
 *
 * 版权所有 2017 陈学芹 联系方式: 7219530@qq.com
 *
 * 本软件在GNU LGPL-V2.1协议下发布，欢迎使用.
 */
var express = require('express');
var router = express.Router();
var pam = require('authenticate-pam');
var logger = require('../lib/logger');
require('./common');
// Authenticate using our plain-object database of doom!

function authenticate(name, pass, fn) {
  pam.authenticate(name, pass, function(err){
    if (err){
      fn(new Error('invalid password or username'));
    }else{
      fn(null, {'name': name});
    }
  }),{serviceName:'login' , remoteHost:'localhost'};
}

router.get('/login', function(req, res){
  res.render('login', loginOptions);
});

router.post('/login', function(req, res){
  authenticate(req.body.username, req.body.password, function(err, user){
    if (user) {
      // Regenerate session when signing in
      // to prevent fixation
      req.session.regenerate(function(){
        // Store the user's primary key
        // in the session store to be retrieved,
        // or in this case the entire user object
        logger.notice("auth %s ok from %s", req.body.username, req.ip);
        req.session.user = user;
        req.session.success = 'Authenticated as ' + user.name
          + ' click to <a href="/logout">logout</a>. '
          + ' You may now access <a href="/restricted">/restricted</a>.';
        res.redirect('/');
      });
    } else {
      req.session.error = 'Authentication failed, please check your '
        + ' username and password.'
        + ' (use "tj" and "foobar")';
      logger.notice("auth %s failed from %s", req.body.username, req.ip);
      res.redirect('/login');
    }
  });
});

module.exports = router;
