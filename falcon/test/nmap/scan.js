var should = require('chai').should();
var  assert = require('assert');
var  expect = require('chai').expect;
var nmap = require('../../nmap/nmap').nodenmap;
var net = require('net');
var _ = require('lodash');

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
      socket.on('error',function(){
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
		  assert.ok(portInclude,'should include watch tcp port');
      server.close(() => {
      done();
        });
      });
      scan.startScan();
    });
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
