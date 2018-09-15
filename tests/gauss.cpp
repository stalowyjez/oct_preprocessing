#include <gtest/gtest.h>
#include <iostream>

#include "../basic.h"
#include "../gaussian.h"

using namespace std;

TEST(reduceMatrix,small) {
	Matrix matrix = vector<vector<Field::Element>> {
		vector<Field::Element>{8,2,4,0,1,5},
		vector<Field::Element>{1,4,0,0,8,9},
		vector<Field::Element>{3,0,1,9,0,3}
	};

	Matrix expected = vector<vector<Field::Element>> {
		vector<Field::Element>{1,0,0,8,16,0},
		vector<Field::Element>{0,1,0,21,21,8},
		vector<Field::Element>{0,0,1,8,21,3}
	};

	auto [result,perm] = reduceMatrix(move(matrix));

	for (int i = 0; i < (int)expected.size(); i++)
		for (int j = 0; j < (int)expected[0].size(); j++)
			ASSERT_EQ(result[i][j],expected[i][j]);

	ASSERT_EQ(perm.size(),6);
	for (int i = 0; i < 6; i++)
		ASSERT_EQ(perm[i],i);
}

TEST(reduceMatrix,medium) {
	Matrix matrix = vector<vector<Field::Element>> {
		vector<Field::Element>{8,2,4,1,0,1,5},
		vector<Field::Element>{1,4,0,8,0,8,9},
		vector<Field::Element>{3,0,1,0,9,0,3},
		vector<Field::Element>{8,6,7,0,0,0,1},
		vector<Field::Element>{8,6,7,0,0,0,1}
	};

	Matrix expected = vector<vector<Field::Element>> {
		vector<Field::Element>{1,0,0,0,10,0,20},
		vector<Field::Element>{0,1,0,0,15,0,17},
		vector<Field::Element>{0,0,1,0,2,0,12},
		vector<Field::Element>{0,0,0,1,20,1,16}
	};

	auto [result,perm] = reduceMatrix(move(matrix));

	for (int i = 0; i < (int)expected.size(); i++)
		for (int j = 0; j < (int)expected[0].size(); j++)
			ASSERT_EQ(result[i][j],expected[i][j]);

	ASSERT_EQ(perm.size(),7);
	for (int i = 0; i < 7; i++)
		ASSERT_EQ(perm[i],i);
}

TEST(reduceMatrix,large) {
	Matrix matrix = vector<vector<Field::Element>> {
		vector<Field::Element>{11,2,1,4,1,0,1,5},
		vector<Field::Element>{1,4,2,0,8,9,7,9},
		vector<Field::Element>{3,0,4,7,0,9,0,12},
		vector<Field::Element>{8,6,17,8,23,6,0,1},
		vector<Field::Element>{8,6,22,7,21,6,0,1},
		vector<Field::Element>{3,6,5,19,0,11,13,1}
	};

	Matrix expected = vector<vector<Field::Element>> {
		vector<Field::Element>{1,0,0,0,0,0,11,5},
		vector<Field::Element>{0,1,0,0,0,0,16,1},
		vector<Field::Element>{0,0,1,0,0,0,6,22},
		vector<Field::Element>{0,0,0,1,0,0,13,19},
		vector<Field::Element>{0,0,0,0,1,0,20,11},
		vector<Field::Element>{0,0,0,0,0,1,4,16}
	};

	auto [result,perm] = reduceMatrix(move(matrix));

	for (int i = 0; i < (int)expected.size(); i++)
		for (int j = 0; j < (int)expected[0].size(); j++)
			ASSERT_EQ(result[i][j],expected[i][j]);

	ASSERT_EQ(perm.size(),8);
	for (int i = 0; i < 8; i++)
		ASSERT_EQ(perm[i],i);
}

TEST(reduceMatrix,perm) {
	Matrix matrix = vector<vector<Field::Element>> {
		vector<Field::Element>{0,1,1,4},
		vector<Field::Element>{1,3,4,1},
		vector<Field::Element>{2,7,9,8}
	};

	Matrix expected = vector<vector<Field::Element>> {
		vector<Field::Element>{1,0,0,1},
		vector<Field::Element>{0,1,0,1},
		vector<Field::Element>{0,0,1,0}
	};

	auto [result,perm] = reduceMatrix(move(matrix));

	for (int i = 0; i < (int)expected.size(); i++)
		for (int j = 0; j < (int)expected[0].size(); j++)
			ASSERT_EQ(result[i][j],expected[i][j]);

	ASSERT_EQ(perm.size(),4);
	ASSERT_EQ(perm[0],1);
	ASSERT_EQ(perm[1],0);
	ASSERT_EQ(perm[2],3);
	ASSERT_EQ(perm[3],2);
}

TEST(repVector,small) {
	Matrix vecs = vector<vector<Field::Element>> {
		vector<Field::Element>{0,1,1,5,4},
		vector<Field::Element>{1,3,4,8,6},
	};

	auto result = getAllDets(vecs);
	ASSERT_EQ(result[0],22);
	ASSERT_EQ(result[1],22);
	ASSERT_EQ(result[2],18);
	ASSERT_EQ(result[3],19);
	ASSERT_EQ(result[4],1);
	ASSERT_EQ(result[5],16);
	ASSERT_EQ(result[6],17);
	ASSERT_EQ(result[7],11);
	ASSERT_EQ(result[8],13);
	ASSERT_EQ(result[9],21);
}

TEST(repVector,medium) {
	Matrix vecs = vector<vector<Field::Element>> {
		vector<Field::Element>{7,1,1,0,0},
		vector<Field::Element>{1,0,4,2,1},
		vector<Field::Element>{1,3,6,8,9}
	};

	auto result = getAllDets(vecs);
	ASSERT_EQ(result[0],9);
	ASSERT_EQ(result[1],21);
	ASSERT_EQ(result[2],17);
	ASSERT_EQ(result[3],19);
	ASSERT_EQ(result[4],18);
	ASSERT_EQ(result[5],1);
	ASSERT_EQ(result[6],3);
	ASSERT_EQ(result[7],10);
	ASSERT_EQ(result[8],10);
	ASSERT_EQ(result[9],10);
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

