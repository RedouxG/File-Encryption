#include "fileAccess.h"

bool FileAccess::delete_file(const std::filesystem::path &filePath) noexcept
{
  return std::filesystem::remove(filePath);
}

bool FileAccess::create_file(const std::filesystem::path &filePath) noexcept
{
  std::fstream file(filePath, std::fstream::in | std::fstream::out | std::fstream::trunc);
  return file.is_open();
}

bool FileAccess::file_exist(const std::filesystem::path &filePath) noexcept
{
  return std::filesystem::is_regular_file(filePath);
}

bool FileAccess::delete_dir(const std::filesystem::path &dirPath) noexcept
{
  return std::filesystem::remove_all(dirPath) > 0;
}

bool FileAccess::create_dir(const std::filesystem::path &dirPath) noexcept
{
  if (dir_exist(dirPath))
  {
    return true;
  }

  return std::filesystem::create_directory(dirPath);
}

bool FileAccess::dir_exist(const std::filesystem::path &dirPath) noexcept
{
  return std::filesystem::is_directory(dirPath);
}

FileAccess::ErrorCode FileAccess::append_bytes_to_file(
    const std::filesystem::path &filePath,
    const std::vector<BYTE> &bytes) noexcept
{
  if (!file_exist(filePath))
  {
    return ErrorCode::FILE_NOT_EXIST;
  }

  std::ofstream file(filePath, static_cast<std::ios::openmode>(OpenMode::APPEND));
  if (!file.is_open())
  {
    return ErrorCode::FILE_FAILED_OPEN;
  }

  file.write(reinterpret_cast<const char *>(bytes.data()), bytes.size());
  if (!file)
  {
    return ErrorCode::FILE_FAILED_WRITE;
  }

  return ErrorCode::OK;
}

FileAccess::ErrorCode FileAccess::read_bytes_from_file(
    const std::filesystem::path &filePath,
    std::vector<BYTE> &bytesByRef,
    const std::uintmax_t fromIndex,
    const u64 byteCount) noexcept
{
  if (!file_exist(filePath))
  {
    return ErrorCode::FILE_NOT_EXIST;
  }

  if (get_file_size(filePath) < fromIndex + byteCount)
  {
    return ErrorCode::FILE_INDEX_OVER_SIZE;
  }

  std::ifstream file(filePath, static_cast<std::ios::openmode>(OpenMode::READ));
  if (!file.is_open())
  {
    return ErrorCode::FILE_FAILED_OPEN;
  }

  file.seekg(fromIndex);
  if (!file)
  {
    return ErrorCode::FILE_FAILED_SEEK;
  }

  bytesByRef.resize(byteCount);
  file.read(
      reinterpret_cast<char *>(bytesByRef.data()),
      static_cast<std::streamsize>(byteCount));
  if (!file)
  {
    bytesByRef.clear();
    return ErrorCode::FILE_FAILED_READ;
  }

  return ErrorCode::OK;
}

std::uintmax_t FileAccess::get_byte_count_left_in_file(
    const std::filesystem::path &filePath,
    const std::uintmax_t fromIndex) noexcept
{
  const std::uintmax_t fileSize = get_file_size(filePath);
  if(fromIndex > fileSize)
  {
    return 0;
  }

  return fileSize - fromIndex;
}

std::uintmax_t FileAccess::get_file_size(
  const std::filesystem::path &filePath) noexcept
{
  if (!file_exist(filePath))
  {
    return 0;
  }

  return std::filesystem::file_size(filePath);
}

std::string FileAccess::get_file_folder_path(
  const std::filesystem::path &filePath) noexcept
{
  std::filesystem::path path(filePath);
  return path.parent_path().string();
}

std::filesystem::path FileAccess::get_exe_folder_path() noexcept
{
  return std::filesystem::current_path().string();
}