# ftp-multi-thread-download
c++编写的linux端ftp多线程下载器
## 使用方法

```
Usage:./my-wget options [filename]
-h --help:Display this usage information.
-s --src:the ftp server's address
-u --username:the ftp server's username
-p --password:the ftp server's password
-o --output:the save file's name <default filename>
```
如果未填写username则默认使用匿名模式(需要服务器支持)

+ 测试语句

```sh
# 单线程
./my-wget -s 124.221.244.253 pub/test_genshin.jpg
# 多线程
./my-wget -s 124.221.244.253 -t 3 pub/test_genshin.jpg
```