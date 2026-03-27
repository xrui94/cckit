// test_fs_basic.cpp - 文件系统基础功能测试
#include <cckit/fs/fs.hpp>
#include <iostream>
#include <cassert>
#include <fstream>
#include <algorithm>

// 测试临时文件清理
std::vector<std::string> tempFiles;
std::vector<std::string> tempDirs;

void cleanup() {
    // 删除临时文件
    for (const auto& file : tempFiles) {
        cckit::fs::removeFile(file);
    }
    tempFiles.clear();

    // 删除临时目录
    for (const auto& dir : tempDirs) {
        cckit::fs::removeDirectoryAll(dir);
    }
    tempDirs.clear();
}

void test_separator()
{
    std::cout << "测试路径分隔符..." << std::endl;

    std::string sep = cckit::fs::getSeparator();
    assert(!sep.empty());
    assert(sep.length() == 1);

#ifdef _WIN32
    assert(sep == "\\");
#else
    assert(sep == "/");
#endif

    std::cout << "✅ 路径分隔符测试通过" << std::endl;
}

void test_path_exists()
{
    std::cout << "测试路径存在检查..." << std::endl;

    // 测试当前目录（应该存在）
    std::string currentDir = cckit::fs::getCurrentDirectory();
    assert(cckit::fs::exists(currentDir));

    // 测试不存在的路径
    std::string nonExistentPath = "non_existent_path_12345";
    assert(!cckit::fs::exists(nonExistentPath));

    std::cout << "✅ 路径存在检查测试通过" << std::endl;
}

void test_file_detection()
{
    std::cout << "测试文件检测..." << std::endl;

    // 创建临时文件
    std::string tempFile = "temp_test_file.txt";
    tempFiles.push_back(tempFile);

    std::ofstream file(tempFile);
    file << "test content";
    file.close();

    assert(cckit::fs::exists(tempFile));
    assert(cckit::fs::isRegularFile(tempFile));
    assert(!cckit::fs::isDirectory(tempFile));

    // 测试目录
    std::string currentDir = cckit::fs::getCurrentDirectory();
    assert(cckit::fs::isDirectory(currentDir));
    assert(!cckit::fs::isRegularFile(currentDir));

    std::cout << "✅ 文件检测测试通过" << std::endl;
}

void test_path_manipulation()
{
    std::cout << "测试路径操作..." << std::endl;

    // 测试 getFileName
    std::string path = "/path/to/file.txt";
    std::string filename = cckit::fs::getFileName(path);
    assert(filename == "file.txt");

#ifdef _WIN32
    path = "C:\\path\\to\\file.txt";
    filename = cckit::fs::getFileName(path);
    assert(filename == "file.txt");
#endif

    // 测试 getFileBase
    path = "/path/to/file.txt";
    std::string base = cckit::fs::getFileBase(path);
    assert(base == "file");

    // 测试 getFileSuffix
    path = "/path/to/file.txt";
    std::string suffix = cckit::fs::getFileSuffix(path);
    assert(suffix == ".txt");

    // 测试没有扩展名的文件
    path = "/path/to/file";
    suffix = cckit::fs::getFileSuffix(path);
    assert(suffix.empty());

    // 测试 getDirectoryPath（传入文件路径）
    path = "/path/to/file.txt";
    std::string dirPath = cckit::fs::getDirectoryPath(path);
    assert(dirPath == "/path/to");  // 文件所在目录的名称

    // 测试 getDirectoryPath（传入文件路径，Windows风格的路径分隔符）
    path = "\\path\\to\\file.txt";
    dirPath = cckit::fs::getDirectoryPath(path);
    assert(dirPath == "/path/to");  // 文件所在目录的名称

    // 测试 getDirectoryPath（传入目录路径，如果路径不存，一定要以“/”结尾，否则将会被认为是文件路径，而非目录路径）
    path = "/path/to/dir/";
    dirPath = cckit::fs::getDirectoryPath(path);
    assert(dirPath == "/path/to/dir/");  // 目录本身的名称

    // 测试 getDirectoryPath（传入目录路径，如果路径不存，一定要以“/”结尾，否则将会被认为是文件路径，而非目录路径，Windows风格的路径分隔符）
    path = "\\path\\to\\dir\\";
    dirPath = cckit::fs::getDirectoryPath(path);
    assert(dirPath == "/path/to/dir/");  // 目录本身的名称

    // 测试 getDirectoryName（传入文件路径）
    path = "/path/to/file.txt";
    std::string dirName = cckit::fs::getDirectoryName(path);
    assert(dirName == "to");  // 文件所在目录的名称

    // 测试 getDirectoryName（传入文件路径，Windows风格的路径分隔符）
    path = "\\path\\to\\file.txt";
    dirName = cckit::fs::getDirectoryName(path);
    assert(dirName == "to");  // 文件所在目录的名称

    // 测试 getDirectoryName（传入目录路径）
    path = "/path/to/dir/";
    dirName = cckit::fs::getDirectoryName(path);
    assert(dirName == "dir");  // 目录本身的名称

    // 测试 getDirectoryName（传入目录路径，Windows风格的路径分隔符）
    path = "\\path\\to\\dir\\";
    dirName = cckit::fs::getDirectoryName(path);
    assert(dirName == "dir");  // 目录本身的名称

    // 测试 getParentPath
    path = "/path/to/file.txt";
    std::string parent = cckit::fs::getParentPath(path);
    assert(parent == "/path/to");

    // 测试 getParentPath（Windows风格的路径分隔符）
    path = "\\path\\to\\file.txt";
    parent = cckit::fs::getParentPath(path);
    assert(parent == "/path/to");

    // 测试根目录
    path = "/";
    parent = cckit::fs::getParentPath(path);
    assert(parent.empty() || parent == "/");

    std::cout << "✅ 路径操作测试通过" << std::endl;
}

void test_current_directory()
{
    std::cout << "测试当前目录..." << std::endl;

    std::string currentDir = cckit::fs::getCurrentDirectory();
    assert(!currentDir.empty());
    assert(cckit::fs::exists(currentDir));
    assert(cckit::fs::isDirectory(currentDir));

    std::cout << "当前目录: " << currentDir << std::endl;
    std::cout << "✅ 当前目录测试通过" << std::endl;
}

void test_absolute_path()
{
    std::cout << "测试绝对路径..." << std::endl;

    // 测试相对路径转绝对路径
    std::string relativePath = ".";
    std::string absolutePath = cckit::fs::getAbsolutePath(relativePath);
    assert(!absolutePath.empty());
    assert(cckit::fs::exists(absolutePath));

    // 测试当前目录的绝对路径
    std::string currentDir = cckit::fs::getCurrentDirectory();
    absolutePath = cckit::fs::getAbsolutePath(currentDir);
    assert(!absolutePath.empty());
    assert(cckit::fs::exists(absolutePath));

    std::cout << "✅ 绝对路径测试通过" << std::endl;
}

void test_generic_path()
{
    std::cout << "测试通用路径..." << std::endl;

#ifdef _WIN32
    std::string windowsPath = "C:\\path\\to\\file.txt";
    std::string genericPath = cckit::fs::getGenericPath(windowsPath);
    assert(genericPath.find('\\') == std::string::npos);
    assert(genericPath.find('/') != std::string::npos);
#else
    std::string unixPath = "/path/to/file.txt";
    std::string genericPath = cckit::fs::getGenericPath(unixPath);
    assert(genericPath == unixPath);
#endif

    std::cout << "✅ 通用路径测试通过" << std::endl;
}

void test_file_operations()
{
    std::cout << "测试文件操作..." << std::endl;

    // 创建源文件
    std::string srcFile = "test_source.txt";
    tempFiles.push_back(srcFile);

    {
        std::ofstream file(srcFile);
        file << "This is test content for file operations.";
        file.close();
    }

    assert(cckit::fs::exists(srcFile));

    // 测试文件大小
    auto fileSize = cckit::fs::getFileSize(srcFile);
    assert(fileSize.has_value());
    assert(fileSize.value() > 0);

    // 复制文件
    std::string dstFile = "test_destination.txt";
    tempFiles.push_back(dstFile);

    assert(cckit::fs::copyFile(srcFile, dstFile));
    assert(cckit::fs::exists(dstFile));

    // 验证文件内容相同
    auto srcSize = cckit::fs::getFileSize(srcFile);
    auto dstSize = cckit::fs::getFileSize(dstFile);
    assert(srcSize == dstSize);

    // 删除目标文件
    assert(cckit::fs::removeFile(dstFile));
    assert(!cckit::fs::exists(dstFile));

    std::cout << "✅ 文件操作测试通过" << std::endl;
}

void test_directory_operations()
{
    std::cout << "测试目录操作..." << std::endl;

    // 创建测试目录
    std::string testDir = "test_directory";
    tempDirs.push_back(testDir);

    // 删除可能存在的旧目录
    if (cckit::fs::exists(testDir)) {
        cckit::fs::removeDirectoryAll(testDir);
    }

    // 创建目录
    assert(cckit::fs::createDirectory(testDir));
    assert(cckit::fs::exists(testDir));
    assert(cckit::fs::isDirectory(testDir));

    // 创建嵌套目录
    std::string nestedDir = "test_directory/nested/deep";
    assert(cckit::fs::ensurePathExists(nestedDir));
    assert(cckit::fs::exists(nestedDir));

    // 创建文件在目录中
    std::string testFile = "test_directory/test.txt";
    tempFiles.push_back(testFile);

    {
        std::ofstream file(testFile);
        file << "test content";
        file.close();
    }

    // 测试 listFiles
    auto files = cckit::fs::listFiles(testDir);
    assert(!files.empty());
    assert(std::find(files.begin(), files.end(), testFile) != files.end());

    // 测试带扩展名过滤的 listFiles
    std::set<std::string> extensions = {".txt"};
    auto txtFiles = cckit::fs::listFiles(testDir, extensions);
    assert(!txtFiles.empty());

    // 删除目录（非递归）
    assert(!cckit::fs::removeDirectory(testDir)); // 非空目录，应该失败

    // 递归删除目录
    assert(cckit::fs::removeDirectoryAll(testDir));
    assert(!cckit::fs::exists(testDir));

    std::cout << "✅ 目录操作测试通过" << std::endl;
}

void test_ensure_path_exists()
{
    std::cout << "测试确保路径存在..." << std::endl;

    std::string deepPath = "ensure_test/level1/level2/level3";
    tempDirs.push_back("ensure_test");

    // 清理可能存在的旧路径
    if (cckit::fs::exists("ensure_test")) {
        cckit::fs::removeDirectoryAll("ensure_test");
    }

    // 确保路径存在
    assert(cckit::fs::ensurePathExists(deepPath));
    assert(cckit::fs::exists(deepPath));
    assert(cckit::fs::isDirectory(deepPath));

    // 测试已存在的路径
    assert(cckit::fs::ensurePathExists(deepPath)); // 应该成功

    std::cout << "✅ 确保路径存在测试通过" << std::endl;
}

void test_list_files_with_filters()
{
    std::cout << "测试文件列表过滤..." << std::endl;

    std::string testDir = "list_test";
    tempDirs.push_back(testDir);

    // 清理可能存在的旧目录
    if (cckit::fs::exists(testDir)) {
        cckit::fs::removeDirectoryAll(testDir);
    }

    // 创建测试目录
    assert(cckit::fs::createDirectory(testDir));

    // 创建不同扩展名的文件
    std::vector<std::string> testFiles = {
        "list_test/file1.txt",
        "list_test/file2.txt",
        "list_test/file1.cpp",
        "list_test/file2.hpp",
        "list_test/data.json"
    };

    for (const auto& file : testFiles) {
        std::ofstream f(file);
        f << "test";
        f.close();
        tempFiles.push_back(file);
    }

    // 测试获取所有文件
    auto allFiles = cckit::fs::listFiles(testDir);
    assert(allFiles.size() >= testFiles.size());

    // 测试只获取 .txt 文件
    std::set<std::string> txtExtensions = {".txt"};
    auto txtFiles = cckit::fs::listFiles(testDir, txtExtensions);
    assert(txtFiles.size() >= 2);

    // 测试获取多个扩展名的文件
    std::set<std::string> multiExtensions = {".txt", ".cpp", ".hpp"};
    auto multiFiles = cckit::fs::listFiles(testDir, multiExtensions);
    assert(multiFiles.size() >= 4);

    std::cout << "✅ 文件列表过滤测试通过" << std::endl;
}

void test_edge_cases()
{
    std::cout << "测试边界情况..." << std::endl;

    // 测试空路径
    std::string emptyPath = "";
    assert(!cckit::fs::exists(emptyPath));

    // 测试相对路径
    std::string relativePath = ".";
    assert(cckit::fs::exists(relativePath));

    // 测试父目录 ".."
    std::string parentPath = "..";
    assert(cckit::fs::exists(parentPath));

    // 测试不存在的文件大小
    std::string nonExistentFile = "non_existent_file_12345.txt";
    auto size = cckit::fs::getFileSize(nonExistentFile);
    assert(!size.has_value());

    // 测试删除不存在的文件
    assert(!cckit::fs::removeFile(nonExistentFile));

    std::cout << "✅ 边界情况测试通过" << std::endl;
}

int main()
{
    std::cout << "=== 文件系统基础功能测试 ===" << std::endl;
    std::cout << std::endl;

    test_separator();
    std::cout << std::endl;

    test_path_exists();
    std::cout << std::endl;

    test_file_detection();
    std::cout << std::endl;

    test_path_manipulation();
    std::cout << std::endl;

    test_current_directory();
    std::cout << std::endl;

    test_absolute_path();
    std::cout << std::endl;

    test_generic_path();
    std::cout << std::endl;

    test_file_operations();
    std::cout << std::endl;

    test_directory_operations();
    std::cout << std::endl;

    test_ensure_path_exists();
    std::cout << std::endl;

    test_list_files_with_filters();
    std::cout << std::endl;

    test_edge_cases();
    std::cout << std::endl;

    cleanup();

    std::cout << "=== 所有测试通过! ===" << std::endl;
    return 0;
}
