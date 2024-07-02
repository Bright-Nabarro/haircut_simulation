#pragma once
#include <vector>
#include "initial.hpp"

int main_loop(MainObjManager& objManager, MainEventManager& eventManager);

std::vector<double> total_revenue(MainObjManager& objManager);
