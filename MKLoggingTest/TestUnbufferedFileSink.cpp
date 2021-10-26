#include <gtest/gtest.h>
#include <LogUnbufferedFileSink.h>
#include <filesystem>
#include "TestUtils.h"

TEST(UnbufferedFileSink, Basic)
{
	std::filesystem::path logFilePath = TEST_OUTPUT_DIRECTORY_PATH;
	logFilePath /= "test.log";

	LogUnbufferedFileSink file;
	EXPECT_TRUE(file.Create(logFilePath));

	std::string expectedText = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Duis eget enim nec purus accumsan viverra eu non arcu. Pellentesque congue diam eu justo interdum sagittis. Duis sagittis vehicula suscipit. Aenean condimentum vestibulum porta. In id dignissim massa. Nullam venenatis dui ultricies justo pharetra, nec molestie sapien lobortis. Sed vulputate mi ac neque aliquam consequat. In nec vulputate mauris, in fringilla justo. Mauris rhoncus dignissim ante, commodo pharetra dolor dapibus a.";
	file.WriteToFile(expectedText);

	std::string actualText = ReadLogFileAsBinary(logFilePath);
	// ignore 3 bytes BOM
	actualText.erase(0, 3);
	EXPECT_EQ(expectedText, actualText);
}

TEST(UnbufferedFileSink, OpenAppend)
{
	std::filesystem::path logFilePath = TEST_OUTPUT_DIRECTORY_PATH;
	logFilePath /= "test.log";

	std::string preExistingLogData = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Duis eget enim nec purus accumsan viverra eu non arcu. Pellentesque congue diam eu justo interdum sagittis. Duis sagittis vehicula suscipit. Aenean condimentum vestibulum porta. In id dignissim massa. Nullam venenatis dui ultricies justo pharetra, nec molestie sapien lobortis. Sed vulputate mi ac neque aliquam consequat. In nec vulputate mauris, in fringilla justo. Mauris rhoncus dignissim ante, commodo pharetra dolor dapibus a.";
	std::string additionalLogData = "Donec aliquam ante a felis fermentum volutpat. Quisque tempor, nibh in efficitur consequat, nunc massa tincidunt enim, sit amet pulvinar justo nulla faucibus massa. Quisque a mollis magna. Sed turpis massa, accumsan eu ultrices ut, imperdiet non risus. Pellentesque eu euismod est. Sed in nibh aliquam, volutpat risus ac, elementum dui. Mauris non metus ligula. Praesent pellentesque mauris quis orci accumsan, et fermentum ipsum auctor. Aliquam scelerisque nisl a massa mattis convallis a eu sapien. Suspendisse lacinia maximus enim, eget commodo urna feugiat id. In porttitor ligula eget velit rutrum tristique. Phasellus at diam sed ligula accumsan sollicitudin. Aenean consequat suscipit massa, suscipit imperdiet tellus scelerisque sit amet.";

	LogUnbufferedFileSink file;
	ASSERT_TRUE(file.Create(logFilePath));
	file.WriteToFile(preExistingLogData);
	file.Close();

	ASSERT_TRUE(file.OpenToAppend(logFilePath));
	file.WriteToFile(additionalLogData);
	file.Close();

	std::string actualLog = ReadLogFileAsBinary(logFilePath);
	EXPECT_NE(actualLog.find(preExistingLogData), std::string::npos);
	EXPECT_NE(actualLog.find(additionalLogData), std::string::npos);
}
