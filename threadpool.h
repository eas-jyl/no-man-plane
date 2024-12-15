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

class Any
{
public:
    Any() = default;
    ~Any() = default;

    Any(const Any&) = delete;
    Any& operator=(const Any&) = default;

    Any(const Any&&) = delete;
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

class Task
{
public:

private:
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


