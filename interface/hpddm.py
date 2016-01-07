#! /usr/bin/env python
# -*- coding: utf-8 -*-

"""
   This file is part of HPDDM.

   Author(s): Pierre Jolivet <pierre.jolivet@enseeiht.fr>
        Date: 2015-11-06

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
"""

from mpi4py import MPI
import sys
import ctypes
import numpy
if 'linux' in sys.platform:
    lib = ctypes.cdll.LoadLibrary('lib/libhpddm_python.so')
elif sys.platform == 'darwin':
    lib = ctypes.cdll.LoadLibrary('lib/libhpddm_python.dylib')
elif sys.platform == 'win32':
    lib = ctypes.cdll.LoadLibrary('lib/libhpddm_python.dll')

numbering = ctypes.c_char.in_dll(lib, 'numbering')

if MPI._sizeof(MPI.Comm) == ctypes.sizeof(ctypes.c_int):
    MPI_Comm = ctypes.c_int
else:
    MPI_Comm = ctypes.c_void_p

if ctypes.c_ushort.in_dll(lib, 'scalar').value == 0:
    scalar = underlying = ctypes.c_float
elif ctypes.c_ushort.in_dll(lib, 'scalar').value == 1:
    scalar = underlying = ctypes.c_double
elif ctypes.c_ushort.in_dll(lib, 'scalar').value == 2:
    scalar = numpy.complex64
    underlying = ctypes.c_float
else:
    scalar = numpy.complex128
    underlying = ctypes.c_double

class Option(ctypes.Structure):
    pass
optionGet = lib.optionGet
optionGet.restype = ctypes.POINTER(Option)
optionGet.argtypes = None
optionParse = lib.optionParse
optionParse.restype = ctypes.c_int
optionParse.argtypes = [ ctypes.POINTER(Option), ctypes.c_char_p, ctypes.c_bool ]
optionParseInts = lib.optionParseInts
optionParseInts.restype = ctypes.c_int
optionParseInts.argtypes = [ ctypes.POINTER(Option), ctypes.c_char_p, ctypes.c_int, ctypes.POINTER(ctypes.c_char_p), ctypes.POINTER(ctypes.c_char_p) ]
optionParseArgs = lib.optionParseArgs
optionParseArgs.restype = ctypes.c_int
optionParseArgs.argtypes = [ ctypes.POINTER(Option), ctypes.c_char_p, ctypes.c_int, ctypes.POINTER(ctypes.c_char_p), ctypes.POINTER(ctypes.c_char_p) ]
optionSet = lib.optionSet
optionSet.restype = ctypes.c_bool
optionSet.argtypes = [ ctypes.POINTER(Option), ctypes.c_char_p ]
optionRemove = lib.optionRemove
optionRemove.restype = None
optionRemove.argtypes = [ ctypes.POINTER(Option), ctypes.c_char_p ]
optionVal = lib.optionVal
optionVal.restype = ctypes.c_double
optionVal.argtypes = [ ctypes.POINTER(Option), ctypes.c_char_p ]
optionAddr = lib.optionAddr
optionAddr.restype = ctypes.POINTER(ctypes.c_double)
optionAddr.argtypes = [ ctypes.POINTER(Option), ctypes.c_char_p ]
optionApp = lib.optionApp
optionApp.restype = ctypes.c_double
optionApp.argtypes = [ ctypes.POINTER(Option), ctypes.c_char_p ]

class MatrixCSR(ctypes.Structure):
    pass
matrixCSRCreate = lib.matrixCSRCreate
matrixCSRCreate.restype = ctypes.POINTER(MatrixCSR)
matrixCSRCreate.argtypes = [ ctypes.c_int, ctypes.c_int, ctypes.c_int, numpy.ctypeslib.ndpointer(scalar, ndim = 1, flags = 'F_CONTIGUOUS'), numpy.ctypeslib.ndpointer(ctypes.c_int, ndim = 1, flags = 'F_CONTIGUOUS'), numpy.ctypeslib.ndpointer(ctypes.c_int, ndim = 1, flags = 'F_CONTIGUOUS'), ctypes.c_bool ]
matrixCSRDestroy = lib.matrixCSRDestroy
matrixCSRDestroy.restype = None
matrixCSRDestroy.argtypes = [ ctypes.POINTER(ctypes.POINTER(MatrixCSR)) ]
wrapperCsrmm = lib.csrmm
wrapperCsrmm.restype = None
wrapperCsrmm.argtypes = [ ctypes.POINTER(MatrixCSR), numpy.ctypeslib.ndpointer(scalar, flags = 'F_CONTIGUOUS'), numpy.ctypeslib.ndpointer(scalar, flags = 'F_CONTIGUOUS'), ctypes.c_int ]
def csrmv(Mat, x, y):
    try:
        m = x.shape[1]
    except IndexError:
        m = 1
    wrapperCsrmm(Mat, x, y, m)

class Subdomain(ctypes.Structure):
    pass
subdomainNumfact = lib.subdomainNumfact
subdomainNumfact.restype = None
subdomainNumfact.argtypes = [ ctypes.POINTER(ctypes.POINTER(Subdomain)), ctypes.POINTER(MatrixCSR) ]
_subdomainSolve = lib.subdomainSolve
_subdomainSolve.restype = None
def subdomainSolve(S, f, sol):
    try:
        n = ctypes.c_ushort(sol.shape[1])
    except IndexError:
        n = ctypes.c_ushort(1)
    _subdomainSolve(S, f, sol, n)
_subdomainSolve.argtypes = [ ctypes.POINTER(Subdomain), numpy.ctypeslib.ndpointer(scalar, flags = 'F_CONTIGUOUS'), numpy.ctypeslib.ndpointer(scalar, flags = 'F_CONTIGUOUS'), ctypes.c_ushort ]
subdomainDestroy = lib.subdomainDestroy
subdomainDestroy.restype = None
subdomainDestroy.argtypes = [ ctypes.POINTER(ctypes.POINTER(Subdomain)) ]


class Preconditioner(ctypes.Structure):
    pass
initializeCoarseOperator = lib.initializeCoarseOperator
initializeCoarseOperator.restype = None
initializeCoarseOperator.argtypes = [ ctypes.POINTER(Preconditioner), ctypes.c_ushort ]
setVectors = lib.setVectors
setVectors.restype = None
setVectors.argtypes = [ ctypes.POINTER(Preconditioner), ctypes.c_int, numpy.ctypeslib.ndpointer(scalar, ndim = 2, flags = 'F_CONTIGUOUS') ]
getCommunicator = lib.getCommunicator
getCommunicator.restype = ctypes.POINTER(MPI_Comm)
getCommunicator.argtypes = [ ctypes.POINTER(Preconditioner) ]

class Schwarz(ctypes.Structure):
    pass
schwarzCreate = lib.schwarzCreate
schwarzCreate.restype = ctypes.POINTER(Schwarz)
schwarzCreate.argtypes = [ ctypes.POINTER(MatrixCSR), ctypes.py_object, ctypes.py_object ]
schwarzInitialize = lib.schwarzInitialize
schwarzInitialize.restype = None
schwarzInitialize.argtypes = [ ctypes.POINTER(Schwarz), numpy.ctypeslib.ndpointer(underlying, ndim = 1, flags = 'F_CONTIGUOUS') ]
schwarzPreconditioner = lib.schwarzPreconditioner
schwarzPreconditioner.restype = ctypes.POINTER(Preconditioner)
schwarzPreconditioner.argtypes = [ ctypes.POINTER(Schwarz) ]
schwarzMultiplicityScaling = lib.schwarzMultiplicityScaling
schwarzMultiplicityScaling.restype = None
schwarzMultiplicityScaling.argtypes = [ ctypes.POINTER(Schwarz), numpy.ctypeslib.ndpointer(underlying, ndim = 1, flags = 'F_CONTIGUOUS') ]
_schwarzScaledExchange = lib.schwarzScaledExchange
_schwarzScaledExchange.restype = None
_schwarzScaledExchange.argtypes = [ ctypes.POINTER(Schwarz), numpy.ctypeslib.ndpointer(scalar, flags = 'F_CONTIGUOUS'), ctypes.c_ushort ]
def schwarzScaledExchange(A, x):
    try:
        mu = ctypes.c_ushort(x.shape[1])
    except IndexError:
        mu = ctypes.c_ushort(1)
    _schwarzScaledExchange(A, x, mu)
schwarzCallNumfact = lib.schwarzCallNumfact
schwarzCallNumfact.restype = None
schwarzCallNumfact.argtypes = [ ctypes.POINTER(Schwarz) ]
schwarzSolveGEVP = lib.schwarzSolveGEVP
schwarzSolveGEVP.restype = None
schwarzSolveGEVP.argtypes = [ ctypes.POINTER(Schwarz), ctypes.POINTER(MatrixCSR), ctypes.POINTER(ctypes.c_ushort), underlying ]
schwarzBuildCoarseOperator = lib.schwarzBuildCoarseOperator
schwarzBuildCoarseOperator.restype = None
schwarzBuildCoarseOperator.argtypes = [ ctypes.POINTER(Schwarz), MPI_Comm ]
_schwarzComputeError = lib.schwarzComputeError
_schwarzComputeError.restype = None
_schwarzComputeError.argtypes = [ ctypes.POINTER(Schwarz), numpy.ctypeslib.ndpointer(scalar, flags = 'F_CONTIGUOUS'), numpy.ctypeslib.ndpointer(scalar, flags = 'F_CONTIGUOUS'), numpy.ctypeslib.ndpointer(underlying, flags = 'F_CONTIGUOUS'), ctypes.c_ushort ]
def schwarzComputeError(A, sol, f, storage):
    try:
        mu = sol.shape[1]
    except IndexError:
        mu = 1
    _schwarzComputeError(A, sol, f, storage, mu)
schwarzDestroy = lib.schwarzDestroy
schwarzDestroy.restype = None
schwarzDestroy.argtypes = [ ctypes.POINTER(ctypes.POINTER(Schwarz)) ]

CG = lib.CG
CG.restype = ctypes.c_int
CG.argtypes = [ ctypes.POINTER(Schwarz), numpy.ctypeslib.ndpointer(scalar, flags = 'F_CONTIGUOUS'), numpy.ctypeslib.ndpointer(scalar, flags = 'F_CONTIGUOUS'), ctypes.POINTER(MPI_Comm) ]
_GMRES = lib.GMRES
_GMRES.restype = ctypes.c_int
_GMRES.argtypes = [ ctypes.POINTER(Schwarz), numpy.ctypeslib.ndpointer(scalar, flags = 'F_CONTIGUOUS'), numpy.ctypeslib.ndpointer(scalar, flags = 'F_CONTIGUOUS'), ctypes.c_int, ctypes.POINTER(MPI_Comm) ]
def GMRES(A, f, sol, comm):
    try:
        mu = f.shape[1]
    except IndexError:
        mu = 1
    return _GMRES(A, f, sol, mu, comm)
_BGMRES = lib.BGMRES
_BGMRES.restype = ctypes.c_int
_BGMRES.argtypes = [ ctypes.POINTER(Schwarz), numpy.ctypeslib.ndpointer(scalar, flags = 'F_CONTIGUOUS'), numpy.ctypeslib.ndpointer(scalar, flags = 'F_CONTIGUOUS'), ctypes.c_int, ctypes.POINTER(MPI_Comm) ]
def BGMRES(A, f, sol, comm):
    try:
        mu = f.shape[1]
    except IndexError:
        mu = 1
    return _BGMRES(A, f, sol, mu, comm)
