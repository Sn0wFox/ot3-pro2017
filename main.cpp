#include <iostream>
#include "src/World.h"
#include "src/Common.h"

using namespace std;

// These defined constants may be changed at compile time
#define SEED1 897685687
#define SEED2 897986875
#define WORLD_SIZE 32

int main() {
  printf("Init binding matrix\n");
  Common::init_binding_matrix(SEED1);

  printf("Create World\n");
  World* world = new World(WORLD_SIZE, WORLD_SIZE, SEED2);

  printf("Initialize environment\n");
  world->init_environment();

	printf("Initialize random population\n");
	world->random_population();

	printf("Run evolution\n");
	world->run_evolution();

  // Allows to put a breakpoint easily once everything is done
  getchar();
}