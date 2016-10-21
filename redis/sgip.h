#ifndef SGIP_H
#define SGIP_H

#include "header.h"

#define   MAX_SUBMITMSG_LEN	161
#define   MAX_PKG_SIZE		350

/************************** 报文命令定义 **************************/
const   unsigned int nSGIP_BIND = 0x1;
const   unsigned int nSGIP_BIND_RESP = 0x80000001;
const   unsigned int nSGIP_UNBIND = 0x2;
const   unsigned int nSGIP_UNBIND_RESP = 0x80000002;
const   unsigned int nSGIP_SUBMIT = 0x3;
const   unsigned int nSGIP_SUBMIT_RESP = 0x80000003;
const   unsigned int nSGIP_DELIVER = 0x4;
const   unsigned int nSGIP_DELIVER_RESP = 0x80000004;
const   unsigned int nSGIP_REPORT = 0x5;
const   unsigned int nSGIP_REPORT_RESP = 0x80000005;
const   unsigned int nSGIP_ADDSP = 0x6;
const   unsigned int nSGIP_ADDSP_RESP = 0x80000006;
const   unsigned int nSGIP_MODIFYSP = 0x7;
const   unsigned int nSGIP_MODIFYSP_RESP = 0x80000007;
const   unsigned int nSGIP_DELETESP = 0x8;
const   unsigned int nSGIP_DELETESP_RESP = 0x80000008;
const   unsigned int nSGIP_QUERYROUTE = 0x9;
const   unsigned int nSGIP_QUERYROUTE_RESP = 0x80000009;
const   unsigned int nSGIP_ADDTELESEG = 0xa;
const   unsigned int nSGIP_ADDTELESEG_RESP = 0x8000000a;
const   unsigned int nSGIP_MODIFYTELESEG = 0xb;
const   unsigned int nSGIP_MODIFYTELESEG_RESP = 0x8000000b;
const   unsigned int nSGIP_DELETETELESEG = 0xc;
const   unsigned int nSGIP_DELETETELESEG_RESP = 0x8000000c;
const   unsigned int nSGIP_ADDSMG = 0xd;
const   unsigned int nSGIP_ADDSMG_RESP = 0x8000000d;
const   unsigned int nSGIP_MODIFYSMG = 0xe;
const   unsigned int nSGIP_MODIFYSMG_RESP = 0x0000000e;
const   unsigned int nSGIP_DELETESMG = 0xf;
const   unsigned int nSGIP_DELETESMG_RESP = 0x8000000f;
const   unsigned int nSGIP_CHECKUSER = 0x10;
const   unsigned int nSGIP_CHECKUSER_RESP = 0x80000010;
const   unsigned int nSGIP_USERRPT = 0x11;
const   unsigned int nSGIP_USERRPT_RESP = 0x80000011;
const   unsigned int nSGIP_TRACE = 0x1000;
const   unsigned int nSGIP_TRACE_RESP = 0x80001000;

/************************** 报文头定义 **************************/
typedef  struct   _SGIP_SEQID
{
    unsigned   int nSrcNodeNum;	//源节点编号
    unsigned   int nDateTime; //日期时间,如:1120203225
    unsigned   int nSeqId; //流水号
}SGIP_SEQID;
//Q_DECLARE_METATYPE(SGIP_SEQID)  // 加上这一句以能在不同的线程间传送

typedef struct   _SGIP_HEAD	//消息头的格式
{
    unsigned   int nMessageLength; // 数据报的长度
    unsigned   int nCommandId; // 命令码
    SGIP_SEQID seqid; // 流水号
}SGIP_HEAD;

/************************** 报文体定义 **************************/
#pragma pack(1)
typedef struct   _SGIP_SUBMIT
{
    char sSpNumber[21]; //sp的接入号码
    char sChargeNumber[21]; //付费号码，手机号码前加“86”国别标志；当且仅当群发且对用户收费时为空；如果为空，则该条短消息产生的费用由UserNumber代表的用户支付；如果为全零字符串“000000000000000000000”，表示该条短消息产生的费用由SP支付。
    unsigned char cUserCount; //1 接收短消息的手机数量
    char sUserNumber[21]; //接受该短消息的手机号
    char sCorpId[5]; //企业代码，0-99999
    char sServiceType[10]; //业务代码，由sp定义
    unsigned char cFeeType; //计费类型
    char sFeeValue[6]; //该条短消息的收费值,单位为分
    char sGivenValue[6]; //赠送用户的话费,0-99999
    unsigned char cAgentFlag; //代收费标志,0:应收;1:实收
    unsigned char cMorelatetoMTFlag; //引起MT消息的原因
    unsigned char cPriority; //优先级0-9,从低到高
    char sExpireTime[16]; //短消息寿命的终止时间,"yymmddhhmmsstnnp","tnnp"取固定值"032+"
    char sScheduleTime[16]; //定时发送时间
    unsigned char cReportFlag; //状态报告标志,0-出错返回状态报告;1-总返回状态报告;2-不要状态报告;3...
    unsigned char cTpPid;
    unsigned char cUdhi;
    unsigned char cMessageCoding; //编码方式,0:Ascii;3:Write   card;4:binary;8:ucs2;15:GBK
    unsigned char cMessageType; //0:短消息信息
    unsigned int nMsgLen; //短消息长度(不调用sgip_submit_sm_set_messagecontent,而手动赋值的话,需要调用函数htonl()转换为网络字节序)
    char sMsgContent[MAX_SUBMITMSG_LEN]; //2048;
    char sLinkId[8];
}SGIP_SUBMIT;

typedef struct   _SGIPG_BIND   //Bind命令的语法
{
    // sgipg_head head; //数据报的包头
    unsigned char cLoginType; //登录类型1-sp连接smg;2-smg连接sp
    char sLoginName[16];   //服务器端给客户端分配的登录名
    char sLoginPwd[16];   //服务器端和Login Name对应的密码
    char sReserve[8];   //保留，扩展用
}SGIPG_BIND;

typedef struct   _SGIPG_BIND_RESP   //Bind命令的语法
{
    unsigned char Result;//Bind执行命令是否成功。0：执行成功 其它：错误码
    char sReserve[8];   //保留，扩展用
}SGIPG_BIND_RESP;

typedef  struct   _SGIP_RESP   //Bind_Resp应答的语法
{
    // sgipg_head head;
    unsigned   char cResult; //0:成功
    char sReserved[8];
}SGIP_RESP;

typedef  struct _SGIP_DELIVER
{
    char sUserNumber[21];//
    char sSPNumber[21];//
    unsigned char cTP_pid;//
    unsigned char cTP_udhi;//
    unsigned char cMessageCoding;//
    int nMessageLength;//
    char sMessageContent[600];
    char cReserve[8];
}SGIP_DELIVER;


typedef struct  _SGIP_REPORT
{
    SGIP_SEQID seqid;			//该命令所涉及的Submit或deliver命令的序列号
    unsigned char cReportType;	//Report命令类型0：对先前一条Submit命令的状态报告1：对先前一条前转Deliver命令的状态报告
    char sUserNumber[21];		//接收短消息的手机号，手机号码前加“86”国别标志
    unsigned char cState;		//该命令所涉及的短消息的当前执行状态0：发送成功1：等待发送2：发送失败
    unsigned char cErrorCode;	//当State=2时为错误码值，否则为0
    char sReserve[8];
}SGIP_REPORT;

//typedef struct _SUBMIT_RESP_DTAT
//{
//    SGIP_SEQID seqid; // 流水号
//    SGIP_RESP resp;
//}SUBMIT_RESP_DTAT;//for buffer

#pragma pack()

/************************** 报文定义 **************************/
typedef	struct _SGIP_PKG
{
    SGIP_HEAD	head;//	包头
    char		data[MAX_PKG_SIZE];	//	数据体
} SGIP_PKG;

/************************** 报文定义 **************************/

/*
0	无错误，命令正确接收
1	非法登录，如登录名、口令出错、登录名与口令不符等。
2	重复登录，如在同一TCP/IP连接中连续两次以上请求登录。
3	连接过多，指单个节点要求同时建立的连接数过多。
4	登录类型错，指bind命令中的logintype字段出错。
5	参数格式错，指命令中参数值与参数类型不符或与协议规定的范围不符。
6	非法手机号码，协议中所有手机号码字段出现非86130号码或手机号码前未加“86”时都应报错。
7	消息ID错
8	信息长度错
9	非法序列号，包括序列号重复、序列号格式错误等
10	非法操作GNS
11	节点忙，指本节点存储队列满或其他原因，暂时不能提供服务的情况
21	目的地址不可达，指路由表存在路由且消息路由正确但被路由的节点暂时不能提供服务的情况
22	路由错，指路由表存在路由但消息路由出错的情况，如转错SMG等
23	路由不存在，指消息路由的节点在路由表中不存在
24	计费号码无效，鉴权不成功时反馈的错误信息
25	用户不能通信（如不在服务区、未开机等情况）
26	手机内存不足
27	手机不支持短消息
28	手机接收短消息出现错误
29	不知道的用户
30	不提供此功能
31	非法设备
32	系统失败
33	短信中心队列满
其它	其它错误码(待定义)
*/

#endif // SGIP_H

