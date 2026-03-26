// test_deadlock_prevention.cpp - 测试死锁防护机制
#include "cckit/logging/Logging.hpp"

#include <iostream>
#include <thread>
#include <chrono>

void test_recursive_logging() {
    std::cout << "=== 测试递归日志（死锁防护） ===" << std::endl;

    using namespace cckit;

    // 设置回调，在回调内部再次调用日志
    logging::setLogCallback(
        [](logging::LogLevel level, const logging::SourceLoc& loc,
           const std::string& msg, void* context) {
            std::cout << "[Callback] 收到日志: " << msg << std::endl;

            // 在回调内部再次调用日志（可能导致死锁的场景）
            std::cout << "[Callback] 尝试递归日志..." << std::endl;
            logging::logInfo("This is a recursive log inside callback");
            std::cout << "[Callback] 递归日志完成" << std::endl;
        },
        nullptr,
        false
    );

    // 主日志调用
    std::cout << "[Main] 调用主日志..." << std::endl;
    logging::logInfo("Main log message");
    std::cout << "[Main] 主日志完成" << std::endl;

    // 清除回调
    logging::setLogCallback(nullptr, nullptr, false);

    std::cout << "递归日志测试完成（应该没有死锁）" << std::endl;
}

void test_deep_recursion() {
    std::cout << "\n=== 测试深层递归 ===" << std::endl;

    using namespace cckit;

    int depth = 0;
    logging::setLogCallback(
        [&depth](logging::LogLevel level, const logging::SourceLoc& loc,
                 const std::string& msg, void* context) {
            std::cout << "[Callback Depth " << depth << "] " << msg << std::endl;

            if (depth < 3) {
                depth++;
                logging::logInfo("Recursive call depth " + std::to_string(depth));
                depth--;
            }
        },
        nullptr,
        false
    );

    depth = 1;
    logging::logInfo("Initial call depth 1");

    logging::setLogCallback(nullptr, nullptr, false);

    std::cout << "深层递归测试完成" << std::endl;
}

void test_multithreaded_recursion() {
    std::cout << "\n=== 测试多线程递归 ===" << std::endl;

    using namespace cckit;

    logging::setLogCallback(
        [](logging::LogLevel level, const logging::SourceLoc& loc,
           const std::string& msg, void* context) {
            std::cout << "[Callback] " << msg << std::endl;
            logging::logInfo("Recursive log from " + std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id())));
        },
        nullptr,
        false
    );

    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([i]() {
            logging::logInfo("Thread " + std::to_string(i) + " message");
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    logging::setLogCallback(nullptr, nullptr, false);

    std::cout << "多线程递归测试完成" << std::endl;
}

int main() {
    std::cout << "开始死锁防护测试..." << std::endl;

    using namespace cckit;

    logging::initializeLogging();

    test_recursive_logging();
    test_deep_recursion();
    test_multithreaded_recursion();

    logging::shutdownLogging();

    std::cout << "\n所有测试完成（如果没有死锁，说明修复成功）" << std::endl;
    return 0;
}
