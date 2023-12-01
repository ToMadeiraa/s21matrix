#include "../s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *A) {
  int status = OK;
  if ((!A) || (rows < 1) || (columns < 1))
    status = INCORRECT_MATRIX;
  else {
    A->matrix = (double **)malloc(sizeof(double *) * rows);
    A->rows = rows;
    A->columns = columns;
    for (int i = 0; i < rows; i++)
      A->matrix[i] = malloc(sizeof(double) * columns);
  }
  return status;
}

void s21_remove_matrix(matrix_t *A) {
  if (A) {
    for (int i = 0; i < A->rows; i++) {
      free(A->matrix[i]);
    }
    free(A->matrix);
    A->matrix = NULL;
    A->rows = 0;
    A->columns = 0;
  }
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int status = SUCCESS;
  if (!A || !B || (A->rows != B->rows) || (A->columns != B->columns)) {
    status = FAILURE;
  } else {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        if (fabs(A->matrix[i][j] - B->matrix[i][j]) > 1e-07) {
          status = FAILURE;
          break;
        }
      }
    }
  }
  return status;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *C) {
  int status = OK;
  if (!A || (A->rows < 1) || (A->columns < 1) || !B || (B->rows < 1) ||
      (B->columns < 1) || !C)
    status = INCORRECT_MATRIX;
  else if ((A->rows != B->rows) || (A->columns != B->columns))
    status = CALCULATION_ERROR;
  else {
    s21_create_matrix(A->rows, A->columns, C);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++)
        C->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
    }
  }
  return status;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *C) {
  int status = OK;
  if (!A || (A->rows < 1) || (A->columns < 1) || !B || (B->rows < 1) ||
      (B->columns < 1) || !C)
    status = INCORRECT_MATRIX;
  else if ((A->rows != B->rows) || (A->columns != B->columns))
    status = CALCULATION_ERROR;
  else {
    s21_create_matrix(A->rows, A->columns, C);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++)
        C->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
    }
  }
  return status;
}

int s21_transpose(matrix_t *A, matrix_t *C) {
  int status = OK;
  if (!A || (A->rows < 1) || (A->columns < 1) || !C)
    status = INCORRECT_MATRIX;
  else {
    s21_create_matrix(A->columns, A->rows, C);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) C->matrix[j][i] = A->matrix[i][j];
    }
  }
  return status;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *C) {
  int status = OK;
  if (!A || (A->rows < 1) || (A->columns < 1) || !C)
    status = INCORRECT_MATRIX;
  else {
    s21_create_matrix(A->rows, A->columns, C);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++)
        C->matrix[i][j] = A->matrix[i][j] * number;
    }
  }
  return status;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *C) {
  int status = OK;
  if (!A || (A->rows < 1) || (A->columns < 1) || !B || (B->rows < 1) ||
      (B->columns < 1) || !C)
    status = INCORRECT_MATRIX;
  else if ((A->rows != B->columns) || (A->columns != B->rows)) {
    status = CALCULATION_ERROR;
  } else {
    s21_create_matrix(A->rows, B->columns, C);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < B->columns; j++) {
        for (int k = 0; k < B->rows; k++)
          C->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
      }
    }
  }
  return status;
}

int s21_calc_complements(matrix_t *A, matrix_t *C) {
  int status = OK;
  if (!A || (A->rows < 1) || (A->columns < 1) || !C)
    status = INCORRECT_MATRIX;
  else if (A->columns != A->rows)
    status = CALCULATION_ERROR;
  else {
    s21_create_matrix(A->columns, A->rows, C);
    if (A->rows != 1) {
      matrix_t B;
      s21_create_matrix(A->rows, A->rows, &B);
      for (int sign = 0, i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          find_minor(A->matrix, B.matrix, i, j, A->rows);
          sign = ((i + j) % 2 == 0) ? 1 : (-1);
          C->matrix[i][j] = sign * find_det(&B, A->rows - 1);
        }
      }
      s21_remove_matrix(&B);
    } else
      C->matrix[0][0] = A->matrix[0][0];
  }
  return status;
}

int s21_determinant(matrix_t *A, double *C) {
  int status = OK;
  if (!A || (A->rows < 1) || (A->columns < 1) || !C)
    status = INCORRECT_MATRIX;
  else if (A->columns != A->rows)
    status = CALCULATION_ERROR;
  else {
    if (A->rows == 1)
      *C = A->matrix[0][0];
    else
      *C = find_det(A, A->rows);
  }
  return status;
}

double find_det(matrix_t *A, int size) {
  double det = 0;
  if (size == 1)
    det = A->matrix[0][0];
  else if (size > 1) {
    matrix_t B;
    s21_create_matrix(size, size, &B);
    for (int sign = 1, i = 0; i < size; i++, sign *= (-1)) {
      find_minor(A->matrix, B.matrix, 0, i, size);
      det += sign * A->matrix[0][i] * find_det(&B, size - 1);
    }
    s21_remove_matrix(&B);
  }
  return det;
}

void find_minor(double **A, double **B, int row_limit, int column_limit,
                int size) {
  for (int row = 0, i = 0; row < size; row++) {
    for (int column = 0, j = 0; column < size; column++) {
      if (row != row_limit && column != column_limit) {
        B[i][j++] = A[row][column];
        if (j == (size - 1)) {
          j = 0;
          i++;
        }
      }
    }
  }
}

int s21_inverse_matrix(matrix_t *A, matrix_t *C) {
  int status = OK;
  if (!A || (A->rows < 1) || (A->columns < 1) || !C)
    status = INCORRECT_MATRIX;
  else if (A->rows != A->columns)
    status = CALCULATION_ERROR;
  else {
    double det = 0.0;
    s21_determinant(A, &det);
    if (fabs(det) < 1e-6)
      status = CALCULATION_ERROR;
    else if (A->rows == 1) {
      s21_create_matrix(1, 1, C);
      C->matrix[0][0] = (1 / det);
    } else {
      matrix_t B;
      s21_calc_complements(A, C);
      s21_transpose(C, &B);
      s21_remove_matrix(C);
      s21_mult_number(&B, (1.0 / det), C);
      s21_remove_matrix(&B);
    }
  }
  return status;
}

void s21_set(double *nums, matrix_t *A) {
  int count = 0;
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      A->matrix[i][j] = nums[count];
      count++;
    }
  }
}