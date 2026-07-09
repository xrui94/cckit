// test_fs_performance.cpp - 文件系统性能测试
#include <cckit/fs/fs.hpp>

#include <gtest/gtest.h>
#include <chrono>
#include <fstream>
#include <vector>

class FsPerfTest : public ::testing::Test {
protected:
    void TearDown() override {
        if (cckit::fs::exists("perf_test_files"))
            cckit::fs::removeDirectoryAll("perf_test_files");
        if (cckit::fs::exists("perf_test_read"))
            cckit::fs::removeDirectoryAll("perf_test_read");
        if (cckit::fs::exists("perf_test_listing"))
            cckit::fs::removeDirectoryAll("perf_test_listing");
        if (cckit::fs::exists("perf_test_copy"))
            cckit::fs::removeDirectoryAll("perf_test_copy");
        if (cckit::fs::exists("perf_test_dirs"))
            cckit::fs::removeDirectoryAll("perf_test_dirs");
    }
};

TEST_F(FsPerfTest, FileCreation)
{
    const int count = 1000;
    std::string testDir = "perf_test_files";
    cckit::fs::ensurePathExists(testDir);

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < count; ++i) {
        std::string filename = testDir + "/file_" + std::to_string(i) + ".txt";
        std::ofstream file(filename);
        file << "test content " << i;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "创建 " << count << " 个文件耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "平均每个文件耗时: " << (duration.count() * 1000.0 / count) << " μs" << std::endl;
    SUCCEED();
}

TEST_F(FsPerfTest, FileRead)
{
    const int count = 1000;
    std::string testDir = "perf_test_read";
    cckit::fs::ensurePathExists(testDir);
    std::vector<std::string> filenames;
    for (int i = 0; i < count; ++i) {
        std::string filename = testDir + "/file_" + std::to_string(i) + ".txt";
        std::ofstream file(filename);
        file << "test content " << i;
        filenames.push_back(filename);
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& filename : filenames) {
        bool exists = cckit::fs::exists(filename);
        (void)exists;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "检查 " << count << " 个文件存在性耗时: " << duration.count() << " ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (const auto& filename : filenames) {
        auto size = cckit::fs::getFileSize(filename);
        (void)size;
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "获取 " << count << " 个文件大小耗时: " << duration.count() << " ms" << std::endl;
    SUCCEED();
}

TEST_F(FsPerfTest, PathOperations)
{
    const int count = 10000;
    std::vector<std::string> paths;
    for (int i = 0; i < count; ++i)
        paths.push_back("/path/to/directory/file_" + std::to_string(i) + ".txt");

    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& path : paths) {
        std::string filename = cckit::fs::getFileName(path);
        (void)filename;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "获取 " << count << " 个文件名耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "平均每次操作耗时: " << (duration.count() * 1000.0 / count) << " μs" << std::endl;
    SUCCEED();
}

TEST_F(FsPerfTest, DirectoryListing)
{
    const int count = 1000;
    std::string testDir = "perf_test_listing";
    cckit::fs::ensurePathExists(testDir);
    for (int i = 0; i < count; ++i) {
        std::string filename = testDir + "/file_" + std::to_string(i) + ".txt";
        std::ofstream file(filename);
        file << "test content " << i;
    }

    auto start = std::chrono::high_resolution_clock::now();
    auto files = cckit::fs::listFiles(testDir);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "列出 " << files.size() << " 个文件耗时: " << duration.count() << " ms" << std::endl;

    std::set<std::string> extensions = {".txt"};
    start = std::chrono::high_resolution_clock::now();
    auto txtFiles = cckit::fs::listFiles(testDir, extensions);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "列出 " << txtFiles.size() << " 个 .txt 文件耗时: " << duration.count() << " ms" << std::endl;
    SUCCEED();
}

TEST_F(FsPerfTest, CopyFile)
{
    const int count = 100;
    std::string testDir = "perf_test_copy";
    cckit::fs::ensurePathExists(testDir);
    std::string srcFile = testDir + "/source.txt";
    {
        std::ofstream file(srcFile);
        for (int i = 0; i < 10000; ++i)
            file << "This is test data for file copy performance testing. ";
    }

    auto fileSize = cckit::fs::getFileSize(srcFile);
    std::cout << "源文件大小: " << fileSize.value() << " bytes" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < count; ++i) {
        std::string dstFile = testDir + "/copy_" + std::to_string(i) + ".txt";
        cckit::fs::copyFile(srcFile, dstFile);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "复制 " << count << " 个文件耗时: " << duration.count() << " ms" << std::endl;

    double totalBytes = fileSize.value() * count;
    double speed = totalBytes / (duration.count() / 1000.0) / 1024.0 / 1024.0;
    std::cout << "复制速度: " << speed << " MB/s" << std::endl;
    SUCCEED();
}

TEST_F(FsPerfTest, DirectoryCreation)
{
    const int count = 1000;
    std::string testDir = "perf_test_dirs";
    if (cckit::fs::exists(testDir))
        cckit::fs::removeDirectoryAll(testDir);

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < count; ++i) {
        std::string dirname = testDir + "/dir_" + std::to_string(i);
        cckit::fs::createDirectory(dirname);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "创建 " << count << " 个单层目录耗时: " << duration.count() << " ms" << std::endl;
    cckit::fs::removeDirectoryAll(testDir);

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < count; ++i) {
        std::string nestedDir = testDir + "/level1_" + std::to_string(i) + "/level2/level3";
        cckit::fs::ensurePathExists(nestedDir);
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "创建 " << count << " 个嵌套目录耗时: " << duration.count() << " ms" << std::endl;
    SUCCEED();
}
