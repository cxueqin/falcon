var should = require('chai').should();
var  assert = require('assert');
var  expect = require('chai').expect;
var nmap = require('../../nmap/nmap').nodenmap;
var net = require('net');
var _ = require('lodash');

describe('NmapScan', function () {

  it('Minimal local loopback basic port NMAP', function (done) {

    this.timeout(10000);
    var scan = new nmap.NmapScan("127.0.0.1");
    scan.on('complete', function (data) {
      expect(data).to.be.instanceOf(Array);
      expect(data).to.not.be.empty;
      expect(data[0]).to.include.keys('hostname', 'ip', 'mac', 'openPorts', 'osNmap');
      //console.log(data);
      done();
    });
    scan.startScan();
  });

  it('Full scan with 39999 tcp port added', function (done) {

    this.timeout(20000);
    var server = net.createServer((socket) => {
      socket.end('goodbye\n');
      socket.on('error',function(){
   	  });
    }).on('error', (err) => {
      // handle errors here
      throw err;
    });

    server.listen(39999, () => {
      //console.log('opened server on', server.address());
      var scan = new nmap.FullScan("127.0.0.1");
      scan.on('complete', function (data) {
        //console.log(data);
        var portInclude = false;
        expect(data).to.be.instanceOf(Array);
        expect(data).to.not.be.empty;
        expect(data[0]).to.include.keys('hostname', 'ip', 'mac', 'openPorts', 'osNmap');
    		_.forEach(data[0].openPorts, (v) => {
    		   if (v.port.portid == 39999) {
    		      portInclude = true;
    		   }
    		});
		assert.equal(portInclude, true, 'should include 39999 tcp port');
        server.close(() => {
          done();
        });
      });
      scan.startScan();
    });
  });

  //TODO: NmapScan should accept multi space
  it.skip('accepts 2+ space separated command', function (done) {

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
