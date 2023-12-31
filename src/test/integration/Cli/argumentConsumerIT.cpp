#include <gtest/gtest.h>
#include <Cli/functionLibrary/consoleFunction/argumentConsumer/argumentConsumer.h>

namespace
{

}

struct ArgumentConsumerIT : public ::testing::Test
{
  virtual void SetUp() override {}

  virtual void TearDown() override {}
};

TEST_F(ArgumentConsumerIT, should_correctly_map_input)
{
  // Given
  int argc = 7;
  char *argv[] = {
      (char *)"exeFileName",
      (char *)"commandName",
      (char *)"-f",
      (char *)"asd",
      (char *)"-c",
      (char *)"123",
      (char *)"sssss"};

  // When
  ArgumentConsumer commandConsumer;
  commandConsumer.parse_input(argc, argv);

  // Then
  ASSERT_EQ("asd", commandConsumer.get_required_argument("-f"));
  ASSERT_EQ("asd", commandConsumer.get_optional_argument("-f"));
  ASSERT_EQ("123", commandConsumer.get_required_argument("-c"));
  ASSERT_EQ("123", commandConsumer.get_optional_argument("-c"));
}

TEST_F(ArgumentConsumerIT, should_return_empty_string_on_missing_optional)
{
  // Given
  int argc = 2;
  char *argv[] = {(char *)"exeFileName", (char *)"commandName"};

  // When
  ArgumentConsumer commandConsumer;
  commandConsumer.parse_input(argc, argv);

  // Then
  ASSERT_EQ("", commandConsumer.get_optional_argument("-a"));
  ASSERT_EQ("", commandConsumer.get_optional_argument("-b"));
  ASSERT_EQ("", commandConsumer.get_optional_argument("-c"));
}

TEST_F(ArgumentConsumerIT, should_return_empty_string_on_missing_required)
{
  // Given
  int argc = 2;
  char *argv[] = {(char *)"exeFileName", (char *)"commandName"};

  // When
  ArgumentConsumer commandConsumer;
  commandConsumer.parse_input(argc, argv);
  bool thrownException = false;

  // Then
  ASSERT_TRUE(commandConsumer.get_required_argument("-a").empty());
}

TEST_F(ArgumentConsumerIT, should_return_false_when_missing_command)
{
  // Given
  int argc = 1;
  char *argv[] = {(char *)"exeFileName"};

  // When
  ArgumentConsumer commandConsumer;
  bool parsedInput = commandConsumer.parse_input(argc, argv);

  // Then
  ASSERT_TRUE(!parsedInput);
}

TEST_F(ArgumentConsumerIT, should_return_true_when_command_present)
{
  // Given
  int argc = 2;
  char *argv[] = {(char *)"exeFileName", (char *)"exampleCommand"};

  // When
  ArgumentConsumer commandConsumer;
  bool parsedInput = commandConsumer.parse_input(argc, argv);

  // Then
  ASSERT_TRUE(parsedInput);
}