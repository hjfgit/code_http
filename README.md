# code_http
一、简要介绍
1. 本项目展示内容为校园美景，给大学生活做一个记录；
2. 基于TCP协议实现HTTP多线程服务器，可以显示静态网页；
3. 使用多线程，使用脚本自动化生成；
4. 支持HTTP协议的GET和POST请求；
5. 支持CGI；
二、如何使用
1、git 到本地之后，进入code_http文件夹后，使用make进行编译
          cd code_http
          make
2、生成一个Http_server 的可执行程序，可直接绑定ip地址进行使用，例如使用20000端口
          ./Http_server 20000

