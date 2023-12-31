#include <gtest/gtest.h>
#include <Api/batching/batching.h>

namespace
{
  const std::string TEST_FOLDER = "TESTS";
  const std::string TEST_FILE_NAME = "File";
  const std::string TEST_FILE_PATH = TEST_FOLDER + "/" + TEST_FILE_NAME;

  template <class ChunkType>
  void write_and_read_chunks_from_file(
      const u64 entireChunkCount,
      const bool random)
  {
    // Given
    const u64 chunkSize = ChunkType::get_desired_chunk_size();
    std::vector<ChunkType> chunks;
    std::vector<ChunkType> readChunks;

    for (u64 i = 0; i < entireChunkCount; i++)
    {
      ChunkType chunk = ChunkFactory::get_empty_chunk<ChunkType>();

      chunk.map_from_bytes(Encryption::get_random_bytes(chunkSize));

      chunks.push_back(chunk);
    }

    if (random)
    {
      ChunkType chunk = ChunkFactory::get_empty_chunk<ChunkType>();

      chunk.map_from_bytes(Encryption::get_random_bytes(
          Global::get_random_u64(1, chunkSize - EncryptedDataChunk::VI_BYTE_SIZE) + EncryptedDataChunk::VI_BYTE_SIZE));

      chunks.push_back(chunk);
    }

    // When
    bool createdFile = FileAccess::create_file(TEST_FILE_PATH);

    for (u64 i = 0; i < chunks.size(); i++)
    {
      Batching::append_chunk_to_file(TEST_FILE_PATH, chunks.at(i));
    }

    for (u64 i = 0; i < chunks.size(); i++)
    {
      readChunks.push_back(
          Batching::read_chunk_from_file<ChunkType>(
              TEST_FILE_PATH, i, chunks.at(i).get_entire_chunk_size()));
    }

    // Then
    ASSERT_TRUE(createdFile);
    for (size_t i = 0; i < chunks.size(); i++)
    {
      ASSERT_EQ(chunks[i].get_entire_chunk(), readChunks[i].get_entire_chunk());
    }
  }

}

struct BatchingIT : public ::testing::Test
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

TEST_F(BatchingIT, write_and_read_single_exact_chunk_from_file)
{
  write_and_read_chunks_from_file<DataChunk>(1, false);
}

TEST_F(BatchingIT, write_and_read_single_random_chunk_from_file)
{
  write_and_read_chunks_from_file<DataChunk>(1, true);
}

TEST_F(BatchingIT, write_and_read_single_exact_encrypted_chunk_from_file)
{
  write_and_read_chunks_from_file<EncryptedDataChunk>(1, false);
}

TEST_F(BatchingIT, write_and_read_single_random_encrypted_chunk_from_file)
{
  write_and_read_chunks_from_file<EncryptedDataChunk>(1, true);
}

TEST_F(BatchingIT, write_and_read_multiple_exact_chunks_from_file)
{
  write_and_read_chunks_from_file<DataChunk>(
      Global::get_random_u64(2, 6), false);
}

TEST_F(BatchingIT, write_and_read_multiple_random_chunks_from_file)
{
  write_and_read_chunks_from_file<DataChunk>(
      Global::get_random_u64(2, 6), true);
}

TEST_F(BatchingIT, write_and_read_multiple_exact_encrypted_chunks_from_file)
{
  write_and_read_chunks_from_file<EncryptedDataChunk>(
      Global::get_random_u64(2, 6), false);
}

TEST_F(BatchingIT, write_and_read_multiple_random_encrypted_chunks_from_file)
{
  write_and_read_chunks_from_file<EncryptedDataChunk>(
      Global::get_random_u64(2, 6), true);
}
