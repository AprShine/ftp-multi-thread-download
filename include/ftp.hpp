/**
 * @file sock.hpp
 * @author aprshine (you@domain.com)
 * @brief 建立ftp socket控制链接,实现控制消息传输,并且在ftp被动模式下协商数据传输端口
 * @version 0.1
 * @date 2022-12-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __FTP_HPP_
#define __FTP_HPP_
#include <string>
#define FTP_BUF_LEN 4096
using namespace std;
class ftp
{
private:
    /* data */
    int ftp_sockfd;//建立sock句柄
    bool is_connect;

public:
    ftp(/* args */);
    ~ftp();

    /**
     * @brief 创建ftp控制链接
     * 
     * @param s_ip 源ip地址
     * @param c_port 数据控制端口
     * @return true 创建成功
     * @return false 创建失败
     */
    bool ftp_connect(const char *s_ip,const int &sc_port);

    /**
     * @brief 断开ftp控制链接
     * 
     * @return true 断开成功
     * @return false 断开失败
     */
    bool ftp_disconnect();
    /**
     * @brief 发送控制流数据
     * 
     * @param send_buf 发送数据内容
     * @param buf_len 发送数据长度
     * @return true 成功
     * @return false 失败
     */
    bool ftp_send(const char *send_buf,const int &send_len);
    /**
     * @brief 接收控制流数据
     * 
     * @param recv_buf 接收数据缓存
     * @param buf_len 接收数据长度
     * @return true 成功
     * @return false 失败
     */
    bool ftp_recv(char *recv_buf,const int &buf_len);

};

#endif

