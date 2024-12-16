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

    // 阻塞等待信号量资源
    void wait() {
        std::unique_lock<std::mutex> lock(mtx);
        m_cond.wait(lock, [&]()->bool {
            return m_limit > 0;
            });
        m_limit--;
    }

    // 释放信号量资源
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

    template<class T> // 类外需要声明一下
    Any(T data)
        : m_base(std::unique_ptr<Derive<T>>(data))
    {}

    template<class T>
    T _cast() {
        Derive<T>* ptr = dynamic_cast<Derive<T>*>(m_base.get());
        if (ptr == nullptr) throw "类型不匹配";
        else return ptr->m_data;
    }

private:
    class Base
    {
    public:
        virtual ~Base() = default; // 抽象类不能实例化
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

    // 将结果给用户
    Any send_result_to_user();

    // Result从Task中获取结果
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

    // 将任务的返回值传给Result对象
    void run_exec();

    // 绑定一个Result对象
    void connect_result(Result* res);

    // 用户定义执行函数
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

    // 创建线程 + 绑定线程回调函数
    void thread_start();

    // 获取线程id
    int get_thread_id() const;

private:
    int m_thread_id; // 当前线程id
    static int m_init_threadid; // 初始化线程id
    thread_func m_function;
};


class threadpool
{
public:
    threadpool();
    ~threadpool();
    
    /*设置线程池模式接口*/
    void start_threadpool(int thread_size = 8);
    void set_mode(PoolMode mode);
    void set_thread_size(int size); 
    void set_thread_threshould(int threshould); 

private:
    /*线程池参数*/
    std::unordered_map<int , std::unique_ptr<thread>> m_thread_pool; // 线程池
    
    std::atomic_bool pool_running_state; // 线程池的状态
    PoolMode pool_mode; // 模式





};


