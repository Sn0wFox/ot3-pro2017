// ****************************************************************************
//
//          PdC Artificial Evolution - In silico experimental evolution platform
//
// ****************************************************************************
//
// Copyright: See the AUTHORS file provided with the package
// or <https://gforge.inria.fr/projects/pdcbioevol>
// Web: https://gforge.inria.fr/projects/pdcbioevol
// E-mail: See <jonathan.rouzaud-cornabas@inria.fr>
// Original Authors : Jonathan Rouzaud-Cornabas
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// ****************************************************************************

#include "World.h"
#include "DNA.h"
#include "Organism.h"
#include "GraphicDisplay.h"


World::World(int width, int height, uint32_t seed, int num_steps) {
  width_ = width;
  height_ = height;
  time_ = 0;
  this->num_steps = num_steps;

  grid_cell_ = new GridCell*[width*height];

  global_seed_ = seed;
  global_gen_.seed(seed);

  std::uniform_int_distribution<uint32_t> dis(0,UINT32_MAX);

  for (int i = 0; i < width; i++) {
    for (int j=0; j < height; j++) {
      grid_cell_[i*width+j] = new GridCell(this,i,j,dis(global_gen_));
    }
  }

  statfile_best_.open("stats_best.txt");
  statfile_mean_.open("stats_mean.txt");
}

void World::random_population() {
  float fitness = 0.0;
  Organism* org = nullptr;
  DNA* dna = nullptr;
  printf("Searching for a viable organism ");

  long i = 0;
  while (fitness <= 0.0) {
    dna = new DNA(grid_cell_[0]);
    org = new Organism(new DNA(dna));
    org->gridcell_ = grid_cell_[0];
    org->init_organism();
    for (int t = 0; t < Common::Number_Degradation_Step; t++)
      org->compute_protein_concentration();
    org->compute_fitness();
    fitness = org->fitness_;
    if (org->dying_or_not()) {
      fitness = 0;
      delete org;
    }
    printf(".");
    delete dna;
    i++;
  }

  min_fitness_ = org->fitness_;
  max_fitness_ = org->fitness_;

  printf("Found !\nFilling the grid\n");
  for (int i = 0; i < width_; i++) {
    for (int j=0; j < height_; j++) {
      grid_cell_[i*width_+j]->organism_ = new Organism(new DNA(org->dna_));
      grid_cell_[i*width_+j]->organism_->init_organism();
      grid_cell_[i*width_+j]->organism_->gridcell_ = grid_cell_[i*width_+j];
    }
  }

  delete org;
}

void World::init_environment() {
  float env[Common::Metabolic_Error_Precision];
  std::uniform_real_distribution<float> dis(0,1);
  for (int i = 0; i < Common::Metabolic_Error_Precision; i++)
    env[i] = dis(global_gen_);

  for (int i = 0; i < width_; i++) {
    for (int j = 0; j < height_; j++) {
      for (int k = 0; k < Common::Metabolic_Error_Precision; k++) {
        grid_cell_[i * width_ + j]->environment_target[k] = env[k];
      }
    }
  }

}

void World::run_evolution() {
#if WITH_GRAPHICS_CONTEXT
  GraphicDisplay* display = new GraphicDisplay(this);
#endif
  while (time_ < num_steps) {
	// Execute a step
    evolution_step();

	// Count the total amount of living organisms
    int living_one = 0;
    for (int i = 0; i < width_; i++) {
      for (int j = 0; j < height_; j++) {
        if (grid_cell_[i * width_ + j]->organism_ != nullptr) {
          living_one++;
        }
      }
    }

#if WITH_GRAPHICS_CONTEXT
    display->display();
#endif
    
	// Compute stats
    stats();

	// Show results to the user
    if (time_%Common::LOG_STEPS == 0) {
      printf(
          "Evolution at step %d -- Number of Organism %d  (Dead: %d -- Mutant: %d)-- Min Fitness: %f -- Max Fitness: %f\n",
          time_, living_one, death_, new_mutant_, min_fitness_, max_fitness_);
      statfile_best_.flush();
      statfile_mean_.flush();
    }
    time_++;
  }
}

void World::evolution_step() {

  // Reset stats
  death_= 0;
  new_mutant_= 0;
  min_fitness_ = 1;
  max_fitness_ = 0;

  // For each cell in the grid,
  // let the organisms live or die
  step_live_or_die();

  // For each cell in the UPDATED grid,
  // compute the fitness and update the best and the worst (stats)
  step_compute_fitness();

  // For each cell in the NEWLY UPDATED grid,
  // let the best organisms reproduce
  step_reproduce();

  // For each cell of the ONCE AGAIN UPDATED grid,
  // diffuse and degrade proteins
  step_diffuse_degrade_proteins();
}

void World::test_mutate() {

  float fitness = 0.0;
  Organism* org = nullptr;
  DNA* dna = nullptr;
  printf("Searching for a viable organism ");

  long i = 0;
  while (fitness <= 0.0) {
    delete org;
    dna = new DNA(grid_cell_[0]);
    org = new Organism(new DNA(dna));
    org->gridcell_ = grid_cell_[0];
    org->init_organism();
    for (int t = 0; t < Common::Number_Degradation_Step; t++)
      org->compute_protein_concentration();
    org->compute_fitness();
    fitness = org->fitness_;
    printf(".");
    delete dna;
    i++;
  }

  min_fitness_ = org->fitness_;
  max_fitness_ = org->fitness_;
  death_ = 0;

  int better = 0;
  int worse = 0;
  int equal = 0;

  int dna_size_larger = 0;
  int dna_size_equal = 0;
  int dna_size_smaller = 0;

  // TEST MUTATE (1,000,000)
  for (int i = 0; i < 10000;i++) {
    if (i%1000==0) printf("%d\n",i);

    Organism* org_new = new Organism(new DNA(org->dna_));
    org_new->gridcell_ = grid_cell_[0];
    org_new->mutate();
    org_new->init_organism();
    org_new->activate_pump();

    for (int t = 0; t < Common::Number_Degradation_Step; t++)
      org_new->compute_protein_concentration();

    if (org_new->dying_or_not()) {
      death_++;
    }

    org_new->compute_fitness();

    if (org->fitness_ == org_new->fitness_)
      equal++;
    else if (org->fitness_ > org_new->fitness_)
      worse++;
    else if (org->fitness_ < org_new->fitness_)
      better++;

    delete org_new;
  }

  printf("Death %d -- Worse %d -- Better %d -- Equal %d\n",death_,worse,better,equal);
}

void World::stats() {
  Organism* best;
  float best_fitness = 1000;

  float avg_fitness = 0;
  float avg_meta_error = 0;
  float avg_dna_size = 0;
  float avg_protein = 0;
  float avg_protein_fitness = 0;
  float avg_protein_pure_TF = 0;
  float avg_protein_poison = 0;
  float avg_protein_anti_poison = 0;
  float avg_pump = 0;
  float avg_move = 0;
  float avg_nb_rna = 0;
  float avg_network_size = 0;
  float avg_life_duration = 0;
  float avg_move_success = 0;
  float avg_dupli_sucess = 0;
  float nb_indiv=0;

  for (int i = 0; i < width_; i++) {
    for (int j = 0; j < height_; j++) {

      if (grid_cell_[i * width_ + j]->organism_ != nullptr) {
        if (grid_cell_[i * width_ + j]->organism_->fitness_< best_fitness) {
          best = grid_cell_[i * width_ + j]->organism_;
          best_fitness = grid_cell_[i * width_ + j]->organism_->fitness_;
        }

        nb_indiv++;

        avg_fitness+=grid_cell_[i * width_ + j]->organism_->fitness_;
        avg_meta_error+=grid_cell_[i * width_ + j]->organism_->sum_metabolic_error;
        avg_dna_size+=grid_cell_[i * width_ + j]->organism_->dna_->bp_list_.size();
        avg_protein+=grid_cell_[i * width_ + j]->organism_->protein_list_map_.size();
        avg_protein_fitness+=grid_cell_[i * width_ + j]->organism_->protein_fitness_list_.size();
        avg_protein_pure_TF+=grid_cell_[i * width_ + j]->organism_->protein_TF_list_.size();
        avg_protein_poison+=grid_cell_[i * width_ + j]->organism_->protein_poison_list_.size();
        avg_protein_anti_poison+=grid_cell_[i * width_ + j]->organism_->protein_antipoison_list_.size();
        avg_pump+=grid_cell_[i * width_ + j]->organism_->pump_list_.size();
        avg_move+=grid_cell_[i * width_ + j]->organism_->move_list_.size();
        avg_nb_rna+=grid_cell_[i * width_ + j]->organism_->rna_list_.size();
        avg_life_duration+=grid_cell_[i * width_ + j]->organism_->life_duration_;
        avg_move_success+=grid_cell_[i * width_ + j]->organism_->move_success_;
        avg_dupli_sucess+=grid_cell_[i * width_ + j]->organism_->dupli_success_;
      }
    }
  }

  avg_fitness/=nb_indiv;
  avg_meta_error/=nb_indiv;
  avg_dna_size/=nb_indiv;
  avg_protein/=nb_indiv;
  avg_protein_fitness/=nb_indiv;
  avg_protein_pure_TF/=nb_indiv;
  avg_protein_poison/=nb_indiv;
  avg_protein_anti_poison/=nb_indiv;
  avg_pump/=nb_indiv;
  avg_move/=nb_indiv;
  avg_nb_rna/=nb_indiv;
  avg_network_size/=nb_indiv;
  avg_life_duration/=nb_indiv;
  avg_move_success/=nb_indiv;
  avg_dupli_sucess/=nb_indiv;

  statfile_mean_<<time_<<","<<avg_fitness<<","<<
                               avg_meta_error<<","<<
                               avg_dna_size<<","<<
                               avg_protein<<","<<
                               avg_protein_fitness<<","<<
                               avg_protein_pure_TF<<","<<
                               avg_protein_poison<<","<<
                               avg_protein_anti_poison<<","<<
                               avg_pump<<","<<
                               avg_move<<","<<
                               avg_nb_rna<<","<<
                               avg_life_duration<<","<<
                               avg_move_success<<","<<
                               avg_dupli_sucess<<std::endl;

  statfile_best_<<time_<<","<<best->fitness_<<","<<
  best->sum_metabolic_error<<","<<
  best->dna_->bp_list_.size()<<","<<
  best->protein_list_map_.size()<<","<<
  best->protein_fitness_list_.size()<<","<<
  best->protein_TF_list_.size()<<","<<
  best->protein_poison_list_.size()<<","<<
  best->protein_antipoison_list_.size()<<","<<
  best->pump_list_.size()<<","<<
  best->move_list_.size()<<","<<
  best->rna_list_.size()<<","<<
  best->life_duration_<<","<<
  best->move_success_<<","<<
  best->dupli_success_<<std::endl;
}


// -------------- Protected methods


void World::step_live_or_die() {
  // For each cell in the grid
  for (int i = 0; i < width_; i++) {
    for (int j = 0; j < height_; j++) {
      // If there's a living organism
      if (grid_cell_[i * width_ + j]->organism_ != nullptr) {
  
        // Feed it
        grid_cell_[i * width_ + j]->organism_->activate_pump();

        for (int t = 0; t < Common::Number_Degradation_Step; t++) {
          // TODO: that's the critical call; enhance it
          grid_cell_[i * width_ + j]->organism_->compute_protein_concentration();
        }

        // Ensure the organism is healthy
        if (grid_cell_[i * width_ + j]->organism_->dying_or_not()) {
          // Oops, it's not ! It just died, so we remove it
          delete grid_cell_[i * width_ + j]->organism_;
          grid_cell_[i * width_ + j]->organism_ = nullptr;
          death_++;
        }
      }
    }
  }
}

void World::step_compute_fitness() {
  for (int i = 0; i < width_; i++) {
    for (int j = 0; j < height_; j++) {
      if (grid_cell_[i * width_ + j]->organism_ != nullptr) {

        grid_cell_[i * width_ + j]->organism_->compute_fitness();

        max_fitness_ = grid_cell_[i * width_ + j]->organism_->fitness_ > max_fitness_
          ? grid_cell_[i * width_ + j]->organism_->fitness_
          : max_fitness_;

        min_fitness_ = grid_cell_[i * width_ + j]->organism_->fitness_ < min_fitness_
          ? grid_cell_[i * width_ + j]->organism_->fitness_
          : min_fitness_;
      }
    }
  }
}

void World::step_reproduce() {
  for (int i = 0; i < width_; i++) {
    for (int j = 0; j < height_; j++) {

      if (grid_cell_[i * width_ + j]->organism_ == nullptr) {
        Organism* org_n = nullptr;

        for (int x = i - Common::Duplicate_Neighbors_Offset; x <= i + Common::Duplicate_Neighbors_Offset; x++) {
          for (int y = j - Common::Duplicate_Neighbors_Offset; y <= j + Common::Duplicate_Neighbors_Offset; y++) {
            if (x >= 0 && x < width_) {
              if (y >= 0 && y < height_) {
                if (grid_cell_[x * width_ + y]->organism_ != nullptr) {
                  if (org_n != nullptr) {
                    org_n = grid_cell_[x * width_ + y]->organism_->fitness_ < org_n->fitness_
                      ? grid_cell_[x * width_ + y]->organism_
                      : org_n;
                  }
                  else {
                    org_n = grid_cell_[x * width_ + y]->organism_;
                  }
                }
              }
            }
          }
        }

        if (org_n != nullptr) {
          new_mutant_++;
          org_n->dupli_success_++;
          grid_cell_[i * width_ + j]->organism_ = new Organism(new DNA(org_n->dna_));
          grid_cell_[i * width_ + j]->organism_->gridcell_ = grid_cell_[i * width_ + j];
          grid_cell_[i * width_ + j]->organism_->mutate();
          grid_cell_[i * width_ + j]->organism_->init_organism();
        }
      }
    }
  }
}

void World::step_diffuse_degrade_proteins() {
  for (int i = 0; i < width_; i++) {
    for (int j = 0; j < height_; j++) {
      grid_cell_[i * width_ + j]->diffuse_protein();
      grid_cell_[i * width_ + j]->degrade_protein();
    }
  }
}
