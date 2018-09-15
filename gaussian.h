#include <vector>

#include "field.h"

#ifndef GAUSSIAN_H

#define GAUSSIAN_H

std::pair<Matrix,std::vector<int>> reduceMatrix(Matrix && rows);
std::vector<int> getSignificantRows(Matrix && family_rows);
int getFirstZeroRow(Matrix && family_rows);
std::vector<Field::Element> getAllDets(Matrix const & matrix);
std::vector<Field::Element> getAllDets2(Matrix const & _matrix);

#endif // GAUSSIAN_H

