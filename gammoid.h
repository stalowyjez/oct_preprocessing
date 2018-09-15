#include <vector>

#include "basic.h"
#include "field.h"

#ifndef GAMMOID_H

#define GAMMOID_H

std::vector<int> sortGammoid(Graph & graph, VertexSet & sources);
std::vector<std::vector<Field::Element>> getGammoidRep(Graph const & graph, VertexSet const & sources);
bool isIndependent(Matrix const & gammoidRep, VertexSet const & X);

#endif // GAMMOID_H

