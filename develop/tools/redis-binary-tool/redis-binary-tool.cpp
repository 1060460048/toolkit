#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include "hiredis-master/hiredis.h"

void Usage(const char * proname)
{
    printf("Usage(用法) \n  %s "
            "[-t] host "
            "[-p] port "
            "[-n] db number "
            "-e command to execute "
            "[-s] separator "
            "[-h] "
            "\n\n",proname);
    printf( "  -t redis host(redis ip地址) \n"
            "  -p redis port(redis 端口号) \n"
            "  -n db number(redis 库号) \n"
            "  -e command to execute(要执行的redis命令) \n"
            "  -s separator(结果显示时的分隔符) \n"
            "  -h show help(显示帮助信息) \n"
            "\n");
}

void GetOptions(int argc, char * argv[], std::string & t, int & p, int & n, std::string & e, std::string & s)
{
    int opt;
    int cnt_arg =0;
    opterr =0;
    
    // 没有冒号表示不带参数
    // 一个冒号表示带参数
    // 两个冒号表示参数可带可不带
    // 必需的选项通过自己的计数器(cnt_arg)来确认，必需选项数量达不到规定的必需选项数则直接退出.
    while( (opt=getopt(argc,argv,"t:p:n:e:s:h"))!=-1 )
    {
        switch(opt)
        {
        case 't':
            t =optarg;
            break;
        case 'p':
            p =atoi(optarg);
            break;
        case 'n':
            n =atoi(optarg);
            break;
        case 'e':
            ++cnt_arg;
            e =optarg;
            break;
        case 'h':
            Usage(argv[0]);
            exit(0);
            break;
        case 's':
            s =optarg;
            break;
        default:
            Usage(argv[0]);
            exit(-1);
            break;
        }
    }
    if(cnt_arg<1)
    {
        Usage(argv[0]);
        exit(-1);
    }
}

redisContext * rbtRedisConnect(const char * host, int port)
{
    struct timeval timeout = { 2, 500000 }; // 2.5 seconds
    redisContext * c = redisConnectWithTimeout(host, port, timeout);
    if (c == NULL || c->err) {
        if (c) {
            printf("Connection error: %s\n", c->errstr);
            redisFree(c);
        } else {
            printf("Connection error: can't allocate redis context\n");
        }
        exit(1);
    }
    return c;
}


void rbtRedisSelectDB(redisContext *c, int dbnumber)
{
    std::string com(64,'\0');
    snprintf(&com[0],com.size(),"select %d",dbnumber);
    com.resize(strlen(com.c_str()));
    redisReply * reply = (redisReply*)redisCommand(c,com.c_str());
    freeReplyObject(reply);
}

redisReply * rbtRedisExeCmd(redisContext *c, const char * cmd, size_t cmdsize)
{
    redisReply *reply;
    reply = (redisReply*)redisCommand(c,cmd);
    printf("cmd{%2d} :\n<%s>\n", int(cmdsize), cmd);
    return reply;
}

void PrintResult(redisReply * reply, const std::string & separator)
{
    // 直接打印结果
    printf("\nresult string{%2d} :\n<%s>\n", int(reply->len), reply->str);
    
    // 打印16进制结果
    printf("\nresult hex{%3d} :\n<", int(reply->len));
    for(int i=0;i<reply->len;i++)
    {
        printf("0x%02x", (reply->str)[i]&0xff);
        if(i!=reply->len-1)
            printf("%s",separator.c_str());
    }
    printf(">\n");    
    
    // 打印有符号10进制结果
    printf("\nresult signed oct{%3d} :\n<", int(reply->len));
    for(int i=0;i<reply->len;i++)
    {
        printf("%d", (reply->str)[i]&0xff);
        if(i!=reply->len-1)
            printf("%s",separator.c_str());
    }
    printf(">\n\n");    
    
    // 打印无符号10进制结果
    printf("\nresult unsigned oct{%3d} :\n<", int(reply->len));
    for(int i=0;i<reply->len;i++)
    {
        printf("%u", unsigned((reply->str)[i]&0xff) );
        if(i!=reply->len-1)
            printf("%s",separator.c_str());
    }
    printf(">\n\n");  
}

int main(int argc, char * argv[])
{
    std::string host ="127.0.0.1";
    int port =6379;
    int dbnumber =0;
    std::string cmd;
    std::string separator =",";
    GetOptions(argc, argv, host, port, dbnumber, cmd, separator);
    
    
    redisContext *c;

    // 连接
    c =rbtRedisConnect(host.c_str(), port);

    // 选库
    if(dbnumber>0)
        rbtRedisSelectDB(c, dbnumber);
    
    // 执行命令
    redisReply * reply =rbtRedisExeCmd(c, cmd.c_str(), cmd.size());
    PrintResult(reply, separator);
    freeReplyObject(reply);
    
    redisFree(c);
    return 0;
}
