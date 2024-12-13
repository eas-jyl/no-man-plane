#pragma once

#include <winsock2.h>
#include <memory>

class Socket
{
public:
	// m_type : 1��ʾtcpЭ�� �� 0��ʾudpЭ��
	Socket(bool m_type = true) {
		m_sock = INVALID_SOCKET;
		if (m_type == true) {
			m_sock = socket(AF_INET, SOCK_STREAM, 0);
		}
		else {
			m_sock = socket(AF_INET, SOCK_DGRAM , 0);
		}
	}
	

	// �ر��׽���
	void Close() {
		if (m_sock != INVALID_SOCKET) {
			SOCKET temp = m_sock; // ����̰߳�ȫ
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

	// �������졢��������
	ESocket(const ESocket& sock)
		:m_ptr(sock.m_ptr)
	{}

	ESocket& operator=(const ESocket& sock) {
		if (this != &sock) m_ptr = sock.m_ptr;
		return *this;
	}

	~ESocket() {
		m_ptr.reset(); // �ͷ�һ������
	}

	// �ض�����ʱ���� �� �ܹ���ESocketת��ΪSocket���ͣ���
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
		WSAStartup(MAKEWORD(2, 2), &wsadata); // ����ֵΪ0 ����ʾ��ʼ���ɹ�
	}
	~SocketInit() {
		WSACleanup();
	}
};

