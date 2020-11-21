#include "matrix.h"

using namespace task;

Matrix::Matrix::Matrix() :  rows_(1), cols_(1) {
  allocSpace();
  data_[0][0] = 1.0;
}

Matrix::Matrix(size_t rows, size_t cols) : rows_(rows), cols_(cols) {
  allocSpace();
  data_[0][0] = 1.0;
  for (size_t i = 0; i < std::min(cols, rows); ++i) {
    data_[i][i] = 1.0;
  }
}

Matrix::Matrix(const Matrix& copy) : rows_(copy.rows_), cols_(copy.cols_) {
  allocSpace();
  for (size_t i = 0; i < rows_; ++i) {
    for (size_t j = 0; j < cols_; ++j) {
      data_[i][j] = copy.data_[i][j];
    }
  }
}

Matrix::~Matrix() {
  for (int i = 0; i < rows_; ++i) {
    delete[] data_[i];
  }
  delete[] data_;
}

Matrix& Matrix::operator=(const Matrix& a) {
  if (&a == this) {
    return *this;
  }
  if (rows_ != a.rows_ || cols_ != a.cols_) {
    for (int i = 0; i < rows_; ++i) {
      delete[] data_[i];
    }
    delete[] data_;

    rows_ = a.rows_;
    cols_ = a.cols_;
    allocSpace();
  }
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      data_[i][j] = a.data_[i][j];
    }
  }
  return *this;
}

double& Matrix::get(size_t row, size_t col) {
  checkBounds(row, col);
  return data_[row][col];
}

const double& Matrix::get(size_t row, size_t col) const {
  checkBounds(row, col);
  return data_[row][col];
}

void Matrix::set(size_t row, size_t col, const double& value) {
  checkBounds(row, col);
  data_[row][col] = value;
}

void Matrix::resize(size_t new_rows, size_t new_cols) {
  if (new_rows < 1 || new_cols < 1) {
    throw OutOfBoundsException();
  }
  double** new_data = new double*[new_rows];
  for (size_t i = 0; i < new_rows; ++i) {
    new_data[i] = new double[new_cols];
  }
  size_t row_size = std::min(rows_, new_rows);
  size_t col_size = std::min(cols_, new_cols);
  for (size_t i = 0; i < row_size; ++i) {
    for (size_t j = 0; j < col_size; ++j) {
      new_data[i][j] = data_[i][j];
    }
  }
  for (int i = 0; i < rows_; ++i) {
    delete[] data_[i];
  }
  delete[] data_;
  data_ = new_data;
  rows_ = new_rows;
  cols_ = new_cols;
}

double* Matrix::operator[](size_t row) {
  checkBounds(row, 0);
  return data_[row];
}

double* Matrix::operator[](size_t row) const {
  checkBounds(row, 0);
  return data_[row];
}

Matrix& Matrix::operator+=(const Matrix& a) {
  checkSize(a);
  for (size_t i = 0; i < rows_; ++i) {
    for (size_t j = 0; j < cols_; ++j) {
      data_[i][j] += a[i][j];
    }
  }
  return *this;
}

Matrix& Matrix::operator-=(const Matrix& a) {
  checkSize(a);
  for (size_t i = 0; i < rows_; ++i) {
    for (size_t j = 0; j < cols_; ++j) {
      data_[i][j] -= a[i][j];
    }
  }
  return *this;
}

Matrix& Matrix::operator*=(const Matrix& a) {
  if (cols_ != a.rows_) {
    throw SizeMismatchException();
  }
  *this = *this * a;
  return *this;
}

Matrix& Matrix::operator*=(const double& number) {
  for (size_t i = 0; i < rows_; ++i) {
    for (size_t j = 0; j < cols_; ++j) {
      data_[i][j] *= number;
    }
  }
  return *this;
}

Matrix Matrix::operator+(const Matrix& a) const {
  checkSize(a);
  Matrix result = *this;
  result += a;
  return result;
}

Matrix Matrix::operator-(const Matrix& a) const {
  checkSize(a);
  Matrix result = *this;
  result -= a;
  return result;
}

Matrix Matrix::operator*(const Matrix& a) const {
  if (cols_ != a.rows_) {
    throw SizeMismatchException();
  }
  Matrix result(rows_, a.cols_);
  for (size_t i = 0; i < rows_; ++i) {
    for (size_t j = 0; j < a.cols_; ++j) {
      result[i][j] = 0.0;
      for (size_t k = 0; k < cols_; ++k) {
        result[i][j] += data_[i][k] * a[k][j];
      }
    }
  }
  return result;
}

Matrix Matrix::operator*(const double& a) const {
  Matrix result = *this;
  result *= a;
  return result;
}

Matrix Matrix::operator-() const {
  Matrix result(rows_, cols_);
  for (size_t i = 0; i < rows_; ++i) {
    for (size_t j = 0; j < cols_; ++j) {
      result[i][j] = -data_[i][j];
    }
  }
  return result;
}

Matrix Matrix::operator+() const {
  return *this;
}

double Matrix::det() const {
  return det(*this);
}

void Matrix::transpose() {
  Matrix copy = *this;
  *this = copy.transposed();
}

Matrix Matrix::transposed() const {
  Matrix t_matrix(cols_, rows_);
  for (size_t i = 0; i < rows_; ++i) {
    for (size_t j = 0; j < cols_; ++j) {
      t_matrix[j][i] = data_[i][j];
    }
  }
  return t_matrix;
}

double Matrix::trace() const {
  if (rows_ != cols_) {
    throw SizeMismatchException();
  }
  double result = 0.0;
  for (size_t i = 0; i < rows_; ++i) {
    result += data_[i][i];
  }
  return result;
}

std::vector<double> Matrix::getRow(size_t row) {
  checkBounds(row, 0);
  std::vector<double> result(cols_);
  for (size_t i = 0; i < cols_; ++i) {
    result.push_back(data_[row][i]);
  }
  return result;
}

std::vector<double> Matrix::getColumn(size_t column) {
  checkBounds(0, column);
  std::vector<double> result(cols_);
  for (size_t i = 0; i < rows_; ++i) {
    result.push_back(data_[i][column]);
  }
  return result;
}

bool Matrix::operator==(const Matrix& a) const {
  checkSize(a);
  for (size_t i = 0; i < rows_; ++i) {
    for (size_t j = 0; j < cols_; ++j) {
      if (fabs(data_[i][j] - a[i][j]) >= EPS) {
        return false;
      }
    }
  }
  return true;
}

bool Matrix::operator!=(const Matrix& a) const {
  return !(*this == a);
}

void Matrix::checkBounds(const size_t& row, const size_t& col) const {
  if (row >= rows_ || col >= cols_) {
    throw OutOfBoundsException();
  }
}

void Matrix::checkSize(const Matrix& a) const {
  if (cols_ != a.cols_ || rows_ != a.rows_) {
    throw SizeMismatchException();
  }
}

double Matrix::det(const Matrix& matrix) const {
  if (matrix.cols_ != matrix.rows_) {
    throw SizeMismatchException();
  }
  double d = 0;
  size_t rows = matrix.getRows();
  size_t cols = matrix.getCols();

  if (rows == 1) {
    return matrix[0][0];
  }

  if (rows == 2) {
    return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
  }

  Matrix submatrix(rows - 1, cols - 1);

  for (size_t currentColumn = 0; currentColumn < cols; ++currentColumn) {
    for (size_t i = 1; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        if (j == currentColumn) {
          continue;
        }

        submatrix[i - 1][j < currentColumn ? j : j - 1] = matrix[i][j];
      }
    }

    d += pow(-1, currentColumn) * matrix[0][currentColumn] * det(submatrix);
  }
  return d;
}

void Matrix::allocSpace() {
  data_ = new double*[rows_];
  for (size_t i = 0; i < rows_; ++i) {
    data_[i] = new double[cols_];
  }
}

size_t Matrix::getRows() const {
  return rows_;
}

size_t Matrix::getCols() const {
  return cols_;
}

Matrix task::operator*(const double& a, const Matrix& b) {
  Matrix result(b);
  result *= a;
  return result;
}

std::ostream& task::operator<<(std::ostream& output, const Matrix& matrix) {

  for (size_t i = 0; i < matrix.getRows(); ++i) {
    for (size_t j = 0; j < matrix.getCols(); ++j) {
      output << matrix[i][j] << " ";
    }
  }

  return output << "\n";
}

std::istream& task::operator>>(std::istream& input, Matrix& matrix) {
  size_t rows, cols;
  double number;
  input >> rows >> cols;
  matrix.resize(rows, cols);
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      input >> number;
      matrix.set(i, j, number);
    }
  }
  return input;
}