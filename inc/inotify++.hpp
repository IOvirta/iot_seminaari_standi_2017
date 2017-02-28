/*
 * C++ wrapper for inotify
 */

#ifndef INOTIFYPP_INOTIFY_HPP
#define INOTIFYPP_INOTIFY_HPP

#include <string>
#include <map>
#include <queue>
#include <list>
#include <functional>
#include <algorithm>

#ifdef INOTIFYPP_THREADS
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#endif // INOTIFYPP_THREADS

#include <cerrno>
#include <cstdlib>

#include <unistd.h>
#include <sys/poll.h>
#include <sys/stat.h>
#include <sys/inotify.h>
#include <linux/limits.h>

namespace inotify
{
namespace detail
{
bool is_directory(const std::string &path)
{
    struct stat s;
    return stat(path.c_str(), &s) == 0 && S_ISDIR(s.st_mode);
}
} // namespace detail

struct Event;
struct WatchDescriptor
{
    int wd;                 /* inotify watch descriptor */
    std::string name;       /* file which is monitored */
    bool is_directory;
    std::function<void(Event*)> callback;
    uint32_t mask;

    WatchDescriptor() {}
    WatchDescriptor(int wd, const std::string &name) : WatchDescriptor(wd, name, nullptr) {}
    WatchDescriptor(int wd, const std::string &name, std::function<void(Event*)> callback) : wd(wd), name(name), is_directory(detail::is_directory(name)), callback(callback) {}
}; // struct WatchDescriptor

struct Event
{
    WatchDescriptor watch;
    std::string name;       /*  */
    uint32_t mask;          /* mask of events */
    uint32_t cookie;        /* unique cookie associating related events */
    Event() : watch(-1, "") {}
    Event(const WatchDescriptor &watch, const std::string &name, uint32_t mask, uint32_t cookie) :
        watch(watch), name(name), mask(mask), cookie(cookie) {}
    explicit operator bool()
    {
        return watch.wd >= 0;
    }
}; // struct Event

class Inotify
{
private:
    int inotify_fd_;
    pollfd poll_fd_;
    std::map<int, WatchDescriptor> watch_descriptors_;
    char *buffer_ = nullptr;           /* buffer for events */
    std::size_t buffer_size_;
    std::queue<Event> events_;

#ifdef INOTIFYPP_THREADS
    std::thread poll_thread_;
    std::mutex events_mutex_;
    std::mutex watch_descriptors_mutex_;
    std::atomic<bool> thread_run_;
    std::condition_variable wait_events_cv_;
#endif // INOTIFYPP_THREADS

#ifdef INOTIFYPP_THREADS
    void async_poll_events()
    {
        while (thread_run_.load())
        {
            if (poll(&poll_fd_, 1, 50) <= 0) /* TODO: errors */
                continue;

            handle_events(read(inotify_fd_, buffer_, buffer_size_));
            wait_events_cv_.notify_all();
        }
    }
#endif // INOTIFYPP_THREADS

    void handle_events(int data_length)
    {
#ifdef INOTIFYPP_THREADS
        std::lock_guard<std::mutex> lock1(events_mutex_);
        std::lock_guard<std::mutex> lock2(watch_descriptors_mutex_);
#endif // INOTIFYPP_THREADS
        for (int i = 0; i < data_length;)
        {
            inotify_event *e = (inotify_event *)&buffer_[i];

            WatchDescriptor wdesc = watch_descriptors_[e->wd];
            Event event(wdesc, "", e->mask, e->cookie);

            if (e->len > 0)
                event.name = e->name;

            if (wdesc.callback != nullptr)
                wdesc.callback(&event);
            else
                events_.push(event);

            if (wdesc.mask & IN_ONESHOT || e->mask & IN_IGNORED)
                watch_descriptors_.erase(e->wd);

            i += sizeof(inotify_event) + e->len;
        }
    }

public:
    Inotify() : Inotify(0) {}

    Inotify(int flags, std::size_t buffer_size = 64) : inotify_fd_(inotify_init1(flags)), buffer_size_(buffer_size * (sizeof(inotify_event) + NAME_MAX + 1))
    {
        if (inotify_fd_ < 0)
        {
            switch (errno)
            {
                case EINVAL:
                    throw std::runtime_error("inotify: a invalid value was specified in flags");
                case EMFILE:
                    throw std::runtime_error("inotify: the user limit on the total number of file descriptors has been reached");
                case ENFILE:
                    throw std::runtime_error("inotify: the system limit on the total number of file descriptors has been reached");
                case ENOMEM:
                    throw std::runtime_error("inotify: insufficient kernel memory is available");
            }
        }

        poll_fd_ = {inotify_fd_, POLLIN, 0};
        buffer_ = new char[buffer_size_];
    }
    ~Inotify()
    {
#ifdef INOTIFYPP_THREADS
        if (poll_thread_.joinable())
        {
            thread_run_.store(false);
            poll_thread_.join();
        }
#endif // INOTIFYPP_THREADS

        if (buffer_ == nullptr)
            delete[] buffer_;

        for (auto &watch : watch_descriptors_)
            inotify_rm_watch(inotify_fd_, watch.first);

        close(inotify_fd_);
    }

    bool add_watch(const std::string &name, uint32_t mask, WatchDescriptor *desc = nullptr)
    {
        int wd = inotify_add_watch(inotify_fd_, name.c_str(), mask);

        if (wd < 0)
            return false; // TODO error

#ifdef INOTIFYPP_THREADS
        std::lock_guard<std::mutex> lock(watch_descriptors_mutex_);
#endif // INOTIFYPP_THREADS
        watch_descriptors_[wd] = WatchDescriptor(wd, name);
        watch_descriptors_[wd].mask = mask;

        if (desc != nullptr)
            *desc = watch_descriptors_[wd];

        return true;
    }

#ifdef INOTIFYPP_THREADS
    bool add_async_watch(const std::string &name, uint32_t mask, std::function<void(Event*)> callback, WatchDescriptor *desc = nullptr)
    {
        if (!poll_thread_.joinable())
        {
            /* thread not running yet so start it */
            thread_run_.store(true);
            poll_thread_ = std::thread(&Inotify::async_poll_events, this);
        }

        if (callback == nullptr)
            return false; /* TODO add error */

        int wd = inotify_add_watch(inotify_fd_, name.c_str(), mask);

        if (wd < 0)
            return false;

        std::lock_guard<std::mutex> lock(watch_descriptors_mutex_);
        watch_descriptors_[wd] = WatchDescriptor(wd, name, callback);
        watch_descriptors_[wd].mask = mask;

        if (desc != nullptr)
            *desc = watch_descriptors_[wd];

        return true;
    }
#endif // INOTIFYPP_THREADS

    bool poll_events(int timeout = 0)
    {
#ifdef INOTIFYPP_THREADS
        if (poll_thread_.joinable())
        {
            /* if poll thread is running events are polled in that thread */
            std::lock_guard<std::mutex> lock(events_mutex_);
            return !events_.empty();
        }
#endif // INOTIFYPP_THREADS
        int ret = poll(&poll_fd_, 1, timeout);
        if (ret < 0)
            return false; // TODO error
        else if (ret == 0)
            return false;

        handle_events(read(inotify_fd_, buffer_, buffer_size_));
        return true;
    }

    void remove_watch(const WatchDescriptor &wd)
    {
#ifdef INOTIFYPP_THREADS
        std::lock_guard<std::mutex> lock(watch_descriptors_mutex_);
#endif // INOTIFYPP_THREADS
        inotify_rm_watch(inotify_fd_, wd.wd);
        watch_descriptors_.erase(wd.wd);
    }

    void wait_events()
    {
#ifdef INOTIFYPP_THREADS
        if (poll_thread_.joinable())
        {
            std::unique_lock<std::mutex> lock(events_mutex_);
            wait_events_cv_.wait(lock, [this]{ return !events_.empty(); });
            return;
        }
#endif // INOTIFYPP_THREADS

        handle_events(read(inotify_fd_, buffer_, buffer_size_));
    }

    Event get_event()
    {
#ifdef INOTIFYPP_THREADS
        std::lock_guard<std::mutex> lock(events_mutex_);
#endif // INOTIFYPP_THREADS
        if (events_.empty())
            return Event();

        Event ret = events_.front();
        events_.pop();

        return ret;
    }

    std::list<WatchDescriptor> watch_list()
    {
#ifdef INOTIFYPP_THREADS
        std::lock_guard<std::mutex> lock(watch_descriptors_mutex_);
#endif // INOTIFYPP_THREADS
        std::list<WatchDescriptor> list;
        std::transform(watch_descriptors_.begin(), watch_descriptors_.end(), std::back_inserter(list), [](const std::map<int, WatchDescriptor>::value_type &v) {return v.second;});
        return list;
    }
}; // class Inotify
} // namespace inotify

#endif // INOTIFYPP_INOTIFY_HPP
