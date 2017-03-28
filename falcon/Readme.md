使用须知
=======
  * 强烈建议用户以非root用户运行猎鹰程序,符合系统安全规定
  * 本帮助假定以运行猎鹰的用户账号为admin,先在系统中创建账号
```sh
     useradd -d /home/admin -m -c "security user" admin  #CentOS下创建用户
     passwd admin  #设置密码admin     
```
  * 猎鹰支持CentOS、Ubuntu、MacOS、树莓派

安装(以centos 7.2为例)
====================
  1. 以admin账号，获取源码包
```sh
     cd /home/admin
     git clone https://git.oschina.net/cxueqin/falcon.git
```
  2. 编译ddos工具,并安装
```sh
     cd /home/admin/falcon
     cd tfn2k
     make  #中间若有输入密码,则输入8个字母或数字，回车即可
     sudo cp td  tfn /usr/local/sbin
```
  3. 安装nmap工具
```sh
     sudo yum install nmap     
```
  4. 安装依赖库
```sh
     sudo yum install pam-devel   #若遗漏此步,安装authenticate-pam时会报头文件缺少错误
```
  5. 安装猎鹰主程序包
```sh
     cd /home/admin/falcon
     cd falcon      #注意本项目下还有一个falcon目录
     npm install    
```
其它平台注意：
     1. 对于Ubuntu 12.04而言，默认系统是gcc-4.7,需要升级到gcc-4.8以及g++-4.8环境, 否者nodejs环境使用了c++11的特性，会出现编译错误. 升级方法见
     http://askubuntu.com/questions/271388/how-to-install-gcc-4-8
     2. 对于Ubuntu环境，需要安装pam开发包，否则npm install编译authenticate-pam时会出现错误,安装命令
```sh
       sudo apt-get install libpam0g-dev
```
运行前准备
=========
  1. 先启动dos守护程序
```sh
     sudo /usr/local/sbin/td &
```
  2. 安装配置文件
```sh
     cd /home/admin/falcon
     cd falcon
     sudo cp system/falcon.conf  /etc/falcon.conf  #猎鹰遵守*Nix配置标准
```
  3. 执行测试，看核心模块是否工作正常(记住：测试驱动的思想)
```sh
     cd /home/admin/falcon
     cd falcon
     npm test     #大部分测试应该是通过的
```
  4. 创建猎鹰数据目录
```sh
     sudo mkdir -p /var/lib/falcon/nmap
     sudo chown -R admin:admin /var/lib/falcon    #设置目录权限为admin可读写
```

运行
======
  测试通过之后，我们就可在admin用户下把主程序运行起来了
```sh
  cd /home/admin/falcon
  cd falcon
  node index.js
```
  然后打开浏览器，访问 http://localhost:8000 就出现登陆页，输入你操作系统中存在的真实用户账号与密码,即可进入. 现在，开始你的网络安全之旅吧:)

启用https
==========
  猎鹰的WEB控制台默认使用http,若要开启https模式,请按照如下步骤
  1. 利用openssl工具制作服务器私钥和证书,system目录中有一个genkey.sh脚本可生成自签名证书,当然您也可自行生成
```sh
     cd system
     ./genkey.sh output localhost 12345678   # output为证书文件生成目录,localhost为证书主机,12345678为CA私钥密码

     #运行之后会在生成output目录，里面有cert目录，其中有ca,client,server,tmp目录,服务器证书就在server目录
```
  2. 将上步生成的服务器证书拷贝到/etc目录
```sh
      sudo cp output/cert/server/server.key /etc/falcon.keys
      sudo cp output/cert/server/server.crt /etc/falcon.crt
```
  3. 编辑配置文件，启用https
```sh
      sudo vim /etc/falcon.conf
      [daemon]
      port_ssl = 8001
      key = /etc/falcon.key
      cert = /etc/falcon.crt
      # https端口将侦听在8001端口，也可更改为其它端口
```
  4. 启动主程序
```sh
      node index.js
```
帮助
=======
  在码云上提交问题或者邮件给我 7219530@qq.com
