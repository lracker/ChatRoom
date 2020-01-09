#pragma once
#include <WinSock2.h>
#include <string>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#pragma comment(lib,"libssl.lib")
#pragma comment(lib,"libcrypto.lib")
#pragma comment(lib, "ws2_32.lib")
#include <stdint.h>
/*数据的类型*/
enum DataType
{
	ERR = -1,
	SYS = 0,
	LOGIN = 2,				// 登录消息
	REGISTE = 3,			// 注册消息
	FRIENDLIST = 4,			// 获取好友列表
	ADDFRIEND = 5,			// 好友申请
	ADDFRIENDCOMFIRM = 6,   // 好友申请回复
	SENDMESSAGE = 7,			// 发送信息
	SEARCHDATA = 8,			// 查看数据库
	CREATEGROUP = 9, 
	GROUPLIST = 10,   //刷新组表
	ADDGROUP = 11,
	GROUPMEMBERFLUSH = 12,	// 刷新出群友列表
	SENDGROUPMESSAGE = 13, //发送群聊信息
	SEARCHGROUPDATA = 14   // 查看群聊的数据库

};

/*客户端发过去的消息结构体*/
typedef struct _SENDDATAPACK
{
	DataType Type;  //数据的类型
	int nSize;		//数据的大小
}SENDDATAPACK;

#pragma pack(push,1)   //让结构体按照一个字节对齐
/*服务器发回来的消息结构体*/
typedef struct _RECVDATASTRUCT
{
	uint32_t Type;  // 数据的类型
	int32_t	 status;  // 状态
	uint32_t nSize;		// 数据的大小
	char data[1];   // 数据，设置为1，方便后面覆盖掉的时候可以通过.data操控数据
}RECVDATASTRUCT;
#pragma pack(pop)

class CMySocket
{
	SOCKET m_Server;
public:
	CMySocket();
	void connect();
	void Send(DataType Type, const char* data);
	RECVDATASTRUCT* Recv();  // 将数据解包再封装起来
	
};
