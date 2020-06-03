#include "ConcurrentBuffer.h"

#include <algorithm>
#include <cmath>
#include <iostream>

std::optional<std::string> ConcurrentBuffer::popFront() {
    std::lock_guard<std::mutex> lock(mutex);
    if (!entries.empty()) {
        auto front = entries.front();
        entries.pop_front();
        return front;
    }
    return {};
}

std::vector<std::string> ConcurrentBuffer::popAll() {
    std::lock_guard<std::mutex> lock(mutex);
    std::vector<std::string> result(entries.begin(), entries.end());
    entries.clear();
    return result;
}

void ConcurrentBuffer::pushBack(const std::string &s) {
    std::lock_guard<std::mutex> lock(mutex);
    entries.push_back(s);
}

void ConcurrentBuffer::pushBackAll(std::vector<std::string> v) {
    std::lock_guard<std::mutex> lock(mutex);
    entries.insert(entries.end(), v.begin(), v.end());
}

std::vector<std::string> ConcurrentBuffer::getLastNLines(size_t lineWidth, ssize_t n) {
    std::lock_guard<std::mutex> lock(mutex);

    std::vector<std::string> result;

    if (n < 0) {
		for (const auto &entry : entries) {
			result.emplace_back(entry);
		}
		return result;
    }

    size_t totalLines = 0, entryLines = 0;
    for (auto it = entries.rbegin(); it != entries.rend(); it++) {
        entryLines = ((*it).size() / lineWidth) + (((*it).size() % lineWidth) > 0);
        if (totalLines + entryLines <= (size_t)n) {
            totalLines += entryLines;
            result.push_back(*it);
        } else {
            break;
        }
    }

    std::reverse(result.begin(), result.end());
    return result;
}