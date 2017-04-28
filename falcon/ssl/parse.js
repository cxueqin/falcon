var cmd_args = process.argv.splice(2);

const x509 = require('x509');
var cert = x509.parseCert(cmd_args[0]);
console.dir(cert);
