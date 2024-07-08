#pragma once
#include "initial.hpp"

int main_loop(MainEventManager& eventManager,
		simulation::SimStatistics& stics);

void display_stics(simulation::SimStatistics& stics);

void set_step();
