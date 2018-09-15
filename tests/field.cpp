#include <gtest/gtest.h>

#include "../field.h"

using namespace std;

TEST(simpleArithmetics, addition) {
	Field::Element A(10);
	Field::Element B(7);

	Field::Element C;
	C = A + B;

	ASSERT_EQ(C, 4);

	A = Field::Element(9);
	C = A + C;

	ASSERT_TRUE(C == 0);
}

TEST(simpleArithmetics, subtraction) {
	Field::Element A(10);
	Field::Element B(7);

	Field::Element C;
	C = A - B;

	ASSERT_EQ(C, 3);

	A = Field::Element(9);
	auto D = C - A;
	C = A - C;

	ASSERT_EQ(D, 7);
	ASSERT_EQ(C, 6);
}

TEST(simpleArithmetics, multiplication) {
	Field::Element A(10);
	Field::Element B(7);

	Field::Element C;
	C = A * B;

	ASSERT_EQ(C, 5);

	A = Field::Element(9);
	C = A * C;

	ASSERT_EQ(C, 6);
}

TEST(simpleArithmetics, inversion) {
	Field::Element A(10);
	Field::Element B(5);
	Field::Element C(12);
	Field::Element D(1);

	ASSERT_EQ(A.inv(), 4);
	ASSERT_EQ(B.inv(), 8);
	ASSERT_EQ(C.inv(), 12);
	ASSERT_EQ(D.inv(), 1);
}

TEST(simpleArithmetics, division) {
	Field::Element A(10);
	Field::Element B(7);

	Field::Element C;
	C = (A / B);

	ASSERT_EQ(C, 7);

	A = Field::Element(9);
	C = A / C;

	ASSERT_EQ(C, 5);
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
