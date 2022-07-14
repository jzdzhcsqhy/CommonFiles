#pragma once

#include <mutex>
#include <queue>
#include <iostream>
#include <condition_variable>
using namespace std;

namespace JP
{
template<typename T = int>
class threadsafe_queue
{
    using milli_t = std::chrono::milliseconds;
public:
    threadsafe_queue( int iSize = 8 )
        :qSize(iSize)
    {

    }
    void push(T& new_value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if (size_flag && data_queue.size() >= static_cast<size_t>(qSize) ) {
            //return;
            data_queue.pop();
            //while (!data_queue.empty()) { data_queue.pop();}
        }
        data_queue.push(new_value);
        data_cond.notify_one();
    }
    bool pop(T& value, int timeout = 1)
    {
        std::unique_lock<std::mutex> lk(mut);
        milli_t ms(timeout);
        bool flag = data_cond.wait_for(lk, ms, [this] {return !data_queue.empty(); });
        if (!flag) return false;


        value = data_queue.front();
        data_queue.pop();

        while (size_flag && data_queue.size() > static_cast<size_t>(qSize) ) {
            value = data_queue.front();
            data_queue.pop();
        }
        //std::cout<<"queue pop " <<data_queue .size()<<" left"<<  std::endl;
        return true;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }
    int size() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return (int)data_queue.size();
    }

    void clear()
    {
        std::lock_guard<std::mutex> lk(mut);
        while (!data_queue.empty())
        {
            data_queue.pop();
        }
    }
    void setflag() {
        size_flag = false;
    }


    void setSize( int iMaxQueueSize )
    {
        qSize = iMaxQueueSize;
    }
private:
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;

    int qSize = 1;
    bool size_flag = true;
};

};

