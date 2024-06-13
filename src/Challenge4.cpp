#include <chrono>
#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx;

class TestObject
{
public:
    TestObject()
    : m_val(2)
    {
    }

    virtual ~TestObject()
    {
        std::cout << "Destroying TestObject" << std::endl;
    }

    void dump()
    {
        std::cout << "m_val: " << m_val << std::endl;
    }
private:
    int m_val;
};

void workerThread()
{
    std::lock_guard<std::mutex> lock(mtx);

    // Simulate some work
    for (int i = 0; i < 5; i++)
    {
        std::cout << "Worker thread: " << std::this_thread::get_id() << " - " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    // Simulate a fault that causes premature exit
    std::cout << "Worker thread: " << std::this_thread::get_id() << " - Fault occurred, exiting prematurely" << std::endl;
    TestObject* testObject = new TestObject();
    testObject->dump();
    delete testObject;
    testObject = nullptr;
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    return; // The thread will exit here while the mutex is still locked
}

int main()
{
    std::thread t1(workerThread);
    std::thread t2(workerThread);

    t1.join();
    t2.join();

    return 0;
}
