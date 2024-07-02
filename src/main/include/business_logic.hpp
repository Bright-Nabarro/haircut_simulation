#pragma once
#include "initial.hpp"

int main_loop(MainObjManager& objManager, MainEventManager& eventManager,
		simulation::SimStatistics& stics);

void display_stics(MainObjManager& objManager, simulation::SimStatistics& stics);

void set_step();
