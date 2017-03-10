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
var execOptions = {title:"端口扫描" };
var setupOptions = {title:"端口规格配置" };
var NmapManagerFactory = require('../nmap/nmap-manager');
var Nmap = require('../nmap/nmap').nodenmap;
var ConfigManager = require('../lib/config');
var manager;
var _ = require('lodash');
var logger = require('../lib/logger');
const util = require('util');

NmapManagerFactory.setHome(ConfigManager.getNmapStorageDir());
manager = NmapManagerFactory.getManager();

router.get('/ui/nmap/exec', function(req, res){
  res.render('nmap_exec', execOptions);
});

router.post('/api/nmap', function(req, res){
  //TODO:处理参数异常情况
  var scan = new Nmap.NmapScan(req.body.target);
  var result = manager.addResult(scan);
  scan.startScan();
  scan.on('complete', function (data) {
    manager.updateResult( result, data)
  });
  //nmap调用可能失败，故在调用方必须增加容错处理
  scan.on('error', function (data) {
    //manager.updateResult( result, data)
  });
  //必须手工设定，否则返回304 Not Modified
  res.status(200).json(result);
  logger.debug("result %s", util.inspect(result, {depth: null}));
});

router.get('/api/nmap', function(req, res){
  logger.debug("query id %s", req.query.id);
  var result = manager.getResultWithDetail(req.query.id);
  res.status(200).json(result);
});

router.get('/ui/nmap/setup', function(req, res){
  res.render('nmap_setup', setupOptions);
});

router.get('/ui/nmap/history', function(req, res){
  var page = req.query.page;
  var limit = req.query.limit;
  var results = manager.getResults();
  var itemCounts = results.length;
  var paginate = res.locals.paginate;

  pageCount = _.ceil(itemCounts / limit);

  var i = 1;
  // add index number to each result
  _.forEach(results, function(item) {
    item.index = i;
    i++;
  });
  //注意：page是从1开始计数的, paginate模块自动算好skip
  results = _.slice(results, req.offset, page*limit );
  var pages = paginate.getArrayPages(10, pageCount, page);
  logger.debug("page: %d pageCount: %d pages: %s",
              page, pageCount, util.inspect(pages, { depth: null }));
  var historyOptions = {
                          title:"端口扫描历史",
                          results: results,
                          pages: pages,
                          pageCount: pageCount,
                          paginate: paginate
                        };
  res.render('nmap_history', historyOptions);
});

module.exports = router;
