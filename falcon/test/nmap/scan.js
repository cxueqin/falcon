var should = require('chai').should();
var assert = require('assert');
var expect = require('chai').expect;
var nmap = require('../../nmap/nmap').nodenmap;
var net = require('net');
var _ = require('lodash');
const debug = require('debug')('test');

const WATCH_PORT = 39999;

describe('NmapScan', function () {

  it('Minimal local loopback basic port NMAP', function (done) {

    this.timeout(20000);
    var scan = new nmap.NmapScan("127.0.0.1");
    scan.on('complete', function (data) {
      expect(data).to.be.instanceOf(Array);
      expect(data).to.not.be.empty;
      expect(data[0]).to.include.keys('hostname', 'ip', 'mac', 'openPorts', 'osNmap');
      //console.log(data);
      done();
    });
    scan.on('error', (data) => {
      console.log(data);
    });
    scan.startScan();
  });

  it('Full scan with watch  tcp port added', function (done) {

    this.timeout(30000);
    var server = net.createServer((socket) => {
      socket.end('goodbye\n');
      socket.on('error', function () {
      });
    }).on('error', (err) => {
      // handle errors here
      console.log(err);
      throw err;
    });

    server.listen(WATCH_PORT, () => {
      //console.log('opened server on', server.address());
      var scan = new nmap.FullScan("127.0.0.1");
      scan.on('complete', function (data) {
        //console.log(data);
        debug("data1=%j", data)
        var portInclude = false;
        expect(data).to.be.instanceOf(Array);
        expect(data).to.not.be.empty;
        expect(data[0]).to.include.keys('hostname', 'ip', 'mac', 'openPorts', 'osNmap');
        _.forEach(data[0].openPorts, (v) => {
          if (v.port.portid == WATCH_PORT) {
            portInclude = true;
          }
        });
        scan.on('error', (data) => {
          console.log(data);
        });
        assert.ok(portInclude, 'should include watch tcp port');
        server.close(() => {
          done();
        });
      });
      scan.startScan();
    });
  });
  it('外网主机可达', function (done) {
    this.timeout(20000)
    let scan = new nmap.NmapScan("baidu.com");
    scan.on('complete', function (data) {
      expect(data).to.be.instanceOf(Array);
      expect(data).to.not.be.empty;
      expect(data[0]).to.include.keys('hostname', 'ip', 'mac', 'openPorts', 'osNmap');
      assert.equal(data[0].openPorts.length, 2)
      assert.equal(data[0].openPorts[0].port.portid, 80)
      assert.equal(data[0].openPorts[1].port.portid, 443)
      debug('data = %j', data);
      done();
    });
    scan.on('error', (data) => {
      console.log(data);
    });
    scan.startScan();
  });

  it('内网主机可达', function (done) {
    this.timeout(20000)
    let scan = new nmap.NmapScan("192.168.3.175");
    scan.on('complete', function (data) {
      expect(data).to.be.instanceOf(Array);
      expect(data).to.not.be.empty;
      expect(data[0]).to.include.keys('hostname', 'ip', 'mac', 'openPorts', 'osNmap');
      expect(data[0].openPorts).to.have.length.above(0);
      debug('data = %j', data);
      done();
    });
    scan.on('error', (data) => {
      console.log(data);
    });
    scan.startScan();
  });



  it('默认端口范围', function (done) {
    this.timeout(20000)
    let scan = new nmap.NmapScan("baidu.com");
    scan.on('complete', function (data) {
      expect(data).to.be.instanceOf(Array);
      expect(data).to.not.be.empty;
      expect(data[0]).to.include.keys('hostname', 'ip', 'mac', 'openPorts', 'osNmap');
      assert.equal(data[0].openPorts.length, 2)
      assert.equal(data[0].openPorts[0].port.portid, 80)
      assert.equal(data[0].openPorts[1].port.portid, 443)
      debug('data = %j', data);
      done();
    });
    scan.on('error', (data) => {
      console.log(data);
    });
    scan.startScan();
  });



  it('端口范围1-100', function (done) {

    this.timeout(20000);
    let spec = [{ begin: 1, end: 100 }]
    let scan = new nmap.NmapScan("baidu.com", spec);
    scan.on('complete', function (data) {
      expect(data).to.be.instanceOf(Array);
      expect(data).to.not.be.empty;
      expect(data[0]).to.include.keys('hostname', 'ip', 'mac', 'openPorts', 'osNmap');
      assert.equal(data[0].openPorts.length, 1)
      assert.equal(data[0].openPorts[0].port.portid, 80)
      debug('data = %j', data);
      done();
    });
    scan.on('error', (data) => {
      console.log(data);
    });
    scan.startScan();
  });

  it('端口范围1-500', function (done) {

    this.timeout(20000);

    let spec = [{ begin: 1, end: 500 }]

    let scan = new nmap.NmapScan("baidu.com", spec);
    scan.on('complete', function (data) {
      expect(data).to.be.instanceOf(Array);
      expect(data).to.not.be.empty;
      expect(data[0]).to.include.keys('hostname', 'ip', 'mac', 'openPorts', 'osNmap');
      assert.equal(data[0].openPorts.length, 2)
      assert.equal(data[0].openPorts[0].port.portid, 80)
      assert.equal(data[0].openPorts[1].port.portid, 443)
      debug('data = %j', data);
      done();
    });
    scan.on('error', (data) => {
      console.log(data);
    });
    scan.startScan();
  });

  it('端口范围1-100,端口范围400-500', function (done) {

    this.timeout(20000);
    let spec = [{ begin: 1, end: 100 }, { begin: 400, end: 500 }]
    let scan = new nmap.NmapScan("baidu.com", spec);
    scan.on('complete', function (data) {
      expect(data).to.be.instanceOf(Array);
      expect(data).to.not.be.empty;
      expect(data[0]).to.include.keys('hostname', 'ip', 'mac', 'openPorts', 'osNmap');
      assert.equal(data[0].openPorts.length, 2)
      assert.equal(data[0].openPorts[0].port.portid, 80)
      assert.equal(data[0].openPorts[1].port.portid, 443)
      debug('data = %j', data);
      done();
    });
    scan.on('error', (data) => {
      console.log(data);
    });
    scan.startScan();
  });

  it('端口范围1-100,指定端口443', function (done) {

    this.timeout(20000);
    let spec = [{ begin: 1, end: 100 }, 443]
    let scan = new nmap.NmapScan("baidu.com", spec);
    scan.on('complete', function (data) {
      expect(data).to.be.instanceOf(Array);
      expect(data).to.not.be.empty;
      expect(data[0]).to.include.keys('hostname', 'ip', 'mac', 'openPorts', 'osNmap');
      assert.equal(data[0].openPorts.length, 2)
      assert.equal(data[0].openPorts[0].port.portid, 80)
      assert.equal(data[0].openPorts[1].port.portid, 443)
      debug('data = %j', data);
      done();
    });
    scan.on('error', (data) => {
      console.log(data);
    });
    scan.startScan();
  });

  it('指定端口80,443', function (done) {

    this.timeout(20000);
    let spec = [80, 443]
    let scan = new nmap.NmapScan("baidu.com", spec);
    scan.on('complete', function (data) {
      expect(data).to.be.instanceOf(Array);
      expect(data).to.not.be.empty;
      expect(data[0]).to.include.keys('hostname', 'ip', 'mac', 'openPorts', 'osNmap');
      assert.equal(data[0].openPorts.length, 2)
      assert.equal(data[0].openPorts[0].port.portid, 80)
      assert.equal(data[0].openPorts[1].port.portid, 443)
      debug('data = %j', data);
      done();
    });
    scan.on('error', (data) => {
      console.log(data);
    });
    scan.startScan();
  });

  it('指定端口80,445', function (done) {

    this.timeout(20000);
    let spec = [80, 445]
    let scan = new nmap.NmapScan("baidu.com", spec);
    scan.on('complete', function (data) {
      expect(data).to.be.instanceOf(Array);
      expect(data).to.not.be.empty;
      expect(data[0]).to.include.keys('hostname', 'ip', 'mac', 'openPorts', 'osNmap');
      assert.equal(data[0].openPorts.length, 2)
      assert.equal(data[0].openPorts[0].port.portid, 80)
      assert.equal(data[0].openPorts[1].port.portid, 445)
      assert.equal(data[0].openPorts[1].state.state, 'filtered')
      debug('data = %j', data);
      done();
    });
    scan.on('error', (data) => {
      console.log(data);
    });
    scan.startScan();
  });

  it('指定端口81,400-500', function (done) {

    this.timeout(20000);
    let spec = [81, { begin: 400, end: 500 }]
    let scan = new nmap.NmapScan("baidu.com", spec);
    scan.on('complete', function (data) {
      expect(data).to.be.instanceOf(Array);
      expect(data).to.not.be.empty;
      expect(data[0]).to.include.keys('hostname', 'ip', 'mac', 'openPorts', 'osNmap');
      assert.equal(data[0].openPorts.length, 1)
      assert.equal(data[0].openPorts[0].port.portid, 443)
      debug('data = %j', data);
      done();
    });
    scan.on('error', (data) => {
      console.log(data);
    });
    scan.startScan();
  });

  it('指定端口1-20', function (done) {

    this.timeout(20000);
    let spec = [{ begin: 1, end: 20 }]
    let scan = new nmap.NmapScan("baidu.com", spec);
    scan.on('complete', function (data) {
      expect(data).to.be.instanceOf(Array);
      expect(data).to.not.be.empty;
      expect(data[0]).to.include.keys('hostname', 'ip', 'mac', 'openPorts', 'osNmap');
      assert.equal(data[0].openPorts.length, 20)
      for (let i = 0; i < 20; i++) {
        assert.equal(data[0].openPorts[i].state.state, 'filtered')
      }
      debug('data = %j', data);
      done();
    });
    scan.on('error', (data) => {
      console.log(data);
    });
    scan.startScan();
  })
  
  it('指定端口81', function (done) {

    this.timeout(20000);
    let spec = [81]
    let scan = new nmap.NmapScan("baidu.com", spec);
    scan.on('complete', function (data) {
      expect(data).to.be.instanceOf(Array);
      expect(data).to.not.be.empty;
      expect(data[0]).to.include.keys('hostname', 'ip', 'mac', 'openPorts', 'osNmap');
      assert.equal(data[0].openPorts.length, 1)
      assert.equal(data[0].openPorts[0].port.portid, 81)
      assert.equal(data[0].openPorts[0].state.state, 'filtered')
      debug('data = %j', data);
      done();
    });
    scan.on('error', (data) => {
      console.log(data);
    });
    scan.startScan();
  });

  //NmapScan should accept multi space
  it.skip('when /etc/resolv.conf is missing, should caught some error', function (done) {
    this.timeout(20000);
    const fs = require('fs');
    const dnspath = '/etc/resolv.conf';
    assert.ok(!fs.existsSync(dnspath), `please remove ${dnspath} by turning off network or other method`)
    var scan = new nmap.NmapScan("127.0.0.1");
    scan.on('complete', function (data) {
      expect(data).to.be.instanceOf(Array);
      expect(data).to.not.be.empty;
      expect(data[0]).to.include.keys('hostname', 'ip', 'mac', 'openPorts', 'osNmap');
      done();
    });
    scan.on('error', function (data) {
      assert.ok(data.indexOf('Unable to open /etc/resolv.conf') != -1);
    });
    scan.startScan();
  });
});
