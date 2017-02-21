使用须知
=======
  * 强烈建议用户以非root用户运行猎鹰程序,符合系统安全规定
  * 本帮助假定以运行猎鹰的用户账号为admin,先在系统中创建账号
     useradd -d /home/admin -m -c "security user" admin  #CentOS下创建用户
     passwd admin  #设置密码admin
  * 猎鹰支持CentOS、Ubuntu、MacOS

安装(以centos 7.2为例)
====================
  1. 以admin账号，获取源码包
     cd /home/admin
     git clone https://git.oschina.net/cxueqin/falcon.git

  2. 编译ddos工具,并安装
     cd /home/admin/falcon
     cd tfn2k
     make  #中间若有输入密码,则输入8个字母或数字，回车即可
     sudo cp td  tfn /usr/local/sbin

  3. 安装nmap工具
     sudo yum install nmap

  4. 安装依赖库
     sudo yum install pam-devel   #若遗漏此步,安装authenticate-pam时会报头文件缺少错误

  5. 安装猎鹰主程序包
     cd /home/admin/falcon
     cd falcon      #注意本项目下还有一个falcon目录
     npm install    

其它平台注意：
     对于Ubuntu 12.04而言，注意需要安装gcc-4.8以及g++-4.8环境,
     否者nodejs环境使用了c++11的特性，会出现编译错误.

运行前准备
=========
  1. 先启动dos守护程序
     sudo /usr/local/sbin/td &

  2. 安装配置文件
     cd /home/admin/falcon
     cd falcon
     sudo cp system/falcon.conf  /etc/falcon.conf  #猎鹰遵守*Nix配置标准

  3. 执行测试，看核心模块是否工作正常(记住：测试驱动的思想)
     cd /home/admin/falcon
     cd falcon
     npm test     #大部分测试应该是通过的

  4. 创建猎鹰数据目录
     sudo mkdir -p /var/lib/falcon/nmap
     sudo chown -r admin:admin /var/lib/falcon    #设置目录权限为admin可读写

运行
======
  测试通过之后，我们就可在admin用户下把主程序运行起来了
  cd /home/admin/falcon
  cd falcon
  node index.js

  然后打开浏览器，访问 http://localhost:8000 就出现登陆页，输入你操作系统中存在的真实用户账号与密码,即可进入. 现在，开始你的网络安全之旅吧:)

帮助
=======
  在码云上提交问题或者邮件给我 7219530@qq.com
