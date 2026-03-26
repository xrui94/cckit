// test_use_after_free.cpp - 测试 Use-After-Free 修复
#include "cckit/logging/Logging.hpp"

#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>

std::atomic<int> callback_count{0};
std::atomic<bool> running{true};

void test_use_after_free() {
    std::cout << "=== 测试 Use-After-Free 修复 ===" << std::endl;

    using namespace cckit;

    // 线程 1：不断设置和清除回调
    std::thread writer_thread([&]() {
        int counter = 0;
        while (running) {
            logging::setLogCallback(
                [counter](logging::LogLevel level, const logging::SourceLoc& loc,
                         const std::string& msg, void* context) {
                    callback_count++;
                    std::cout << "[Callback " << counter << "] " << msg << std::endl;
                },
                nullptr,
                false
            );

            std::this_thread::sleep_for(std::chrono::milliseconds(10));

            logging::setLogCallback(nullptr, nullptr, false);

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            counter++;
        }
    });

    // 线程 2：不断记录日志
    std::thread reader_thread([&]() {
        int counter = 0;
        while (running) {
            logging::logInfo("Log message " + std::to_string(counter++));
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    });

    // 运行 5 秒
    std::this_thread::sleep_for(std::chrono::seconds(5));

    running = false;

    writer_thread.join();
    reader_thread.join();

    std::cout << "回调调用次数: " << callback_count.load() << std::endl;
    std::cout << "Use-After-Free 测试完成（如果没有崩溃，说明修复成功）" << std::endl;
}

void test_concurrent_callback_access() {
    std::cout << "\n=== 测试并发回调访问 ===" << std::endl;

    using namespace cckit;

    callback_count = 0;
    running = true;

    // 设置初始回调
    logging::setLogCallback(
        [](logging::LogLevel level, const logging::SourceLoc& loc,
           const std::string& msg, void* context) {
            callback_count++;
        },
        nullptr,
        false
    );

    // 多个线程并发记录日志
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([i]() {
            int counter = 0;
            while (running) {
                logging::logInfo("Thread " + std::to_string(i) + " message " + std::to_string(counter++));
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        });
    }

    // 主线程不断更换回调
    int callback_counter = 0;
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        logging::setLogCallback(
            [callback_counter](logging::LogLevel level, const logging::SourceLoc& loc,
                             const std::string& msg, void* context) {
                callback_count++;
            },
            nullptr,
            false
        );

        callback_counter++;
    }

    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "回调调用次数: " << callback_count.load() << std::endl;
    std::cout << "并发访问测试完成" << std::endl;
}

void test_callback_lifetime() {
    std::cout << "\n=== 测试回调生命周期 ===" << std::endl;

    using namespace cckit;

    // 设置回调
    logging::setLogCallback(
        [](logging::LogLevel level, const logging::SourceLoc& loc,
           const std::string& msg, void* context) {
            std::cout << "Callback 1: " << msg << std::endl;
        },
        nullptr,
        false
    );

    logging::logInfo("Message 1");

    // 更换回调
    logging::setLogCallback(
        [](logging::LogLevel level, const logging::SourceLoc& loc,
           const std::string& msg, void* context) {
            std::cout << "Callback 2: " << msg << std::endl;
        },
        nullptr,
        false
    );

    logging::logInfo("Message 2");

    // 清除回调
    logging::setLogCallback(nullptr, nullptr, false);

    logging::logInfo("Message 3 (should not trigger callback)");

    std::cout << "生命周期测试完成" << std::endl;
}

int main() {
    std::cout << "开始 Use-After-Free 测试..." << std::endl;

    using namespace cckit;

    logging::initializeLogging();

    test_use_after_free();
    test_concurrent_callback_access();
    test_callback_lifetime();

    logging::shutdownLogging();

    std::cout << "\n所有测试完成（如果没有崩溃，说明 Use-After-Free 问题已修复）" << std::endl;
    return 0;
}
