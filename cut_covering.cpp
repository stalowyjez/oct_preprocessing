#include <vector>
#include <iostream>

#include "graph.h"
#include "cut_covering.h"
#include "matroid.h"
#include "gammoid.h"
#include "gaussian.h"

using namespace std;

pair<Graph,vector<int>> addSinks(Graph my_graph, VertexSet sinks) {
	vector<int> sink_copy(my_graph.size(),0);
	for (int v : sinks) {
		sink_copy[v] = my_graph.size();
		my_graph.push_back(vector<int>());
	}

	for (int v = 1; v < ((int)sink_copy.size()); v++) {
		int k = my_graph[v].size();
		for (int i = 0; i < k; i++)
			if (sink_copy[my_graph[v][i]] != 0)
				my_graph[v].push_back(sink_copy[my_graph[v][i]]);
	}

	return mp(my_graph,sink_copy);
}

int findIrrelevantVertex(Graph my_graph, VertexSet const & S, VertexSet const & T, VertexSet const & forbidden, int min_cut) {
	int n = my_graph.size()-1;

	VertexSet core_vertices;
	for (int v = 1; v <= n; v++)
		if (S.count(v) == 0 and T.count(v) == 0 and my_graph[v].size() > 0)
			core_vertices.insert(v);

	if (core_vertices.size() == 0l)
		return -1;

	auto [with_sinks,sink_map] = addSinks(my_graph,core_vertices);
	auto [t_with_sinks,t_sink_map] = addSinks(transposeGraph(my_graph),core_vertices);

	auto uniform = getUniformMatroidRep(n,min_cut);
	auto gammoid_S = getGammoidRep(with_sinks,S);
	auto gammoid_T = getGammoidRep(t_with_sinks,T);

	vector<int> vert_map;
	for (int v : forbidden)
		vert_map.push_back(v);
	for (int v : core_vertices) if (forbidden.count(v) == 0)
		vert_map.push_back(v);

	Matrix vert_reps;
	for (int v : vert_map) {
		vector<Field::Element> rep_vec;
		for (Field::Element x1 : uniform[v-1])
			for (Field::Element x2 : gammoid_S[sink_map[v]-1])
				for (Field::Element x3 : gammoid_T[t_sink_map[v]-1])
					rep_vec.push_back(x1*x2*x3);
		vert_reps.push_back(rep_vec);
	}

	int res = getFirstZeroRow(move(vert_reps));

	if (res == -1)
		return -1;
	else
		return vert_map[res];
}

VertexSet selectCCVertices(Graph graph, VertexSet const & S, VertexSet const & T, VertexSet const & forbidden, int min_cut) {
	VertexSet result;
	int v;

	cerr << "UNDELETABLE VERTICES:";
	while ((v = findIrrelevantVertex(graph,S,T,forbidden,min_cut)) != -1) {
		result.insert(v);
		graph = bypassVertex(move(graph),v);
		cerr << " " << v;
	}
	cerr << endl;

	return result;
}

