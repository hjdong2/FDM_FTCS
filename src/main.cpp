
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <tuple>
#include <functional>
#include <utility>
#include <vector>
#include "Domain.h"
#include "Mesh.h"
#include "BoundaryCondition.h"
#include "writeVTK.h"

constexpr double pi = 3.14159265358979323846;

std::vector<double> makeInitialCondition1D (Mesh1D& msh){
    int nnodes = msh.nnodes;
    std::vector<double> u(nnodes);
    for (int i = 0; i < nnodes; ++i) {
        u[i] = std::sin(pi * msh.getX(i));
    }

    return u;
}

std::vector<double> makeInitialCondition2D (Mesh2D& msh){
    int nx = msh.getNx(), ny = msh.getNy();
    double dx = msh.getDx(), dy = msh.getDy();
    double lx = msh.getLx(), ly = msh.getLy();

    std::vector<double> u(msh.size());
    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            int idx = msh.getIdx(i, j);
            double x = msh.getX(i), y = msh.getY(j);
            // need to think about how to get Lx and Ly
            u[idx] = std::sin(pi * x / lx) * std::sin(pi * y / ly);
        }
    }

    return u;
}

double exactSoln1D (double x, double alpha, double t) {
    return std::exp(-alpha * pi * pi * t) * std::sin(pi * x);
}

double exactSoln2D (double x, double y, double lx, double ly, double alpha, double t) {
    double coeff = pi * pi *(1 / std::pow(lx, 2) + 1 / std::pow(ly, 2));
    return std::exp(-alpha * coeff * t) * std::sin(pi * x / lx) * std::sin(pi * y / ly);
}

int main () {
    /*
    Domain1D dom{1.0, 20.0, 0.01};
    int nnodes = 21;
    Mesh1D msh(dom, nnodes);
    // msh.getDx();
    std::vector<double> u = makeInitialCondition1D(msh); // IG
    BoundaryCondition1D bc = {0.0, 0.0};
    bc.apply(u);

    // std::cout << u[0] << '\n';
    double coeff = 0.75;  // ensure conditional stable < 0.5, try 0.60
    double dt = coeff * msh.getDx() * msh.getDx() / dom.alpha;
    int nsteps = std::ceil(dom.time / dt);
    dt = dom.time / nsteps; // update dt after ceil fcn
    std::vector<double> u_new(u.size());

    for (int nstep = 1; nstep <= nsteps; ++nstep) {
        double t = nstep * dt;
        for (int i = 1; i < nnodes - 1; ++i) {
            u_new[i] = u[i] + coeff * (u[i + 1] - 2 * u[i] + u[i - 1]);
        }
        bc.apply(u_new);
        std::swap(u, u_new);

        if (nstep % 10 == 0) {
            for (int i = 0; i < nnodes; ++i) {
                double u_exact = exactSoln1D(msh.getX(i), dom.alpha, t);
                std::cout << "x= " << msh.getX(i) << " " << "u= " << u[i] << '\n';
                std::cout << "x= " << msh.getX(i) << " " << "u_ext= " << u_exact << '\n';
            }
        }
    }
    */

    Domain2D dom{1.0, 1.0, 5.0, 0.025};
    Mesh2D msh{dom, 41, 31};
    int nx = msh.getNx(), ny = msh.getNy();
    std::cout << "nx= " << nx << " ny= " << ny << '\n';

    double dx = msh.getDx(), dy = msh.getDy();
    std::cout << "dx= " << dx << " dy= " << dy << '\n';

    // int idx0 = msh.getIdx(0, 0);
    // std::cout << "idx0= " << idx0 << '\n';

    // int idx1 = msh.getIdx(0, 2);
    // std::cout << "idx1= " << idx1 << '\n';

    // int idx2 = msh.getIdx(3, 2);
    // std::cout << "idx2= " << idx2 << '\n';

    int sz = msh.size();
    std::cout << "msh size= " << sz << '\n';
    std::vector<double> u = makeInitialCondition2D(msh);
    BoundaryCondition2D bc = {0.0, 0.0, 0.0, 0.0};
    bc.apply(u, msh);

    for (int i = 0; i < ny; ++i) {
        for (int j = 0; j < nx; ++j) {
            int idx = msh.getIdx(j, i);
            std::cout << u[idx] << "        ";
        }
        std::cout << '\n';
    }

    double factor = 0.85; // compute dt to ensure stable
    double dt = factor / dom.alpha / ((1 / dx / dx) + (1 / dy /dy)) / 2;
    int nsteps = std::ceil(dom.time / dt);
    dt = dom.time / nsteps;
    double c1 = dom.alpha * dt / (dx * dx);
    double c2 = dom.alpha * dt / (dy * dy);

    if (c1 + c2 > 0.5) {
        throw std::invalid_argument("Unstable!");
    }

    std::vector<double> u_new(msh.size());

    for (int nstep = 1; nstep <= nsteps; ++nstep) {
        double t = nstep * dt;
        for (int j = 1; j < ny - 1; ++j) {
            for (int i = 1; i < nx - 1; ++i) {
                int idx = msh.getIdx(i, j);
                int idxr = msh.getIdx(i + 1, j);
                int idxl = msh.getIdx(i - 1, j);
                int idxu = msh.getIdx(i, j+ 1);
                int idxd = msh.getIdx(i, j - 1);

                u_new[idx] = u[idx] + c1 * (u[idxr] - 2 * u[idx] + u[idxl]) + 
                             c2 * (u[idxu] - 2 * u[idx] + u[idxd]);
            }
        }
        bc.apply(u_new, msh);
        std::swap(u, u_new);

        if (nstep % 10 == 0) {
            for (int j = 0; j < ny; ++j) {
                for (int i = 0; i < nx; ++i) {
                    double u_exact = exactSoln2D(msh.getX(i), msh.getY(j), msh.getLx(), msh.getLy(), dom.alpha, t);
                    // std::cout << "i= " << i << ", j = " << j << ", u_ext= " << u_exact << '\n';
                    // std::cout << "i= " << i << ", j = " << j << ", u= " << u[msh.getIdx(i, j)] << '\n';
                }
            }
        }
    }

    writeVTK(msh, u, nsteps);

    return 0;
}