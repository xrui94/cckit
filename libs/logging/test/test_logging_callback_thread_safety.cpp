// test_callback_thread_safety.cpp - 测试回调线程安全和内存管理
#include "cckit/logging/Logging.hpp"

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <atomic>

std::atomic<int> callback_count{0};

void test_thread_safety() {
    std::cout << "=== 测试线程安全性 ===" << std::endl;

    using namespace cckit;

    // 创建多个线程同时设置回调
    std::vector<std::thread> threads;

    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([i]() {
            for (int j = 0; j < 100; ++j) {
                // 设置回调
                logging::setLogCallback(
                    [i, j](logging::LogLevel level, const logging::SourceLoc& loc,
                           const std::string& msg, void* context) {
                        callback_count++;
                    },
                    nullptr,
                    false
                );

                // 记录日志
                logging::logInfo("Thread " + std::to_string(i) + " iteration " + std::to_string(j));

                std::this_thread::sleep_for(std::chrono::microseconds(100));
            }
        });
    }

    // 等待所有线程完成
    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "回调调用次数: " << callback_count.load() << std::endl;
    std::cout << "线程安全测试完成" << std::endl;
}

void test_memory_management() {
    std::cout << "\n=== 测试内存管理 ===" << std::endl;

    using namespace cckit;

    // 多次设置和清除回调
    for (int i = 0; i < 1000; ++i) {
        logging::setLogCallback(
            [i](logging::LogLevel level, const logging::SourceLoc& loc,
                 const std::string& msg, void* context) {
                // 回调逻辑
            },
            nullptr,
            false
        );

        // 记录日志
        logging::logInfo("Iteration " + std::to_string(i));

        // 清除回调
        logging::setLogCallback(nullptr, nullptr, false);
    }

    std::cout << "内存管理测试完成（应该没有内存泄漏）" << std::endl;
}

void test_callback_lifetime() {
    std::cout << "\n=== 测试回调生命周期 ===" << std::endl;

    using namespace cckit;

    // 设置回调
    logging::setLogCallback(
        [](logging::LogLevel level, const logging::SourceLoc& loc,
            const std::string& msg, void* context) {
                std::cout << "收到日志: " << msg << std::endl;
        },
        nullptr,
        false
    );

    // 记录日志
    logging::logInfo("测试消息 1");
    logging::logInfo("测试消息 2");

    // 清除回调
    logging::setLogCallback(nullptr, nullptr, false);

    // 记录日志（应该不会触发回调）
    logging::logInfo("这条消息不应该触发回调");

    std::cout << "回调生命周期测试完成" << std::endl;
}

int main() {
    std::cout << "开始测试..." << std::endl;

    using namespace cckit;

    logging::initializeLogging();

    test_thread_safety();
    test_memory_management();
    test_callback_lifetime();

    logging::shutdownLogging();

    std::cout << "\n所有测试完成" << std::endl;
    return 0;
}
