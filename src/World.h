//
// Created by arrouan on 28/09/16.
//

#ifndef PDC_EVOL_MODEL_WORLD_H
#define PDC_EVOL_MODEL_WORLD_H



//#include "GridCell.h"

#include <cstdint>
#include <random>
#include <iostream>
#include <fstream>

class GridCell;

class World {
  public:
    World(int width, int height, uint32_t seed, int num_steps);
    ~World() { statfile_best_.close(); statfile_mean_.close(); }

    int   width_;
    int   height_;
    int   time_;
    int   death_;
    int   new_mutant_;
    float min_fitness_, max_fitness_;
	int num_steps;

    uint32_t      global_seed_;
    GridCell**    grid_cell_;
    std::mt19937  global_gen_;
    std::ofstream statfile_best_;
    std::ofstream statfile_mean_;

    void random_population();
    void test_mutate();
    void run_evolution();
    void evolution_step();
    void init_environment();
    void stats();

  protected:
    void step_live_or_die();
    void step_compute_fitness();
    void step_reproduce();
    void step_diffuse_degrade_proteins();
};


#endif //PDC_EVOL_MODEL_WORLD_H
