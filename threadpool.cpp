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


/*Result��ӿ�*/
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




/*Task��ӿ�*/
void Task::run_exec(){
	if (m_result != nullptr) {
		// ��ִ������ Ȼ�󴫸��������
		m_result->get_result_from_task(task_run());
	}
}

void Task::connect_result(Result* res) {
	m_result = res;
}





