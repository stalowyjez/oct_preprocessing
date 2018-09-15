#include <vector>
#include <cstdio>
#include <iostream>

#include "basic.h"
#include "gaussian.h"

using namespace std;

#define max(a,b) (a>b?a:b)

struct GaussianEliminator {
	vector<vector<Field::Element>> & matrix;
	int rows, cols;

	vector<int> rowsMap;
	vector<int> colsMap;

	GaussianEliminator(vector<vector<Field::Element>> & _matrix) :
		matrix(_matrix),
		rows(_matrix.size()),
		cols(_matrix[0].size())
	{
		rowsMap = vector<int>(rows);
		colsMap = vector<int>(cols);

		for(int i = 0; i < rows; i++)
			rowsMap[i] = i;
		for(int i = 0; i < cols; i++)
			colsMap[i] = i;
	}

	int findNonZeroInRow(int row, int begin) {
		for (int i = begin; i < cols; i++)
			if (getVal(row,i) != 0)
				return i;
		return -1;
	}

	vector<int> getColsMap() {
		return colsMap;
	}

	void swapCols(int A, int B) {
		if (A == B)
			return;

		colsMap[A] ^= colsMap[B];
		colsMap[B] ^= colsMap[A];
		colsMap[A] ^= colsMap[B];
	}

	void swapRows(int A, int B) {
		if (A == B)
			return;

		rowsMap[A] ^= rowsMap[B];
		rowsMap[B] ^= rowsMap[A];
		rowsMap[A] ^= rowsMap[B];
	}

	void normalize_row(int row, int col) {
		auto inv_val = getVal(row,col).inv();
		for (int i = col; i < cols; i++)
			setVal(row,i,getVal(row,i)*inv_val);
	}

	void subRowFromAllBelow(int row, int begin) {
		for (int i = row+1; i < rows; i++)
			if (not getVal(i,begin) == 0) {
				auto mult = getVal(i,begin);
				for(int j = begin; j < cols; j++)
					setVal(i,j,getVal(i,j)-(getVal(row,j)*mult));
			}
	}

	inline Field::Element getVal(int r, int c) {
		return matrix[rowsMap[r]][colsMap[c]];
	}

	inline void setVal(int r, int c, Field::Element val) {
		matrix[rowsMap[r]][colsMap[c]] = val;
	}

	void standardElimination() {
		int currentRow = 0, currentCol = 0;
		while (currentRow < rows && currentCol < cols) {
			if (getVal(currentRow,currentCol) != 0) {
				if (getVal(currentRow,currentCol) != 1)
					normalize_row(currentRow,currentCol);

				subRowFromAllBelow(currentRow,currentCol);

				currentRow++;
				currentCol++;
			} else {
				int zero = findNonZeroInRow(currentRow,currentCol);
				if(zero != -1) {
					swapCols(currentCol, zero);
				} else {
					swapRows(currentRow, rows-1);
					rows--;
				}
			}
		}
	}

	void fullReduction() {
		standardElimination();

		for (int row = rows-1; row >= 1; row--)
			for (int cur_row = row-1; cur_row >= 0; cur_row--) {
				auto val = getVal(cur_row,row);
				setVal(cur_row,row,0);
				for (int i = rows; i < cols; i++)
					setVal(cur_row,i,getVal(cur_row,i)-val*getVal(row,i));
			}
	}

	vector<int> getSignificantRows() {
		standardElimination();

		vector<int> result;
		for (int i = 0; i < min(cols,rows); i++)
			result.push_back(rowsMap[i]);

		return result;
	}

	int getFirstZeroRow() {
		int currentRow = 0, currentCol = 0;
		while (currentRow < rows && currentCol < cols) {
			if (getVal(currentRow,currentCol) != 0) {
				if (getVal(currentRow,currentCol) != 1)
					normalize_row(currentRow,currentCol);

				subRowFromAllBelow(currentRow,currentCol);

				currentRow++;
				currentCol++;
			} else {
				int zero = findNonZeroInRow(currentRow,currentCol);
				if(zero != -1)
					swapCols(currentCol, zero);
				else
					return currentRow;
			}
		}

		return -1;
	}

	Matrix getMatrix() {
		Matrix result(rows,vector<Field::Element>(cols,Field::Element(0)));

		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				result[i][j] = getVal(i,j);

		return result;
	}
};

pair<Matrix,vector<int>> reduceMatrix(Matrix && rows) {
	GaussianEliminator eliminator(rows);
	eliminator.fullReduction();
	return mp(eliminator.getMatrix(),invPerm(eliminator.getColsMap()));
}

vector<int> getSignificantRows(Matrix && family_rows) {
	GaussianEliminator eliminator(family_rows);
	return eliminator.getSignificantRows();
}

int getFirstZeroRow(Matrix && family_rows) {
	GaussianEliminator eliminator(family_rows);
	return eliminator.getFirstZeroRow();
}

//
// Getting vector of determinants
//

struct GMatrix {
	bool zero_det = false;
	bool neg = false;

	vector<vector<Field::Element>> matrix;
	vector<int> colsMap;

	void addRowAndReduce(vector<Field::Element> row);
	Field::Element getDet();

private:
	void swapCols(int A, int B) {
		if (A == B)
			return;

		if ((A-B)*(A-B) % 2 == 1)
			neg = not neg;

		colsMap[A] ^= colsMap[B];
		colsMap[B] ^= colsMap[A];
		colsMap[A] ^= colsMap[B];
	}
};

void GMatrix::addRowAndReduce(vector<Field::Element> row) { // only reduced matrix
	if (zero_det)
		return;
	
	// add row, if it's first row set number of columns
	if ((int)matrix.size() == 0) {
		colsMap = vector<int>(row.size());
		for (int i = 0; i < (int)row.size(); i++)
			colsMap[i] = i;
	}
	matrix.push_back(row);

	// reduce row
	for (int row = 0; row < ((int)matrix.size())-1; row++) {
		auto mult = matrix[row][colsMap[row]].inv()*matrix[matrix.size()-1][colsMap[row]];
		for (int i = row; i < (int)matrix[0].size(); i++)
			matrix[matrix.size()-1][colsMap[i]] = matrix[matrix.size()-1][colsMap[i]] - matrix[row][colsMap[i]]*mult;
	}

	// find nonzero col or mark zero determinant
	for (int i = ((int)matrix.size())-1; i < (int)matrix[0].size(); i++) 
		if (matrix[matrix.size()-1][colsMap[i]] != 0) {
			swapCols((int)matrix.size()-1,i);
			return;
		}
	zero_det = true;
}

Field::Element GMatrix::getDet() { // only reduced matrix, only when num of columns is smaller
	if (zero_det)
		return 0;

	Field::Element result(1);
	for (int i = 0; i < (int)matrix.size(); i++)
		result = result * matrix[i][colsMap[i]];
	if (neg)
		result = Field::Element(0)-result;
	return result;
}

vector<pair<GMatrix,int>> _getAllDets(Matrix const & matrix, int k) {
	if (k == 0)
		return vector<pair<GMatrix,int>>{mp(GMatrix(),-1)};

	int n = matrix.size();

	// create submatrix

	Matrix submatrix = matrix;
	submatrix.pop_back();

	// compute smaller case
	
	vector<pair<GMatrix,int>> ind_step = _getAllDets(submatrix,k-1);

	// add rows

	vector<pair<GMatrix,int>> result;
	for (auto p : ind_step) {
		for (int i = p.second+1; i < n; i++) {
			auto new_p = p;
			new_p.first.addRowAndReduce(matrix[i]);
			new_p.second = i;
			result.push_back(new_p);
		}
	}

	return result;
}

Matrix transpose(Matrix const & matrix) {
	Matrix result(matrix[0].size(),vector<Field::Element>(matrix.size(),Field::Element(0)));

	for (int i = 0; i < (int)matrix.size(); i++)
		for (int j = 0; j < (int)matrix[0].size(); j++)
			result[j][i] = matrix[i][j];

	return result;
}

vector<Field::Element> getAllDets(Matrix const & matrix) {
	auto gmatrices = _getAllDets(transpose(matrix),matrix.size());

	vector<Field::Element> result;
	for (auto p : gmatrices)
		result.push_back(p.first.getDet());
	return result;
}

vector<Field::Element> getAllDets2(Matrix const & _matrix) {
	auto matrix = transpose(_matrix);
	vector<Field::Element> result;
	auto col_sets = binomial(matrix.size(),matrix[0].size());
	for (auto set : col_sets)
		result.push_back(
			matrix[0][0]*matrix[1][1]*matrix[2][2]
			- matrix[0][0]*matrix[1][2]*matrix[2][1]
			+ matrix[0][1]*matrix[1][2]*matrix[2][0]
			- matrix[0][1]*matrix[1][0]*matrix[2][2]
			+ matrix[0][2]*matrix[1][0]*matrix[2][1]
			- matrix[0][2]*matrix[1][1]*matrix[2][0]
		);
	return result;
}

