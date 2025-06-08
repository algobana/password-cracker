#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <limits>

constexpr const char charset[] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "0123456789"
    "!@#$%^&*()-_=+";

constexpr size_t charset_len = sizeof(charset) - 1;
constexpr const char target[] = "A2#ka";
constexpr size_t target_len = sizeof(target) - 1;

std::mutex mtx;
bool found = false;
std::string result;

void bruteForceRecursive(std::string& current, size_t maxLength,
                         const std::string& localCharset) {
    if (found) return;

    if (current.length() == maxLength) {
        if (current == target) {
            std::lock_guard<std::mutex> lock(mtx);
            found = true;
            result = current;
        }
        return;
    }

    for (char c : localCharset) {
        if (found) return;
        current.push_back(c);
        bruteForceRecursive(current, maxLength, localCharset);
        current.pop_back();
    }
}

void threadWorker(size_t threadId, size_t threadCount, size_t maxLength) {
    std::string current;
    std::string threadCharset;

    for (size_t i = threadId; i < charset_len; i += threadCount) {
        threadCharset += charset[i];
    }

    for (char c : threadCharset) {
        current = c;
        bruteForceRecursive(current, maxLength, charset);
        if (found) break;
    }
}

double runWithThreads(unsigned int threadCount) {
    found = false;
    result.clear();

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    for (unsigned int i = 0; i < threadCount; ++i) {
        threads.emplace_back(threadWorker, i, threadCount, target_len);
    }
    for (auto& t : threads) t.join();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    return elapsed.count();
}

int main() {
    std::vector<unsigned int> threadOptions = {1, 2, 4, 8, 12, 16, 20};
    double bestTime = std::numeric_limits<double>::max();
    unsigned int bestThreadCount = 1;

    std::cout << "Auto-tuning started...\n";
    for (unsigned int count : threadOptions) {
        double duration = runWithThreads(count);
        std::cout << count << " threads took " << duration << " seconds.\n";

        if (duration < bestTime) {
            bestTime = duration;
            bestThreadCount = count;
        }
    }

    std::cout << "\nBest thread count: " << bestThreadCount
              << " (" << bestTime << " seconds)\n";

    // Final run with best thread count (optional, for reporting)
    found = false;
    result.clear();
    auto finalStart = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> finalThreads;
    for (unsigned int i = 0; i < bestThreadCount; ++i) {
        finalThreads.emplace_back(threadWorker, i, bestThreadCount, target_len);
    }
    for (auto& t : finalThreads) t.join();

    auto finalEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> finalElapsed = finalEnd - finalStart;

    if (found) {
        std::cout << "Password found: " << result << "\n";
    } else {
        std::cout << "Password not found.\n";
    }

    std::cout << "Final execution time: " << finalElapsed.count() << " seconds\n";

    return 0;
}

