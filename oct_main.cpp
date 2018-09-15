#include "oct.h"

int main() {
	int n,m;
	std::cin >> n >> m;

	Graph graph(n+1);

	for (int i = 1; i <= m; i++) {
		int v,w;
		std::cin >> v >> w;
		graph[v].push_back(w);
		graph[w].push_back(v);
	}

	VertexSet undeletable = OCT::getUndeletable(graph);

	/*std::cout << "UNDELETABLE VERTICES (" << undeletable.size() << "):" << std::endl;
	for (int v : undeletable)
		std::cout << v << " ";
	std::cout << std::endl;*/

	std::cout << undeletable.size() << std::endl;

}
