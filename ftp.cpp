#include <iostream>
#include <sys/socket.h>

#include "include/ftp.hpp"
using namespace std;

ftp::ftp(){
    cout<<"创建ftp对象"<<endl;
}

ftp::~ftp(){
    cout<<"ftp对象销毁"<<endl;
}

bool ftp::ftp_connect(const string &s_ip,const int &c_port){
    cout<<"found ftp connect success."<<endl;
    return true;
}

bool ftp::ftp_disconnect(){
    cout<<"remove ftp connect success."<<endl;
    return true;
}
