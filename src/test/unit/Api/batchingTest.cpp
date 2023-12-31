#include <gtest/gtest.h>
#include <Api/batching/batching.h>

namespace
{
  const std::filesystem::path TEST_FOLDER = "TESTS";
}

struct BatchingTest : public ::testing::Test
{
  virtual void SetUp() override
  {
    std::filesystem::create_directory(TEST_FOLDER);
    }

  virtual void TearDown() override
  {
    std::filesystem::remove_all(TEST_FOLDER);
  }
};

TEST_F(BatchingTest, should_correctly_count_chunks_in_file)
{
  // Given
  const std::filesystem::path filePathLess = TEST_FOLDER / "1";
  const std::filesystem::path filePathExactly = TEST_FOLDER / "2";
  const std::filesystem::path filePathMore = TEST_FOLDER / "3";

  // When
  bool createdFileLess = FileAccess::create_file(filePathLess);
  bool createdFileExactly = FileAccess::create_file(filePathExactly);
  bool createdFileMore = FileAccess::create_file(filePathMore);

  FileAccess::ErrorCode writtenFileLess =
      FileAccess::append_bytes_to_file(
          filePathLess,
          Encryption::get_random_bytes(DataChunk::DATA_BYTE_SIZE - 1));
  FileAccess::ErrorCode writtenFileExactly =
      FileAccess::append_bytes_to_file(
          filePathExactly,
          Encryption::get_random_bytes(DataChunk::DATA_BYTE_SIZE));
  FileAccess::ErrorCode writtenFileMore =
      FileAccess::append_bytes_to_file(
          filePathMore,
          Encryption::get_random_bytes(DataChunk::DATA_BYTE_SIZE + 1));

  u64 chunkCountLess = Batching::get_chunk_count_in_file(
      filePathLess, DataChunk::DATA_BYTE_SIZE);
  u64 chunkCountExactly = Batching::get_chunk_count_in_file(
      filePathExactly, DataChunk::DATA_BYTE_SIZE);
  u64 chunkCountMore = Batching::get_chunk_count_in_file(
      filePathMore, DataChunk::DATA_BYTE_SIZE);

  // Then
  ASSERT_TRUE(createdFileLess);
  ASSERT_TRUE(createdFileExactly);
  ASSERT_TRUE(createdFileMore);

  ASSERT_EQ(FileAccess::ErrorCode::OK, writtenFileLess);
  ASSERT_EQ(FileAccess::ErrorCode::OK, writtenFileExactly);
  ASSERT_EQ(FileAccess::ErrorCode::OK, writtenFileMore);

  ASSERT_EQ(1, chunkCountLess);
  ASSERT_EQ(1, chunkCountExactly);
  ASSERT_EQ(2, chunkCountMore);
}