#include <vector>
#include <list>
#include <set>
#include <algorithm>

#ifndef BASIC_H

#define BASIC_H 1

#define INF 1000000001

#define mp make_pair
#define pb bush_back

#define edge(a,b) (a>b?mp(b,a):mp(a,b))
#define getSecond(e,v) ((e).first == (v) ? (e).second : (e).first)

typedef std::set<std::pair<int,int>> EdgeSet;
typedef std::set<int> VertexSet;
typedef std::vector<std::vector<int>> Graph;

std::vector<int> invPerm(std::vector<int> const &);

std::vector<std::vector<int>> binomial(int n, int k);

std::set<int> changeSetRep(std::vector<bool>);
std::vector<bool> changeSetRep(std::list<int>, int n);

template <class T>
std::vector<std::vector<T>> transpose(std::vector<std::vector<T>> const & matrix) {
	std::vector<std::vector<T>> result(matrix[0].size(),std::vector<T>(matrix.size(),0));

	for (int i = 0; i < (int)matrix[0].size(); i++)
		for (int j = 0; j < (int)matrix.size(); j++)
			result[i][j] = matrix[j][i];

	return result;
}

#endif // BASIC_H

