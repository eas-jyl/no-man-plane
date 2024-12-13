#pragma once

#include <winsock2.h>
#include <memory>

class Socket
{
public:
	// m_type : 1表示tcp协议 ， 0表示udp协议
	Socket(bool m_type = true) {
		m_sock = INVALID_SOCKET;
		if (m_type == true) {
			m_sock = socket(AF_INET, SOCK_STREAM, 0);
		}
		else {
			m_sock = socket(AF_INET, SOCK_DGRAM , 0);
		}
	}
	

	// 关闭套接字
	void Close() {
		if (m_sock != INVALID_SOCKET) {
			SOCKET temp = m_sock; // 提高线程安全
			m_sock = INVALID_SOCKET; 
			closesocket(temp);
		}
	}

	~Socket() {
		Close();
	}

private:
	SOCKET m_sock;
};


class ESocket
{
public:
	ESocket(bool m_type = true)
		:m_ptr(new Socket(m_type))
	{}

	// 拷贝构造、复制重载
	ESocket(const ESocket& sock)
		:m_ptr(sock.m_ptr)
	{}

	ESocket& operator=(const ESocket& sock) {
		if (this != &sock) m_ptr = sock.m_ptr;
		return *this;
	}

	~ESocket() {
		m_ptr.reset(); // 释放一个对象
	}

	// 重定义临时对象 ： 能够将ESocket转换为Socket类型（）
	operator Socket() {
		return *m_ptr;
	}
private:
	std::shared_ptr<Socket> m_ptr;
};

class SocketInit
{
public:
	SocketInit() {
		WSADATA wsadata;
		WSAStartup(MAKEWORD(2, 2), &wsadata); // 返回值为0 ，表示初始化成功
	}
	~SocketInit() {
		WSACleanup();
	}
};

