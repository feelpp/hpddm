/*
   This file is part of HPDDM.

   Author(s): Pierre Jolivet <pierre.jolivet@inf.ethz.ch>
        Date: 2014-08-04

   Copyright (C) 2011-2014 Université de Grenoble
                 2015      Eidgenössische Technische Hochschule Zürich

   HPDDM is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published
   by the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   HPDDM is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with HPDDM.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _WRAPPER_
#define _WRAPPER_

namespace HPDDM {
template<class T>
struct underlying_type_spec {
    typedef T type;
};
template<class T>
struct underlying_type_spec<std::complex<T>> {
    typedef T type;
};

template <class T>
using underlying_type = typename underlying_type_spec<T>::type;
/* Class: Wrapper
 *
 *  A class for handling all dense and sparse linear algebra.
 *
 * Template Parameter:
 *    K              - Scalar type. */
template<class K>
class Wrapper {
    public:
        /* Function: mpi_type
         *  Returns the MPI datatype of the template parameter of <Wrapper>. */
        static MPI_Datatype mpi_type();
        /* Typedef: ul_type
         *  Scalar underlying type, e.g. double (resp. float) for std::complex<double> (resp. std::complex<float>). */
        typedef underlying_type<K> ul_type;
        /* Variable: transc
         *  Transposed real operators or conjugated transposed complex operators. */
        static const char transc;
        /* Variable: I
         *  Numbering of a sparse <MatrixCSR>. */
        static const char I;
        /* Variable: d__0
         *  Zero. */
        static const K d__0;
        /* Variable: d__1
         *  One. */
        static const K d__1;
        /* Variable: d__2
         *  Minus one. */
        static const K d__2;

        /* Function: axpy
         *  Computes a scalar-vector product and adds the result to a vector. */
        static void axpy(const int* const, const K* const, const K* const, const int* const, K* const, const int* const);
        /* Function: scal
         *  Computes a scalar-vector product. */
        static void scal(const int* const, const K* const, K* const, const int* const);
        /* Function: nrm2
         *  Computes the Euclidean norm of a vector. */
        static ul_type nrm2(const int* const, const K* const, const int* const);
        /* Function: dot
         *  Computes a vector-vector dot product. */
        static ul_type dot(const int* const, const K* const, const int* const, const K* const, const int* const);
        /* Function: lacpy
         *  Copies all or part of a two-dimensional matrix. */
        static void lacpy(const char* const, const int* const, const int* const, const K* const, const int* const, K* const, const int* const);

        /* Function: symv
         *  Computes a symmetric scalar-matrix-vector product. */
        static void symv(const char* const, const int* const, const K* const, const K* const, const int* const,
                         const K* const, const int* const, const K* const, K* const, const int* const);
        /* Function: gemv
         *  Computes a scalar-matrix-vector product. */
        static void gemv(const char* const, const int* const, const int* const, const K* const, const K* const,
                         const int* const, const K* const, const int* const, const K* const, K* const, const int* const);

        /* Function: symm
         *  Computes a symmetric scalar-matrix-matrix product. */
        static void symm(const char* const, const char* const, const int* const, const int* const, const K* const, const K* const,
                         const int* const, const K* const, const int* const, const K* const, K* const, const int* const);
        /* Function: gemm
         *  Computes a scalar-matrix-matrix product. */
        static void gemm(const char* const, const char* const, const int* const, const int* const, const int* const, const K* const, const K* const,
                         const int* const, const K* const, const int* const, const K* const, K* const, const int* const);

        /* Function: csrmv(square)
         *  Computes a sparse square matrix-vector product. */
        template<char>
        static void csrmv(bool, const int* const, const K* const, const int* const, const int* const, const K* const, K* const);
        /* Function: csrmv
         *  Computes a scalar-sparse matrix-vector product. */
        template<char>
        static void csrmv(const char* const, const int* const, const int* const, const K* const, bool,
                          const K* const, const int* const, const int* const, const K* const, const K* const, K* const);
        /* Function: csrmm(square)
         *  Computes a sparse square matrix-matrix product. */
        template<char>
        static void csrmm(bool, const int* const, const int* const, const K* const, const int* const, const int* const, const K* const, K* const);
        /* Function: csrmm
         *  Computes a scalar-sparse matrix-matrix product. */
        template<char>
        static void csrmm(const char* const, const int* const, const int* const, const int* const, const K* const, bool,
                          const K* const, const int* const, const int* const, const K* const, const int* const,
                          const K* const, K* const, const int* const);

        /* Function: csrcsc
         *  Converts a matrix stored in Compressed Sparse Row format into Compressed Sparse Column format. */
        template<char>
        static void csrcsc(const int* const, K* const, int* const, int* const, K* const, int* const, int* const);
        /* Function: gthr
         *  Gathers the elements of a full-storage sparse vector into compressed form. */
        static void gthr(const int&, const K* const, K* const, const int* const);
        /* Function: sctr
         *  Scatters the elements of a compressed sparse vector into full-storage form. */
        static void sctr(const int&, const K* const, const int* const, K* const);
        /* Function: diag(in-place)
         *  Computes a vector-vector element-wise multiplication. */
        static void diag(const int&, const ul_type* const, K* const);
        /* Function: diag
         *  Computes a vector-vector element-wise multiplication. */
        static void diag(const int&, const ul_type* const, const K* const, K* const);
        /* Function: diag(in-place)
         *  Computes a vector-matrix element-wise multiplication. */
        static void diag(const int&, const int&, const ul_type* const, K* const);
        /* Function: diag
         *  Computes a vector-matrix element-wise multiplication. */
        static void diag(const int&, const int&, const ul_type* const, const K* const, K* const);
        /* Function: axpby
         *  Computes two scalar-vector products. */
        static void axpby(const int&, const K&, const K* const, const int&, const K&, K* const, const int&);
        /* Function: conjugate
         *  Conjugates all elements of a matrix. */
        template<class T, typename std::enable_if<!std::is_same<T, typename Wrapper<T>::ul_type>::value>::type* = nullptr>
        static void conjugate(const int& m, const int& n, const int& ld, T* const in) {
            for(int i = 0; i < n; ++i)
                std::for_each(in + i * ld, in + i * ld + m, [](T& z) { z = std::conj(z); });
        }
        template<class T, typename std::enable_if<std::is_same<T, typename Wrapper<T>::ul_type>::value>::type* = nullptr>
        static void conjugate(const int&, const int&, const int&, T* const) { }
        /* Function: transpose
         *  Transposes a dense matrix in-place. */
        static void transpose(K* const, const std::size_t, const std::size_t);
};

template<>
inline MPI_Datatype Wrapper<float>::mpi_type() { return MPI_FLOAT; }
template<>
inline MPI_Datatype Wrapper<double>::mpi_type() { return MPI_DOUBLE; }
template<>
inline MPI_Datatype Wrapper<std::complex<float>>::mpi_type() { return MPI_COMPLEX; }
template<>
inline MPI_Datatype Wrapper<std::complex<double>>::mpi_type() { return MPI_DOUBLE_COMPLEX; }

template<class K>
const char Wrapper<K>::transc = std::is_same<K, ul_type>::value ? 'T' : 'C';

template<class K>
const K Wrapper<K>::d__0 = K(0.0);
template<class K>
const K Wrapper<K>::d__1 = K(1.0);
template<class K>
const K Wrapper<K>::d__2 = K(-1.0);

#define HPDDM_GENERATE_BLAS(C, T)                                                                            \
template<>                                                                                                   \
inline void Wrapper<T>::axpy(const int* const n, const T* const a,                                           \
                             const T* const x, const int* const incx,                                        \
                             T* const y, const int* const incy) {                                            \
    HPDDM_F77(C ## axpy)(n, a, x, incx, y, incy);                                                            \
}                                                                                                            \
template<>                                                                                                   \
inline void Wrapper<T>::scal(const int* const n, const T* const a, T* const x, const int* const incx) {      \
    HPDDM_F77(C ## scal)(n, a, x, incx);                                                                     \
}                                                                                                            \
template<>                                                                                                   \
inline void Wrapper<T>::lacpy(const char* const uplo, const int* const m, const int* const n,                \
                              const T* const a, const int* const lda, T* const b, const int* const ldb) {    \
    HPDDM_F77(C ## lacpy)(uplo, m, n, a, lda, b, ldb);                                                       \
}                                                                                                            \
                                                                                                             \
template<>                                                                                                   \
inline void Wrapper<T>::symv(const char* const uplo, const int* const n,                                     \
                             const T* const alpha, const T* const a, const int* const lda,                   \
                             const T* const b, const int* const ldb, const T* const beta,                    \
                             T* const c, const int* const ldc) {                                             \
    HPDDM_F77(C ## symv)(uplo, n, alpha, a, lda, b, ldb, beta, c, ldc);                                      \
}                                                                                                            \
template<>                                                                                                   \
inline void Wrapper<T>::gemv(const char* const trans, const int* const m, const int* const n,                \
                             const T* const alpha, const T* const a, const int* const lda,                   \
                             const T* const b, const int* const ldb, const T* const beta,                    \
                             T* const c, const int* const ldc) {                                             \
    HPDDM_F77(C ## gemv)(trans, m, n, alpha, a, lda, b, ldb, beta, c, ldc);                                  \
}                                                                                                            \
                                                                                                             \
template<>                                                                                                   \
inline void Wrapper<T>::symm(const char* const side, const char* const uplo,                                 \
                             const int* const m, const int* const n,                                         \
                             const T* const alpha, const T* const a, const int* const lda,                   \
                             const T* const b, const int* const ldb, const T* const beta,                    \
                             T* const c, const int* const ldc) {                                             \
    HPDDM_F77(C ## symm)(side, uplo, m, n, alpha, a, lda, b, ldb, beta, c, ldc);                             \
}                                                                                                            \
template<>                                                                                                   \
inline void Wrapper<T>::gemm(const char* const transa, const char* const transb, const int* const m,         \
                             const int* const n, const int* const k,                                         \
                             const T* const alpha, const T* const a, const int* const lda,                   \
                             const T* const b, const int* const ldb, const T* const beta,                    \
                             T* const c, const int* const ldc) {                                             \
    HPDDM_F77(C ## gemm)(transa, transb, m, n, k, alpha, a, lda, b, ldb, beta, c, ldc);                      \
}
#if HPDDM_MKL || defined(__APPLE__)
#define HPDDM_GENERATE_DOTC(C, T, U)                                                                         \
template<>                                                                                                   \
inline U Wrapper<T>::dot(const int* const n, const T* const x, const int* const incx,                        \
                         const T* const y, const int* const incy) {                                          \
    T res;                                                                                                   \
    C ## dotc(&res, n, x, incx, y, incy);                                                                    \
    return std::real(res);                                                                                   \
}
#define HPDDM_GENERATE_AXPBY(C, T, B, U)                                                                     \
template<>                                                                                                   \
inline void Wrapper<U>::axpby(const int& n, const U& alpha, const U* const u, const int& incx,               \
                              const U& beta, U* const v, const int& incy) {                                  \
    HPDDM_PREFIX_AXPBY(B ## axpby)(n, alpha, u, incx, beta, v, incy);                                        \
}                                                                                                            \
template<>                                                                                                   \
inline void Wrapper<T>::axpby(const int& n, const T& alpha, const T* const u, const int& incx,               \
                              const T& beta, T* const v, const int& incy) {                                  \
    HPDDM_PREFIX_AXPBY(C ## axpby)(n, &alpha, u, incx, &beta, v, incy);                                      \
}
#else
#define HPDDM_GENERATE_DOTC(C, T, U)                                                                         \
template<>                                                                                                   \
inline U Wrapper<T>::dot(const int* const n, const T* const x, const int* const incx,                        \
                         const T* const y, const int* const incy) {                                          \
    T res = HPDDM_F77(C ## dotc)(n, x, incx, y, incy);                                                       \
    return std::real(res);                                                                                   \
}
#endif
#define HPDDM_GENERATE_BLAS_COMPLEX(C, T, B, U)                                                              \
template<>                                                                                                   \
inline U Wrapper<U>::nrm2(const int* const n, const U* const x, const int* const incx) {                     \
    return HPDDM_F77(B ## nrm2)(n, x, incx);                                                                 \
}                                                                                                            \
template<>                                                                                                   \
inline U Wrapper<T>::nrm2(const int* const n, const T* const x, const int* const incx) {                     \
    return HPDDM_F77(B ## C ## nrm2)(n, x, incx);                                                            \
}                                                                                                            \
template<>                                                                                                   \
inline U Wrapper<U>::dot(const int* const n, const U* const x, const int* const incx,                        \
                         const U* const y, const int* const incy) {                                          \
    return HPDDM_F77(B ## dot)(n, x, incx, y, incy);                                                         \
}                                                                                                            \
HPDDM_GENERATE_DOTC(C, T, U)
HPDDM_GENERATE_BLAS(s, float)
HPDDM_GENERATE_BLAS(d, double)
HPDDM_GENERATE_BLAS(c, std::complex<float>)
HPDDM_GENERATE_BLAS(z, std::complex<double>)
HPDDM_GENERATE_BLAS_COMPLEX(c, std::complex<float>, s, float)
HPDDM_GENERATE_BLAS_COMPLEX(z, std::complex<double>, d, double)

template<class K>
inline void Wrapper<K>::diag(const int& n, const ul_type* const d, K* const in) {
    diag(n, d, nullptr, in);
}
template<class K>
inline void Wrapper<K>::diag(const int& m, const int& n, const ul_type* const d, K* const in) {
    diag(m, n, d, nullptr, in);
}

#if HPDDM_MKL
template<class K>
const char Wrapper<K>::I = 'F';

template<char N>
struct matdescr {
    static const char a[];
    static const char b[];
};

template<char N>
const char matdescr<N>::a[6] { 'G', '0', '0', N, '0', '0' };
template<char N>
const char matdescr<N>::b[6] { 'S', 'L', 'N', N, '0', '0' };

#define HPDDM_GENERATE_MKL(C, T)                                                                             \
template<>                                                                                                   \
template<char N>                                                                                             \
inline void Wrapper<T>::csrmv(bool sym, const int* const n, const T* const a, const int* const ia,           \
                              const int* const ja, const T* const x, T* const y) {                           \
    static_assert(N == 'C', "Unsupported matrix indexing");                                                  \
    if(sym)                                                                                                  \
        mkl_cspblas_ ## C ## csrsymv("L", HPDDM_CONST(int, n), HPDDM_CONST(T, a), HPDDM_CONST(int, ia),      \
                                     HPDDM_CONST(int, ja), HPDDM_CONST(T, x), y);                            \
    else                                                                                                     \
        mkl_cspblas_ ## C ## csrgemv(HPDDM_CONST(char, &transa), HPDDM_CONST(int, n), HPDDM_CONST(T, a),     \
                                     HPDDM_CONST(int, ia), HPDDM_CONST(int, ja), HPDDM_CONST(T, x), y);      \
}                                                                                                            \
template<>                                                                                                   \
template<char N>                                                                                             \
inline void Wrapper<T>::csrmv(const char* const trans, const int* const m, const int* const k,               \
                              const T* const alpha, bool sym, const T* const a, const int* const ia,         \
                              const int* const ja, const T* const x, const T* const beta, T* const y) {      \
    mkl_ ## C ## csrmv(HPDDM_CONST(char, trans), HPDDM_CONST(int, m), HPDDM_CONST(int, k),                   \
                       HPDDM_CONST(T, alpha), HPDDM_CONST(char, sym ? matdescr<N>::b : matdescr<N>::a),      \
                       HPDDM_CONST(T, a), HPDDM_CONST(int, ja), HPDDM_CONST(int, ia),                        \
                       HPDDM_CONST(int, ia) + 1, HPDDM_CONST(T, x), HPDDM_CONST(T, beta), y);                \
}                                                                                                            \
template<>                                                                                                   \
template<char N>                                                                                             \
inline void Wrapper<T>::csrmm(const char* const trans, const int* const m, const int* const n,               \
                              const int* const k, const T* const alpha, bool sym,                            \
                              const T* const a, const int* const ia, const int* const ja,                    \
                              const T* const x, const int* const ldb, const T* const beta,                   \
                              T* const y, const int* const ldc) {                                            \
    if(*n != 1) {                                                                                            \
        if(N != 'F') {                                                                                       \
            std::for_each(const_cast<int*>(ja), const_cast<int*>(ja) + ia[*m], [](int& i) { ++i; });         \
            std::for_each(const_cast<int*>(ia), const_cast<int*>(ia) + *m + 1, [](int& i) { ++i; });         \
        }                                                                                                    \
        mkl_ ## C ## csrmm(HPDDM_CONST(char, trans), HPDDM_CONST(int, m), HPDDM_CONST(int, n),               \
                           HPDDM_CONST(int, k), HPDDM_CONST(T, alpha),                                       \
                           HPDDM_CONST(char, sym ? matdescr<'F'>::b : matdescr<'F'>::a), HPDDM_CONST(T, a),  \
                           HPDDM_CONST(int, ja), HPDDM_CONST(int, ia), HPDDM_CONST(int, ia) + 1,             \
                           HPDDM_CONST(T, x), HPDDM_CONST(int, ldb), HPDDM_CONST(T, beta),  y,               \
                           HPDDM_CONST(int, ldc));                                                           \
        if(N != 'F') {                                                                                       \
            std::for_each(const_cast<int*>(ia), const_cast<int*>(ia) + *m + 1, [](int& i) { --i; });         \
            std::for_each(const_cast<int*>(ja), const_cast<int*>(ja) + ia[*m], [](int& i) { --i; });         \
        }                                                                                                    \
    }                                                                                                        \
    else                                                                                                     \
        csrmv<N>(trans, m, k, alpha, sym, a, ia, ja, x, beta, y);                                            \
}                                                                                                            \
                                                                                                             \
template<>                                                                                                   \
template<char N>                                                                                             \
inline void Wrapper<T>::csrcsc(const int* const n, T* const a, int* const ja, int* const ia,                 \
                               T* const b, int* const jb, int* const ib) {                                   \
    int job[6] { 0, 0, N == 'F', 0, 0, 1 };                                                                  \
    int error;                                                                                               \
    mkl_ ## C ## csrcsc(job, HPDDM_CONST(int, n), a, ja, ia, b, jb, ib, &error);                             \
}                                                                                                            \
template<>                                                                                                   \
inline void Wrapper<T>::gthr(const int& n, const T* const y, T* const x, const int* const indx) {            \
    cblas_ ## C ## gthr(n, y, x, indx);                                                                      \
}                                                                                                            \
template<>                                                                                                   \
inline void Wrapper<T>::sctr(const int& n, const T* const x, const int* const indx, T* const y) {            \
    cblas_ ## C ## sctr(n, x, indx, y);                                                                      \
}                                                                                                            \
template<>                                                                                                   \
inline void Wrapper<T>::transpose(T* const a, const std::size_t n, const std::size_t m) {                    \
    mkl_ ## C ## imatcopy('C', 'T', m, n, d__1, a, m, n);                                                    \
}
#define HPDDM_GENERATE_MKL_VML(C, T)                                                                         \
template<>                                                                                                   \
inline void Wrapper<T>::diag(const int& m, const int& n, const T* const d,                                   \
                             const T* const in, T* const out) {                                              \
    if(in)                                                                                                   \
        for(int i = 0; i < n; ++i)                                                                           \
            v ## C ## Mul(m, d, in + i * m, out + i * m);                                                    \
    else                                                                                                     \
        for(int i = 0; i < n; ++i)                                                                           \
            v ## C ## Mul(m, d, out + i * m, out + i * m);                                                   \
}                                                                                                            \
template<>                                                                                                   \
inline void Wrapper<T>::diag(const int& n, const T* const d, const T* const in, T* const out) {              \
    diag(n, i__1, d, in, out);                                                                               \
}
HPDDM_GENERATE_MKL(s, float)
HPDDM_GENERATE_MKL(d, double)
HPDDM_GENERATE_MKL(c, std::complex<float>)
HPDDM_GENERATE_MKL(z, std::complex<double>)
HPDDM_GENERATE_MKL_VML(s, float)
HPDDM_GENERATE_MKL_VML(d, double)
HPDDM_GENERATE_AXPBY(c, std::complex<float>, s, float)
HPDDM_GENERATE_AXPBY(z, std::complex<double>, d, double)
#else
template<class K>
const char Wrapper<K>::I = 'C';

template<class K>
template<char N>
inline void Wrapper<K>::csrmv(bool sym, const int* const n, const K* const a, const int* const ia, const int* const ja, const K* const x, K* const y) {
    csrmv<N>(&transa, n, n, &d__1, sym, a, ia, ja, x, &d__0, y);
}
template<class K>
template<char N>
inline void Wrapper<K>::csrmv(const char* const trans, const int* const m, const int* const k, const K* const alpha, bool sym,
                              const K* const a, const int* const ia, const int* const ja, const K* const x, const K* const beta, K* const y) {
    if(trans == &transa) {
        if(sym) {
            if(beta == &d__0)
                std::fill(y, y + *m, K());
            else if(beta != &d__1)
                scal(m, beta, y, &i__1);
            for(int i = 0; i < *m; ++i) {
                if(ia[i + 1] != ia[i]) {
                    K res = K();
                    int l = ia[i] - (N == 'F');
                    int j = ja[l] - (N == 'F');
                    while(l < ia[i + 1] - 1 - (N == 'F')) {
                        res += a[l] * x[j];
                        y[j] += *alpha * a[l] * x[i];
                        j = ja[++l] - (N == 'F');
                    }
                    if(i != j) {
                        res += a[l] * x[j];
                        y[j] += *alpha * a[l] * x[i];
                        y[i] += *alpha * res;
                    }
                    else
                        y[i] += *alpha * (res + a[l] * x[i]);
                }
            }
        }
        else {
            if(beta == &d__0)
                std::fill(y, y + *m, K());
#pragma omp parallel for schedule(static, HPDDM_GRANULARITY)
            for(int i = 0; i < *m; ++i) {
                K res = K();
                for(int l = ia[i] - (N == 'F'); l < ia[i + 1] - (N == 'F'); ++l)
                    res += a[l] * x[ja[l] - (N == 'F')];
                y[i] = *alpha * res + *beta * y[i];
            }
        }
    }
    else {
        if(beta == &d__0)
            std::fill(y, y + *k, K());
        else if(beta != &d__1)
            scal(k, beta, y, &i__1);
        if(sym) {
            for(int i = 0; i < *m; ++i) {
                K res = K();
                for(int l = ia[i] - (N == 'F'); l < ia[i + 1] - (N == 'F'); ++l) {
                    int j = ja[l] - (N == 'F');
                    y[j] += *alpha * a[l] * x[i];
                    if(i != j)
                        res += a[l] * x[j];
                }
                y[i] += *alpha * res;
            }
        }
        else {
            for(int i = 0; i < *m; ++i)
                for(int l = ia[i] - (N == 'F'); l < ia[i + 1] - (N == 'F'); ++l)
                    y[ja[l] - (N == 'F')] += *alpha * a[l] * x[i];
        }
    }
}
template<class K>
template<char N>
inline void Wrapper<K>::csrmm(const char* const trans, const int* const m, const int* const n, const int* const k, const K* const alpha, bool sym,
                              const K* const a, const int* const ia, const int* const ja, const K* const x, const int* const ldb, const K* const beta, K* const y, const int* const ldc) {
    if(trans == &transa) {
        if(*ldb != *k || *ldc != *m)
            return;
        int dimY = *m;
        K* res;
        if(sym) {
            int j;
            int dimX = *k;
            int dimNY = dimY * *n;
            if(beta == &d__0)
                std::fill(y, y + dimNY, K());
            else if(beta != &d__1)
                scal(&dimNY, beta, y, &i__1);
            res = new K[*n];
            for(int i = 0; i < dimY; ++i) {
                std::fill(res, res + *n, K());
                for(int l = ia[i] - (N == 'F'); l < ia[i + 1] - (N == 'F'); ++l) {
                    j = ja[l] - (N == 'F');
                    if(i != j)
                        for(int r = 0; r < *n; ++r) {
                            res[r] += a[l] * x[j + r * dimX];
                            y[j + r * *ldb] += *alpha * a[l] * x[i + r * *ldc];
                        }
                    else
                        axpy(n, a + l, x + j, k, res, &i__1);
                }
                axpy(n, alpha, res, &i__1, y + i, m);
            }
            delete [] res;
        }
        else {
#pragma omp parallel private(res)
            {
                res = new K[*n];
#pragma omp for schedule(static, HPDDM_GRANULARITY)
                for(int i = 0; i < dimY; ++i) {
                    std::fill(res, res + *n, K());
                    for(int l = ia[i] - (N == 'F'); l < ia[i + 1] - (N == 'F'); ++l)
                        axpy(n, a + l, x + ja[l] - (N == 'F'), k, res, &i__1);
                    axpby(*n, *alpha, res, 1, *beta, y + i, dimY);
                }
                delete [] res;
            }
        }
    }
    else {
        if(*ldb != *m || *ldc != *k)
            return;
        int dimX = *m;
        int dimY = *k;
        int dimNY = dimY * *n;
        if(beta == &d__0)
            std::fill(y, y + dimNY, K());
        else if(beta != &d__1)
            scal(&dimNY, beta, y, &i__1);
        if(sym) {
            K* res = new K[*n];
            for(int i = 0; i < *m; ++i) {
                std::fill(res, res + *n, K());
                for(int l = ia[i] - (N == 'F'); l < ia[i + 1] - (N == 'F'); ++l) {
                    int j = ja[l] - (N == 'F');
                    if(i != j)
                        for(int r = 0; r < *n; ++r) {
                            y[j + r * dimY] += *alpha * a[l] * x[i + r * dimX];
                            res[r] += a[l] * x[j + r * dimX];
                        }
                    else {
                        const K scal = *alpha * a[l];
                        axpy(n, &scal, x + i, m, y + j, k);
                    }
                }
                axpy(n, alpha, res, &i__1, y + i, k);
            }
            delete [] res;
        }
        else {
            for(int i = 0; i < *m; ++i)
                for(int l = ia[i] - (N == 'F'); l < ia[i + 1] - (N == 'F'); ++l) {
                    const K scal = *alpha * a[l];
                    axpy(n, &scal, x + i, m, y + ja[l] - (N == 'F'), k);
                }
        }
    }
}

template<class K>
template<char N>
inline void Wrapper<K>::csrcsc(const int* const n, K* const a, int* const ja, int* const ia, K* const b, int* const jb, int* const ib) {
    int nnz = ia[*n];
    std::fill(ib, ib + *n + 1, 0);
    for(int i = 0; i < nnz; ++i)
        ib[ja[i] + 1]++;
    std::partial_sum(ib, ib + *n + 1, ib);
    for(int i = 0; i < *n; ++i)
        for(int j = ia[i]; j < ia[i + 1]; ++j) {
            int k = ib[ja[j]]++;
            jb[k] = i + (N == 'F');
            b[k] = a[j];
        }
    for(int i = *n; i > 0; --i)
        ib[i] = ib[i - 1] + (N == 'F');
    ib[0] = (N == 'F');
}
template<class K>
inline void Wrapper<K>::gthr(const int& n, const K* const y, K* const x, const int* const indx) {
    for(int i = 0; i < n; ++i)
        x[i] = y[indx[i]];
}
template<class K>
inline void Wrapper<K>::sctr(const int& n, const K* const x, const int* const indx, K* const y) {
    for(int i = 0; i < n; ++i)
        y[indx[i]] = x[i];
}
#ifdef __APPLE__
HPDDM_GENERATE_AXPBY(c, std::complex<float>, s, float)
HPDDM_GENERATE_AXPBY(z, std::complex<double>, d, double)
#else
template<class K>
inline void Wrapper<K>::axpby(const int& n, const K& alpha, const K* const u, const int& incx, const K& beta, K* const v, const int& incy) {
    if(beta == d__0)
        for(unsigned int i = 0; i < n; ++i)
            v[i * incy] = alpha * u[i * incx];
    else
        for(unsigned int i = 0; i < n; ++i)
            v[i * incy] = alpha * u[i * incx] + beta * v[i * incy];
}
#endif // __APPLE__
template<class K>
inline void Wrapper<K>::transpose(K* const a, const std::size_t n, const std::size_t m) {
    if(n != m) {
        if(n != 1 && m != 1) {
            const int size = n * m - 1;
            std::bitset<1024> b;
            b[0] = b[size] = 1;
            int i = 1;
            while(i < size) {
                int it = i;
                K t = a[i];
                do {
                    int next = (i * n) % size;
                    std::swap(a[next], t);
                    b[i] = 1;
                    i = next;
                } while(i != it);

                for(i = 1; i < size && b[i]; i++);
            }
        }
    }
    else {
        for(int i = 0; i < n - 1; ++i)
            for(int j = i + 1; j < n; ++j)
                std::swap(a[i * n + j], a[j * n + i]);
    }
}
#endif // HPDDM_MKL

template<class K>
inline void Wrapper<K>::diag(const int& n, const ul_type* const d, const K* const in, K* const out) {
    diag(n, i__1, d, in, out);
}
template<class K>
inline void Wrapper<K>::diag(const int& m, const int& n, const ul_type* const d, const K* const in, K* const out) {
    if(in)
        for(int i = 0; i < n; ++i)
            for(int j = 0; j < m; ++j)
                out[j + i * m] = d[j] * in[j + i * m];
    else
        for(int i = 0; i < n; ++i)
            for(int j = 0; j < m; ++j)
                out[j + i * m] *= d[j];
}
template<class K>
template<char N>
inline void Wrapper<K>::csrmm(bool sym, const int* const n, const int* const m, const K* const a, const int* const ia, const int* const ja, const K* const x, K* const y) {
    csrmm<N>(&transa, n, m, n, &d__1, sym, a, ia, ja, x, n, &d__0, y, n);
}

template<class Idx, class T>
inline void reorder(const Idx& i, const Idx& j, const T& v) {
    std::swap(v[i], v[j]);
}
template<class Idx, class First, class... Rest>
inline void reorder(const Idx& i, const Idx& j, const First& first, const Rest&... rest) {
    std::swap(first[i], first[j]);
    reorder(i, j, rest...);
}
/* Function: reorder
 *  Rearranges an arbitrary number of containers based on the permutation defined by the first argument. */
template<class T, class... Args>
inline void reorder(std::vector<T>& order, const Args&... args) {
    static_assert(sizeof...(args) > 0, "Nothing to reorder");
    for(T i = 0; i < order.size() - 1; ++i) {
        T j = order[i];
        if(j != i) {
            T k = i + 1;
            while(order[k] != i)
                ++k;
            std::swap(order[i], order[k]);
            reorder(i, j, args...);
        }
    }
}
} // HPDDM
#endif // _WRAPPER_
