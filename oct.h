#include <vector>
#include <set>
#include <algorithm>
#include <iostream>

#include "graph.h"

#ifndef OCT_H

#define OCT_H

namespace OCT {
	struct Instance {
		std::vector<std::string> vertex_name;
		Graph graph;
	};

	VertexSet getApproxSol(Graph const & graph);

	VertexSet undeletableByApproxSol(Graph,VertexSet);
	VertexSet getUndeletable(Graph);
	Graph reduceByUndeletable(Graph,VertexSet);
}

std::istream & operator>>(std::istream &, OCT::Instance &);
std::ostream & operator<<(std::ostream &, OCT::Instance const&);

#endif // OCT_H

