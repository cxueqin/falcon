var should = require('chai').should();
var  assert = require('assert');
var  expect = require('chai').expect;
var nmap = require('../../nmap/nmap').nodenmap;
var net = require('net');

describe('NmapScan', function () {

  it('Minimal local loopback basic port NMAP', function (done) {

    this.timeout(10000);
    var scan = new nmap.NmapScan("127.0.0.1");
    scan.on('complete', function (data) {
      expect(data).to.be.instanceOf(Array);
      expect(data).to.not.be.empty;
      expect(data[0]).to.include.keys('hostname', 'ip', 'mac', 'openPorts', 'osNmap');
      console.log(data);
      done();
    });
    scan.startScan();
  });

  it('extra scan with 39999 tcp port added', function (done) {

    this.timeout(10000);
    var server = net.createServer((socket) => {
      socket.end('goodbye\n');
    }).on('error', (err) => {
      // handle errors here
      throw err;
    });

    server.listen(39999, () => {
      console.log('opened server on', server.address());
      var scan = new nmap.FullScan("127.0.0.1");
      scan.on('complete', function (data) {
        expect(data).to.be.instanceOf(Array);
        expect(data).to.not.be.empty;
        expect(data[0]).to.include.keys('hostname', 'ip', 'mac', 'openPorts', 'osNmap');
        server.close(() => {
          done();
        });
      });
      scan.startScan();
    });
  });

  //TODO: NmapScan should accept multi space
  it('accepts 2+ space separated command', function (done) {

    this.timeout(10000);
    var scan = new nmap.NmapScan("127.0.0.1");
    scan.on('complete', function (data) {

      expect(data).to.be.instanceOf(Array);
      expect(data).to.not.be.empty;
      expect(data[0]).to.include.keys('hostname', 'ip', 'mac', 'openPorts', 'osNmap');
      done();
    });
    scan.startScan();
  });
});
