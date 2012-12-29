//    Copyright (C) 2012 Dirk Vanden Boer <dirk.vdb@gmail.com>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#include <gtest/gtest.h>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <algorithm>

#include "utils/fileoperations.h"

using namespace std;
using namespace utils::fileops;

TEST(FileOperationsTest, ReadTextFile)
{
    ofstream file("testfile.txt");
    file << "line1\nline2\nline3\n";
    file.close();
    
    string contents;
    EXPECT_TRUE(readTextFile(contents, "testfile.txt"));
    EXPECT_EQ("line1\nline2\nline3\n", contents);
    deleteFile("testfile.txt");
}
        
TEST(FileOperationsTest, GetFileExtension)
{
    EXPECT_EQ("txt", getFileExtension("file.txt"));
    EXPECT_EQ("longextension", getFileExtension("this.file.longextension"));
    EXPECT_EQ("", getFileExtension("file."));
    EXPECT_EQ("", getFileExtension("file"));
}

TEST(FileOperationsTest, PathExists)
{
    ofstream file("testfile.txt");
    EXPECT_TRUE(pathExists("testfile.txt"));
    EXPECT_FALSE(pathExists("somefile.txt"));
    deleteFile("testfile.txt");
}

TEST(FileOperationsTest, DeleteFile)
{
    ofstream file("testfile.txt");
    EXPECT_TRUE(pathExists("testfile.txt"));
    deleteFile("testfile.txt");
    EXPECT_FALSE(pathExists("testfile.txt"));
}

TEST(FileOperationsTest, DeleteNonExistingFile)
{
    EXPECT_THROW(deleteFile("somefile.txt"), logic_error);
}

TEST(FileOperationsTest, DeleteFileOnDir)
{
    createDirectory("temp");
    EXPECT_TRUE(pathExists("temp"));
    EXPECT_THROW(deleteFile("temp"), logic_error);
    EXPECT_TRUE(pathExists("temp"));
    deleteDirectory("temp");
}

TEST(FileOperationsTest, GetPathFromFilePath)
{
    string path;
    
    EXPECT_THROW(getPathFromFilepath("/temp/", path), logic_error);

    getPathFromFilepath("/temp", path);
    EXPECT_EQ("/", path);

    getPathFromFilepath("/temp/file.txt", path);
    EXPECT_EQ("/temp", path);

    getPathFromFilepath("temp/aFile", path);
    EXPECT_EQ("temp", path);
}

TEST(FileOperationsTest, CombinePath)
{
    EXPECT_EQ("/temp/dir", combinePath("/temp", "dir"));
    EXPECT_EQ("/temp/dir", combinePath("/temp/", "dir"));
    EXPECT_EQ("./dir", combinePath(".", "dir"));
    EXPECT_THROW(combinePath("", "dir"), logic_error);
}

TEST(FileOperationsTest, CreateDirectory)
{
    EXPECT_FALSE(pathExists("temp"));
    createDirectory("temp");
    EXPECT_TRUE(pathExists("temp"));
    deleteDirectory("temp");
}

TEST(FileOperationsTest, CreateDirectoryThatExists)
{
    createDirectory("temp");
    EXPECT_TRUE(pathExists("temp"));
    EXPECT_THROW(createDirectory("temp"), logic_error);
    deleteDirectory("temp");
}

TEST(FileOperationsTest, DeleteDirectory)
{
    createDirectory("temp");
    createDirectory("temp/temp");
    ofstream file("temp/afile.txt");
    file.close();
    EXPECT_TRUE(pathExists("temp"));
    EXPECT_TRUE(pathExists("temp/temp"));
    deleteDirectory("temp");
    EXPECT_FALSE(pathExists("temp"));
}

class IteratorTester : public IFileIterator
{
public:
    bool onFile(const std::string& filename)
    {
        files.push_back(filename);
        return true;
    }

    vector<string> files;
};

TEST(FileOperationsTest, IterateDirectory)
{
    createDirectory("temp");
    createDirectory("temp/temp");
    ofstream file1("temp/afile1.txt");
    file1.close();
    ofstream file2("temp/afile2.txt");
    file2.close();
    ofstream file3("temp/temp/afile.txt");
    file3.close();

    IteratorTester iter;

    iterateDirectory("temp", iter);
    deleteDirectory("temp");
    
    ASSERT_EQ(3, iter.files.size());
    sort(iter.files.begin(), iter.files.end());
    EXPECT_EQ("temp/afile1.txt", iter.files[0]);
    EXPECT_EQ("temp/afile2.txt", iter.files[1]);
    EXPECT_EQ("temp/temp/afile.txt", iter.files[2]);
}

TEST(FileOperationsTest, GetFileSize)
{
    ofstream file("test.file");
    file << "tata";
    file.close();

    int64_t size;
    getFileSize("test.file", size);
    EXPECT_EQ(4, size);
    deleteFile("test.file");
}

TEST(FileOperationsTest, GetFileSizeBadFile)
{
    int64_t size;
    EXPECT_THROW(getFileSize("noexist", size), logic_error);
}

TEST(FileOperationsTest, GetFileInfo)
{
    ofstream file("test.file");
    file << "tatata";
    file.close();

    int64_t size;
    uint32_t modified;
    
    getFileInfo("test.file", size, modified);
    EXPECT_EQ(6, size);
    deleteFile("test.file");
}

TEST(FileOperationsTest, GetFileInfoBadFile)
{
    int64_t size;
    uint32_t modified;
    EXPECT_THROW(getFileInfo("noexist", size, modified), logic_error);
}
