/*
   This file is part of HPDDM.

   Author(s): Pierre Jolivet <pierre.jolivet@enseeiht.fr>
        Date: 2015-10-29

   Copyright (C) 2015      Eidgenössische Technische Hochschule Zürich
                 2016-     Centre National de la Recherche Scientifique

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

#include "schwarz.hpp"

struct CustomOperator : public HPDDM::CustomOperator<HPDDM::MatrixCSR<K>, K> {
    using HPDDM::CustomOperator<HPDDM::MatrixCSR<K>, K>::CustomOperator;
    template<bool = true>
    void apply(const K* const in, K* const out, const unsigned short& mu = 1, K* = nullptr, const unsigned short& = 0) const {
        for(int i = 0; i < _n; ++i) {
            int mid = (_A->_sym ? (_A->_ia[i + 1] - _A->_ia[0]) : std::distance(_A->_ja, std::upper_bound(_A->_ja + _A->_ia[i] - _A->_ia[0], _A->_ja + _A->_ia[i + 1] - _A->_ia[0], i + _A->_ia[0]))) - 1;
            for(unsigned short nu = 0; nu < mu; ++nu)
                out[nu * _n + i] = in[nu * _n + i] / _A->_a[mid];
        }
    }
};

int main(int argc, char **argv) {
#if !((OMPI_MAJOR_VERSION > 1 || (OMPI_MAJOR_VERSION == 1 && OMPI_MINOR_VERSION >= 7)) || MPICH_NUMVERSION >= 30000000)
    MPI_Init(&argc, &argv);
#else
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, NULL);
#endif
    /*# Init #*/
    int rankWorld, sizeWorld;
    MPI_Comm_size(MPI_COMM_WORLD, &sizeWorld);
    MPI_Comm_rank(MPI_COMM_WORLD, &rankWorld);
    HPDDM::Option& opt = *HPDDM::Option::get();
    opt.parse(argc, argv, rankWorld == 0, {
        std::forward_as_tuple("Nx=<100>", "Number of grid points in the x-direction.", HPDDM::Option::Arg::integer),
        std::forward_as_tuple("Ny=<100>", "Number of grid points in the y-direction.", HPDDM::Option::Arg::integer),
        std::forward_as_tuple("overlap=<1>", "Number of grid points in the overlap.", HPDDM::Option::Arg::integer),
        std::forward_as_tuple("generate_random_rhs=<0>", "Number of generated random right-hand sides.", HPDDM::Option::Arg::integer),
        std::forward_as_tuple("symmetric_csr=(0|1)", "Assemble symmetric matrices.", HPDDM::Option::Arg::argument),
        std::forward_as_tuple("nonuniform=(0|1)", "Use a different number of eigenpairs to compute on each subdomain.", HPDDM::Option::Arg::argument)
    });
    std::vector<std::vector<int>> mapping;
    mapping.reserve(8);
    std::list<int> o; // at most eight neighbors in 2D
    HPDDM::MatrixCSR<K>* Mat, *MatNeumann = nullptr;
    K* f, *sol;
    HPDDM::underlying_type<K>* d;
    int ndof;
    generate(rankWorld, sizeWorld, o, mapping, ndof, Mat, MatNeumann, d, f, sol);
    int mu = opt.app()["generate_random_rhs"];
    int status = 0;
    if(sizeWorld > 1) {
        /*# Creation #*/
        HPDDM::Schwarz<SUBDOMAIN, COARSEOPERATOR, symCoarse, K> A;
        /*# CreationEnd #*/
        /*# Initialization #*/
        A.Subdomain::initialize(Mat, o, mapping);
        decltype(mapping)().swap(mapping);
        A.multiplicityScaling(d);
        A.initialize(d);
        if(mu != 0)
            A.scaledExchange<true>(f, mu);
        else
            mu = 1;
        /*# InitializationEnd #*/
        if(opt.set("schwarz_coarse_correction")) {
            /*# Factorization #*/
            unsigned short nu = opt["geneo_nu"];
            if(nu > 0) {
                if(opt.app().find("nonuniform") != opt.app().cend())
                    nu += std::max(-opt["geneo_nu"] + 1, std::pow(-1, rankWorld) * rankWorld);
                HPDDM::underlying_type<K> threshold = std::max(0.0, opt.val("geneo_threshold"));
                A.solveGEVP<HPDDM::Arpack>(MatNeumann, nu, threshold);
                opt["geneo_nu"] = nu;
            }
            else {
                nu = 1;
                K** deflation = new K*[1];
                *deflation = new K[ndof];
                std::fill(*deflation, *deflation + ndof, 1.0);
                A.setVectors(deflation);
            }
            A.super::initialize(nu);
            A.buildTwo(MPI_COMM_WORLD);
            /*# FactorizationEnd #*/
        }
        A.callNumfact();
        if(rankWorld != 0)
            opt.remove("verbosity");
        /*# Solution #*/
        int it = HPDDM::IterativeMethod::solve(A, f, sol, mu, A.getCommunicator());
        /*# SolutionEnd #*/
        HPDDM::underlying_type<K>* storage = new HPDDM::underlying_type<K>[2 * mu];
        A.computeError(sol, f, storage, mu);
        if(rankWorld == 0)
            for(unsigned short nu = 0; nu < mu; ++nu) {
                if(nu == 0)
                    std::cout << " --- error = ";
                else
                    std::cout << "             ";
                std::cout << std::scientific << storage[1 + 2 * nu] << " / " << storage[2 * nu];
                if(mu > 1)
                    std::cout << " (rhs #" << nu + 1 << ")";
                std::cout << std::endl;
            }
        if(it > 45)
            status = 1;
        else {
            for(unsigned short nu = 0; nu < mu; ++nu)
                 if(storage[1 + 2 * nu] / storage[2 * nu] > 1.0e-2)
                     status = 1;
        }
        delete [] storage;
    }
    else {
        mu = std::max(1, mu);
        int it = 0;
        std::string filename = opt.prefix("dump_local_matrices", true);
        if(!filename.empty()) {
            std::ofstream output { filename };
            output << *Mat;
        }
        if(opt["schwarz_method"] != 5) {
            SUBDOMAIN<K> S;
            S.numfact(Mat);
            S.solve(f, sol, mu);
        }
        else
            it = HPDDM::IterativeMethod::solve(CustomOperator(Mat), f, sol, mu, MPI_COMM_SELF);
        HPDDM::underlying_type<K>* nrmb = new HPDDM::underlying_type<K>[2 * mu];
        for(unsigned short nu = 0; nu < mu; ++nu)
            nrmb[nu] = HPDDM::Blas<K>::nrm2(&ndof, f + nu * ndof, &(HPDDM::i__1));
        K* tmp = new K[mu * ndof];
        HPDDM::Wrapper<K>::csrmm(Mat->_sym, &ndof, &mu, Mat->_a, Mat->_ia, Mat->_ja, sol, tmp);
        ndof *= mu;
        HPDDM::Blas<K>::axpy(&ndof, &(HPDDM::Wrapper<K>::d__2), f, &(HPDDM::i__1), tmp, &(HPDDM::i__1));
        ndof /= mu;
        HPDDM::underlying_type<K>* nrmAx = nrmb + mu;
        for(unsigned short nu = 0; nu < mu; ++nu) {
            nrmAx[nu] = HPDDM::Blas<K>::nrm2(&ndof, tmp + nu * ndof, &(HPDDM::i__1));
            if(nu == 0)
                std::cout << " --- error = ";
            else
                std::cout << "             ";
            std::cout << std::scientific << nrmAx[nu] << " / " << nrmb[nu];
            if(mu > 1)
                std::cout << " (rhs #" << nu + 1 << ")";
            std::cout << std::endl;
            if(nrmAx[nu] / nrmb[nu] > (std::is_same<double, HPDDM::underlying_type<K>>::value ? 1.0e-6 : 1.0e-2))
                status = 1;
        }
        if(it > 75)
            status = 1;
        delete [] tmp;
        delete [] nrmb;
        delete Mat;
    }
    delete [] d;

    if(opt.set("schwarz_coarse_correction") && opt["geneo_nu"] > 0)
        delete MatNeumann;
    delete [] sol;
    delete [] f;
    MPI_Finalize();
    return status;
}
