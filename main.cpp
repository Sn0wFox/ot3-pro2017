#include <iostream>
#include "src/World.h"
#include "src/Common.h"
#include <omp.h>

using namespace std;
#define SEED1 897685687
#define SEED2 897986875
#define WORLD_SIZE 32

int main(int argc, char * argv[]) {
	int num_steps = 1000;
	int world_size = 32;
	
	if (argc >= 2)
	{
		num_steps = atoi(argv[1]);
	}
	if (argc >= 3)
	{
		world_size = atoi(argv[2]);
	}
	if (argc >= 4)
	{
		omp_set_dynamic(0);
		omp_set_num_threads(atoi(argv[3]));
	}

  printf("Init binding matrix\n");
  Common::init_binding_matrix(897685687);

  printf("Create World\n");
  World* world = new World(world_size, world_size, SEED2, num_steps);

  printf("Initialize environment\n");
  world->init_environment();

  bool test = false;
  if (test) {
    world->test_mutate();
  } else {
    printf("Initialize random population\n");
    world->random_population();

    printf("Run evolution\n");
    world->run_evolution();
  }
}
