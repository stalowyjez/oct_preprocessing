#include <list>

#include "basic.h"

#ifndef GRAPH_H

#define GRAPH_H

#define edge(a,b) (a>b?mp(b,a):mp(a,b))
#define getSecond(e,v) ((e).first == (v) ? (e).second : (e).first)

typedef std::set<std::pair<int,int>> EdgeSet;
typedef std::set<int> VertexSet;
typedef std::vector<std::vector<int>> Graph;

// Simple info stuff
int maxDegreeVertex(Graph const&);
bool hasEdges(Graph const&);

// Simple transformations stuff
Graph removeVertex(Graph && graph, int w);
Graph bypassVertex(Graph && graph, int v);
Graph mergeVertices(Graph && graph, VertexSet const & vertices);
Graph transposeGraph(Graph const &);
Graph inducedSubgraph(Graph const&,std::vector<bool>const&);
Graph inducedSubgraph(Graph const&,std::set<int>const&);
Graph changeLabels(Graph const &, std::vector<int>const&);

// Min-cut stuff
int minVertexCut(Graph const& graph, VertexSet const& S, VertexSet const& T);

// Bipartite stuff
bool isBipartite(Graph const&);
std::pair<VertexSet,VertexSet> getParts(Graph const&);

// Biconnected components stuff
std::list<std::list<int>> getBCC(Graph const& graph);

// Strongly connected components
std::pair<std::vector<int>,Graph> getSSS(Graph const&);

// Components
std::vector<VertexSet> getComponents(Graph const&);

// Add copies
Graph addSinkCopies(Graph graph, VertexSet const &);
std::pair<Graph,VertexSet> addSourceCopies(Graph, VertexSet const &);
std::pair<Graph,VertexSet> copyVertexKTimes(Graph const &, int v, int k);
std::pair<Graph,VertexSet> copySourceKTimes(Graph const &, int v, int k);
std::pair<Graph,VertexSet> addSinksAfter(Graph, VertexSet const &);
std::pair<Graph,VertexSet> addSourcesBefore(Graph, VertexSet const &);

std::pair<Graph,std::vector<int>> duplicateGraph(Graph const &);

// Matching
std::vector<std::pair<int,int>> maxMatching(Graph const &);

// Get edges
std::vector<std::pair<int,int>> getEdges(Graph const & graph);
std::vector<std::pair<int,int>> getArcs(Graph const & graph);

#endif // GRAPH_H

