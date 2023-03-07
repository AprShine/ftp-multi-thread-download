## ftp-multi-thread-download
### 项目介绍
一个c++编写的基于FTP协议的多线程下载器，使用shell形式运行，可以对支持续传的FTP文件进行多线程下载

### 主要原理

使用续传方式从文件的不同index进行下载，同时下载多个文件后，对该文件各个部分按编号进行合并，并删除temp文件。

### 安装教程

项目需要gcc编译器以及linux开发环境，可以使用WSL等等

```sh
# 使用make命令进行编译即可，可以自动生成可运行文件
make
```

### 使用方法

```sh
#可以使用 -h/--help查看程序帮助
my-wget -h
my-wget --help
```

程序帮助如下：

```
Usage:./my-wget options [filename]
-h --help:Display this usage information.
-s --src:the ftp server's address
-u --username:the ftp server's username
-p --password:the ftp server's password
-o --output:the save file's name <default filename>
-t --thread:the download thread num
```
如果未填写username则默认使用匿名模式(需要服务器支持)

+ 测试语句

```sh
# 单线程
./my-wget -s [FTP Server IP] pub/test_genshin.jpg
# 多线程
./my-wget -s [FTP Server IP] -t 3 pub/test_genshin.jpg
```

*tips：现在FTP协议Inet上没什么人用了，建议自己搭个FTP服务器试，开源的FTP服务器实现有很多。*

### 最后

该程序是某个课程作业写的小玩意，仅供参考，如果感到有帮助的话点个star喵~