var tls = require('tls');

/* 修正测试结果中的环形指针,避免无限循环 */
function fixCyclic (obj) {
  var seenObjects = [];

  function detect (obj) {
    if (obj && typeof obj === 'object') {
      if (seenObjects.indexOf(obj) !== -1) {
        return true;
      }
      seenObjects.push(obj);
      for (var key in obj) {
        if (obj.hasOwnProperty(key) && detect(obj[key])) {
          return true;
        }
      }
    }
    return false;
  }

  seenObjects.push(obj);
  for (var key in obj) {
    if (obj.hasOwnProperty(key) && detect(obj[key])) {
      /*
        key属性指向已有的一个值, 判断发生短路, 需要断开此环路
        注意使用delete,而非将属性设为 undefined
       */
      delete obj[key];
    }
  }
}


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
        certificate.cipher = socket.getCipher();
        fixCyclic(certificate);
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

/* 证书检查回调版本 */
function connect(host, port, callback) {
  if (!host || typeof host !== 'string') {
    throw Error("A valid host is required");
  }

  var myport = 443;
  if (port) {
    myport = port;
  }

  if (typeof callback !== 'function') {
    throw Error("Callback functioin is required");
  }

  var options = {
    host: host,
    port: myport,
    rejectUnauthorized: false,
    ciphers: "ALL",
  };

  var socket = tls.connect(options, () => {
    var certificate = socket.getPeerCertificate(true);
    if (isEmpty(certificate) || certificate === null) {
      callback(new Error('The server did not provide a certificate'));
    } else {
      /*mixin Cipher property into certificate*/
      certificate.cipher = socket.getCipher();
      fixCyclic(certificate);
      callback(null, certificate);
    }
    /* 立即关闭socket */
    socket.end();
  });

  socket.on('error', function(e) {
    callback(e);
    socket.end();
  });

}

module.exports = {
  get: get,
  connect: connect
};
