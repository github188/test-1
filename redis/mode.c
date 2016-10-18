#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <hiredis/hiredis.h>

int main(int argc, char** argv)
{
    // 连接
    redisContext* c = redisConnect("127.0.0.1", 6379);
    if ( c->err ){
        redisFree(c);
        printf("Connect to redisServer faile!\n");
        return -1;
    }
    printf("Connect to redisServer Success!\n");


    const char* command1 = "set stest1 value1";
    
	// 执行命令 获得回复句柄
	redisReply* r = (redisReply*)redisCommand(c, command1);

	// 判断回复 是否为空
    if( NULL == r){
        printf("Execut command1 failure\n");
        redisFree(c);
        return -1;
    }

	// 判断执行状态
    if( !(r->type == REDIS_REPLY_STATUS && strcasecmp(r->str,"OK")==0)){
        printf("Failed to execute command[%s]\n",command1);
        freeReplyObject(r);
        redisFree(c);
        return -1;
    }

	// 清空回复句柄
    freeReplyObject(r);
    printf("Succeed to execute command[%s]\n", command1);

	// 执行命令2
    const char* command2 = "strlen stest1";
    r = (redisReply*)redisCommand(c, command2);
    if ( r->type != REDIS_REPLY_INTEGER){
        printf("Failed to execute command[%s]\n",command2);
        freeReplyObject(r);
        redisFree(c);
        return -1;
    }

    int length =  r->integer;
    freeReplyObject(r);
    printf("The length of 'stest1' is %d.\n", length);
    printf("Succeed to execute command[%s]\n", command2);


    const char* command3 = "get stest1";
    r = (redisReply*)redisCommand(c, command3);
    if ( r->type != REDIS_REPLY_STRING)
    {
        printf("Failed to execute command[%s]\n",command3);
        freeReplyObject(r);
        redisFree(c);
        return -1;
    }
    printf("The value of 'stest1' is %s\n", r->str);
    freeReplyObject(r);
    printf("Succeed to execute command[%s]\n", command3);

    const char* command4 = "get stest2";
    r = (redisReply*)redisCommand(c, command4);
    if ( r->type != REDIS_REPLY_NIL)
    {
        printf("Failed to execute command[%s]\n",command4);
        freeReplyObject(r);
        redisFree(c);
        return -1;
    }
    printf("The value of 'stest2' is %s\n", r->str);
    freeReplyObject(r);
    printf("Succeed to execute command[%s]\n", command4);


    redisFree(c);

	return 0;
}
