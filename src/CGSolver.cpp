//
// Created by Luwei Yang on 6/11/15.
//

#include "include/LinearSolver.h"

using namespace Eigen;

template <typename T>
int CGSolver<T>::Solve(const Eigen::SparseMatrix<T> &A, const Eigen::Matrix<T, Eigen::Dynamic, 1> &b,
                       Eigen::Matrix<T, Eigen::Dynamic, 1> &x) {

    ConjugateGradient<SparseMatrix<T> > cg;
    cg.compute(A);
    x = cg.solve(b);

    // Temporarily can not got any error
    return 0;
}

template <typename T>
int CGSolver<T>::Solve(const Eigen::SparseMatrix<T> &A, const Eigen::Matrix<T, Eigen::Dynamic, 1> &b,
                       Eigen::Matrix<T, Eigen::Dynamic, 1> &x,
                       const int max_iteration, const double torlerance)
{

    ConjugateGradient<SparseMatrix<T> > cg;
    cg.compute(A);

    cg.setMaxIterations(max_iteration);
    cg.setTolerance(torlerance);

    x = cg.solve(b);

    // Temporarily can not got any error
    return 0;
}

template class CGSolver<double>;
