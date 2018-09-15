#include <gtest/gtest.h>
#include <iostream>

#include "../basic.h"
#include "../gammoid.h"

using namespace std;


struct BipartiteGraph {
	int m, n;
	EdgeSet edges;
};

BipartiteGraph getBipartiteDual(Graph const & graph, int num_of_sources);

TEST(bipartiteDual, simpleGraph1) {
	Graph graph{
		vector<int>{},
		vector<int>{3,2},
		vector<int>{1,3,4,5},
		vector<int>{1,2,4},
		vector<int>{2,3,5},
		vector<int>{2,4}
	};

	auto result = getBipartiteDual(graph,2);

	ASSERT_EQ(result.n,5);
	ASSERT_EQ(result.m,3);

	ASSERT_EQ(result.edges.count(mp(1,1)),1);
	ASSERT_EQ(result.edges.count(mp(1,2)),1);
	ASSERT_EQ(result.edges.count(mp(1,3)),1);
	ASSERT_EQ(result.edges.count(mp(2,1)),1);
	ASSERT_EQ(result.edges.count(mp(2,2)),1);
	ASSERT_EQ(result.edges.count(mp(2,3)),1);
	ASSERT_EQ(result.edges.count(mp(3,1)),1);
	ASSERT_EQ(result.edges.count(mp(3,2)),1);
	ASSERT_EQ(result.edges.count(mp(3,3)),1);
	ASSERT_EQ(result.edges.count(mp(4,2)),1);
	ASSERT_EQ(result.edges.count(mp(4,3)),1);
	ASSERT_EQ(result.edges.count(mp(5,2)),1);

	ASSERT_EQ(result.edges.size(),12);
}

TEST(bipartiteDual, simpleGraph2) {
	Graph graph{
		vector<int>{},
		vector<int>{3,4},
		vector<int>{4},
		vector<int>{1},
		vector<int>{3},
		vector<int>{2,3},
		vector<int>{1,4},
		vector<int>{1}
	};

	auto result = getBipartiteDual(graph,3);

	ASSERT_EQ(result.n,7);
	ASSERT_EQ(result.m,4);

	ASSERT_EQ(result.edges.count(mp(1,1)),1);
	ASSERT_EQ(result.edges.count(mp(2,2)),1);
	ASSERT_EQ(result.edges.count(mp(3,3)),1);
	ASSERT_EQ(result.edges.count(mp(4,4)),1);

	ASSERT_EQ(result.edges.count(mp(1,3)),1);
	ASSERT_EQ(result.edges.count(mp(1,4)),1);
	ASSERT_EQ(result.edges.count(mp(2,4)),1);
	ASSERT_EQ(result.edges.count(mp(3,1)),1);
	ASSERT_EQ(result.edges.count(mp(4,3)),1);

	ASSERT_EQ(result.edges.count(mp(5,2)),1);
	ASSERT_EQ(result.edges.count(mp(5,3)),1);
	ASSERT_EQ(result.edges.count(mp(6,4)),1);
	ASSERT_EQ(result.edges.count(mp(6,1)),1);
	ASSERT_EQ(result.edges.count(mp(7,1)),1);

	ASSERT_EQ(result.edges.size(),14);
}

TEST(bipartiteDual, simpleGraph3) {
	Graph graph{
		vector<int>{},
		vector<int>{2,3},
		vector<int>{3},
		vector<int>{},
		vector<int>{7,2,6,1},
		vector<int>{2,6},
		vector<int>{5},
		vector<int>{1},
		vector<int>{9},
		vector<int>{1,8,3}
	};

	auto result = getBipartiteDual(graph,4);

	ASSERT_EQ(result.n,9);
	ASSERT_EQ(result.m,5);
	
	set<pair<int,int>> correct_edge_set{
		mp(1,1),mp(2,2),mp(3,3),mp(4,4),mp(5,5),
		mp(1,3),mp(1,2),
		mp(2,3),
		mp(4,1),mp(4,2),
		mp(5,2),
		mp(6,5),
		mp(7,1),
		mp(9,1),mp(9,3)
	};

	for (auto p : correct_edge_set) ASSERT_EQ(result.edges.count(p),1);
	for (auto p : result.edges) ASSERT_EQ(correct_edge_set.count(p),1);
}

Matrix bipartiteAsMatrix(BipartiteGraph const & graph);

TEST(bipartiteToMatrix,simple1) {
	BipartiteGraph bp;
	bp.n = bp.m = 5;
	bp.edges = set<pair<int,int>>{
		mp(1,3),mp(1,4),mp(2,2),mp(3,2),mp(4,1),mp(4,2),mp(4,5),mp(5,3),mp(5,5)
	};

	vector<vector<int>> exp_mat = vector<vector<int>> {
		vector<int>{0,0,1,1,0},
		vector<int>{0,1,0,0,0},
		vector<int>{0,1,0,0,0},
		vector<int>{1,1,0,0,1},
		vector<int>{0,0,1,0,1}
	};

	exp_mat = transpose(exp_mat);

	auto res = bipartiteAsMatrix(bp);

	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			if (exp_mat[i][j] == 0)
				ASSERT_TRUE((res[i][j] == 0));
			else
				ASSERT_FALSE((res[i][j] == 0));
}

TEST(bipartiteToMatrix,simple2) {
	BipartiteGraph bp;
	bp.n = 6; bp.m = 4;
	bp.edges = set<pair<int,int>>{
		mp(1,3),mp(2,2),mp(3,1),mp(3,2),mp(3,4),mp(4,1),mp(4,2),mp(5,3),mp(5,4),mp(6,3)
	};

	vector<vector<int>> exp_mat = vector<vector<int>> {
		vector<int>{0,0,1,1,0,0},
		vector<int>{0,1,1,1,0,0},
		vector<int>{1,0,0,0,1,1},
		vector<int>{0,0,1,0,1,0}
	};

	auto res = bipartiteAsMatrix(bp);

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 6; j++)
			if (exp_mat[i][j] == 0)
				ASSERT_TRUE((res[i][j] == 0));
			else
				ASSERT_FALSE((res[i][j] == 0));
}

Matrix dualRep(pair<Matrix,vector<int>> && matrix);

TEST(dualRep,simple1) {
	Matrix matrix = vector<vector<Field::Element>> {
		vector<Field::Element>{1,0,0,4,3},
		vector<Field::Element>{0,1,0,3,8},
		vector<Field::Element>{0,0,1,9,2}
	};

	Matrix expected = vector<vector<Field::Element>> {
		vector<Field::Element>{4,3},
		vector<Field::Element>{3,8},
		vector<Field::Element>{9,2},
		vector<Field::Element>{1,0},
		vector<Field::Element>{0,1}
	};

	vector<int> vec_map{0,1,2,3,4};

	auto result = dualRep(mp(matrix,vec_map));

	for (int i = 0; i < (int)expected.size(); i++)
		for (int j = 0; j < (int)expected[0].size(); j++)
			ASSERT_EQ(result[i][j],expected[i][j]);
}

TEST(dualRep,simple2) {
	Matrix matrix = vector<vector<Field::Element>> {
		vector<Field::Element>{1,0,0,0,0,5,7,3,7},
		vector<Field::Element>{0,1,0,0,0,9,0,1,1},
		vector<Field::Element>{0,0,1,0,0,3,1,0,3},
		vector<Field::Element>{0,0,0,1,0,1,0,1,0},
		vector<Field::Element>{0,0,0,0,1,2,1,9,1}
	};

	Matrix expected = vector<vector<Field::Element>> {
		vector<Field::Element>{5,7,3,7},
		vector<Field::Element>{9,0,1,1},
		vector<Field::Element>{3,1,0,3},
		vector<Field::Element>{1,0,1,0},
		vector<Field::Element>{2,1,9,1},
		vector<Field::Element>{1,0,0,0},
		vector<Field::Element>{0,1,0,0},
		vector<Field::Element>{0,0,1,0},
		vector<Field::Element>{0,0,0,1}
	};

	vector<int> vec_map{0,1,2,3,4,5,6,7,8};

	auto result = dualRep(mp(matrix,vec_map));

	for (int i = 0; i < (int)expected.size(); i++)
		for (int j = 0; j < (int)expected[0].size(); j++)
			ASSERT_EQ(result[i][j],expected[i][j]);
}

TEST(wholeGammoid,small) {
	Graph graph{
		vector<int>{},
		vector<int>{5},
		vector<int>{1,6},
		vector<int>{1,4},
		vector<int>{2,5,6},
		vector<int>{6},
		vector<int>{7,8},
		vector<int>{6,8},
		vector<int>{6,7}
	};

	auto rep = getGammoidRep(graph,VertexSet{3,4});

	// independent sets
	
	ASSERT_TRUE(isIndependent(rep,VertexSet{3,4}));
	ASSERT_TRUE(isIndependent(rep,VertexSet{3,2}));
	ASSERT_TRUE(isIndependent(rep,VertexSet{1,4}));
	ASSERT_TRUE(isIndependent(rep,VertexSet{5,6}));
	ASSERT_TRUE(isIndependent(rep,VertexSet{6}));
	ASSERT_TRUE(isIndependent(rep,VertexSet{8}));

	// dependent sets
	
	ASSERT_FALSE(isIndependent(rep,VertexSet{6,8}));
	ASSERT_FALSE(isIndependent(rep,VertexSet{1,2,3}));
	ASSERT_FALSE(isIndependent(rep,VertexSet{2,4}));
}

TEST(wholeGammoid,medium) {
	Graph graph{
		vector<int>{},
		vector<int>{2,3},
		vector<int>{6},
		vector<int>{5},
		vector<int>{3},
		vector<int>{8,9},
		vector<int>{11,8},
		vector<int>{3,10},
		vector<int>{},
		vector<int>{2},
		vector<int>{5},
		vector<int>{2}
	};

	auto rep = getGammoidRep(graph,VertexSet{1,4,7});

	// independent sets
	
	ASSERT_TRUE(isIndependent(rep,VertexSet{2,3,7}));
	ASSERT_TRUE(isIndependent(rep,VertexSet{2,7}));
	ASSERT_TRUE(isIndependent(rep,VertexSet{5,6}));
	ASSERT_TRUE(isIndependent(rep,VertexSet{11,8}));
	ASSERT_TRUE(isIndependent(rep,VertexSet{8,9}));
	ASSERT_TRUE(isIndependent(rep,VertexSet{10,5,11}));
	ASSERT_TRUE(isIndependent(rep,VertexSet{1,3,11}));

	// dependent sets
	
	ASSERT_FALSE(isIndependent(rep,VertexSet{2,6}));
	ASSERT_FALSE(isIndependent(rep,VertexSet{11,8,9}));
	ASSERT_FALSE(isIndependent(rep,VertexSet{5,9}));
	ASSERT_FALSE(isIndependent(rep,VertexSet{11,2}));
	ASSERT_FALSE(isIndependent(rep,VertexSet{6,5,8}));
}

TEST(wholeGammoid,large) {
	Graph graph{
		vector<int>{},
		vector<int>{2,3},
		vector<int>{9},
		vector<int>{6},
		vector<int>{1},
		vector<int>{1,2},
		vector<int>{4},
		vector<int>{20},
		vector<int>{6,9},
		vector<int>{8,16},
		vector<int>{8,14},
		vector<int>{9,20},
		vector<int>{10},
		vector<int>{12},
		vector<int>{13},
		vector<int>{18},
		vector<int>{10,15,20},
		vector<int>{19},
		vector<int>{15,16,20},
		vector<int>{18,20},
		vector<int>{2}
	};

	auto rep = getGammoidRep(graph,VertexSet{5,7,11,17});

	// independent sets
	
	ASSERT_TRUE(isIndependent(rep,VertexSet{6,7,8,15}));
	ASSERT_TRUE(isIndependent(rep,VertexSet{20}));
	ASSERT_TRUE(isIndependent(rep,VertexSet{4,3,13}));
	ASSERT_TRUE(isIndependent(rep,VertexSet{6,8,9,20}));
	ASSERT_TRUE(isIndependent(rep,VertexSet{2,16,15}));

	// dependent sets
	
	ASSERT_FALSE(isIndependent(rep,VertexSet{10,13}));
	ASSERT_FALSE(isIndependent(rep,VertexSet{4,3,6}));
	ASSERT_FALSE(isIndependent(rep,VertexSet{6,1,8}));
	ASSERT_FALSE(isIndependent(rep,VertexSet{1,9,10,6}));
	ASSERT_FALSE(isIndependent(rep,VertexSet{16,12}));
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

