#include "threadpool.h"

/*�̳߳���ӿ�*/
void threadpool::start_threadpool(int thread_size) {
	// ��ʼ������

	// 1. �����̶߳���

	// 2. ���������߳�
}
void threadpool::set_mode(PoolMode mode) {

}

void threadpool::set_thread_size(int size) {

}

void threadpool::set_thread_threshould(int threshould) {

}


/*�߳���ӿ�*/
thread::thread(thread_func m_fun)
	: m_function(m_fun)
	, m_thread_id(m_init_threadid++)
{}


void thread::thread_start() {
	std::thread t1(m_function, m_thread_id);
	t1.detach();
}

int thread::get_thread_id() const {
	return m_thread_id;
}


