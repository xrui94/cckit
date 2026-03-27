// test_fs_performance.cpp - 文件系统性能测试
#include <cckit/fs/fs.hpp>
#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include <algorithm>

void test_file_creation_performance()
{
    std::cout << "测试文件创建性能..." << std::endl;

    const int count = 1000;
    std::string testDir = "perf_test_files";
    cckit::fs::ensurePathExists(testDir);

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < count; ++i)
    {
        std::string filename = testDir + "/file_" + std::to_string(i) + ".txt";
        std::ofstream file(filename);
        file << "test content " << i;
        file.close();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "创建 " << count << " 个文件耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "平均每个文件耗时: " << (duration.count() * 1000.0 / count) << " μs" << std::endl;

    // 清理
    cckit::fs::removeDirectoryAll(testDir);

    std::cout << "✅ 文件创建性能测试通过" << std::endl;
}

void test_file_read_performance()
{
    std::cout << "测试文件读取性能..." << std::endl;

    const int count = 1000;
    std::string testDir = "perf_test_read";
    cckit::fs::ensurePathExists(testDir);

    // 创建测试文件
    std::vector<std::string> filenames;
    for (int i = 0; i < count; ++i)
    {
        std::string filename = testDir + "/file_" + std::to_string(i) + ".txt";
        std::ofstream file(filename);
        file << "test content " << i << " with some more data to read";
        file.close();
        filenames.push_back(filename);
    }

    // 测试文件存在检查性能
    auto start = std::chrono::high_resolution_clock::now();

    for (const auto& filename : filenames)
    {
        bool exists = cckit::fs::exists(filename);
        (void)exists;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "检查 " << count << " 个文件存在性耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "平均每次检查耗时: " << (duration.count() * 1000.0 / count) << " μs" << std::endl;

    // 测试文件大小获取性能
    start = std::chrono::high_resolution_clock::now();

    for (const auto& filename : filenames)
    {
        auto size = cckit::fs::getFileSize(filename);
        (void)size;
    }

    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "获取 " << count << " 个文件大小耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "平均每次获取耗时: " << (duration.count() * 1000.0 / count) << " μs" << std::endl;

    // 清理
    cckit::fs::removeDirectoryAll(testDir);

    std::cout << "✅ 文件读取性能测试通过" << std::endl;
}

void test_path_operations_performance()
{
    std::cout << "测试路径操作性能..." << std::endl;

    const int count = 10000;
    std::vector<std::string> paths;

    // 生成测试路径
    for (int i = 0; i < count; ++i)
    {
        paths.push_back("/path/to/directory/file_" + std::to_string(i) + ".txt");
    }

    // 测试 getFileName 性能
    auto start = std::chrono::high_resolution_clock::now();

    for (const auto& path : paths)
    {
        std::string filename = cckit::fs::getFileName(path);
        (void)filename;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "获取 " << count << " 个文件名耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "平均每次操作耗时: " << (duration.count() * 1000.0 / count) << " μs" << std::endl;

    // 测试 getFileBase 性能
    start = std::chrono::high_resolution_clock::now();

    for (const auto& path : paths)
    {
        std::string base = cckit::fs::getFileBase(path);
        (void)base;
    }

    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "获取 " << count << " 个文件基名耗时: " << duration.count() << " ms" << std::endl;

    // 测试 getFileSuffix 性能
    start = std::chrono::high_resolution_clock::now();

    for (const auto& path : paths)
    {
        std::string suffix = cckit::fs::getFileSuffix(path);
        (void)suffix;
    }

    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "获取 " << count << " 个文件扩展名耗时: " << duration.count() << " ms" << std::endl;

    std::cout << "✅ 路径操作性能测试通过" << std::endl;
}

void test_directory_listing_performance()
{
    std::cout << "测试目录列表性能..." << std::endl;

    const int count = 1000;
    std::string testDir = "perf_test_listing";
    cckit::fs::ensurePathExists(testDir);

    // 创建测试文件
    for (int i = 0; i < count; ++i)
    {
        std::string filename = testDir + "/file_" + std::to_string(i) + ".txt";
        std::ofstream file(filename);
        file << "test content " << i;
        file.close();
    }

    // 测试 listFiles 性能（无过滤）
    auto start = std::chrono::high_resolution_clock::now();

    auto files = cckit::fs::listFiles(testDir);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "列出 " << files.size() << " 个文件耗时: " << duration.count() << " ms" << std::endl;

    // 测试 listFiles 性能（带过滤）
    std::set<std::string> extensions = {".txt"};
    start = std::chrono::high_resolution_clock::now();

    auto txtFiles = cckit::fs::listFiles(testDir, extensions);

    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "列出 " << txtFiles.size() << " 个 .txt 文件耗时: " << duration.count() << " ms" << std::endl;

    // 清理
    cckit::fs::removeDirectoryAll(testDir);

    std::cout << "✅ 目录列表性能测试通过" << std::endl;
}

void test_copy_file_performance()
{
    std::cout << "测试文件复制性能..." << std::endl;

    const int count = 100;
    std::string testDir = "perf_test_copy";
    cckit::fs::ensurePathExists(testDir);

    // 创建源文件
    std::string srcFile = testDir + "/source.txt";
    {
        std::ofstream file(srcFile);
        for (int i = 0; i < 10000; ++i)
        {
            file << "This is test data for file copy performance testing. ";
        }
        file.close();
    }

    auto fileSize = cckit::fs::getFileSize(srcFile);
    std::cout << "源文件大小: " << fileSize.value() << " bytes" << std::endl;

    // 测试文件复制性能
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < count; ++i)
    {
        std::string dstFile = testDir + "/copy_" + std::to_string(i) + ".txt";
        cckit::fs::copyFile(srcFile, dstFile);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "复制 " << count << " 个文件耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "平均每个文件耗时: " << (duration.count() * 1000.0 / count) << " ms" << std::endl;

    // 计算复制速度
    double totalBytes = fileSize.value() * count;
    double totalSeconds = duration.count() / 1000.0;
    double speed = totalBytes / totalSeconds / 1024.0 / 1024.0;
    std::cout << "复制速度: " << speed << " MB/s" << std::endl;

    // 清理
    cckit::fs::removeDirectoryAll(testDir);

    std::cout << "✅ 文件复制性能测试通过" << std::endl;
}

void test_directory_creation_performance()
{
    std::cout << "测试目录创建性能..." << std::endl;

    const int count = 1000;
    std::string testDir = "perf_test_dirs";

    // 清理可能存在的旧目录
    if (cckit::fs::exists(testDir))
    {
        cckit::fs::removeDirectoryAll(testDir);
    }

    // 测试单层目录创建性能
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < count; ++i)
    {
        std::string dirname = testDir + "/dir_" + std::to_string(i);
        cckit::fs::createDirectory(dirname);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "创建 " << count << " 个单层目录耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "平均每个目录耗时: " << (duration.count() * 1000.0 / count) << " ms" << std::endl;

    // 清理
    cckit::fs::removeDirectoryAll(testDir);

    // 测试嵌套目录创建性能
    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < count; ++i)
    {
        std::string nestedDir = testDir + "/level1_" + std::to_string(i) + "/level2/level3";
        cckit::fs::ensurePathExists(nestedDir);
    }

    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "创建 " << count << " 个嵌套目录耗时: " << duration.count() << " ms" << std::endl;

    // 清理
    cckit::fs::removeDirectoryAll(testDir);

    std::cout << "✅ 目录创建性能测试通过" << std::endl;
}

int main()
{
    std::cout << "=== 文件系统性能测试 ===" << std::endl;
    std::cout << std::endl;

    test_file_creation_performance();
    std::cout << std::endl;

    test_file_read_performance();
    std::cout << std::endl;

    test_path_operations_performance();
    std::cout << std::endl;

    test_directory_listing_performance();
    std::cout << std::endl;

    test_copy_file_performance();
    std::cout << std::endl;

    test_directory_creation_performance();
    std::cout << std::endl;

    std::cout << "=== 所有性能测试完成! ===" << std::endl;
    return 0;
}
