#include <tuple>
#include <queue>
#include <stack>
#include <cassert>
#include <iostream>

#include "basic.h"
#include "graph.h"

using namespace std;

Graph transposeGraph(Graph const & graph) {
	Graph result(graph.size());

	for (int v = 0; v < ((int)graph.size()); v++)
		for (int w : graph[v])
			result[w].push_back(v);

	return result;
}

Graph addSinkCopies(Graph my_graph, VertexSet const & vertices) {
	
	cout << "GRAPH:" << endl;
	for (int v = 1; v < (int)my_graph.size(); v++) {
		cout << v << ":";
		for (int w : my_graph[v])
			cout << " " << w;
		cout << endl;
	}
	vector<int> sink_copy(my_graph.size(),0);
	for (int v : vertices) {
		sink_copy[v] = my_graph.size();
		my_graph.push_back(vector<int>());
	}

	for (int v = 1; v < ((int)sink_copy.size()); v++) {
		int k = (int)my_graph[v].size();
		for (int i = 0; i < k; i++)
			if (sink_copy[my_graph[v][i]] != 0)
				my_graph[v].push_back(sink_copy[my_graph[v][i]]);
	}

	return my_graph;
}

//
// Min vertex cut
//

vector<int> appendToVec(vector<int> && vec, int element) {
	vec.push_back(element);
	return vec;
}

vector<int> getParentPath(int v, vector<int> const& parent) {
	if (parent[v] == v)
		return vector<int>{v};
	else
		return appendToVec(getParentPath(parent[v],parent),v);
}

pair<bool,vector<int>> findIncPath(vector<VertexSet> const& residual_network, int s, int t) {
	vector<int> parent(residual_network.size(),-1);
	queue<int> Q;
	Q.push(s);
	parent[s] = s;
	while (not Q.empty()) {
		int v = Q.front(); Q.pop();
		
		for (int w : residual_network[v]) if (parent[w] == -1) {
			parent[w] = v;
			if (w == t)
				return mp(true,getParentPath(t,parent));

			Q.push(w);
		}
	}

	return mp(false,vector<int>{});
}

vector<VertexSet> repairResidualNetwork(vector<VertexSet> && old_rn, vector<int> const& path) {
	for (int i = 1; i < ((int)path.size()); i++) {
		int v = path.at(i-1);
		int w = path.at(i);

		old_rn[v].erase(w);
		old_rn[w].erase(v);
	}

	return old_rn;
}

int findMaxFlow(Graph const& graph, int s, int t) {
	vector<VertexSet> residual_network(graph.size());
	for (auto neigh : graph) residual_network.push_back(VertexSet(neigh.begin(),neigh.end()));

	int result = 0;
	while (true) {
		auto [success,path] = findIncPath(residual_network,s,t);
		if (success) {
			residual_network = repairResidualNetwork(move(residual_network),path);
			result++;
		} else {
			break;
		}
	}

	return result;
}


tuple<Graph,int,int> createVertexCutGraph(Graph const& graph, VertexSet const S, VertexSet const& T) {
	Graph result;
	vector<int> in_vertex(graph.size()), out_vertex(graph.size());

	for (int v = 1; v < ((int)graph.size()); v++) {
		in_vertex[v] = result.size();
		result.push_back(vector<int>());
		out_vertex[v] = result.size();
		result.push_back(vector<int>());
	}

	for (int v = 1; v < ((int)graph.size()); v++)
		for (int w : graph[v]) {
			result[out_vertex[v]].push_back(in_vertex[w]);
			result[out_vertex[w]].push_back(in_vertex[v]);
		}

	int s = result.size();
	for (int v : S)
		result[s].push_back(in_vertex[v]);

	int t = result.size();
	for (int v : T)
		result[out_vertex[v]].push_back(t);

	return make_tuple(result,s,t);
}



int minVertexCut(Graph const& graph, VertexSet const& S, VertexSet const& T) {
	auto [vertex_cut_graph, s, t] = createVertexCutGraph(graph,S,T);
	return findMaxFlow(vertex_cut_graph,s,t);
}

//
// Biconnected components
//

pair<list<int>,int> bcDFS(int v, int parent, Graph const & graph, vector<int> & depth, list<list<int>> & result) {
	int low = v;
	if (parent != -1)
		depth[v] = depth[parent]+1;
	else
		depth[v] = 0;

	list<int> my_bcc{v};

	for (int w : graph[v]) if (w != parent) {
		if (depth[w] == -1) {
			auto p = bcDFS(w,v,graph,depth,result);
			auto w_bcc = p.first;
			auto w_low = p.second;

			if (depth[w_low] >= depth[v]) {
				w_bcc.push_back(v);
				result.push_back(w_bcc);
			} else {
				my_bcc.splice(my_bcc.end(),move(w_bcc));
			}

			low = (depth[w_low] < depth[low] ? w_low : low);
		} else {
			low = (depth[w] < depth[low] ? w : low);
		}
	}

	return make_pair(my_bcc,low);
}

list<list<int>> getBCC(Graph const& graph) {
	vector<int> low(graph.size());
	vector<int> depth(graph.size(),-1);
	list<list<int>> result;

	for (int v = 1; v < ((int)graph.size()); v++)
		if (depth[v] == -1)
			bcDFS(v,-1,graph,depth,result);

	return result;
}


int maxDegreeVertex(Graph const & graph) {
	int result=0, deg=-1;

	for (int v = 1; v < ((int)graph.size()); v++)
		if (((int)graph[v].size()) > deg) {
			result = v;
			deg = graph[v].size();
		}
	
	return result;
}

Graph removeVertex(Graph && graph, int v) {
	Graph result(graph.size());

	for (int w = 1; w < ((int)graph.size()); w++)
		if (w != v)
			for (int u : graph[w])
				if (u != v)
					result[w].push_back(u);

	return result;
}

Graph bypassVertex(Graph && graph, int v) {
	VertexSet in_set, out_set(graph[v].begin(),graph[v].end());

	graph[v] = vector<int>();

	for (int w = 1; w < (int)graph.size(); w++)
		for (int u : graph[w])
			if (u == v)
				in_set.insert(w);

	for (int w : in_set) {
		VertexSet current_neigh(graph[w].begin(),graph[w].end());
		for (int u : out_set)
			if (current_neigh.count(u) == 0 and w != u)
				current_neigh.insert(u);
		current_neigh.erase(v);
		graph[w] = vector<int>(current_neigh.begin(),current_neigh.end());
	}

	return graph;
}

bool hasEdges(Graph const& graph) {
	for (int v = 1; v < ((int)graph.size()); v++)
		if (((int)graph[v].size()) > 0)
			return true;

	return false;
}

Graph inducedSubgraph(Graph const& graph,std::vector<bool> const& is_chosen) {
	Graph result(graph.size());

	for (int v = 1; v < ((int)graph.size()); v++)
		for (int w : graph[v])
			if (is_chosen[v] and is_chosen[w])
				result[v].push_back(w);

	return result;
}

Graph inducedSubgraph(Graph const& graph,std::set<int> const& verts) {
	Graph result(graph.size());

	for (int v = 1; v < ((int)graph.size()); v++)
		for (int w : graph[v])
			if (verts.count(v) and verts.count(w))
				result[v].push_back(w);

	return result;
}

enum Color {
	RED,
	BLUE,
	NONE
};

bool isBipartite(Graph const& graph) {
	vector<Color> color(graph.size(),NONE);
	queue<int> Q;

	for (int vertex = 1; vertex < ((int)graph.size()); vertex++) if (color[vertex] == NONE) {
		Q.push(vertex);
		color[vertex] = RED;

		while (not Q.empty()) {
			auto v = Q.front(); Q.pop();

			for (int w : graph[v])
				if (color[w] == NONE) {
					color[w] = (color[v] == BLUE ? RED : BLUE);
					Q.push(w);
				} else if (color[w] == color[v]) {
					return false;
				}
		}
	}

	return true;
}

pair<VertexSet,VertexSet> getParts(Graph const& graph) {
	vector<Color> color(graph.size(),NONE);

	for (int v = 1; v < ((int)graph.size()); v++) if (color[v] == NONE) {
		queue<int> Q;
		Q.push(v);
		color[v] = RED;

		while (not Q.empty()) {
			auto w = Q.front(); Q.pop();

			for (int u : graph[w])
				if (color[u] == NONE) {
					color[u] = (color[w] == BLUE ? RED : BLUE);
					Q.push(u);
				}
		}
	}

	VertexSet red, blue;

	for (int v = 1; v < ((int)graph.size()); v++)
		if (color[v] == BLUE)
			blue.insert(v);
		else
			red.insert(v);

	return mp(red,blue);
}

//
// Components
//

void stupidDFS(int v, Graph const & graph, VertexSet & actual_component, vector<bool> & visited) {
	actual_component.insert(v);
	visited[v] = true;

	for (int w : graph[v])
		if (not visited[w])
			stupidDFS(w,graph,actual_component,visited);
}

vector<VertexSet> getComponents(Graph const& graph) {
	vector<VertexSet> result;
	vector<bool> visited(graph.size(),false);
	for (int v = 1; v < ((int)graph.size()); v++)
		if (not visited[v]) {
			VertexSet actual_component;
			stupidDFS(v,graph,actual_component,visited);
			result.push_back(move(actual_component));
		}
	return result;
}

Graph mergeVertices(Graph && graph, VertexSet const & vertices) {
	// get smallest

	int smallest = INF;
	for (int v : vertices)
		if (v < smallest)
			smallest = v;

	// in_set and out_set

	Graph result(graph.size());
	for (int v : vertices)
		for (int w : graph[v])
			if (vertices.count(w) == 0)
				result[smallest].push_back(w);

	for (int v = 1; v < ((int)graph.size()); v++) if (vertices.count(v) == 0) {
		bool has_out_edge = false;
		for (int w : graph[v])
			if (vertices.count(w) == 0)
				result[v].push_back(w);
			else
				has_out_edge = true;
		if (has_out_edge)
			result[v].push_back(smallest);
	}

	return result;
}

Graph changeLabels(Graph const & graph, std::vector<int> const & perm) {
	Graph result(graph.size());

	for (int v = 1; v < ((int)graph.size()); v++)
		for (int w : graph[v])
			result[perm[v]].push_back(perm[w]);

	return result;
}

//
// Strongly Connencted Components
//

void sssDFS(int v, stack<int> & S, vector<bool> & used, Graph const & graph) {
	used[v] = true;

	for (int w : graph[v])
		if (not used[w])
			sssDFS(w,S,used,graph);

	S.push(v);
}


void sssDFS2(int v, vector<int> & component, int component_num, Graph const & graph) {
	component[v] = component_num;

	for (int w : graph[v])
		if (component[w] == -1)
			sssDFS2(v,component,component_num,graph);
}

std::pair<std::vector<int>,Graph> getSSS(Graph const& graph) {
	Graph t_graph = transposeGraph(graph);

	vector<int> component(graph.size(),-1);
	vector<bool> used(graph.size(), false);
	int last_component = 0;
	for (int v = 1; v < ((int)graph.size()); v++)
		if (component[v] == -1) {
			stack<int> S;
			sssDFS(v,S,used,graph);

			while (not S.empty()) {
				int v = S.top(); S.pop();
				if (component[v] == -1) {
					last_component++;
					sssDFS2(v,component,last_component,t_graph);
				}
			}
		}

	vector<set<int>> _component_graph(last_component+1);
	for (int v = 1; v <= last_component; v++)
		for (int w : graph[v])
			_component_graph[component[v]].insert(component[w]);

	Graph component_graph(_component_graph.size());
	for (int v = 1; v < ((int)_component_graph.size()); v++)
		for (int w : _component_graph[v])
			component_graph[v].push_back(w);

	return mp(component,component_graph);
}

//
// Copy certain vertex
//

pair<Graph,VertexSet> copyVertexKTimes(Graph const & graph, int v, int k) {
	Graph result = graph;
	VertexSet sources;

	for (int i = 1; i <= k; i++) {
		sources.insert(result.size());
		result.push_back(graph[v]);
	}

	for (int w = 1; w < ((int)graph.size()); w++)
		for (int u : graph[w])
			if (u == v)
				for (int v1 : sources)
					result[w].push_back(v1);

	return mp(result,sources);
}

pair<Graph,VertexSet> copySourceKTimes(Graph const & graph, int v, int k) {
	Graph result = graph;
	VertexSet sources{v};

	for (int i = 1; i <= k; i++) {
		sources.insert(result.size());
		result.push_back(graph[v]);
	}

	return mp(result,sources);
}

//
// Max matching -- stupid greedy algorithm
//

vector<pair<int,int>> maxMatching(Graph const &) {
	// blossom implementation

	assert(false);
}

//
// getEdges
//

vector<pair<int,int>> getEdges(Graph const & graph) {
	vector<pair<int,int>> result;

	for (int v = 1; v < ((int)graph.size()); v++)
		for (int w : graph[v])
			if (v < w)
				result.push_back(mp(v,w));

	return result;
}

vector<pair<int,int>> getArcs(Graph const & graph) {
	vector<pair<int,int>> result;

	for (int v = 1; v < ((int)graph.size()); v++)
		for (int w : graph[v])
			result.push_back(mp(v,w));

	return result;
}

pair<Graph,VertexSet> addSourceCopies(Graph graph, VertexSet const & vertices_to_copy) {
	VertexSet copies;
	for (int v : vertices_to_copy) {
		int v_copy = graph.size();
		graph.push_back(vector<int>());
		copies.insert(v_copy);
		for (int w : graph[v])
			graph[v_copy].push_back(w);
	}
	return mp(graph,copies);
}

pair<Graph,VertexSet> addSinksAfter(Graph graph, VertexSet const & S) {
	VertexSet sink_set;
	for (int v : S) {
		sink_set.insert(graph.size());
		graph[v].push_back(graph.size());
		graph.push_back(vector<int>());
	}

	return mp(graph,sink_set);
} 

pair<Graph,VertexSet> addSourcesBefore(Graph graph, VertexSet const & S) {
	VertexSet source_set;
	for (int v : S) {
		source_set.insert((int)graph.size());
		graph.push_back(vector<int>());
		graph[graph.size()-1].push_back(v);
	}

	return mp(graph,source_set);
}

//
// Duplicate graph
//

pair<Graph,vector<int>> duplicateGraph(Graph const & graph) {
	Graph result = graph;
	vector<int> copy_map(graph.size());

	for (int v = 1; v < (int)graph.size(); v++) {
		copy_map.push_back(result.size());
		result.push_back(vector<int>());
	}

	for (int v = 1; v < (int)graph.size(); v++)
		for (int u : graph[v])
			result[v].push_back(copy_map[u]);

	return mp(result,copy_map);
}

