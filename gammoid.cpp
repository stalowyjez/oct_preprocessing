#include <vector>
#include <unordered_map>
#include <iostream>

#include "gammoid.h"
#include "field.h"
#include "gaussian.h"
#include "graph.h"

using namespace std;

//
// Sorting gammoid
//

vector<int> sortGammoid(Graph & graph, VertexSet && sources) {
	int c_num=1,s_num=(graph.size()-sources.size());

	// getting new names

	vector<int> new_name(graph.size());
	for (int v = 1; v < ((int)graph.size()); v++)
		if (sources.count(v) > 0) {
			new_name[v] = s_num;
			s_num++;
		} else {
			new_name[v] = c_num;
			c_num++;
		}

	// rewrite graph

	Graph result_graph(graph.size());
	for (int v = 1; v < ((int)graph.size()); v++)
		for (int w : graph[v])
			result_graph[new_name[v]].push_back(new_name[w]);
	graph.swap(result_graph);

	return new_name;
}

//
// Representation of gammoid
//

struct BipartiteGraph {
	int m, n;
	EdgeSet edges;
};

BipartiteGraph getBipartiteDual(Graph const & graph, int num_of_sources) {
	BipartiteGraph result;
	result.n = graph.size() - 1;
	result.m = graph.size() - 1 - num_of_sources;

	for (int v = 1; v <= result.m; v++)
		result.edges.insert(mp(v,v));

	for (int v = 1; v < ((int)graph.size()); v++)
		for (int w : graph[v])
			if (w <= result.m)
				result.edges.insert(mp(v,w));

	return result;
}

Matrix bipartiteAsMatrix(BipartiteGraph const & graph) { // WARNING : vertices change number by one
	vector<vector<Field::Element>> result;

	for (int v = 1; v <= graph.m; v++) {
		result.push_back(vector<Field::Element>());

		for (int w = 1; w <= graph.n; w++)
			if (graph.edges.count(mp(w,v)) > 0)
				result[v-1].push_back(Field::Element());
			else
				result[v-1].push_back(Field::Element(0));
	}

	return result;
}

Matrix dualRep(pair<Matrix,vector<int>> && p) { // WARNING: input must be normalized !!!
	auto matrix = p.first;

	int m = matrix.size();
	int n = matrix[0].size();

	Matrix result(n,vector<Field::Element>(n-m,Field::Element(0)));

	for (int i = 0; i < n-m; i++)
		result[i+m][i] = Field::Element(1);

	for (int i = 0; i < m; i++)
		for (int j = 0; j < n-m; j++)
			result[i][j] = matrix[i][j+m];

	return result;
}

Matrix getGammoidRep(Graph const & graph, VertexSet const & sources) { // WARNING : row which represents vertex is lowered by one
	int source_num=graph.size()-sources.size(),non_source_num=1;
	vector<int> num_map(graph.size());
	for (int v = 1; v < ((int)graph.size()); v++)
		if (sources.count(v) == 0)
			num_map[v] = non_source_num++;
		else
			num_map[v] = source_num++;

	Graph sorted_copy = changeLabels(graph,num_map);

	auto robust_result = dualRep(reduceMatrix(bipartiteAsMatrix(getBipartiteDual(sorted_copy,(int)sources.size()))));
	Matrix result(robust_result.size());

	auto inv_num_map = invPerm(num_map);
	for (int i = 0; i < ((int)inv_num_map.size())-1; i++)
		inv_num_map[i] = inv_num_map[i+1]-1;
	inv_num_map.pop_back();
	for (int i = 0; i < (int)num_map.size()-1; i++)
		result[i] = robust_result[num_map[i+1]-1];

	return result;
}

bool isIndependent(Matrix const & gammoid_rep, VertexSet const & X) {
	Matrix vec_set;

	for (int x : X)
		vec_set.push_back(gammoid_rep[x-1]);

	return X.size() == getSignificantRows(move(vec_set)).size();
}

