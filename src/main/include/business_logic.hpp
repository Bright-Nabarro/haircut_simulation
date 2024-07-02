#pragma once
#include <vector>
#include "initial.hpp"

int main_loop(MainObjManager& objManager, MainEventManager& eventManager);

//income, worktime
std::pair<double, double> statistics(MainObjManager& objManager);
