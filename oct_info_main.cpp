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

	VertexSet approx_sol = OCT::getApproxSol(graph);

	std::cout << n << "," << m << "," << approx_sol.size() << std::endl;

}
