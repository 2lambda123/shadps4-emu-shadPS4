// SPDX-FileCopyrightText: Copyright 2021 yuzu Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include <vector>

#include "common/alignment.h"
#include "common/assert.h"
#include "common/error.h"
#include "common/io_file.h"
#include "common/logging/log.h"
#include "common/path_util.h"

#ifdef _WIN32
#include <io.h>
#include <share.h>
#include <windows.h>
#else
#include <unistd.h>
#endif

#ifdef _MSC_VER
#define fileno _fileno
#define fseeko _fseeki64
#define ftello _ftelli64
#endif

namespace Common::FS {

namespace fs = std::filesystem;

namespace {

#ifdef _WIN32

[[nodiscard]] constexpr const wchar_t* AccessModeToWStr(FileAccessMode mode, FileType type) {
    switch (type) {
    case FileType::BinaryFile:
        switch (mode) {
        case FileAccessMode::Read:
            return L"rb";
        case FileAccessMode::Write:
            return L"wb";
        case FileAccessMode::Append:
            return L"ab";
        case FileAccessMode::ReadWrite:
            return L"r+b";
        case FileAccessMode::ReadAppend:
            return L"a+b";
        }
        break;
    case FileType::TextFile:
        switch (mode) {
        case FileAccessMode::Read:
            return L"r";
        case FileAccessMode::Write:
            return L"w";
        case FileAccessMode::Append:
            return L"a";
        case FileAccessMode::ReadWrite:
            return L"r+";
        case FileAccessMode::ReadAppend:
            return L"a+";
        }
        break;
    }

    return L"";
}

[[nodiscard]] constexpr int ToWindowsFileShareFlag(FileShareFlag flag) {
    switch (flag) {
    case FileShareFlag::ShareNone:
    default:
        return _SH_DENYRW;
    case FileShareFlag::ShareReadOnly:
        return _SH_DENYWR;
    case FileShareFlag::ShareWriteOnly:
        return _SH_DENYRD;
    case FileShareFlag::ShareReadWrite:
        return _SH_DENYNO;
    }
}

#else

[[nodiscard]] constexpr const char* AccessModeToStr(FileAccessMode mode, FileType type) {
    switch (type) {
    case FileType::BinaryFile:
        switch (mode) {
        case FileAccessMode::Read:
            return "rb";
        case FileAccessMode::Write:
            return "wb";
        case FileAccessMode::Append:
            return "ab";
        case FileAccessMode::ReadWrite:
            return "r+b";
        case FileAccessMode::ReadAppend:
            return "a+b";
        }
        break;
    case FileType::TextFile:
        switch (mode) {
        case FileAccessMode::Read:
            return "r";
        case FileAccessMode::Write:
            return "w";
        case FileAccessMode::Append:
            return "a";
        case FileAccessMode::ReadWrite:
            return "r+";
        case FileAccessMode::ReadAppend:
            return "a+";
        }
        break;
    }

    return "";
}

#endif

[[nodiscard]] constexpr int ToSeekOrigin(SeekOrigin origin) {
    switch (origin) {
    case SeekOrigin::SetOrigin:
    default:
        return SEEK_SET;
    case SeekOrigin::CurrentPosition:
        return SEEK_CUR;
    case SeekOrigin::End:
        return SEEK_END;
    }
}

} // Anonymous namespace

IOFile::IOFile() = default;

IOFile::IOFile(const std::string& path, FileAccessMode mode, FileType type, FileShareFlag flag) {
    Open(path, mode, type, flag);
}

IOFile::IOFile(std::string_view path, FileAccessMode mode, FileType type, FileShareFlag flag) {
    Open(path, mode, type, flag);
}

IOFile::IOFile(const fs::path& path, FileAccessMode mode, FileType type, FileShareFlag flag) {
    Open(path, mode, type, flag);
}

IOFile::~IOFile() {
    Close();
}

IOFile::IOFile(IOFile&& other) noexcept {
    std::swap(file_path, other.file_path);
    std::swap(file_access_mode, other.file_access_mode);
    std::swap(file_type, other.file_type);
    std::swap(file, other.file);
}

IOFile& IOFile::operator=(IOFile&& other) noexcept {
    std::swap(file_path, other.file_path);
    std::swap(file_access_mode, other.file_access_mode);
    std::swap(file_type, other.file_type);
    std::swap(file, other.file);
    return *this;
}

void IOFile::Open(const fs::path& path, FileAccessMode mode, FileType type, FileShareFlag flag) {
    Close();

    file_path = path;
    file_access_mode = mode;
    file_type = type;

    errno = 0;

#ifdef _WIN32
    if (flag != FileShareFlag::ShareNone) {
        file = _wfsopen(path.c_str(), AccessModeToWStr(mode, type), ToWindowsFileShareFlag(flag));
    } else {
        _wfopen_s(&file, path.c_str(), AccessModeToWStr(mode, type));
    }
#else
    file = std::fopen(path.c_str(), AccessModeToStr(mode, type));
#endif

    if (!IsOpen()) {
        const auto ec = std::error_code{errno, std::generic_category()};
        LOG_ERROR(Common_Filesystem, "Failed to open the file at path={}, ec_message={}",
                  PathToUTF8String(file_path), ec.message());
    }
}

void IOFile::Close() {
    if (!IsOpen()) {
        return;
    }

    errno = 0;

    const auto close_result = std::fclose(file) == 0;

    if (!close_result) {
        const auto ec = std::error_code{errno, std::generic_category()};
        LOG_ERROR(Common_Filesystem, "Failed to close the file at path={}, ec_message={}",
                  PathToUTF8String(file_path), ec.message());
    }

    file = nullptr;

#ifdef _WIN64
    if (file_mapping) {
        CloseHandle(file_mapping);
    }
#endif
}

void* IOFile::GetFileMapping() {
#ifdef _WIN64
    if (file_mapping) {
        return file_mapping;
    }
    const int fd = fileno(file);
    HANDLE hfile = reinterpret_cast<HANDLE>(_get_osfhandle(fd));
    file_mapping =
        CreateFileMapping2(hfile, NULL, FILE_MAP_READ, PAGE_READONLY, SEC_COMMIT, 0, NULL, NULL, 0);
    ASSERT_MSG(file_mapping, "{}", Common::GetLastErrorMsg());
    return file_mapping;
#endif
}

std::string IOFile::ReadString(size_t length) const {
    std::vector<char> string_buffer(length);

    const auto chars_read = ReadSpan<char>(string_buffer);
    const auto string_size = chars_read != length ? chars_read : length;

    return std::string{string_buffer.data(), string_size};
}

bool IOFile::Flush() const {
    if (!IsOpen()) {
        return false;
    }

    errno = 0;

#ifdef _WIN32
    const auto flush_result = std::fflush(file) == 0;
#else
    const auto flush_result = std::fflush(file) == 0;
#endif

    if (!flush_result) {
        const auto ec = std::error_code{errno, std::generic_category()};
        LOG_ERROR(Common_Filesystem, "Failed to flush the file at path={}, ec_message={}",
                  PathToUTF8String(file_path), ec.message());
    }

    return flush_result;
}

bool IOFile::Commit() const {
    if (!IsOpen()) {
        return false;
    }

    errno = 0;

#ifdef _WIN32
    const auto commit_result = std::fflush(file) == 0 && _commit(fileno(file)) == 0;
#else
    const auto commit_result = std::fflush(file) == 0 && fsync(fileno(file)) == 0;
#endif

    if (!commit_result) {
        const auto ec = std::error_code{errno, std::generic_category()};
        LOG_ERROR(Common_Filesystem, "Failed to commit the file at path={}, ec_message={}",
                  PathToUTF8String(file_path), ec.message());
    }

    return commit_result;
}

bool IOFile::SetSize(u64 size) const {
    if (!IsOpen()) {
        return false;
    }

    errno = 0;

#ifdef _WIN32
    const auto set_size_result = _chsize_s(fileno(file), static_cast<s64>(size)) == 0;
#else
    const auto set_size_result = ftruncate(fileno(file), static_cast<s64>(size)) == 0;
#endif

    if (!set_size_result) {
        const auto ec = std::error_code{errno, std::generic_category()};
        LOG_ERROR(Common_Filesystem, "Failed to resize the file at path={}, size={}, ec_message={}",
                  PathToUTF8String(file_path), size, ec.message());
    }

    return set_size_result;
}

u64 IOFile::GetSize() const {
    if (!IsOpen()) {
        return 0;
    }

    // Flush any unwritten buffered data into the file prior to retrieving the file size.
    std::fflush(file);

    std::error_code ec;

    const auto file_size = fs::file_size(file_path, ec);

    if (ec) {
        LOG_ERROR(Common_Filesystem, "Failed to retrieve the file size of path={}, ec_message={}",
                  PathToUTF8String(file_path), ec.message());
        return 0;
    }

    return file_size;
}

bool IOFile::Seek(s64 offset, SeekOrigin origin) const {
    if (!IsOpen()) {
        return false;
    }

    errno = 0;

    const auto seek_result = fseeko(file, offset, ToSeekOrigin(origin)) == 0;

    if (!seek_result) {
        const auto ec = std::error_code{errno, std::generic_category()};
        LOG_ERROR(Common_Filesystem,
                  "Failed to seek the file at path={}, offset={}, origin={}, ec_message={}",
                  PathToUTF8String(file_path), offset, static_cast<u32>(origin), ec.message());
    }

    return seek_result;
}

s64 IOFile::Tell() const {
    if (!IsOpen()) {
        return 0;
    }

    errno = 0;

    return ftello(file);
}

} // namespace Common::FS
