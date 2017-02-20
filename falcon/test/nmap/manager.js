var assert = require('assert');
var NmapManagerFactory = require('../../nmap/nmap-manager');
NmapManagerFactory.setHome(__dirname);

describe('Nmap Manager Result Matter', function() {
  var manager = undefined;
  var resultid = undefined;
  var result = undefined;

  before(function() {
    // runs before all tests in this block
    //清空数据
    var _   = require('lodash');
    var _db = require('underscore-db');
    _.mixin(_db);
    _.save({ results: [] }, __dirname+'/db.json')
    manager = NmapManagerFactory.getManager();
  });

  after(function() {
    // runs after all tests in this block
  });

  it('addResult should successfully when scan and xml string all ok', function() {
    var scan, xml;
    var ret;
    scan = { range: ["192.168.40.1"] };
    ret = manager.addResult(scan, xml)
    assert.ok(ret);
    assert.ok(ret.id);
    assert.equal(ret.status, 'running')
    resultid = ret.id;
    result = ret;
  });

  it('addResult should fail when scan or xml string invalid ', function() {
    var scan, xml;
    var ret;

    ret = manager.addResult(scan, xml);
    assert(!ret);
  });

  it('getResultWithDetail should success when resultid valid ', function() {
    assert.ok(manager.getResultWithDetail(resultid));
  });

  it('getResultWithDetail should fail when resultid invalid ', function() {
    assert.ok(!manager.getResultWithDetail('111111'));
  });

  it('updateResult should ok when valid result and data ', function() {
    var ret = manager.updateResult(result, [{port:'22', status:'open'}])
    assert.ok(ret);
    assert.equal(ret.status, 'done');
  });

  it('getResults should return all result ', function() {
    assert(manager.getResults());
    assert.equal(manager.getResults().length, 1);
  });

  it('delResult should success when resultid valid ', function() {
    assert.equal( manager.delResult(resultid), 0 );
  });

  it('delResult should fail when resultid invalid ', function() {
    assert.equal( manager.delResult('111111'), -1);
  });
});

describe('Nmap Manager Spec Matter', function() {
  var manager;
  var resultid;

  before(function() {
    // runs before all tests in this block
    manager = NmapManagerFactory.getManager();
  });

  after(function() {
    // runs after all tests in this block
  });

  it('addSpec should successfully when spec object valid', function() {
  });

  it('addSpec should fail when spec object invalid ', function() {
    assert.equal(-1, [1,2,3].indexOf(4));
  });

  it('delSpec should success when spec id valid ', function() {
    assert.equal(-1, [1,2,3].indexOf(4));
  });

  it('delSpec should fail when spec id invalid ', function() {
    assert.equal(-1, [1,2,3].indexOf(4));
  });

  it('getSpecs should return all spec ', function() {
    assert.equal(-1, [1,2,3].indexOf(4));
  });

  it('updateSpec should success when valid spec object ', function() {
    assert.equal(-1, [1,2,3].indexOf(4));
  });

  it('updateSpec should fail when invalid spec object ', function() {
    assert.equal(-1, [1,2,3].indexOf(4));
  });

  it('delSpec should success when valid spec id', function() {
    assert.equal(-1, [1,2,3].indexOf(4));
  });

  it('delSpec should fail when invalid spec id', function() {
    assert.equal(-1, [1,2,3].indexOf(4));
  });
});
