#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <thread>
#include <iostream>
#include <unordered_map>

enum class PoolMode {
    Mode_Fixed,
    Mode_Cached,
};

class Semaphere
{
public:
    Semaphere(int limit = 0)
        : m_limit(limit)
    {}

    ~Semaphere() {}

    // �����ȴ��ź�����Դ
    void wait() {
        std::unique_lock<std::mutex> lock(mtx);
        m_cond.wait(lock, [&]()->bool {
            return m_limit > 0;
            });
        m_limit--;
    }

    // �ͷ��ź�����Դ
    void post() {
        std::unique_lock<std::mutex> lock(mtx);
        m_limit++;
        m_cond.notify_all();
    }

private:
    int m_limit;
    std::mutex mtx;
    std::condition_variable m_cond;
};

class Any
{
public:
    Any() = default;
    ~Any() = default;

    Any(const Any&) = delete;
    Any& operator=(const Any&) = delete;

    Any(Any&&) = default;
    Any& operator=(Any&&) = default;

    template<class T> // ������Ҫ����һ��
    Any(T data)
        : m_base(std::unique_ptr<Derive<T>>(data))
    {}

    template<class T>
    T _cast() {
        Derive<T>* ptr = dynamic_cast<Derive<T>*>(m_base.get());
        if (ptr == nullptr) throw "���Ͳ�ƥ��";
        else return ptr->m_data;
    }

private:
    class Base
    {
    public:
        virtual ~Base() = default; // �����಻��ʵ����
    };

    template <class T>
    class Derive : public Base
    {
    public:
        Derive(T data)
            : m_data(data)
        {}

        T m_data;
    };

private:
    std::unique_ptr<Base> m_base;
};

class Task;
class Result
{
public:
    Result(std::shared_ptr<Task> task, bool isvaild);

    ~Result() = default;

    // ��������û�
    Any send_result_to_user();

    // Result��Task�л�ȡ���
    void get_result_from_task(Any value);

private:
    Semaphere m_sem;
    Any m_any;
    std::shared_ptr<Task> m_task;
    std::atomic_bool result_status;
};


class Task
{
public:
    Task() : m_result(nullptr)
    {}

    ~Task() = default;

    // ������ķ���ֵ����Result����
    void run_exec();

    // ��һ��Result����
    void connect_result(Result* res);

    // �û�����ִ�к���
    virtual Any task_run() = 0;


private:
    Result* m_result;
};


class thread
{
public:
    using thread_func = std::function<void(int)>;
    thread(thread_func m_fun);
    ~thread() {}; 

    // �����߳� + ���̻߳ص�����
    void thread_start();

    // ��ȡ�߳�id
    int get_thread_id() const;

private:
    int m_thread_id; // ��ǰ�߳�id
    static int m_init_threadid; // ��ʼ���߳�id
    thread_func m_function;
};


class threadpool
{
public:
    threadpool();
    ~threadpool();
    
    /*�����̳߳�ģʽ�ӿ�*/
    void start_threadpool(int thread_size = 8);
    void set_mode(PoolMode mode);
    void set_thread_size(int size); 
    void set_thread_threshould(int threshould); 

private:
    /*�̳߳ز���*/
    std::unordered_map<int , std::unique_ptr<thread>> m_thread_pool; // �̳߳�
    
    std::atomic_bool pool_running_state; // �̳߳ص�״̬
    PoolMode pool_mode; // ģʽ





};


