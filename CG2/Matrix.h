#pragma once
#include <utility>
#include <iostream>
#include <vector>
#include <type_traits>
#include <algorithm>

namespace cg {
	template <typename T, typename U>
	concept IsSummable = requires(T left, U right) {
		{left + right};
	};

	template <typename T, typename U>
	concept IsDeductible = requires(T left, U right) {
		{left - right};
	};

	template <typename T, typename U>
	concept IsMultiplied = requires(T left, U right) {
		{left* right};
	};

	template <typename T, typename U>
	concept IsAssignable = requires(T & left, U right) {
		{left = right};
	};
	template <typename T>
	concept IsAccumalated = requires(T & left, T right) {
		{left += right};
	};

}


template <typename Info = float>
class Matrix
{
private:
	int n_nRows;
	int n_nCols;
	std::vector<std::vector<Info>> m_cells;
	void AllocateCells(int, int);
public:
	Matrix() : n_nRows(0), n_nCols(0), m_cells(nullptr) {}
	Matrix(const Matrix&);
	Matrix(int, int);
	Matrix(int, int, Info*);

	int nRows();
	int nColumns();
	float ReturnValue(int, int);

	void setValue(int, int, Info);

	Info& operator()(int i, int j) { return m_cells[i - 1][j - 1]; }

	template<typename RCell> requires cg::IsAssignable<Info, RCell>
	Matrix<Info>& operator=(const Matrix<RCell>& right)
	{
		if (n_nCols != right.n_nCols || n_nRows != right.n_nRows) {
			throw std::invalid_argument("Не совпадают размеры матриц");
		}
		for (int i = 0; i < n_nRows; i++) for (int j = 0; j < n_nCols; j++) m_cells[i][j] = right.m_cells[i][j];
	}
	template <typename LCell, typename RCell>
		requires cg::IsSummable<LCell, RCell>
	friend auto operator+(const Matrix<LCell>& left, const Matrix<RCell>& right)
	{
		if (left.nRows != right.nRows || left.nColumns != right.nColumns) throw std::invalid_argument("не совпадают размеры матриц");
		Matrix<decltype(std::declval<LCell>() + std::declval<RCell>())> res(left.n_nRows, left.nColumns);
		for (int i = 0; i < left.n_nRows; i++)
			for (int j = 0; j < left.n_nCols; j++)
				res.m_cells[i][j] = left.m_cells[i][j] + right.m_cells[i][j];
		return res;
	}
	template <typename LCell, typename RCell>
		requires cg::IsDeductible<LCell, RCell>
	friend auto operator-(const Matrix<LCell>& left, const Matrix<RCell>& right)
	{
		if (left.nRows != right.nRows || left.nColumns != right.nColumns) throw std::invalid_argument("не совпадают размеры матриц");
		Matrix<decltype(std::declval<LCell>() - std::declval<RCell>())> res(left.n_nRows, left.nColumns);
		for (int i = 0; i < res.n_nRows; i++)
			for (int j = 0; j < res.n_nCols; j++)
				res.m_cells[i][j] = left.m_cells[i][j] + right.m_cells[i][j];
		return res;
	}
	template <typename LCell, typename RCell>
		requires cg::IsMultiplied<LCell, RCell>
	friend auto operator*(const Matrix<LCell>& left, const Matrix<RCell>& right)
	{
		if (left.n_nCols != right.n_nRows) throw std::invalid_argument("не совпадают размеры матриц");
		Matrix<decltype(std::declval<LCell>()* std::declval<RCell>())> res(left.n_nRows, left.n_nCols);
		{
			for (int i = 0; i < left.n_nRows; i++)
			{
				for (int j = 0; j < right.n_nCols; j++)
				{
					res.m_cells[i][j] = 0;
					for (int k = 0; k < left.n_nCols; k++)
					{
						res.m_cells[i][j] += left.m_cells[i][k] * right.m_cells[k][j];

					}

				}
			}

		}
		return res;
	}

	friend std::istream& operator >> (std::istream&, Matrix&);
	friend std::ostream& operator << (std::ostream&, const Matrix&);
};

template <typename Info>
Matrix<Info>::Matrix(const Matrix<Info>& M)
{
	AllocateCells(M.n_nRows, M.n_nCols);
	for (int i = 0; i < n_nRows; i++)
		for (int j = 0; j < n_nCols; j++)
			m_cells[i][j] = M.m_cells[i][j];
}

template <typename Info>
Matrix<Info>::Matrix(int n_nRows, int n_nCols)
{
	AllocateCells(n_nRows, n_nCols);
	for (int i = 0; i < n_nRows; i++)
		for (int j = 0; j < n_nCols; j++)
			m_cells[i][j] = 0;
}

template <typename Info>
Matrix<Info>::Matrix(int n_nRows, int n_nCols, Info* list)
{
	int k = 0;
	AllocateCells(n_nRows, n_nCols);
	for (int i = 0; i < n_nRows; i++)
		for (int j = 0; j < n_nCols; j++)
		{
			m_cells[i][j] = list[k];
			k++;
		}
}


template <typename Info>
void Matrix<Info>::AllocateCells(int nRows, int nCols)
{
	n_nRows = nRows;
	n_nCols = nCols;

	m_cells.resize(nRows);
	std::for_each(
		m_cells.begin(),
		m_cells.end(),
		[nCols](std::vector<Info>& elem) {elem.resize(nCols); });
}

template <typename Info>
std::istream& operator >> (std::istream& fi, Matrix<Info>& M)
{
	for (int i = 0; i < M.n_nRows; i++)
		for (int j = 0; j < M.n_nCols; j++)
			fi >> M.m_cells[i][j];
	return fi;
}

template <typename Info>
std::ostream& operator << (std::ostream& fo, const Matrix<Info>& M)
{
	for (int i = 0; i < M.n_nRows; i++)
	{
		fo << "  ";
		for (int j = 0; j < M.n_nCols; j++)
			fo << M.m_cells[i][j] << " \t";
		fo << std::endl;
	}
	return fo;
}

template <typename Info>
int Matrix<Info>::nRows()
{
	return n_nRows;
}

template <typename Info>
int Matrix<Info>::nColumns()
{
	return n_nCols;
}

template <typename Info>
void Matrix<Info>::setValue(int i, int j, Info value)
{
	m_cells[i][j] = value;
}
template <typename Info>
float Matrix<Info>::ReturnValue(int i, int j)
{
	return m_cells[i][j];
}