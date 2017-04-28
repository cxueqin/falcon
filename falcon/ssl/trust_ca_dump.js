'use strict';

var fs = require('fs');

var ca_list = new Array();

function readLines(input, func) {
  var remaining = '';

  input.on('data', function(data) {
    remaining += data;
    var index = remaining.indexOf('\n');
    while (index > -1) {
      var line = remaining.substring(0, index);
      remaining = remaining.substring(index + 1);
      func(line);
      index = remaining.indexOf('\n');
    }
  });

  input.on('end', function() {
    if (remaining.length > 0) {
      func(remaining);
    }
    ca_list.sort();

    console.log(JSON.stringify(ca_list,null, '\t'));
  });
}

function func(data) {

  var rePattern = new RegExp(/^# Issuer: CN=([^,]*),(.*)$/);

  //var arrMatches = data.match(/^# Issuer: CN=(.*)OU=(.*)C=(.*)$/);
  var arrMatches = data.match(rePattern);
  if (arrMatches) {
    //console.log('ALL:' + arrMatches[0]);
    //console.log('CN:' + arrMatches[1]);

    ca_list.push(arrMatches[1]);
  }
  //console.log('Line :', data);

}

var input = fs.createReadStream('trust_ca.txt');
readLines(input, func);
