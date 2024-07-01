#pragma once
#include "customer.hpp"
#include "barber.hpp"
#include "chair.hpp"
#include "object_manager.hpp"
using MainObjManager = 
	simulation::ObjectManager<simulation::Customer, simulation::Barber, simulation::Chair>;

void load_saved_data(MainObjManager& objManager);

void handle_argument(int argc, char* argv[], MainObjManager& objManager);

void generate_customer(MainObjManager& objManager);
