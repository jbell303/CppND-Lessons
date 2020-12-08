#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <algorithm>
#include <queue>

template<class T> 
class MessageQueue
{
public:
    MessageQueue() {}

    T receive()
    {
        // perform vector modification under the lock
        std::unique_lock<std::mutex> uLock(_mutex);
        _cond.wait(uLock, [this] { return !_messages.empty(); }); // pass unique lock to condition variable

        // remove last vector element from queue
        T message = std::move(_messages.front());
        _messages.pop_front();

        return message; // will not be copied due to return value optimization (RVO) in C++
    }

    void send(T &&message)
    {
        // simulate some work
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // perform vector modification under the lock
        std::lock_guard<std::mutex> uLock(_mutex);

        // add vector to queue
        //std::cout << "   Message #" << message << " will be added to the queue" << std::endl;
        _messages.push_back(std::move(message));
        _cond.notify_one(); // notify client after pushing new Vehicle into vector
    }

private:
    std::mutex _mutex;
    std::condition_variable _cond;
    std::deque<T> _messages; // list of all vehicles waiting to enter this intersection
};

int main()
{
    // create monitor object as a shared pointer to enable access by multiple threads
    std::shared_ptr<MessageQueue<int>> buildQueue(new MessageQueue<int>);
    std::shared_ptr<MessageQueue<int>> processQueue(new MessageQueue<int>);
    std::shared_ptr<MessageQueue<int>> processPairQueue(new MessageQueue<int>);
    int ctr = 0;

    std::cout << "Spawning threads..." << std::endl;
    std::thread buildThread([&]() {
        for (size_t i; i < 10; i++) {
            buildQueue->send(std::move(i));
            std::cout << "   Message: #" << i << " sent" << std::endl;
        }
    });

    std::thread processingThread([&]() {
        while (ctr < 10) {
            int i = buildQueue->receive();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            int j = i;
            std::cout << "   Message #" << i << " has been processed" << std::endl;
            processQueue->send(std::move(i));
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            processPairQueue->send(std::move(j));
        }
        
    });


    std::cout << "Collecting results..." << std::endl;
    while (ctr < 10)
    {
        int message = processQueue->receive();
        int pair = processPairQueue->receive();
        std::cout << "   Message pair #" << message << "/" << pair << " have been removed from the queue" << std::endl;
        ctr++;
    }

    // std::for_each(futures.begin(), futures.end(), [](std::future<void> &ftr) {
    //     ftr.wait();
    //});
    buildThread.join();
    processingThread.join();

    std::cout << "Finished!" << std::endl;

    return 0;
}