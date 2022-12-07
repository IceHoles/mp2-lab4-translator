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
	EXPECT_EQ(3.0, s.Calculate());
}
TEST(Translator, POSITIVE_NEGATIVE_MINUS_MULTIPLY) {
	Translator s("2.0 - ( -1.0 * 5.0 )");
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

TEST(Translator, MULTIPLY_NEGATIVE) {
	Translator s("2.0*(-2.0)");
	EXPECT_EQ(-4.0, s.Calculate());
}


TEST(Translator, NEGATIVE_SQUARE) {
	Translator s("-2.0*(-2.0)");
	EXPECT_EQ(4.0, s.Calculate());
}

TEST(Translator, POSITIVE_DIVIDE) {
	Translator s("2.0/2.0");
	EXPECT_EQ(1.0, s.Calculate());
}

TEST(Translator, NEGATIVE_DIVIDE) {
	Translator s("-2.0/2.0");
	EXPECT_EQ(-1.0, s.Calculate());
}

TEST(Translator, DIVIDE_NEGATIVE) {
	Translator s("2.0/(-2.0)");
	EXPECT_EQ(-1.0, s.Calculate());
}

TEST(Translator, NEGATIVE_DIVIDE_NEGATIVE) {
	Translator s("-2.0/(-2.0)");
	EXPECT_EQ(1.0, s.Calculate());
}

TEST(Translator, ZERO_DIVIDE) {
	Translator s("0.0/2.0");
	EXPECT_EQ(0.0, s.Calculate());
}


TEST(Translator, NEGATIVE_MULTIPLY_BRACKETS) {
	Translator s("(-2.0)*2.0");
	EXPECT_EQ(-4.0, s.Calculate());
}

TEST(Translator, PI_MULTIPLY) {
	Translator s("3.1415926535*2");
	EXPECT_EQ(6.2831853070, s.Calculate());
}

TEST(Translator, VARIABLE_TEST) {
	Translator s("A+B");
	std::istringstream values("2 2");
	std::ostream p(nullptr);
	EXPECT_EQ(4.0, s.Calculate(values, p));
}


TEST(Translator, VARIABLE_MULTIPLY) {
	Translator s("A*B");
	std::istringstream values("2 2");
	std::ostream p(nullptr);
	EXPECT_EQ(4.0, s.Calculate(values, p));
}

TEST(Translator, VARIABLE_NEGATIVE) {
	Translator s("A-B");
	std::istringstream values("2 2");
	std::ostream p(nullptr);
	EXPECT_EQ(0.0, s.Calculate(values, p));
}

TEST(Translator, VARIABLE_NEGATIVE_MULTIPLY) {
	Translator s("A*B");
	std::istringstream values("2 -2");
	std::ostream p(nullptr);
	EXPECT_EQ(-4.0, s.Calculate(values, p));
}

TEST(Translator, VARIABLE_DIVIDE) {
	Translator s("A/B");
	std::istringstream values("2 4");
	std::ostream p(nullptr);
	EXPECT_EQ(0.5, s.Calculate(values, p));
}

TEST(Translator, VARIABLE_DIVIDE_NEGATIVE) {
	Translator s("A/B");
	std::istringstream values("2 -4");
	std::ostream p(nullptr);
	EXPECT_EQ(-0.5, s.Calculate(values, p));
}

TEST(Translator, VARIABLE_BRACKETS) {
	Translator s("A-(B)");
	std::istringstream values("2 -2");
	std::ostream p(nullptr);
	EXPECT_EQ(4.0, s.Calculate(values, p));
}

TEST(Translator, MULTIPLE_VARIABLES) {
	Translator s("A+A");
	std::istringstream values("2");
	std::ostream p(nullptr);
	EXPECT_EQ(4.0, s.Calculate(values, p));
}
   
TEST(Translator, MULTIPLE_MULPIPLE_VARIABLES) {
	Translator s("A+A+B+B");
	std::istringstream values("2 2");
	std::ostream p(nullptr);
	EXPECT_EQ(8.0, s.Calculate(values, p));
}

TEST(Translator, VARIABLE_COMPLICATED_TEST) {
	Translator s("A+B-(A*B*2-3*B)/(2*B-A)");
	std::istringstream values("6 7");
	std::ostream p(nullptr);
	EXPECT_EQ(5.125, s.Calculate(values, p));
}

TEST(Translator, VARIABLE_SPACES) {
	Translator s("A          + B");
	std::istringstream values("2 2");
	std::ostream p(nullptr);
	EXPECT_EQ(4.0, s.Calculate(values, p));
}

TEST(Translator, VARIABLE_MULTIPLE_CHARACTERS) {
	Translator s("Aa1+Bb2");
	std::istringstream values("2 2");
	std::ostream p(nullptr);
	EXPECT_EQ(4.0, s.Calculate(values, p));
}

TEST(Translator, VARIABLE_MULTIPLE_CHARACTERS_COMPLICATED) {
	Translator s("Aa1+Bb2-(Aa1*Bb2*2-3*Bb2)/(2*Bb2-Aa1)");
	std::istringstream values("6 7");
	std::ostream p(nullptr);
	EXPECT_EQ(5.125, s.Calculate(values, p));
}

TEST(Translator, VARIABLE_MULTIPLE_CHARACTERS_COMPLICATED_SPACES) {
	Translator s("Aa1      + Bb2  - (  Aa1  *  Bb2* 2-  3* Bb2  ) / (  2 * Bb2 - Aa1  ) ");
	std::istringstream values("6 7");
	std::ostream p(nullptr);
	EXPECT_EQ(5.125, s.Calculate(values, p));
}

TEST(Translator, EXCEPTION_BRACKETS) {
	ASSERT_ANY_THROW(Translator s("1)+3("););
}

TEST(Translator, EXCEPTION_BRACKETS_FIRST) {
	ASSERT_ANY_THROW(Translator s(")1+2("););
}

TEST(Translator, EXCEPTION_BRACKETS_LAST) {
	ASSERT_ANY_THROW(Translator s("1+2)("););
}