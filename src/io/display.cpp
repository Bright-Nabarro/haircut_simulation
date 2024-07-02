#include <print>
#include "display.hpp"
using namespace std;

namespace simulation
{

Display::Display(MainObjManager& objManager,
		CustomerWaitingQue<MainObjManager>& customerQue,
		bool printSimStatus):
	m_objManager { objManager },
	m_customerQue { customerQue },
	m_printSimStatus { printSimStatus }
{ }

void Display::operator()(std::string_view sv)
{
	std::println("{}", sv.data());
	if (m_printSimStatus)
	{
		print_sim_status();
	}
}

void Display::print_sim_status()
{
	//Print Chair
	for (auto itr { m_objManager.cbegin<Chair>() };
		 itr != m_objManager.cend<Chair>();
		 ++itr)
	{
		auto pChair { itr->second };
		if (!pChair->busy())
			println("chair[{}]: empty", pChair->get_id().get_id_number());
		else
			println("chair[{}]: Barber[{}] -> Customer[{}]", pChair->get_id().get_id_number(),
					pChair->get_barber_id().get_id_number(), pChair->get_customer_id().get_id_number());
	}
	//Print Level quesize
	for (const auto& level : s_kLevels)
	{
		println("Level: [{}] queue size: {}", get_level_str(level),
				m_customerQue.get_que_size(level));
	}
}

}	//namespace simulation
