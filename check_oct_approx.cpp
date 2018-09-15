#include <iostream>

#include "graph.h"
#include "oct.h"

using namespace std;

VertexSet getApproxSol1(Graph graph);
VertexSet getApproxSol2(Graph graph);

bool checkIfCorrect(VertexSet const & sol, Graph graph) {
	for (int v : sol)
		graph = removeVertex(move(graph),v);

	return isBipartite(graph);
}

int main() {
	OCT::Instance I;
	cin >> I;

	if(checkIfCorrect(getApproxSol1(I.graph),I.graph))
		cout << "APPROX1: CORRECT(" << getApproxSol1(I.graph).size() << ")" << endl;
	else
		cout << "APPROX1: INCORRECT" << endl;

	if(checkIfCorrect(getApproxSol2(I.graph),I.graph))
		cout << "APPROX2: CORRECT(" << getApproxSol2(I.graph).size() << ")" << endl;
	else
		cout << "APPROX2: INCORRECT" << endl;
}
