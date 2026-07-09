// test_fs_basic.cpp - 文件系统基础功能测试
#include <cckit/fs/fs.hpp>

#include <gtest/gtest.h>
#include <fstream>
#include <algorithm>
#include <vector>

class FsTest : public ::testing::Test {
protected:
    std::vector<std::string> tempFiles;
    std::vector<std::string> tempDirs;

    void TearDown() override {
        for (const auto& file : tempFiles)
            cckit::fs::removeFile(file);
        tempFiles.clear();
        for (const auto& dir : tempDirs)
            cckit::fs::removeDirectoryAll(dir);
        tempDirs.clear();
    }

    std::string makeTempFile(const std::string& name, const std::string& content = "test") {
        std::ofstream file(name);
        file << content;
        file.close();
        tempFiles.push_back(name);
        return name;
    }
};

TEST_F(FsTest, Separator)
{
    std::string sep = cckit::fs::getSeparator();
    EXPECT_FALSE(sep.empty());
    EXPECT_EQ(sep.length(), 1u);
#ifdef _WIN32
    EXPECT_EQ(sep, "\\");
#else
    EXPECT_EQ(sep, "/");
#endif
}

TEST_F(FsTest, PathExists)
{
    std::string currentDir = cckit::fs::getCurrentDirectory();
    EXPECT_TRUE(cckit::fs::exists(currentDir));
    EXPECT_FALSE(cckit::fs::exists("non_existent_path_12345"));
}

TEST_F(FsTest, FileDetection)
{
    std::string tempFile = makeTempFile("temp_test_file.txt", "test content");
    EXPECT_TRUE(cckit::fs::exists(tempFile));
    EXPECT_TRUE(cckit::fs::isRegularFile(tempFile));
    EXPECT_FALSE(cckit::fs::isDirectory(tempFile));

    std::string currentDir = cckit::fs::getCurrentDirectory();
    EXPECT_TRUE(cckit::fs::isDirectory(currentDir));
    EXPECT_FALSE(cckit::fs::isRegularFile(currentDir));
}

TEST_F(FsTest, PathManipulation)
{
    std::string path = "/path/to/file.txt";
    EXPECT_EQ(cckit::fs::getFileName(path), "file.txt");
    EXPECT_EQ(cckit::fs::getFileBase(path), "file");
    EXPECT_EQ(cckit::fs::getFileSuffix(path), ".txt");

    path = "/path/to/file";
    EXPECT_EQ(cckit::fs::getFileSuffix(path), "");

    path = "/path/to/file.txt";
    EXPECT_EQ(cckit::fs::getDirectoryPath(path), "/path/to");
    EXPECT_EQ(cckit::fs::getDirectoryName(path), "to");
    EXPECT_EQ(cckit::fs::getParentPath(path), "/path/to");

#ifdef _WIN32
    path = "C:\\path\\to\\file.txt";
    EXPECT_EQ(cckit::fs::getFileName(path), "file.txt");
#endif
}

TEST_F(FsTest, CurrentDirectory)
{
    std::string currentDir = cckit::fs::getCurrentDirectory();
    EXPECT_FALSE(currentDir.empty());
    EXPECT_TRUE(cckit::fs::exists(currentDir));
    EXPECT_TRUE(cckit::fs::isDirectory(currentDir));
}

TEST_F(FsTest, AbsolutePath)
{
    std::string relativePath = ".";
    std::string absolutePath = cckit::fs::getAbsolutePath(relativePath);
    EXPECT_FALSE(absolutePath.empty());
    EXPECT_TRUE(cckit::fs::exists(absolutePath));

    std::string currentDir = cckit::fs::getCurrentDirectory();
    absolutePath = cckit::fs::getAbsolutePath(currentDir);
    EXPECT_FALSE(absolutePath.empty());
    EXPECT_TRUE(cckit::fs::exists(absolutePath));
}

TEST_F(FsTest, GenericPath)
{
#ifdef _WIN32
    std::string windowsPath = "C:\\path\\to\\file.txt";
    std::string genericPath = cckit::fs::getGenericPath(windowsPath);
    EXPECT_EQ(genericPath.find('\\'), std::string::npos);
    EXPECT_NE(genericPath.find('/'), std::string::npos);
#endif
}

TEST_F(FsTest, FileOperations)
{
    std::string srcFile = makeTempFile("test_source.txt", "This is test content for file operations.");
    EXPECT_TRUE(cckit::fs::exists(srcFile));

    auto fileSize = cckit::fs::getFileSize(srcFile);
    ASSERT_TRUE(fileSize.has_value());
    EXPECT_GT(fileSize.value(), 0u);

    std::string dstFile = "test_destination.txt";
    tempFiles.push_back(dstFile);
    EXPECT_TRUE(cckit::fs::copyFile(srcFile, dstFile));
    EXPECT_TRUE(cckit::fs::exists(dstFile));

    auto srcSize = cckit::fs::getFileSize(srcFile);
    auto dstSize = cckit::fs::getFileSize(dstFile);
    EXPECT_EQ(srcSize, dstSize);

    EXPECT_TRUE(cckit::fs::removeFile(dstFile));
    EXPECT_FALSE(cckit::fs::exists(dstFile));
}

TEST_F(FsTest, DirectoryOperations)
{
    std::string testDir = "test_directory";
    tempDirs.push_back(testDir);
    if (cckit::fs::exists(testDir))
        cckit::fs::removeDirectoryAll(testDir);

    EXPECT_TRUE(cckit::fs::createDirectory(testDir));
    EXPECT_TRUE(cckit::fs::exists(testDir));
    EXPECT_TRUE(cckit::fs::isDirectory(testDir));

    std::string nestedDir = "test_directory/nested/deep";
    EXPECT_TRUE(cckit::fs::ensurePathExists(nestedDir));
    EXPECT_TRUE(cckit::fs::exists(nestedDir));

    std::string testFile = "test_directory/test.txt";
    tempFiles.push_back(testFile);
    {
        std::ofstream file(testFile);
        file << "test content";
    }

    auto files = cckit::fs::listFiles(testDir);
    EXPECT_FALSE(files.empty());
    EXPECT_NE(std::find(files.begin(), files.end(), testFile), files.end());

    std::set<std::string> extensions = {".txt"};
    auto txtFiles = cckit::fs::listFiles(testDir, extensions);
    EXPECT_FALSE(txtFiles.empty());

    EXPECT_FALSE(cckit::fs::removeDirectory(testDir));
    EXPECT_TRUE(cckit::fs::removeDirectoryAll(testDir));
    EXPECT_FALSE(cckit::fs::exists(testDir));
}

TEST_F(FsTest, EnsurePathExists)
{
    std::string deepPath = "ensure_test/level1/level2/level3";
    tempDirs.push_back("ensure_test");
    if (cckit::fs::exists("ensure_test"))
        cckit::fs::removeDirectoryAll("ensure_test");

    EXPECT_TRUE(cckit::fs::ensurePathExists(deepPath));
    EXPECT_TRUE(cckit::fs::exists(deepPath));
    EXPECT_TRUE(cckit::fs::isDirectory(deepPath));
    EXPECT_TRUE(cckit::fs::ensurePathExists(deepPath));
}

TEST_F(FsTest, ListFilesWithFilters)
{
    std::string testDir = "list_test";
    tempDirs.push_back(testDir);
    if (cckit::fs::exists(testDir))
        cckit::fs::removeDirectoryAll(testDir);
    EXPECT_TRUE(cckit::fs::createDirectory(testDir));

    std::vector<std::string> testFiles = {
        "list_test/file1.txt", "list_test/file2.txt",
        "list_test/file1.cpp", "list_test/file2.hpp", "list_test/data.json"
    };
    for (const auto& file : testFiles) {
        std::ofstream f(file); f << "test"; f.close();
        tempFiles.push_back(file);
    }

    auto allFiles = cckit::fs::listFiles(testDir);
    EXPECT_GE(allFiles.size(), testFiles.size());

    std::set<std::string> txtExtensions = {".txt"};
    auto txtFiles = cckit::fs::listFiles(testDir, txtExtensions);
    EXPECT_GE(txtFiles.size(), 2u);

    std::set<std::string> multiExtensions = {".txt", ".cpp", ".hpp"};
    auto multiFiles = cckit::fs::listFiles(testDir, multiExtensions);
    EXPECT_GE(multiFiles.size(), 4u);
}

TEST_F(FsTest, EdgeCases)
{
    EXPECT_FALSE(cckit::fs::exists(""));
    EXPECT_TRUE(cckit::fs::exists("."));
    EXPECT_TRUE(cckit::fs::exists(".."));

    std::string nonExistentFile = "non_existent_file_12345.txt";
    auto size = cckit::fs::getFileSize(nonExistentFile);
    EXPECT_FALSE(size.has_value());
    EXPECT_FALSE(cckit::fs::removeFile(nonExistentFile));
}
