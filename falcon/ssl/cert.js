var tls = require('tls');

function isEmpty(object) {
  for(var prop in object) {
    if(object.hasOwnProperty(prop))
      return false;
  }

  return true;
}

function get(host, port) {
  if (!host || typeof host !== 'string') {
    throw Error("A valid host is required");
  }
  var myport = 443;
  if (port) {
    myport = port;
  }

  var options = {
    host: host,
    port: myport,
    rejectUnauthorized: false,
    ciphers: "ALL",
  };

  return new Promise(function (resolve, reject) {
    var socket = tls.connect(options, () => {
      var certificate = socket.getPeerCertificate(true);
      if (isEmpty(certificate) || certificate === null) {
        reject(new Error('The server did not provide a certificate'));
      } else {
        resolve(certificate);
      }
      /* 立即关闭socket */
      socket.end();
    });

    socket.on('error', function(e) {
      reject(e);
      socket.end();
    });
  });
}

module.exports = {
  get: get,
};
