/**
 * @file main.cpp
 * @author aprshine (you@domain.com)
 * @brief 目标是与ftp服务器建立sock链接,并实现多线程下载,其他功能暂时不考虑
 * @version 0.1
 * @date 2022-12-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "include/ftp.hpp"
#include <iostream>
#include <getopt.h>
#include <fstream>
using namespace std;

const char *program_name;
const int port=21;

void output_info(ostream & os,int exit_code){
    os<<"Usage:"<<program_name<<" options [filename]"<<endl;
	os<<"-h --help:Display this usage information."<<endl;
    os<<"-s --src:the ftp server's address"<<endl;
    os<<"-u --username:the ftp server's username"<<endl;
	os<<"-p --password:the ftp server's password"<<endl;
	os<<"-o --output:the save file's name <default filename>"<<endl;
	exit(exit_code);
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
    //参数指定用户名
    const char* server_name=NULL;
    //参数指定密码
    const char* server_pwd=NULL;
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
    ftp f=ftp();
    f.ftp_connect(server_addr,port);
    f.ftp_disconnect();
    return 0;
}
