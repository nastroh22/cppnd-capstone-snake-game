#ifndef QUEUE_H
#define QUEUE_H

#include <vector>
#include <string>
#include <deque>
#include <mutex>
#include <thread>
#include <iostream>
#include <optional>
#include <atomic>
#include <memory>

/* 
    MesageQueue is to comunicate positions between main thread and planner 
    thread. Queue may also be used to communicate between game and io 
    threads. Following pattern from module 4 on concurrency. Message 
    queue is header-only as it is a relativley simple definition.
*/

// Possibly Move this to Utils as Well


template <typename T>
class MessageQueue {
public:
    MessageQueue(std::shared_ptr<std::atomic<bool>> shutdown_flag) : _shutdown_flag(shutdown_flag) {};
    ~MessageQueue() = default;


    // queue needs a send function
    // to me feels more intuitive to actually call this "receive" since it is called by the consumer
    std::optional<T> send() {

        // review: needs a mutex
        std::unique_lock<std::mutex> lock(_mutex);
        _cond1.wait(lock, [this] { 
            return (!_messages.empty() || _shutdown_flag->load());}); // wait until not empty or shutdown is true
        if (_shutdown_flag->load()){
            std::cout << "Shutdown in send q received? " << std::endl;
            _cond2.notify_all();
            return std::nullopt;
        }
        T msg = std::move(_messages.back());
        _messages.pop_back();
        _cond2.notify_one();
        return msg;
    };
    
    void receive(T&& msg) {

        std::unique_lock<std::mutex> lock(_mutex);
        _cond2.wait(lock, [this] { 
            return (_messages.size() <= _maxSize || _shutdown_flag->load());});
        if (_shutdown_flag->load()){
            std::cout << "Shutdown in receive q received? " << std::endl;
            _cond1.notify_all(); // (Found bug) No need for it to be empty, just notify all to shutdown
            return;
        }
        _messages.push_front(std::move(msg));
        _cond1.notify_one(); // notify client that a new message is available
    };

    void size(){
        std::lock_guard<std::mutex> lock(_mutex);
        std::cout << "Queue Size: " << _messages.size() << std::endl;
    }

    void clear(){
        std::lock_guard<std::mutex> lock(_mutex);
        _messages.clear();
    }

    void shutdown(){
        // std::lock_guard<std::mutex> lock(_mutex);
        // _shutdown_flag->store(true); // redundant I think 
        _messages.clear();
        _cond1.notify_all();
        _cond2.notify_all();
    };

    // queue needs a receive function


private:
    std::mutex _mutex;
    std::deque<T> _messages;
    std::condition_variable _cond1;
    std::condition_variable _cond2;
    size_t _maxSize = 5;
    std::shared_ptr<std::atomic<bool>> _shutdown_flag;

};

#endif