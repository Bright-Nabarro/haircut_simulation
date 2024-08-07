#pragma once
#include "initial.hpp"
#include "tracking.hpp"

int main_loop(MainEventManager& eventManager,
		simulation::SimStatistics& stics);

void display_stics(simulation::SimStatistics& stics);

void set_step();
