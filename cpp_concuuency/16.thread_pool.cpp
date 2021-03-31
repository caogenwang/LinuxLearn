#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>
using namespace std;
class thread_pool {
    std::mutex m;
    std::condition_variable cv;
    bool done = false; // 线程池析构标志
    std::queue<std::function<void()>> q; // 存储任务的队列
public:
    explicit thread_pool(unsigned n) // n为线程数
    {
        for (unsigned i = 0; i < n; ++i)
        {
            std::thread{
                [this]
                {
                    std::unique_lock l(m);
                    for (;;)
                    {
                        if (!q.empty())
                        {
                            auto task = std::move(q.front());
                            q.pop();
                            l.unlock();
                            task();
                            l.lock();
                        }
                        else if (done) // if (q.empty() && done)
                        {
                            break;
                        }
                        else // if (q.empty() && !done)
                        {
                            cv.wait(l);
                        } // 等价于cv.wait(l, [this] { return done || q.empty(); });
                    }
                }
            }.detach();
        }
    }

    ~thread_pool()
    {
        {
            std::scoped_lock l(m);
            done = true; // 加锁的原因是cv.wait使用了done作为判断条件
        }
        cv.notify_all();
    }

    template<typename F>
    void submit(F&& f)
    {
        {
            std::scoped_lock l(m);
            q.emplace(std::forward<F>(f));
        }
        cv.notify_one();
    }
};