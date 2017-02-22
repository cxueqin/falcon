var assert = require('assert');
var FloodAgent = require('../../ddos/agent');
const AGENT_IP = "127.0.0.1";
const TARGET_IP = "127.0.0.1";

describe('FloodAgent Interface Static Test', function() {
  var agent;

  before(function() {
    // runs before all tests in this block
    agent = new FloodAgent(AGENT_IP);
  });

  after(function() {
    // runs after all tests in this block
  });

  it('agent instance property should be valid after successfully created', function() {
    assert.equal( AGENT_IP, agent.host );
    assert.equal( "idle", agent.state );
  });

  it('agent should start flood when call start api', function(done) {
    agent.onStart(done);
    assert.doesNotThrow(function () { agent.start(TARGET_IP, {type: 'icmp'}); });
  });

  it('agent static data should be correct when call processTraffic api', function() {
    agent.processTraffic({packet: 10, byte: 1000});
    var stat = agent.getTraffic();
    assert.equal(stat.packetSent, 10);
    assert.equal(stat.byteSent, 1000);
  });

  it('agent static data should be correct when twice processTraffic api call', function() {
    agent.processTraffic({packet: 1, byte: 100});
    var stat = agent.getTraffic();
    assert.equal(stat.packetSent, 11);
    assert.equal(stat.byteSent, 1100);
  });

  it('agent should stop flood when call stop api', function(done) {
    agent.onStop(done);
    assert.doesNotThrow(function () { agent.stop(); });
  });

  it('agent static data should reset when call resetTraffic api', function() {
    agent.resetTraffic();
    var stat = agent.getTraffic();
    assert.equal(stat.packetSent, 0);
    assert.equal(stat.byteSent, 0);
  });

});
