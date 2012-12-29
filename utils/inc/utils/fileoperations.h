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

#ifndef UTILS_FILE_OPERATIONS_H
#define UTILS_FILE_OPERATIONS_H

#include <string>
#include <vector>
#include <memory>
#include <stack>

namespace utils
{
namespace fileops
{

class Directory
{
public:
    class DirectoryHandle;
    
    Directory(const std::string& path);
    Directory(const Directory&) = delete;
    Directory(Directory&&);
    
    std::string path() const;
    
    DirectoryHandle& handle() const;
    
private:
    std::string                         m_Path;
    std::shared_ptr<DirectoryHandle>    m_DirHandle;
};

enum class FileSystemEntryType
{
    File,
    Directory,
    SymbolicLink,
    Unknown
};

struct FileSystemEntryInfo
{
    uint64_t                sizeInBytes;
    uint64_t                accessTime;
    uint64_t                modifyTime;
    uint64_t                createTime;
    FileSystemEntryType     type;
};

class FileSystemEntry
{
public:
    FileSystemEntry() = default;
    FileSystemEntry(const std::string& path, FileSystemEntryType type);
    FileSystemEntry(FileSystemEntry&&) = default;
    
    FileSystemEntry& operator=(FileSystemEntry&& other);
    
    const std::string& path() const;
    FileSystemEntryType type() const;
    
private:
    std::string             m_Path;
    FileSystemEntryType     m_Type;
};

class FileSystemIterator
{
public:
    FileSystemIterator();
    FileSystemIterator(const Directory& path);
    FileSystemIterator(const FileSystemIterator&) = delete;
    FileSystemIterator(FileSystemIterator&&) = default;
    
    FileSystemIterator& operator ++();
    FileSystemIterator& operator =(FileSystemIterator&& other);
    bool operator ==(const FileSystemIterator& other) const;
    bool operator !=(const FileSystemIterator& other) const;
    const FileSystemEntry& operator *();
    const FileSystemEntry* operator ->();
    
private:
    void nextFile();
    
    const Directory*                m_pDir;
    
    struct IteratorData;
    std::shared_ptr<IteratorData>   m_IterData;
};

// support for range based for loops
inline FileSystemIterator begin(const Directory& dir)
{
    return FileSystemIterator(dir);
}

inline FileSystemIterator end(const Directory&)
{
    return FileSystemIterator();
}

enum class IterationType
{
    Recursive,
    NonRecursive
};

bool readTextFile(std::string& contents, const std::string& filename);
bool readFile(std::vector<uint8_t>& contents, const std::string& filename);

std::string getFileExtension(const std::string& filepath);
std::string getFileName(const std::string& filepath);
uint64_t getFileSize(const std::string& filepath);
FileSystemEntryInfo getFileInfo(const std::string& filepath);

bool pathExists(const std::string& filepath);
void deleteFile(const std::string& filepath);
void getPathFromFilepath(const std::string& filepath, std::string& path);
std::string combinePath(const std::string& left, const std::string& right);

void createDirectory(const std::string& path);
void deleteDirectory(const std::string& path);
void deleteDirectoryRecursive(const std::string& path);
void changeDirectory(const std::string& dir);

uint64_t countFilesInDirectory(const std::string& path, IterationType iterType = IterationType::Recursive);
uint64_t calculateDirectorySize(const std::string& path, IterationType iterType = IterationType::Recursive);

std::string getHomeDirectory();
std::string getConfigDirectory();
std::string getDataDirectory();

}
}

#endif
