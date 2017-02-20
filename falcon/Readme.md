
安装(以centos 7.2为例)
====================
  1. 获取源码包
     git clone https://git.oschina.net/cxueqin/falcon.git

  2. 编译ddos工具,并安装
     cd /path/to/falcon
     cd tfn2k
     make  #中间若有输入密码,则输入8个字母或数字，回车即可
     sudo cp td  tfn /usr/local/sbin

  3. 安装nmap工具
     sudo yum install nmap

  4. 在猎鹰主程序包
     cd /path/to/falcon
     cd falcon      #注意本项目下还有一个falcon目录
     npm install

其它平台注意：
     对于Ubuntu 12.04而言，注意需要安装gcc-4.8以及g++-4.8环境,
     否者nodejs环境使用了c++11的特性，会出现编译错误.

运行前准备
=========
  1. 先启动dos守护程序
     sudo /usr/local/sbin/td &
  2. 执行测试，看核心模块是否工作正常(记住：测试驱动的思想)
     cd /path/to/falcon
     cd falcon
     npm test     #大部分测试应该是通过的

运行
======
  测试通过之后，我们就可把主程序运行期来了
  cd /path/to/falcon
  cd falcon
  node index.js

  然后打开浏览器，访问 http://localhost:8000 就出现登陆页，输入你操作系统中存在的真实用户账号与密码,即可进入. 现在，开始你的网络安全之旅吧:)

帮助
=======
  在码云上提交问题或者邮件给我 7219530@qq.com
