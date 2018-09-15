
#include <vector>

#include "field.h"

#ifndef MATROID_H

#define MATROID_H

typedef std::vector<int> ISet;

namespace std {
	template<>
	class hash<ISet> {
		public:
		size_t operator() (ISet const & iset) const;
	};
}

typedef std::vector<ISet> ISetFamily;

bool operator<(ISet const &, ISet const &);
bool operator==(ISet const &, ISet const &);


ISetFamily getMatroidRepSet(Matrix const & matroid, ISetFamily const & family);
ISetFamily getMatroid2RepSet(Matrix const & matroid, ISetFamily const & family);

Matrix getUniformMatroidRep(int size, int rank);

Matrix matroidRepSimpleSum(Matrix && A, Matrix && B);

#endif // MATROID_H
