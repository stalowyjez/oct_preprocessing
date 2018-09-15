#include "basic.h"

using namespace std;

vector<int> invPerm(vector<int> const & perm) {
	vector<int> result(perm.size());

	for (int i = 0; i < ((int)perm.size()); i++)
		result[perm[i]] = i;

	return result;
}

std::vector<std::vector<int>> binomial(int n, int k) {
	if (k == 0)
		return vector<vector<int>>{vector<int>()};

	auto ind_step = binomial(n-1,k-1);
	vector<vector<int>> result;
	for (auto const & ind_step_set : ind_step) {
		auto tmp_set = ind_step_set;
		tmp_set.push_back(-1);
		for (int i = ind_step_set.back()+1; i <= n; i++) {
			tmp_set[tmp_set.size()-1] = i;
			result.push_back(tmp_set);
		}
	}

	return result;
}

set<int> changeSetRep(vector<bool> const & chr_rep) {
	set<int> result;
	for (int i = 1; i < ((int)chr_rep.size()); i++)
		if (chr_rep[i])
			result.insert(i);
	return result;
}

vector<bool> changeSetRep(list<int> const & set_rep, int n) {
	vector<bool> result(n+1,false);
	for (int i : set_rep)
		result[i] = true;
	return result;
}

