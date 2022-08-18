# TherminaCorr - an extension of THERMINATOR 2 therm2_hbtfit functionality 

The TherminaCorr package allows for an easier and improved analysis of HBT radii as a function of transverse momentum. It has been created to consolidate all my macros that were used during the writing of my MSc thesis and focuses on femtoscopic correlation.

## Table of Contents
* [General Info](#general-info)
* [Technologies](#technologies)
* [Setup](#setup)

## General Info

The original wersion of THERMINATOR 2 allows for an extraxtion fo a single radii. TherminaCorr is based on the therm2_hbtfit.cxx file and adjusted for the needs of my analysis. It creates a working TGraph with R(k_T) function for a predefined set of k_T values. It includes 1D correlation and most importantly can substitute the original therm2_hbtfit from THERMINATOR 2 ([arXiv: 1102.0273](#https://arxiv.org/abs/1102.0273)).

Moreover, it allows to perform a fit between modeled and experimental HBT results. Current implementation consists of Q2 (mean relative error) and Chi2 (just google it) tests.

Macros for plotting correlation functions, HBT radii dependence, fit results and position - momentum correlation can be found in the `./macros/` folder (some of them may require THERMINATOR source code to work).

## Technologies

* Used OS: Ubuntu 20.04 LTS
* C++ version: 9.4.0
* CERN ROOT version: 6.26/00
* Black Magic (probably)

## Setup

Setup couldn't be more simple. After cloning the repository cd to the working direcory and use make to complie the program:

```
$ cd TherminaCorr/
$ make
```

A use of debug options is possible, the same as in the THERMINATOR 2.