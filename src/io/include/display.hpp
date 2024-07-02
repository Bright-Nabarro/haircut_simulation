#pragma once
#include <string_view>
#include "object_manager.hpp"
#include "customer.hpp"
#include "barber.hpp"
#include "chair.hpp"

namespace simulation
{
//直接输出到控制台
class Display
{
using MainObjManager = ObjectManager<Customer, Barber, Chair>;

public:
	Display(MainObjManager& objManager,
			CustomerWaitingQue<MainObjManager>& customerQue,
			bool printSimStatus = false);
	void operator()(std::string_view sv);
private:
	void print_sim_status();
	inline static const Level s_kLevels[] { Level::BEG, Level::INT, Level::ADV };
	MainObjManager& m_objManager;
	CustomerWaitingQue<MainObjManager>& m_customerQue;
	const bool m_printSimStatus;
};

}	//namespace simulation
