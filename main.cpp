/**
 * @file main.cpp
 * @author aprshine (you@domain.com)
 * @brief 目标是与ftp服务器建立sock链接,并实现多线程下载(关键在于是否支持续传),其他功能暂时不考虑
 * @version 0.1
 * @date 2022-12-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "include/ftp.hpp"
#include "include/m_thread.hpp"
#include <iostream>
#include <fstream>
#include <string.h>
#include <getopt.h>

using namespace std;

const char *program_name;
const int sc_port=21;

void output_info(ostream & os,int exit_code){
    os<<"Usage:"<<program_name<<" options [filename]"<<endl;
	os<<"-h --help:Display this usage information."<<endl;
    os<<"-s --src:the ftp server's address"<<endl;
    os<<"-u --username:the ftp server's username"<<endl;
	os<<"-p --password:the ftp server's password"<<endl;
	os<<"-o --output:the save file's name <default filename>"<<endl;
	exit(exit_code);
}

bool get_file(const char* server_ip,const int &port,const char *username,const char *pwd,const char *remote_fname,const char *local_fname){
    
    //接收信息的缓存
    char recv_buf[FTP_BUF_LEN]={0};
    //FTP 控制指令
    string ftp_cmd;
    //ftp 控制TCP链接
    ftp control_link=ftp();

    if(!control_link.ftp_connect(server_ip,port)){
        //链接sock失败
        cout<<"error:can't connect ftp server."<<endl;
        return false;
    }
    if(control_link.ftp_recv(recv_buf,FTP_BUF_LEN)>0){
        cout<<"get the server msg:"<<recv_buf;
    }else{
        cout<<"error:can't recv server login msg."<<endl;
        return false;
    }
    //传输用户名
    ftp_cmd="USER ";
    ftp_cmd.append(username).append("\n");
    memset(recv_buf,0,FTP_BUF_LEN);
    if(!control_link.ftp_talk(ftp_cmd.c_str(),ftp_cmd.length(),recv_buf,FTP_BUF_LEN)){
        cout<<"error:send username "<<username<<" failed:"<<recv_buf;
        return false;
    }
    if(username=="ftp"||username=="anonymous"){
        cout<<"-----------------using anonymous model.------------------"<<endl;
    }
    //传输密码
    ftp_cmd="PASS ";
    ftp_cmd.append(pwd).append("\n");
    memset(recv_buf,0,FTP_BUF_LEN);
    if(!control_link.ftp_talk(ftp_cmd.c_str(),ftp_cmd.length(),recv_buf,FTP_BUF_LEN)){
        cout<<"error:login failed:"<<recv_buf;
        return false;
    }
    //测试是否支持续传
    ftp_cmd="REST 100\n";
    memset(recv_buf,0,FTP_BUF_LEN);
    if(!control_link.ftp_talk(ftp_cmd.c_str(),ftp_cmd.length(),recv_buf,FTP_BUF_LEN)){
        cout<<"server can't use multi-thread download."<<endl;
    }else{
        cout<<"----------server can use multi-thread download.----------"<<endl;
    }
    //使用被动模式,并从服务器发送来的报文中获取数据传输的sock
    ftp_cmd="PASV\n";
    memset(recv_buf,0,FTP_BUF_LEN);
    if(!control_link.ftp_talk(ftp_cmd.c_str(),ftp_cmd.length(),recv_buf,FTP_BUF_LEN)){
        return false;
    }

    return true;
}

int main(int argc, char *argv[]){
    //全部段选项的合并字符，":"表示带有附加参数
	const char* const short_opts = "hs:u:p:o:";
	//长选项数组，定义在前面注释已经标明
	const struct option long_opts[] =
	{
		{"help",0,NULL,'h'},
        {"src",1,NULL,'s'},
        {"username",1,NULL,'u'},
        {"password",1,NULL,'p'},
		{"output",1,NULL,'o'},
		{NULL,0,NULL,0}
	};
	//参数指定的输出文件名
	const char* output_filename = NULL;
    //参数指定服务器ip地址或DNS地址
    const char* server_addr=NULL;
    //参数指定用户名,如果没写的话默认是匿名模式
    const char* server_name="ftp";
    //参数指定密码
    const char* server_pwd="";
    //参数指定ftp对应文件名
    const char* filename=NULL;
	//保存程序名
	program_name = argv[0];
	//如果为长选项，第五个参数输出该选项在长选项数组中的索引
	int opt = getopt_long(argc,argv,short_opts,long_opts,NULL);
	//使用选项的返回值单独处理 返回值为获取到的参数,附加参数由optarg提供
	//使用循环处理所有的参数
    while(opt != -1)
	{
		switch(opt){
		case 'h'://-h 或 --help
			output_info(cout,0);
		case 'o'://-o 或 --output，附加参数由optarg提供
			output_filename = optarg; 
            break;
        case 's'://-s 服务器源地址
            server_addr = optarg;
            break;
        case 'u'://-u 服务器用户名
            server_name=optarg;
            break;
        case 'p'://-p ftp服务器密码
            server_pwd = optarg;
            break;
		case '?'://用户输入了无效参数
			output_info(cerr,1);
		default://未知错误
			abort();
		}
		opt = getopt_long(argc,argv,short_opts,long_opts,NULL);
	}
    if(optind<argc) filename=argv[optind];
    //未检测到有用参数
    if(server_addr==NULL||filename==NULL){
        cout<<"\nseem can't get hostname or filename\n\n";
        output_info(cout,1);
    }
    //获取到相应的文件,并保存在当前路径中
    get_file(server_addr,sc_port,server_name,server_pwd,filename,output_filename);
    return 0;
}
