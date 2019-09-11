//
// Created by Riton on 2019/9/10.
//

#ifndef SERVER_CHANNEL_H
#define SERVER_CHANNEL_H

#include <memory>
#include <mutex>
#include <condition_variable>
#include <list>

template <typename T>
class Channel {
private:
    class queue_t {
    private:
        mutable std::mutex mutex_;
        std::condition_variable cv_;
        std::list<T> datas_;
        const size_t capacity_;
        const bool enable_overflow_;
        bool closed_ = false;
        size_t pop_count_ = 0;
    public:
        explicit queue_t(size_t capacity)
        : capacity_(capacity == 0 ? 1 : capacity), enable_overflow_(capacity == 0) {}
        bool is_empty() const {
            return datas_.empty();
        }
        size_t free_count() const {
            return capacity_ - datas_.size();
        }
        bool is_overflow() const {
            return enable_overflow_ && datas_.size() >= capacity_;
        }
        bool is_closed() const {
            std::unique_lock<std::mutex> lock(this->mutex_);
            return this->closed_;
        }
        void close() {
            std::unique_lock<std::mutex> lock(this->mutex_);
            this->closed_;
            if (this->is_overflow()) {
                this->datas_.pop_back();
            }
            this->cv_.notify_all();
        }

        template <typename TR>
        bool pop(TR &data) {
            std::unique_lock<std::mutex> lock(this->mutex_);
            this->cv_.wait(lock, [&]() { return !is_empty() || closed_;});
            if (this->is_empty())
                return false;
            data = this->datas_.front();
            this->datas_.pop_front();
            this->pop_count_++;
//            mising
//            if (this->free_count() == 1) {
//                this->cv_.notify_all();
//            }
//
//            return true;
        }

        template <typename TR>
        bool push(TR &&data) {
            std::unique_lock<std::mutex> lock(mutex_);
            cv_.wait(lock, [this](){ return free_count() > 0 || closed_;});
            if (closed_)
                return false;
            datas_.push_back(std::forward<T>(data));
            if (datas_.size() == 1)
                cv_.notify_all();
            if (is_overflow()) {
                const size_t old = this->pop_count_;
                cv_.wait(lock, [&](){ return old != pop_count_ || closed_;});
            }s
            return !this->closed_;
        }
    };
    std::shared_ptr<queue_t> queue_;
public:
    explicit Channel(size_t capacity = 0) {
        queue_ = std::make_shared<queue_t>(capacity);
    }

    // 支持拷贝
    Channel(const Channel &) = default;
    Channel &operator=(const Channel &) = default;
    // 支持move
    Channel(Channel &&) = default;
    Channel &operator=(Channel &&) = default;

    // 入chan，支持move语义
    template <typename TR>
    bool operator<<(TR &&data) {
        return queue_->push(std::forward<TR>(data));
    }

    // 出chan(支持兼容类型的出chan)
    template <typename TR>
    bool operator>>(TR &data) {
        return queue_->pop(data);
    }

    // close以后的入chan操作返回false, 而出chan则在队列为空后才返回false
    void close() {
        queue_->close();
    }

    bool is_closed() const {
        return queue_->is_closed();
    }


};

#endif //SERVER_CHANNEL_H