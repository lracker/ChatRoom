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
/*���ݵ�����*/
enum DataType
{
	ERR = -1,
	SYS = 0,
	LOGIN = 2,				// ��¼��Ϣ
	REGISTE = 3,			// ע����Ϣ
	FRIENDLIST = 4,			// ��ȡ�����б�
	ADDFRIEND = 5,			// ��������
	ADDFRIENDCOMFIRM = 6,   // ��������ظ�
	SENDMESSAGE = 7,			// ������Ϣ
	SEARCHDATA = 8,			// �鿴���ݿ�
	CREATEGROUP = 9, 
	GROUPLIST = 10,   //ˢ�����
	ADDGROUP = 11,
	GROUPMEMBERFLUSH = 12,	// ˢ�³�Ⱥ���б�
	SENDGROUPMESSAGE = 13, //����Ⱥ����Ϣ
	SEARCHGROUPDATA = 14   // �鿴Ⱥ�ĵ����ݿ�

};

/*�ͻ��˷���ȥ����Ϣ�ṹ��*/
typedef struct _SENDDATAPACK
{
	DataType Type;  //���ݵ�����
	int nSize;		//���ݵĴ�С
}SENDDATAPACK;

#pragma pack(push,1)   //�ýṹ�尴��һ���ֽڶ���
/*����������������Ϣ�ṹ��*/
typedef struct _RECVDATASTRUCT
{
	uint32_t Type;  // ���ݵ�����
	int32_t	 status;  // ״̬
	uint32_t nSize;		// ���ݵĴ�С
	char data[1];   // ���ݣ�����Ϊ1��������渲�ǵ���ʱ�����ͨ��.data�ٿ�����
}RECVDATASTRUCT;
#pragma pack(pop)

class CMySocket
{
	SOCKET m_Server;
public:
	CMySocket();
	void connect();
	void Send(DataType Type, const char* data);
	RECVDATASTRUCT* Recv();  // �����ݽ���ٷ�װ����
	
};
