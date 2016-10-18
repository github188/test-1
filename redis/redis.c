#include "cJSON.h"
#include "base64.h"

unsigned int getTime()
{
    time_t tm = time(0);
    struct tm *st = localtime(&tm);
    unsigned int t = (st->tm_mon + 1) * 100000000 + st->tm_mday * 1000000
        + st->tm_hour * 10000 + st->tm_min * 100 + st->tm_sec;
    return t;
}

int main(int argc, char** argv)
{
	int insert_num = 10000;
	if(argc == 2){
		insert_num = atoi(argv[1]);
	}
	printf("insert num is %d\n", insert_num);
	
	char json[10000] = {0};
	snprintf(json, 9999, "{\"SpNumber\":\"123456\",\"ChargeNumber\":\"000000000000000000000\",\"UserCount\":1,\"UserNumber\":\"8613869564585\",\"CorpId\":\"123456\",\"ServiceType\":\"123456\",\"FeeType\":1,\"FeeValue\":\"0\",\"GivenValue\":\"0\",\"AgentFlag\":0,\"MorelatetoMTFlag\":2,\"Priority\":0,\"ExpireTime\":\"161015235959032\",\"ScheduleTime\":\"\",\"ReportFlag\":1,\"TpPid\":1,\"Udhi\":1,\"MessageCoding\":15,\"MessageType\":0,\"MsgLen\":161,\"MsgContent\":\"你好猴子!\", \"LinkId\":\"12345678\"}");

	size_t base64_json_len;
	char* base64_json = base64_encode(json, strlen(json), &base64_json_len);

	printf("\n\n%lu -- %s\n\nto base64 i\n\n%lu -- %s\n\n", strlen(json), json, base64_json_len, base64_json);

	char comm[10000] = {0};
	snprintf(comm, 9999, "LPUSH to_send_list_1 %s", base64_json);

	printf("\n%s\n\n", comm);
	
	// 连接
    redisContext* c = redisConnect("127.0.0.1", 6379);
    if ( c->err ){
        printf("Connect to redisServer faile!\n");
        goto CONNECTED_ERROR;
    }
    printf("Connect to redisServer Success!\n");
	

#if 0
	char *result = "eyJTcE51bWJlciI6IjEyMzQ1NiIsIkNoYXJnZU51bWJlciI6IjAwMDAwMDAwMDAwMDAwMDAwMDAwMCIsIlVzZXJDb3VudCI6MSwiVXNlck51bWJlciI6Ijg2MTM4Njk1NjQ1ODUiLCJDb3JwSWQiOiIxMjM0NTYiLCJTZXJ2aWNlVHlwZSI6IjEyMzQ1NiIsIkZlZVR5cGUiOjEsIkZlZVZhbHVlIjoiMCIsIkdpdmVuVmFsdWUiOiIwIiwiQWdlbnRGbGFnIjowLCJNb3JlbGF0ZXRvTVRGbGFnIjoyLCJQcmlvcml0eSI6MCwiRXhwaXJlVGltZSI6IjE2MTAxNTIzNTk1OTAzMiIsIlNjaGVkdWxlVGltZSI6IiIsIlJlcG9ydEZsYWciOjEsIlRwUGlkIjoxLCJVZGhpIjoxLCJNZXNzYWdlQ29kaW5nIjoxNSwiTWVzc2FnZVR5cGUiOjAsIk1zZ0xlbiI6MTYxLCJNc2dDb250ZW50Ijoi5L2g5aW954y05a2QISJ9";

	size_t base64_temp2_len;
	char* base64_temp2 = base64_decode(result, strlen(result), &base64_temp2_len);
	printf("base64_temp2 = %s, base64_temp2_len = %lu\n", base64_temp2, base64_temp2_len);


#else
	int i = 0;
	for(; i < insert_num; i++){

		if(i % 10000 == 0){
			printf("%d -- %u\n", i, getTime());
		}
    
		// 执行命令 获得回复句柄
		redisReply* r = (redisReply*)redisCommand(c, comm);

		// 判断回复 是否为空
	    if( NULL == r ){
	        printf("Execut command <%s> failure\n", comm);
	        goto CONNECTED_ERROR;
		}

		// 判断执行状态
		if( r->type != REDIS_REPLY_INTEGER){// && r->integer == 1) ){
			printf("Execut command error! type is %d\n", r->type);
			printf("Comm str:\n\n%s\n\n",comm);
			freeReplyObject(r);
	        goto CONNECTED_ERROR;
		}

	}
	

	printf("%d -- %u\n", i, getTime());
#endif
CONNECTED_ERROR:
	redisFree(c);
    return -1;
}
