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


/*Result类接口*/
Result::Result(std::shared_ptr<Task> task, bool isvaild) : result_status(isvaild) , m_task(task)
{
	m_task->connect_result(this); 
}


void Result::get_result_from_task(Any value) {
	this->m_any = std::move(value);
	m_sem.post();
}


Any Result::send_result_to_user() {
	if (result_status == false) return "";
	m_sem.wait();
	return std::move(m_any);
}




/*Task类接口*/
void Task::run_exec(){
	if (m_result != nullptr) {
		// 先执行任务 然后传给结果对象
		m_result->get_result_from_task(task_run());
	}
}

void Task::connect_result(Result* res) {
	m_result = res;
}





