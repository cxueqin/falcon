var assert = require('assert');
var floodController = require('../../ddos/controller');
var FloodAgent = require('../../ddos/agent');
var FloodListener = require('../../ddos/listener');
var sleep = require('sleep').sleep;

const AGENT_IP = "127.0.0.1";
const TARGET_IP = "127.0.0.1";

describe('FloodController Interface Static Test', function() {
  var controller;

  before(function() {
    // runs before all tests in this block
    assert.ok(floodController);
    assert.ok(floodController.init);
    assert.ok(floodController.addAgent);
    assert.ok(floodController.removeAgent);
    assert.ok(floodController.getAgents);
    assert.ok(floodController.findAgent);
    assert.ok(floodController.startFlood);
    assert.ok(floodController.stopFlood);
    assert.ok(floodController.getState);
    assert.ok(floodController.getTrafficStat);
  });

  after(function() {
    // runs after all tests in this block
  });

  it('controller should set correct internal data  after init ', function() {
    floodController.init();
    var listener = new FloodListener( 8000, floodController);
    listener.start();

    var state = floodController.getState();
    assert.equal('idle', state.state);
    var agents = floodController.getAgents();
    assert.equal(0, agents.length);

    var data = floodController.getState();

  });

  it(' agent list should have more elements when addAgent invoked', function() {
    floodController.addAgent(new FloodAgent(AGENT_IP));
    var agents = floodController.getAgents();
    assert.equal(1, agents.length);
  });

  it(' controller should find agent when given ip', function() {
    var selector = { host: AGENT_IP};
    var agent = floodController.findAgent(selector);
    assert.ok(agent);
  });


  it(' agent should be in started state when startFlood ', function() {
    assert.doesNotThrow(
      () => {
        floodController.startFlood(TARGET_IP, {type: 'icmp'})
      }
    );
    var state = floodController.getState();
    assert.equal( 'started', state.state);
  });

  it(' controller should reject  state when already startFlood ', function() {
    assert.throws(
      () => {
        floodController.startFlood(TARGET_IP, {type: 'icmp'});
      }
    );
  });

  it(' traffic stat should change  when get every 2 secnonds ', function(done) {
    setTimeout( () => {
      var trafficInfo = floodController.getTrafficStat();
      console.log("trafficInfo %o", trafficInfo);
      assert.ok(trafficInfo.packetSent>1000);
      assert.ok(trafficInfo.byteSent>1000);
      done();
    }, 1000);
  });

  it(' agent should be in idle state when stopFood', function() {
    assert.doesNotThrow(
      () => {
        floodController.stopFlood();
      }
    );
  });

  it(' controller should reject stopFlood when flood not started ', function() {
    assert.throws(
      () => {
        floodController.stopFlood();
      }
    );
  });

  it(' agent list should remove specific element when removeAgent invoked', function() {
    var cond = { host: AGENT_IP};
    var agent = floodController.findAgent(cond);
    assert.ok(agent);
    var ret = floodController.removeAgent(agent);
    assert.equal(0, ret);
  });
});
