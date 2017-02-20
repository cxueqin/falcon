var assert = require('assert');
var FloodListener = require('../../ddos/listener');
const LISTEN_PORT = 3333;

describe('FloodListener Interface Static Test', function() {
  before(function() {
    // runs before all tests in this block

  });

  after(function() {
    // runs after all tests in this block
  });

  it('constructor should has port parameter', function() {
    var listener = undefined;

    listener = new FloodListener(LISTEN_PORT);
    assert.ok(listener);
  });

  it('constructor no port parameter should throw exception', function() {

    assert.throws( function () { new FloodListener();}, Error);

  });

  it('constructor may have second controller parameter', function() {
    var listener = undefined;
    var controller = null;

    listener = new FloodListener(LISTEN_PORT, controller);
    assert.ok(listener);
  });

});

describe('FloodListener Interface Dynamic Test', function() {
  var listener;
  var sleep = require('sleep').sleep;

  before(function() {
    // runs before all tests in this block
    listener = new FloodListener(LISTEN_PORT);
    listener.start();
  });

  after(function() {
    // runs after all tests in this block
    listener.stop();
  });

  it('state should be started after 3 seconds', function(done) {
    sleep(1);
    //assert.equal( "started", listener.getState());
    done();
  });

  it('should handle packet correctly when receive valid report packet', function(done) {
    var dgram = require('dgram');
    var message = Buffer.from("packets=2&bytes=200");
    var client = dgram.createSocket("udp4");


    client.send(message, LISTEN_PORT, 'localhost', function(err) {
      //console.log(' message has been sent.');
      client.close();
    });
    // 注意，利用定时器，不阻塞执行线程,让服务端回调接口得以正确处理
    setTimeout(() => {
      var report = listener.getTraffic();
      //console.log('getTraffic '+JSON.stringify(report));
      assert.equal(report.packetSent, 2);
      assert.equal(report.byteSent, 200);
      done();
    }, 1000);
  });

  it('should handle packet correctly when receive invalid report packet', function() {
    var dgram = require('dgram');
    var message = new Buffer("Something Wrong");
    var client = dgram.createSocket("udp4");

    client.send(message, 0, message.length, LISTEN_PORT, "localhost", function(err) {
      client.close();
    });
    setTimeout(() => {
      var stat = listener.getTraffic();
      assert.equal(stat.packetSent, 2);
      assert.equal(stat.byteSent, 200);
      done();
    }, 1000);
  });

});
