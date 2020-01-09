#include "pch.h"
#include "CMySocket.h"
#include <ws2tcpip.h>
#include "Data.h"
using namespace std;
//base64编码默认自带换行
char* Base64Encode(const char* input, int length, bool with_new_line);
char* Base64Decode(char* input, int length, bool with_new_line);


char* Base64Encode(const char* input, int length, bool with_new_line)
{
	BIO* bmem = NULL;
	BIO* b64 = NULL;
	BUF_MEM* bptr = NULL;
	b64 = BIO_new(BIO_f_base64());
	if (!with_new_line)
	{
		BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
	}
	bmem = BIO_new(BIO_s_mem());
	b64 = BIO_push(b64, bmem);
	BIO_write(b64, input, length);
	BIO_flush(b64);
	BIO_get_mem_ptr(b64, &bptr);
	char* buff = (char*)malloc(bptr->length + 1);
	memcpy(buff, bptr->data, bptr->length);
	buff[bptr->length] = 0;
	BIO_free_all(b64);
	return buff;
}
char* Base64Decode(char* input, int length, bool with_new_line)
{
	BIO* b64 = NULL;
	BIO* bmem = NULL;
	char* buffer = (char*)malloc(length);
	memset(buffer, 0, length);
	b64 = BIO_new(BIO_f_base64());
	if (!with_new_line)
	{
		BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
	}
	bmem = BIO_new_mem_buf(input, length);
	bmem = BIO_push(b64, bmem);
	BIO_read(bmem, buffer, length);
	BIO_free_all(bmem);
	return buffer;
}

CMySocket::CMySocket()
{
}

void CMySocket::connect()
{
	// 初始化套接字环境
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	// 创建套接字
	m_Server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// 配置服务器地址和端口
	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6666);	// 端口为666
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
	// 连接服务器
	::connect(m_Server, (sockaddr*)& addr, sizeof(addr));
}

void CMySocket::Send(DataType Type, const char* data)
{
	string enc_input = data;
	char* enc_output = Base64Encode(enc_input.c_str(), enc_input.length(), true);
	int nSize = strlen(enc_output);
	SENDDATAPACK SendDataPack = { Type,nSize };
	// 先发送八个字节的结构体给服务器
	::send(m_Server, (const char*)& SendDataPack, sizeof(SENDDATAPACK), 0);
	// 再发送数据过去
	::send(m_Server, enc_output, nSize, 0);
}


RECVDATASTRUCT* CMySocket::Recv() // 将数据解包再封装起来 CMySocket::Recv() void connect()
{
	RECVDATASTRUCT head = { 0 };
	if (::recv(m_Server, (char*)& head, sizeof(head) - 1, 0) != sizeof(head) - 1)
		return NULL;
	RECVDATASTRUCT* pBuff = (RECVDATASTRUCT*)malloc(head.nSize + sizeof(head));
	memset(pBuff, 0, head.nSize + sizeof(head));
	memcpy_s(pBuff, sizeof(head) - 1, &head, sizeof(head) - 1);  // 将head头部复制进去
	if (::recv(m_Server, pBuff->data, head.nSize, 0) != head.nSize)  // 把数据部分也复制
	{
		free(pBuff);
		pBuff = NULL;
		return NULL;
	}
	return pBuff;
}
