/*!
 * 扫描管理类
 *
 * 版权所有 2017 陈学芹 联系方式: 7219530@qq.com
 *
 * 本软件在GNU LGPL-V2.1协议下发布，欢迎使用.
 */

'use strict';

exports.setHome =  setHome;
exports.getManager = getManager;

var low = require('lowdb');
var fs = require('fs');
var _ = require('lodash');
var sprintf = require('sprintf-js').sprintf;
var dataHome = '.';
var manager;

function setHome(path) {
  if (!path || !fs.existsSync(path)) {
    throw new TypeError(`${path} does not exist`)
  }
  dataHome = path;
}

function getManager() {
  if (manager)
    return manager;

  manager = new NmapManager(dataHome);
  return manager;
}

function NmapManager(home) {
  //参数必须赋值，否则抛出异常
  if (!home || !fs.existsSync(home)) {
    throw new TypeError('arg home must be real filesytem path')
  }

  this.db = low(home+'/db.json');
  this.db._.mixin(require('underscore-db'))
  //初始化时，先要初始化数据为合法方式，否则insert插入不了
  //this.db.setState({'results': []});
  //初始化后，就可以调用insert接口，数据将自动存储
  this.db.defaults( {results: []} ).write()
  //this.db.get('results').push({a:"aaa"})
}

// Encapsulation in JavaScript 封装，继承，多态CRUD就属于多态一种
// The one disadvantage of overwriting the prototype is that
// the constructor property no longer points to the prototype,
// so we have to set it manually.
NmapManager.prototype = {
  constructor: NmapManager,
  addResult : function (scan, xml) {
    var item

    if (scan) {
      var row = { host: scan.range[0], time: _.now(), status: 'running'}
      item = this.db.get('results').insert(row).value()
      this.db.write()
    }
    return item;
  },

  updateResult : function (result, data) {
    var item = this.db.get('results').find( {id: result.id})

    var clone = item.value();
    clone.data = data;
    clone.status = 'done';
    item.assign(clone)
    this.db.write()
    return clone;
  },

  delResult : function (rid) {
    //必须加value()方可触发写入，切记
    var item = this.db.get('results').removeById(rid).value()
    if (item) {
      this.db.write();
      return 0;
    } else {
      return -1
    }
  },

  getResultWithDetail : function (rid) {
    var result = this.db.get('results').getById(rid).value();

    if (result )
      this._addTimeString(result);
    return result;
  },

  getResults : function () {
    var results = this.db.get('results').sortBy('time').reverse().value();
    _.forEach(results, this._addTimeString);
    return results;
  },

  _addTimeString : function(o) {
    var date = new Date();

    if (!o.time)
      return o;

    date.setTime(o.time);
    o.timeString = sprintf("%d/%02d/%02d %02d:%02d",
      date.getFullYear(),
      date.getMonth()+1,
      date.getDate(),
      date.getHours(),
      date.getMinutes())
    return o;
  }
};
