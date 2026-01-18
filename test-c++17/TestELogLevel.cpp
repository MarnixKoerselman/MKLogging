#include "MKLogging/Logger.h"
#include <gtest/gtest.h>

TEST(ELogLevel, FromString)
{
  EXPECT_EQ(MKLogging::ELogLevel::Error, MKLogging::ELogLevel_FromString("Error"));
  EXPECT_EQ(MKLogging::ELogLevel::Error, MKLogging::ELogLevel_FromString("eRror"));
  EXPECT_EQ(MKLogging::ELogLevel::Error, MKLogging::ELogLevel_FromString("erRor"));
  EXPECT_EQ(MKLogging::ELogLevel::Error, MKLogging::ELogLevel_FromString("errOr"));
  EXPECT_EQ(MKLogging::ELogLevel::Error, MKLogging::ELogLevel_FromString("erroR"));
  EXPECT_EQ(MKLogging::ELogLevel::Error, MKLogging::ELogLevel_FromString("ERROR"));

  EXPECT_EQ(MKLogging::ELogLevel::None, MKLogging::ELogLevel_FromString("NONE"));
  EXPECT_EQ(MKLogging::ELogLevel::Warning, MKLogging::ELogLevel_FromString("WARNING"));
  EXPECT_EQ(MKLogging::ELogLevel::Info, MKLogging::ELogLevel_FromString("INFO"));
  EXPECT_EQ(MKLogging::ELogLevel::Debug, MKLogging::ELogLevel_FromString("DEBUG"));
  EXPECT_EQ(MKLogging::ELogLevel::Verbose, MKLogging::ELogLevel_FromString("VERBOSE"));
}

TEST(ELogLevel, FromStringEmpty)
{
  EXPECT_THROW(MKLogging::ELogLevel_FromString(""), std::runtime_error);
  EXPECT_THROW(MKLogging::ELogLevel_FromString(nullptr), std::runtime_error);
}
