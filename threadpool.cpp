#include "threadpool.h"

/*线程池类接口*/
void threadpool::start_threadpool(int thread_size) {
	// 初始化变量

	// 1. 创建线程对象

	// 2. 启动所有线程
}
void threadpool::set_mode(PoolMode mode) {

}

void threadpool::set_thread_size(int size) {

}

void threadpool::set_thread_threshould(int threshould) {

}


/*线程类接口*/
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


