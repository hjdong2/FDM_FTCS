
#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include "Mesh.h"

inline void writeVTK(Mesh2D& msh, const std::vector<double>& u, int nstep) {
    int nx = msh.getNx(), ny = msh.getNy();
    double dx = msh.getDx(), dy = msh.getDy();

    
    std::ostringstream name;
    name << "out_" << std::setfill('0') << std::setw(4) << nstep << ".vtk";
    std::ofstream out(name.str());
    if (!out.is_open()) {
        std::cerr << "Error: Could not open file " << name.str() << " for writing.\n";
        return;
    }

    out << "# vtk DataFile Version 3.0\n";
    out << "VTK file generated for heat2D\n";
    out << "ASCII\n";
    out << "DATASET STRUCTURED_POINTS\n";
    out << "DIMENSIONS " << nx << " " << ny << " 1\n";
    out << "ORIGIN 0 0 0\n";
    out << "SPACING " << dx << " " << dy << " 1\n";
    out << "POINT_DATA " << nx * ny << '\n';
    out << "SCALARS temperature double 1\n";
    out << "LOOKUP_TABLE default\n";

    out << std::setprecision(10);
    for (int j = 0; j < ny; ++j) {
        for (int i = 0; i < nx; ++i) {
            int idx = msh.getIdx(i, j);
            out << u[idx] << '\n';
        }
    }    
}