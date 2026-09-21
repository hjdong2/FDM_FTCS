
#pragma once
#include <stdexcept>
#include <vector>
#include <tuple>
#include "Domain.h"

class Mesh1D {
    double dx;
    std::vector<double> x;

    public:
        int nnodes;

        Mesh1D(Domain1D& dom, int nnodes) {
            if (nnodes < 2) {
                throw std::invalid_argument("Number of nodes less than 2!");
	        }
            this->nnodes = nnodes;
            this->x.resize(nnodes);
            dx = dom.length / (nnodes - 1);
            for (int i = 0; i < nnodes; ++i) {
                this->x[i] = i * dx;
            }	
	}

        double getDx(){
            return this->dx;
        }

        double getX(int i){
            return this->x[i];
	}
};

class Mesh2D {
    int nx;
    int ny;
    double dx;
    double dy;
    double lx;
    double ly;

    public:    
    Mesh2D(Domain2D& dom, int nx, int ny) {
        if (nx < 2 || ny < 2) {
            throw std::invalid_argument("Number of nodes less than 2!");
        }

        this->nx = nx;
        this->ny = ny;
        
        this->dx = dom.lx / (nx - 1);
        this->dy = dom.ly / (ny - 1);

        this->lx = dom.lx;
        this->ly = dom.ly;
    }

    int getNx() {
        return nx;
    }

    int getNy() {
        return ny;
    }

    double getDx() {
        return dx;
    }

    double getDy() {
        return dy;
    }

    double getX(int i) {
        return i * dx;
    }

    double getY(int j) {
        return j * dy;
    }

    int getIdx(int i, int j) {
        return i * ny + j;
    }

    int size() {
        return nx * ny;
    }

    double getLx() {
        return lx;
    }

    double getLy() {
        return ly;
    }

};
