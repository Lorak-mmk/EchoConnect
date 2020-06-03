#ifndef DEMOAPP_CONCURRENT_BUFFER_H
#define DEMOAPP_CONCURRENT_BUFFER_H

#include <deque>
#include <mutex>
#include <optional>
#include <string>
#include <vector>

class ConcurrentBuffer {
public:
    ConcurrentBuffer(){};
    std::optional<std::string> popFront();
    std::vector<std::string> popAll();
    void pushBack(const std::string &s);
    void pushBackAll(std::vector<std::string> v);
    std::vector<std::string> getLastNLines(size_t lineWidth, ssize_t n);

private:
    std::mutex mutex;
    std::deque<std::string> entries;
};

#endif  // DEMOAPP_CONCURRENT_BUFFER_H