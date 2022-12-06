#include <gtest.h>
#include "Translator.h"

TEST(Translator, POSITIVE_PLUS) {
	Translator s("2.0+1.0");
	EXPECT_EQ(3.0, s.Calculate());
}

TEST(Translator, POSITIVE_PLUS_INTEGERS) {
	Translator s("2+1");
	EXPECT_EQ(3.0, s.Calculate());
}

TEST(Translator, POSITIVE_PLUS_WITH_SPACES) {
	Translator s("  2.0      + 1.0   ");
	EXPECT_EQ(3.0, s.Calculate());
}

TEST(Translator, POSITIVE_MINUS) {
	Translator s("2.0-1.0");
	EXPECT_EQ(1.0, s.Calculate());
}

TEST(Translator, POSITIVE_NEGATIVE_MINUS) {
	Translator s("2.0-(-1.0)");
	std::cout << s.GetPostfix();
	EXPECT_EQ(3.0, s.Calculate());
}
TEST(Translator, POSITIVE_NEGATIVE_MINUS_MULTIPLY) {
	Translator s("2.0 - ( -1.0 * 5.0 )");
	std::cout << s.GetPostfix();
	EXPECT_EQ(7.0, s.Calculate());
}

TEST(Translator, NEGATIVE_PLUS) {
	Translator s("-2.0+1.0");
	EXPECT_EQ(-1.0, s.Calculate());
}

TEST(Translator, NEGATIVE_MULTIPLY) {
	Translator s("-2.0*2.0");
	EXPECT_EQ(-4.0, s.Calculate());
}