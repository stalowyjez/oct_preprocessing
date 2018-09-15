#include <vector>
#include <list>
#include <iostream>

#include "basic.h"
#include "field.h"
#include "matroid.h"
#include "gaussian.h"

using namespace std;

Matrix getUniformMatroidRep(int size, int rank) {
	Matrix result;

	for (int i = 0; i < size; i++) {
		vector<Field::Element> current_col{Field::Element(1)};
		Field::Element e(i+1);

		for (int i = 1; i <= rank-1; i++)
			current_col.push_back(current_col[i-1]*e);

		result.push_back(current_col);
	}

	return result;
}

Matrix matroidRepSimpleSum(Matrix && A, Matrix && B) {
	int a_x = A[0].size();
	int b_x = B[0].size();

	vector<Field::Element> zero_end(b_x,Field::Element(0));

	for (auto & row : A)
		row.insert(row.end(),zero_end.begin(),zero_end.end());

	for (auto & row : B) {
		vector<Field::Element> new_row(a_x,Field::Element(0));
		new_row.insert(new_row.end(),row.begin(),row.end());
		A.push_back(new_row);
	}

	return A;
}

bool operator<(ISet const & A, ISet const & B) {
	if (A.size() != B.size())
		return A.size() < B.size();

	for (int i = 0; i < ((int)A.size()); i++)
		if (A[i] != B[i])
			return A[i] < B[i];

	return false;
}

bool operator==(ISet const & A, ISet const & B) {
	if (A.size() != B.size())
		return false;

	for (int i = 0; i < ((int)A.size()); i++)
		if (A[i] != B[i])
			return false;

	return true;
}

#define ISET_MUL 5501
#define ISET_MOD 1230599

size_t hash<ISet>::operator() (ISet const & iset) const {
	size_t result = 0L;

	for (int col : iset)
		result = (result * ISET_MUL + col) % ISET_MOD;

	return result;
}

ISetFamily getMatroidRepSet(Matrix const & matroid, ISetFamily const & family) {
	Matrix family_rows;
	for (auto set : family) {
		Matrix set_rep;
		for (auto idx : set)
			set_rep.push_back(matroid[idx]);
		family_rows.push_back(getAllDets(set_rep));
	}

	auto rep_set_idx = getSignificantRows(move(family_rows));

	ISetFamily result;
	for (int idx : rep_set_idx)
		result.push_back(family[idx]);
	return result;
}

