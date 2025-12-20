#include "MKLogging/Logger.h"
#include <gtest/gtest.h>

using namespace MKLogging;

TEST(ELogLevel, FromString)
{
  EXPECT_EQ(ELogLevel::Error, ELogLevel_FromString("Error"));
  EXPECT_EQ(ELogLevel::Error, ELogLevel_FromString("eRror"));
  EXPECT_EQ(ELogLevel::Error, ELogLevel_FromString("erRor"));
  EXPECT_EQ(ELogLevel::Error, ELogLevel_FromString("errOr"));
  EXPECT_EQ(ELogLevel::Error, ELogLevel_FromString("erroR"));
  EXPECT_EQ(ELogLevel::Error, ELogLevel_FromString("ERROR"));

  EXPECT_EQ(ELogLevel::None, ELogLevel_FromString("NONE"));
  EXPECT_EQ(ELogLevel::Warning, ELogLevel_FromString("WARNING"));
  EXPECT_EQ(ELogLevel::Info, ELogLevel_FromString("INFO"));
  EXPECT_EQ(ELogLevel::Debug, ELogLevel_FromString("DEBUG"));
  EXPECT_EQ(ELogLevel::Verbose, ELogLevel_FromString("VERBOSE"));
}