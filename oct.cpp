#include <sstream>
#include <map>
#include <queue>

#include "graph.h"
#include "oct.h"
#include "cut_covering.h"
#include "cassert"

//#define DEBUG

using namespace std;

//
// Approximate solution
//

Graph removeProperBC(Graph && graph) {
	auto BCCs = getBCC(graph);

	// make edge set
	
	vector<pair<int,int>> edges = getArcs(graph);
	vector<bool> is_edge_deleted(edges.size(),false);

	// find biconnected components
	
	for (auto _bcc : BCCs) {
		set<int> bcc(_bcc.begin(),_bcc.end()); 
		if (isBipartite(inducedSubgraph(graph,bcc)))
			for (int i = 0; i < ((int)edges.size()); i++)
				if (bcc.count(edges[i].first) > 0 and bcc.count(edges[i].second) > 0)
					is_edge_deleted[i] = true;
	}

	Graph result(graph.size(), vector<int>());

	for (int i = 0; i < ((int)edges.size()); i++)
		if (not is_edge_deleted[i])
			result[edges[i].first].push_back(edges[i].second);

	return result;
}

bool checkOCTSol(VertexSet const & sol, Graph graph) {
	for (int v : sol)
		graph = removeVertex(move(graph),v);

	return isBipartite(graph);
}

VertexSet reduceApprox(Graph const & graph, VertexSet result) {
	VertexSet reduced_result = result;
	for (int v : result) {
		reduced_result.erase(v);
		if (not checkOCTSol(reduced_result,graph))
			reduced_result.insert(v);
	}

	return reduced_result;
}

VertexSet getApproxSol1(Graph graph) {
	auto graph_copy = graph;
	graph = removeProperBC(move(graph));

	VertexSet result;
	while (hasEdges(graph)) {
		int v = maxDegreeVertex(graph);

		result.insert(v);
		graph = removeProperBC(removeVertex(move(graph),v));
	}

	return reduceApprox(graph_copy,result);
}

//
// Approximate solution 2
//

enum Color {
	RED,
	BLUE,
	NONE
};

VertexSet getApproxSol2(Graph graph) {
	VertexSet result;
	vector<Color> color(graph.size(),NONE);

	queue<int> Q;
	for (int vertex = 1; vertex < ((int)graph.size()); vertex++) if (color[vertex] == NONE) {
		Q.push(vertex);
		color[vertex] = RED;

		while (not Q.empty()) {
			auto v = Q.front(); Q.pop();
			if (result.count(v) > 0)
				continue;

			for (int w : graph[v])
				if (color[w] == color[v] and result.count(w) == 0)
					result.insert(v);

			if (result.count(v) > 0)
				continue;

			for (int w : graph[v])
				if (color[w] == NONE) {
					color[w] = (color[v] == BLUE ? RED : BLUE);
					Q.push(w);
				}
		}
	}

	return result;
}

//
// Joint approximations
//

VertexSet OCT::getApproxSol(Graph const & graph) {
	VertexSet sol1 = getApproxSol1(graph);
	VertexSet sol2 = getApproxSol2(graph);

	if (sol1.size() < sol2.size())
		return sol1;
	else
		return sol2;
}

//
// Getting kernel from approximate solution
//

VertexSet OCT::undeletableByApproxSol(Graph graph, VertexSet approx_sol) {
	// Delete vertices from solution
	
	Graph bipartite_one(graph.size(),vector<int>());

	// delete vertices from solution

	for (int v = 1; v < ((int)bipartite_one.size()); v++)
		if (approx_sol.count(v) == 0)
			for (int w : graph[v])
				if (approx_sol.count(w) == 0)
					bipartite_one[v].push_back(w);

	auto [red, blue] = getParts(bipartite_one);

	// Add solution vertices

	vector<int> blue_copy(graph.size(),-1);
	for (int v : approx_sol) {
		blue_copy[v] = bipartite_one.size();
		bipartite_one.push_back(vector<int>());

		red.insert(v);
		blue.insert(blue_copy[v]);
	}

	for (int v : approx_sol)
		for (int w : graph[v])
			if (approx_sol.count(w) > 0) {
			} else if (red.count(w) > 0) {
				bipartite_one[blue_copy[v]].push_back(w);
				bipartite_one[w].push_back(blue_copy[v]);
			} else {
				bipartite_one[v].push_back(w);			
				bipartite_one[w].push_back(v);			
			}
		
	VertexSet extended_as = approx_sol;
	for (int v : approx_sol)
		extended_as.insert(blue_copy[v]);

#ifdef DEBUG
	cout << "RED:";
	for (int v : red)
		cout << " " << v;
	cout << endl;

	cout << "BLUE:";
	for (int v : blue)
		cout << " " << v;
	cout << endl;

	cout << "RED SPECIALS:";
	for (int v : red) if (extended_as.count(v) > 0)
		cout << " " << v;
	cout << endl;

	cout << "BLUE SPECIALS:";
	for (int v : blue) if (extended_as.count(v) > 0)
		cout << " " << v;
	cout << endl;

	for (int v = 1; v < ((int)bipartite_one.size()); v++) {
		cout << v << " (";
		if (red.count(v) > 0) cout << "R"; else cout << "B";
		if (extended_as.count(v)) cout << ",S";
		cout << ") : ";
		for (int w : bipartite_one[v]) cout << " " << w;
		cout << endl;
	}
#endif

	auto p = addSourcesBefore(bipartite_one,extended_as);
	auto S = p.second;
	p = addSinksAfter(p.first,extended_as);
	auto G = p.first;
	auto T = p.second;

	return selectCCVertices(G,S,T,extended_as,extended_as.size());
}

//
// Getting kernel
//

VertexSet OCT::getUndeletable(Graph graph) {
	return undeletableByApproxSol(graph,getApproxSol(graph));
}

//
// Final reduction
//

Graph reduceByUndeletable(Graph graph,VertexSet undeletable) {
	vector<bool> undeletable_mask(graph.size(),false);
	for (auto v : undeletable)
		undeletable_mask[v] = true;

	auto undeletable_graph = inducedSubgraph(graph,undeletable_mask);
	auto [ red, blue ] = getParts(undeletable_graph);
	auto components = getComponents(undeletable_graph);

	for (auto const & component : components) {
		// intersect component with red and blue
		VertexSet c_red, c_blue;

		for (int v : component)
			if (red.count(v) > 1)
				c_red.insert(v);
			else
				c_blue.insert(v);
		
		graph = mergeVertices(move(graph),move(c_blue));
		graph = mergeVertices(move(graph),move(c_red));
	}

	return graph;
}

//
// I/O
//

std::istream & operator>>(std::istream & stream, OCT::Instance & I) {
	char buffer[200];

	stream.getline(buffer,200);					// -> comment
	stream.getline(buffer,200);					// -> name

	int n;
	stream.getline(buffer,200);					// -> comment
	stream.getline(buffer,200);					// -> number of vertices
	stringstream(string(buffer)) >> n;

	int m;
	stream.getline(buffer,200);					// -> comment
	stream.getline(buffer,200);					// -> number of edges
	stringstream(string(buffer)) >> m;

	I.vertex_name = vector<string>{""};
	I.graph = Graph{vector<int>()};
	map<string,int> vertex_by_name;

	stream.getline(buffer,200);					// -> comment
	for (int v = 1; v <= n; v++) {
		stream.getline(buffer,200);				// -> vertex_name

		string name;
		stringstream(string(buffer)) >> name;
		vertex_by_name[name] = v;
		I.vertex_name.push_back(name);

		I.graph.push_back(vector<int>());
	}

	stream.getline(buffer,200);					// -> comment
	for (int e = 1; e <= m; e++) {
		stream.getline(buffer,200);				// -> edge

		string v, w;
		stringstream(string(buffer)) >> v >> w;

		I.graph[vertex_by_name[v]].push_back(vertex_by_name[w]);
		I.graph[vertex_by_name[w]].push_back(vertex_by_name[v]);
	}

	return stream;
}

std::ostream & operator<<(std::ostream & stream, OCT::Instance const & I) {
	stream << endl;
	stream << "Compressed instance" << endl;

	stream << "Number of vertices." << endl;
	stream << I.graph.size()-1 << endl;

	int m = 0;
	for (int v = 1; v < ((int)I.graph.size()); v++)
		m += I.graph.size();
	m = m/2;
	
	stream << "Number of vertices." << endl;
	stream << m << endl;

	stream << "Vertex names" << endl;
	for (int v = 1; v < ((int)I.graph.size()); v++)
		cout << I.vertex_name[v] << endl;

	stream << "Edges" << endl;
	for (int v = 1; v < ((int)I.graph.size()); v++)
		for (int w : I.graph[v])
			if (v < w)
				stream << I.vertex_name[v] << " " << I.vertex_name[v] << endl;

	return stream;
}

