#include <gtest/gtest.h>
#include <iostream>

#include "../graph.h"

using namespace std;

TEST(bypassVertex,simplestPossible) {
	Graph graph{
		vector<int>{},
		vector<int>{2},
		vector<int>{1,3},
		vector<int>{2}
	};

	Graph expected_result{
		vector<int>{},
		vector<int>{3},
		vector<int>{},
		vector<int>{1}
	};

	auto result = bypassVertex(move(graph),2);

	ASSERT_EQ(result.size(),4);

	ASSERT_EQ(result[0].size(),0);
	ASSERT_EQ(result[1].size(),1);
	ASSERT_EQ(result[2].size(),0);
	ASSERT_EQ(result[3].size(),1);

	ASSERT_EQ(result[1][0],3);
	ASSERT_EQ(result[3][0],1);
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

