//
// Created by Luwei Yang on 6/11/15.
// This Header declears the interface of Eigen for multiple linear solver.
//

#ifndef RECONSTRUCTOR_LINEARSOLVER_H
#define RECONSTRUCTOR_LINEARSOLVER_H

#include <Eigen/Sparse>
#include <Eigen/Dense>


enum SolverType
{
    kLUDecomposition = 0,
    kQRDecomposition,
    kCholeskyDecomposition,
    kConjugateGradient
};

enum ErrorType
{
    kDecompositionError = 0,
    kSolveError,
    kRankEfficientError,
    kMatrixSingularError,
};

template <typename T>
class LinearSolver
{
    SolverType solver_type_;
public:

    //Return the solver type
    SolverType solverType()
    {
        return solver_type_;
    }

    // Solve Ax = b
    // static int solve(const SparseMatrix<T>& A, const Matrix<T, Eigen::Dynamic, 1>  &b, Matrix<T, Eigen::Dynamic, 1> &x);
};

// Iterative Conjugate Graident solver, solving the linear equation Ax = b
// where the matrix A is symmetric and positive-definite.
template <typename T>
class CGSolver:public LinearSolver<T>
{
public:
    // Solve Ax = b via interation
    static int Solve(const Eigen::SparseMatrix<T> &A,                       // symmetric and positive-definite matrix A
                     const Eigen::Matrix<T, Eigen::Dynamic, 1> &b,          // right part b
                     Eigen::Matrix<T, Eigen::Dynamic, 1> &x);               // solution

    // Solve Ax = b via iteration with external parameter of max iteration
    // and torlerance
    static int Solve(const Eigen::SparseMatrix<T> &A,                       // symmetric and positive-definite matrix A
                     const Eigen::Matrix<T, Eigen::Dynamic, 1> &b,          // right part b
                     Eigen::Matrix<T, Eigen::Dynamic, 1> &x,                // solution
                     const int max_iteration, const double torlerance);      // parameters, max iteration and torlerance
};

// TODO[Luwei]: implement other linear solver

// LU decomposition solver, solving the linear equation Ax = b
template <typename T>
class LUSolver:public LinearSolver<T>
{
    // ptr to lower triangle matrix
    Eigen::SparseMatrix<T> *l_;

    // ptr to upper triangle matrix
    Eigen::SparseMatrix<T> *u_;

public:
    // Constructor
    LUSolver();

    // Deconstructor
    ~LUSolver();

    // Solve Ax = b via LU decomposition
    static int Solve(const Eigen::SparseMatrix<T> &A,                                  // matrix A
                     const Eigen::Matrix<T, Eigen::Dynamic, 1> &b,                     // right part b
                     Eigen::Matrix<T, Eigen::Dynamic, 1> &x);                          // solution

    // Solve least square probelm Ax = b, where A(m,n) and m > n,
    // solving using A'Ax = A'b
    static int SolveLeastSquare(const Eigen::SparseMatrix<T> &A,                       // matrix A(m,n), where m > n
                                const Eigen::Matrix<T, Eigen::Dynamic, 1> &b,          // right part b
                                Eigen::Matrix<T, Eigen::Dynamic, 1> &x);               // solution

    // Solve least norm probelm Ax = b, where A(m,n) and m < n,
    // solving using x = A'(AA')^(-1)b
    static int SolveLeastNorm(const Eigen::SparseMatrix<T> &A,                         // matrix A(m,n), where m > n
                              const Eigen::Matrix<T, Eigen::Dynamic, 1> &b,            // right part b
                              Eigen::Matrix<T, Eigen::Dynamic, 1> &x);                 // solution

    // Return the ptr of lower triangle matrix
    inline Eigen::SparseMatrix<T>* L() const;

    // Return the ptr of upper triangle matrix
    inline Eigen::SparseMatrix<T>* U() const;

    // Factorize the matrix A, [L,U] = lu(A) in matlab
    // return non-zero if there was any error.
    int factorize(const Eigen::SparseMatrix<T> &A);                                    // matrix A

    // Factorize the matrix A, [L,U] = lu(A) in matlab
    // return non-zero and pointers to l and u
    int factorize(const Eigen::SparseMatrix<T> &A,                                     // matrix A
                        Eigen::SparseMatrix<T> *L,                                     // lower triangle matrix L
                        Eigen::SparseMatrix<T> *U);                                    // upper triangle matrix U

    // Solve the matrix via factorized, note that factorized matrix L,U is
    // prerequisite, return non-zero.
    int solveFactorizedMatrix(const Eigen::Matrix<T, Eigen::Dynamic, 1> &b,            // right part b
                              Eigen::Matrix<T, Eigen::Dynamic, 1> &x);                 // solution
};

// QR decomposition solver, solving the linear equation Ax = b
template <typename T>
class QRSolver:public LinearSolver<T>
{
    // ptr to orthogonal matrix Q
    Eigen::SparseMatrix<T> *q_;

    // ptr to upper triangle matrix R
    Eigen::SparseMatrix<T> *r_;

public:
    // Constructor
    QRSolver();

    // Deconstructor
    ~QRSolver();

    // Solve Ax = b via QR decomposition
    static int Solve(const Eigen::SparseMatrix<T> &A,                                  // matrix A
                     const Eigen::Matrix<T, Eigen::Dynamic, 1> &b,                     // right part b
                     Eigen::Matrix<T, Eigen::Dynamic, 1> &x);                          // solution

    // Solve least square probelm Ax = b, where A(m,n) and m > n,
    // solving using x=R\(Q’*y), where [Q,R] = qr(A)
    static int SolveLeastSquare(const Eigen::SparseMatrix<T> &A,                       // matrix A(m,n), where m > n
                                const Eigen::Matrix<T, Eigen::Dynamic, 1> &b,          // right part b
                                Eigen::Matrix<T, Eigen::Dynamic, 1> &x);               // solution

    // Solve least norm probelm Ax = b, where A(m,n) and m < n,
    // solving using  x=Q*(R’\y), where [Q,R] = qr(A)
    static int SolveLeastNorm(const Eigen::SparseMatrix<T> &A,                         // matrix A(m,n), where m > n
                              const Eigen::Matrix<T, Eigen::Dynamic, 1> &b,            // right part b
                              Eigen::Matrix<T, Eigen::Dynamic, 1> &x);                 // solution

    // Return the ptr of orthogonal matrix
    inline Eigen::SparseMatrix<T>* L() const;

    // Return the ptr of upper triangle matrix
    inline Eigen::SparseMatrix<T>* U() const;

    // Factorize the matrix A, [Q,R] = qr(A) in matlab
    // return non-zero if there was any error.
    int factorize(const Eigen::SparseMatrix<T> &A);                                    // matrix A

    // Factorize the matrix A, [Q,R] = qr(A) in matlab
    // return non-zero and pointers to l and u
    int factorize(const Eigen::SparseMatrix<T> &A,                                     // matrix A
                  Eigen::SparseMatrix<T> *Q,                                           // Orthogonal matrix Q
                  Eigen::SparseMatrix<T> *R);                                          // upper triangle matrix R

    // Solve the matrix via factorized, note that factorized matrix Q,R is
    // prerequisite, return non-zero.
    int solveFactorizedMatrix(const Eigen::Matrix<T, Eigen::Dynamic, 1> &b,            // right part b
                              Eigen::Matrix<T, Eigen::Dynamic, 1> &x);                 // solution
};

// Cholesky decomposition solver, solving the linear equation Ax = b
template <typename T>
class CholeskySolver:public LinearSolver<T>
{

    // ptr to  matrix, r*r' = A
    Eigen::SparseMatrix<T> *r_;

public:
    // Constructor
    CholeskySolver();

    // Deconstructor
    ~CholeskySolver();

    // Solve Ax = b via QR decomposition
    static int Solve(const Eigen::SparseMatrix<T> &A,                                  // matrix A
                     const Eigen::Matrix<T, Eigen::Dynamic, 1> &b,                     // right part b
                     Eigen::Matrix<T, Eigen::Dynamic, 1> &x);                          // solution

    // Solve least square probelm Ax = b, where A(m,n) and m > n,
    // solving using A'Ax = b
    static int SolveLeastSquare(const Eigen::SparseMatrix<T> &A,                       // matrix A(m,n), where m > n
                                const Eigen::Matrix<T, Eigen::Dynamic, 1> &b,          // right part b
                                Eigen::Matrix<T, Eigen::Dynamic, 1> &x);               // solution

    // Solve least norm probelm Ax = b, where A(m,n) and m < n,
    static int SolveLeastNorm(const Eigen::SparseMatrix<T> &A,                         // matrix A(m,n), where m > n
                              const Eigen::Matrix<T, Eigen::Dynamic, 1> &b,            // right part b
                              Eigen::Matrix<T, Eigen::Dynamic, 1> &x);                 // solution

    // Return the ptr of lower triangle matrix
    inline Eigen::SparseMatrix<T>* R() const;

    // Factorize the matrix A, R = chol(A) in matlab
    // return non-zero if there was any error.
    int factorize(const Eigen::SparseMatrix<T> &A);                                    // matrix A

    // Factorize the matrix A, R = chol(A) in matlab
    // return non-zero and pointers to r
    int factorize(const Eigen::SparseMatrix<T> &A,                                     // matrix A
                  Eigen::SparseMatrix<T> *R);                                          // matrix R

    // Solve the matrix via factorized, note that factorized matrix R is
    // prerequisite, return non-zero.
    int solveFactorizedMatrix(const Eigen::Matrix<T, Eigen::Dynamic, 1> &b,            // right part b
                              Eigen::Matrix<T, Eigen::Dynamic, 1> &x);                 // solution
};

#endif //RECONSTRUCTOR_LINEARSOLVER_H
