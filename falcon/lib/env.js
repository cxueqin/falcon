/*!
 * 运行环境检查模块,目前限定linux和darwin平台
 *
 * 版权所有 2017 陈学芹 联系方式: 7219530@qq.com
 *
 * 本软件在GNU LGPL-V2.1协议下发布，欢迎使用.
 */

function validate() {
  const os = require('os');

  var platform = os.platform();

  if (platform != 'linux' && platform != 'darwin') {
    throw new Error(`sorry, no support for ${platform} platform`)
  }

  if (process.geteuid) {
    var euid = process.geteuid()
    if (euid == '0') {
      throw new Error(`sorry, falcon should not run under root`)
    }
  }

  //TODO: 检查nodejs的版本，太低则拒绝运行
  //TODO: 检查nmap是否可用,dos测试命令是否可用，用程序做到必要检查,让用户可自行发现问题
}

exports.validate = validate;
