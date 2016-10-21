#include "sgip.h"

/**
 *@brief  连接引导服务器 并做初始认证
 *@return succ 创建的sockfd failed -1
 */
int conn_yzsp()
{
	int sockfd;
    struct sockaddr_in addr;	
	
	//socket初始化
	while(( sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		printf("%d:socket()\n", __LINE__);
		sleep(10);
	}
	
	//连接引导服务器 失败的话 休眠3秒 重新连接
	addr.sin_family			= AF_INET;
	addr.sin_port			= htons( 17890 );
	addr.sin_addr.s_addr	= inet_addr( "10.10.10.84" );
	while( connect(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr)) == -1){
		printf("%d:connect(), error num is%d \n", __LINE__, errno);
		sleep(3);
	}

	int seqid_count = 0;


	char recv_buf[1000] = {0};

	//发送bind信息
	SGIP_PKG pkg;
	pkg.head.nMessageLength = htonl(sizeof(SGIP_HEAD) + sizeof(SGIPG_BIND));
	pkg.head.nCommandId = htonl(nSGIP_BIND);
	pkg.head.seqid.nSrcNodeNum = htonl(5);
	pkg.head.seqid.nDateTime = htonl(1021160605);
	pkg.head.seqid.nSeqId = htonl(seqid_count++);
	
	SGIPG_BIND *bind = (SGIPG_BIND*)pkg.data;
	bind->cLoginType = 2;
	snprintf(bind->sLoginName, 15, "hyjnzzwx");
	snprintf(bind->sLoginPwd, 15, "hyjnzzwx");

	int ret = send(sockfd, &pkg, ntohl(pkg.head.nMessageLength), 0);
	if( ret != ntohl(pkg.head.nMessageLength)){
		printf("%d:send error, ret is %d, should be %u\n", __LINE__, ret, ntohl(pkg.head.nMessageLength));
		goto SOCK_ERR;
	}

	ret =  recv(sockfd, recv_buf, sizeof(SGIP_HEAD) + sizeof(SGIPG_BIND_RESP), 0);
	if( ret != sizeof(SGIP_HEAD) + sizeof(SGIPG_BIND_RESP) ){
		printf("%d:send error, ret is %d, should be %lu\n", __LINE__, ret, sizeof(SGIP_HEAD) + sizeof(SGIPG_BIND_RESP));
		goto SOCK_ERR;
	}

	while(1){
		printf("%d:send a report!\n", __LINE__);
		// report
		memset(&pkg, 0, sizeof(SGIP_PKG));
		pkg.head.nMessageLength = htonl(sizeof(SGIP_HEAD) + sizeof(SGIP_REPORT));
		pkg.head.nCommandId = htonl(nSGIP_REPORT);
		pkg.head.seqid.nSrcNodeNum = htonl(5);
		pkg.head.seqid.nDateTime = htonl(1021160605);
		pkg.head.seqid.nSeqId = htonl(seqid_count++);
	
		SGIP_REPORT *report = (SGIP_REPORT*)pkg.data;
		report->seqid.nSrcNodeNum = htonl(5);
		report->seqid.nDateTime = htonl(1021160600);
		report->seqid.nSeqId = htonl(seqid_count++);
		report->cReportType = 0;
		snprintf(report->sUserNumber, 20, "8618866120427");
		report->cState = 2;
		report->cErrorCode = 69;
		
		ret = send(sockfd, &pkg, ntohl(pkg.head.nMessageLength), 0);
		if( ret != ntohl(pkg.head.nMessageLength)){
			printf("%d:send error, ret is %d, should be %u\n", __LINE__, ret, ntohl(pkg.head.nMessageLength));
			goto SOCK_ERR;
		}

		ret =  recv(sockfd, recv_buf, sizeof(SGIP_HEAD) + sizeof(SGIP_RESP), 0);
		if( ret != sizeof(SGIP_HEAD) + sizeof(SGIPG_BIND_RESP) ){
			printf("%d:send error, ret is %d, should be %lu\n", __LINE__, ret, sizeof(SGIP_HEAD) + sizeof(SGIPG_BIND_RESP));
			goto SOCK_ERR;
		}
		printf("%d:send a report over!\n", __LINE__);
		sleep(30);
	}


	return 0;
SOCK_ERR:
	close( sockfd );		//出现错误关闭sockfd 并重新连接
	sockfd = -1;
	return -1;
}


int main(int argc, char** argv)
{
	conn_yzsp();
	return 0;
}
