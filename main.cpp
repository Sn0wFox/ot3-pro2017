#include <iostream>
#include <chrono>
#include "src/World.h"
#include "src/Common.h"
#include <omp.h>

using namespace std;

// These defined constants may be changed at compile time
#define SEED1 897685687
#define SEED2 897986875
#define WORLD_HEIGHT 16
#define WORLD_WIDTH 32

int main(int argc, char * argv[]) {
	int num_steps = Common::Number_Evolution_Step;
	int world_height = WORLD_HEIGHT;
	int world_width = WORLD_WIDTH;
	
	if (argc >= 2)
	{
		num_steps = atoi(argv[1]);
	}
	if (argc >= 3)
	{
		world_height = atoi(argv[2]);
	}
	if (argc >= 4)
	{
		world_width = atoi(argv[3]);
	}
	if (argc >= 5)
	{
		omp_set_dynamic(0);
		omp_set_num_threads(atoi(argv[4]));
	}


  printf("Init binding matrix\n");
  Common::init_binding_matrix(SEED1);

  printf("Create World\n");
  World* world = new World(world_height, world_width, SEED2, num_steps);

  printf("Initialize environment\n");
  world->init_environment();

	printf("Initialize random population\n");
	world->random_population();

	auto start = std::chrono::high_resolution_clock::now();
	printf("Run evolution\n");
	world->run_evolution();
	auto end = std::chrono::high_resolution_clock::now();
	auto diff = chrono::duration_cast<chrono::milliseconds>(end - start);
	
	std::cerr << "Time taken : " << (float)diff.count() / 1000.0f << std::endl;
}