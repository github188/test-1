#include "cJSON.h"
#include "base64.h"

/*


   eyJkZXNjIjoiIiwiZXJyb3JfY29kZSI6MCwibWV0YWRhdGEiOiJleUpUVUU1MWJXSmxjaUk2SWlJc0lrTm9ZWEpuWlU1MWJXSmxjaUk2SWlJc0lsVnpaWEpPZFcxaVpYSWlPaUl4TXpnMk5ERXlNRGcyTkNJc0lrTnZjbkJKWkNJNklqUXhNekExTUNJc0lsTmxjblpwWTJWVWVYQmxJam9pSWl3aVJtVmxWSGx3WlNJNk1Td2lSbVZsVm1Gc2RXVWlPaUl3SWl3aVIybDJaVzVXWVd4MVpTSTZJakFpTENKQloyVnVkRVpzWVdjaU9qRXNJazF2Y21Wc1lYUmxkRzlOVkVac1lXY2lPaklzSWxCeWFXOXlhWFI1SWpvMUxDSkZlSEJwY21WVWFXMWxJam9pSWl3aVUyTm9aV1IxYkdWVWFXMWxJam9pSWl3aVVtVndiM0owUm14aFp5STZNQ3dpVkZCZmNHbGtJam94TENKVVVGOTFaR2hwSWpveExDSk5aWE56WVdkbFEyOWthVzVuSWpvd0xDSk5aWE56WVdkbFZIbHdaU0k2TUN3aVRXVnpjMkZuWlVOdmJuUmxiblFpT2lKYmMzVndaWEpkYm1sb1lXOHNkR1Z6ZE9hMWkraXZsZWVmcmVTL29TSXNJbE5sYm1SVWFXMWxJam9pTURBd01TMHdNUzB3TVNBd01Eb3dNRG93TUNJc0lrVnljbTl5UTI5a1pTSTZiblZzYkN3aVJHOXVaVlJwYldVaU9pSXdNREF4TFRBeExUQXhJREF3T2pBd09qQXdJaXdpUkc5dVpWSmxjRzl5ZENJNmJuVnNiQ3dpWkY5MFpIcG9JanB1ZFd4c0xDSmtYM1ZwWkNJNmJuVnNiQ3dpWkY5bWJHRm5Jam93TENKa1gzUmtZbWdpT201MWJHd3NJbVJmWm1sa0lqcHVkV3hzTENKTVgxTmxibVJVZVhCbElqb3dMQ0pNWDJ4bGRtVnNJam93ZlE9PSIsInJlcG9ydHRpbWUiOiIwIiwicmVzdWx0Ijoic3VjY2VzcyIsInN1Ym1pdHRpbWUiOiIxMDE5MTY0MTI1In0=
 * */

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
	char json[10000] = {0};
	snprintf(json, 9999,
			"{\"SpNumber\":\"10691029\",\"ChargeNumber\":\"10691029\",\"UserCount\":1,\"UserNumber\":\"8615562621875\",\"CorpId\":\"98835\",\"ServiceType\":\"9991801021\",\"FeeType\":2,\"FeeValue\":\"0\",\"GivenValue\":\"0\",\"AgentFlag\":0,\"MorelatetoMTFlag\":2,\"Priority\":9,\"ExpireTime\":\"\",\"ScheduleTime\":\"\",\"ReportFlag\":1,\"TpPid\":0,\"Udhi\":0,\"MessageCoding\":15,\"MessageType\":0,\"MsgLen\":161,\"MessageContent\":\"您的验证码是0254![元征科技]\", \"LinkId\":\"\"}");

	size_t base64_json_len;
	char* base64_json = base64_encode(json, strlen(json), &base64_json_len);

	char comm[10000] = {0};
	snprintf(comm, 9999, "LPUSH to_send_list_1130 %s", base64_json);

	printf("\n%s\n\n", comm);
	
	// 连接
    //redisContext* c = redisConnect("127.0.0.1", 6379);
    redisContext* c = redisConnect("10.140.244.131", 6379);
    if ( c->err ){
        printf("Connect to redisServer faile!\n");
        goto CONNECTED_ERROR;
    }
    printf("Connect to redisServer Success!\n");
	
	// 认证
	redisReply* r = (redisReply*)redisCommand(c, "AUTH YzWx2016");
	if( NULL == r ){
	    printf("Execut command <%s> failure\n", comm);
	    goto CONNECTED_ERROR;
	}
	if( r->type != REDIS_REPLY_STATUS && r->integer != 0 ){
		printf("Execut command error! type is %d\n", r->type);
		printf("Comm str:\n\n%s\n\n",comm);
		freeReplyObject(r);
	    goto CONNECTED_ERROR;
	}

	//char *result = "eyJTcE51bWJlciI6IjEyMzQ1NiIsIkNoYXJnZU51bWJlciI6IjAwMDAwMDAwMDAwMDAwMDAwMDAwMCIsIlVzZXJDb3VudCI6MSwiVXNlck51bWJlciI6Ijg2MTM4Njk1NjQ1ODUiLCJDb3JwSWQiOiIxMjM0NTYiLCJTZXJ2aWNlVHlwZSI6IjEyMzQ1NiIsIkZlZVR5cGUiOjEsIkZlZVZhbHVlIjoiMCIsIkdpdmVuVmFsdWUiOiIwIiwiQWdlbnRGbGFnIjowLCJNb3JlbGF0ZXRvTVRGbGFnIjoyLCJQcmlvcml0eSI6MCwiRXhwaXJlVGltZSI6IjE2MTAxNTIzNTk1OTAzMiIsIlNjaGVkdWxlVGltZSI6IiIsIlJlcG9ydEZsYWciOjEsIlRwUGlkIjoxLCJVZGhpIjoxLCJNZXNzYWdlQ29kaW5nIjoxNSwiTWVzc2FnZVR5cGUiOjAsIk1zZ0xlbiI6MTYxLCJNc2dDb250ZW50Ijoi5L2g5aW954y05a2QISJ9";
	//size_t base64_temp2_len;
	//char* base64_temp2 = base64_decode(result, strlen(result), &base64_temp2_len);
	//printf("base64_temp2 = %s, base64_temp2_len = %lu\n", base64_temp2, base64_temp2_len);

	int insert_num = 1;
	if(argc == 2){
		insert_num = atoi(argv[1]);
	}
	printf("insert num is %d\n", insert_num);
	
	int i = 0;
	for(; i < insert_num; i++){

		if(i % 10000 == 0){
			printf("%d -- %u\n", i, getTime());
		}
#if 1
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
#else
		redisReply* r = (redisReply*)redisCommand(c, "LPOP to_send_list_1130");
		if( NULL == r ){
		    printf("Execut command <%s> failure\n", comm);
		    goto CONNECTED_ERROR;
		}
		if( r->type != REDIS_REPLY_STATUS && r->integer != 0 ){
			printf("Execut command error! type is %d\n", r->type);
			printf("Comm str:\n\n%s\n\n",comm);
			freeReplyObject(r);
			goto CONNECTED_ERROR;
		}
#endif
		freeReplyObject(r);
	}

	printf("%d -- %u\n", i, getTime());
CONNECTED_ERROR:
	redisFree(c);
    return -1;
}
