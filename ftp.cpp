#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>

#include "include/ftp.hpp"
using namespace std;

ftp::ftp(){
    cout<<"创建ftp对象"<<endl;
    ftp_sockfd=socket(AF_INET,SOCK_STREAM,0);
}

ftp::~ftp(){
    if(is_connect) ftp_disconnect();
    cout<<"销毁ftp对象"<<endl;
}

bool ftp::ftp_connect(const char *s_ip, const int &sc_port){
    if(ftp_sockfd==-1){
        cout<<"error:seem to init socket error, retrying..."<<endl;
        ftp_sockfd=socket(AF_INET,SOCK_STREAM,0);
        if(ftp_sockfd==-1){
            cout<<"retry failed!"<<endl;
            return false;
        }
    }

    sockaddr_in addr_info;
    memset(&addr_info,0,sizeof(sockaddr_in));
    addr_info.sin_family=AF_INET;
    addr_info.sin_addr.s_addr=inet_addr(s_ip);
    addr_info.sin_port=htons(sc_port);
    
    if(connect(ftp_sockfd,(sockaddr *)(&addr_info),sizeof(sockaddr_in))==-1){//异常(超时处理)
        timeval timeout={
            .tv_sec=10,
            .tv_usec=0
        };//轮询,设置超时时间
        fd_set read_set;
        int error;
        FD_ZERO(&read_set);
        FD_SET(ftp_sockfd,&read_set);
        if(select(ftp_sockfd+1,&read_set,NULL,NULL,&timeout)>0){
            getsockopt(ftp_sockfd,SOL_SOCKET,SO_ERROR,&error,NULL);
            if(error==0){
                is_connect=true;
                return true;
            }
        }
        cout<<"error:server timeout, please check you ip address and Internet."<<endl;
        is_connect=false;
        return false;
    }
    is_connect=true;
    cout<<"ftp connect create success."<<endl;
    return true;
}

bool ftp::ftp_disconnect(){
    if(is_connect){
        close(ftp_sockfd);
        cout<<"ftp connect close success."<<endl;
        is_connect=false;
        return true;
    }
    return false;
}

bool ftp::ftp_send(const char *send_buf,const int &send_len){
    if(ftp_sockfd==-1){//套接字未初始化
        cout<<"error:socket not init."<<endl;
        return false;
    }
    timeval timeout={
        .tv_sec=1,
        .tv_usec=0
    };
    
    fd_set write_set;
    FD_ZERO(&write_set);
    FD_SET(ftp_sockfd,&write_set);

    if(select(ftp_sockfd+1,NULL,&write_set,NULL,&timeout)>0){
        int already_send=0;
        int cnt_send=0;
        while(already_send!=send_len){
            cnt_send=send(ftp_sockfd,send_buf,send_len,0);//等价于write
            if(cnt_send=-1){
                cout<<"error:send msg error."<<endl;
                return false;
            }
            already_send+=cnt_send;
        }
        return true;
    }
    //超时了
        return false;
}