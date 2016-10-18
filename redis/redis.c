#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <hiredis/hiredis.h>
#include "cJSON.h"

#include <stdint.h>
#include <stdlib.h>


static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};
static char *decoding_table = NULL;
static int mod_table[] = {0, 2, 1};


char *base64_encode(const unsigned char *data,
                    size_t input_length,
                    size_t *output_length) {

    *output_length = 4 * ((input_length + 2) / 3);

    char *encoded_data = malloc(*output_length);
    if (encoded_data == NULL) return NULL;

    for (int i = 0, j = 0; i < input_length;) {

        uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }

    for (int i = 0; i < mod_table[input_length % 3]; i++)
        encoded_data[*output_length - 1 - i] = '=';

    return encoded_data;
}


void build_decoding_table() {

    decoding_table = malloc(256);

    for (int i = 0; i < 64; i++)
        decoding_table[(unsigned char) encoding_table[i]] = i;
}



unsigned char *base64_decode(const char *data,
                             size_t input_length,
                             size_t *output_length) {

    if (decoding_table == NULL) build_decoding_table();

    if (input_length % 4 != 0) return NULL;

    *output_length = input_length / 4 * 3;
    if (data[input_length - 1] == '=') (*output_length)--;
    if (data[input_length - 2] == '=') (*output_length)--;

    unsigned char *decoded_data = malloc(*output_length);
    if (decoded_data == NULL) return NULL;

    for (int i = 0, j = 0; i < input_length;) {

        uint32_t sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];

        uint32_t triple = (sextet_a << 3 * 6)
        + (sextet_b << 2 * 6)
        + (sextet_c << 1 * 6)
        + (sextet_d << 0 * 6);

        if (j < *output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
    }

    return decoded_data;
}

void base64_cleanup() {
    free(decoding_table);
}

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

/*
	char* temp = "中国";
	size_t base64_temp_len;
	char* base64_temp = base64_encode(temp, sizeof(temp), &base64_temp_len);
	printf("base64_temp = %s, base64_temp_len = %lu\n", base64_temp, base64_temp_len);

	size_t base64_temp1_len;
	char* base64_temp1 = base64_decode(base64_temp, sizeof(base64_temp), &base64_temp1_len);
	printf("base64_temp1 = %s, base64_temp1_len = %lu\n", base64_temp1, base64_temp1_len);
*/

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
