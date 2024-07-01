#pragma once
#include "customer.hpp"
#include "barber.hpp"
#include "chair.hpp"
#include "object_manager.hpp"
#include "event_manager.hpp"
#include "event.hpp"


using MainObjManager = 
	simulation::ObjectManager<simulation::Customer, simulation::Barber, simulation::Chair>;
using MainEventManager = simulation::EventManager<simulation::Event>;
using MainCustWaitingQue = simulation::CustomerWaitingQue<MainObjManager>;
using MainBarberManager = simulation::BarberManager<MainObjManager>;
using MainChairManager = simulation::ChairManager<MainObjManager>;


void load_saved_data(MainObjManager& objManager);

void handle_argument(int argc, char* argv[], MainObjManager& objManager);

void generate_initial_event(MainObjManager& objManager, MainEventManager& eventManager,
		std::function<void(std::string_view sv)> output,
		MainCustWaitingQue& customerWaitingQue, MainBarberManager& barberManager,
		MainChairManager& chairManager);
