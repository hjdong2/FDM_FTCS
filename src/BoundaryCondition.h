
#pragma once
#include <vector>
#include <stdexcept>
#include "Mesh.h"

// only apply DBC now
struct BoundaryCondition1D {
    double u_left;
    double u_right;

    void apply(std::vector<double>& u) {
        if (u.size() == 1) {
            throw std::invalid_argument("Number of nodes less than 2!");
        }
        u[0] = u_left;
        u[u.size() - 1] = u_right;
    }
};

// apply BCs for 2D
struct BoundaryCondition2D {
    double u_top;
    double u_bottom;
    double u_left;
    double u_right;

    void apply(std::vector<double>& u, Mesh2D& msh) {
        int nx = msh.getNx(), ny = msh.getNy();
        // apply left BC
        for (int j = 0; j < ny; ++j) {
            int idx = msh.getIdx(0, j);
            u[idx] = u_left;
        }

        for (int j = 0; j < ny; ++j) {
            int idx = msh.getIdx(nx - 1, j);
            u[idx] = u_right;
        }

        for (int i = 0; i < nx; ++i) {
            int idx = msh.getIdx(i, 0);
            u[idx] = u_bottom;
        }

        for (int i = 0; i < nx; ++i) {
            int idx = msh.getIdx(i, ny - 1);
            u[idx] = u_top;
        }
    }
};
