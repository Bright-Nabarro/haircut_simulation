#pragma once

namespace simulation
{

	template<typename TyManager>
	bool save_barber(const TyManager& objManager);
	bool load_barber();
	template<typename TyManager>
	bool save_chair(const TyManager& objManager);
	bool load_chair();

#ifdef DEBUG
	struct IoBarA {};
	struct IoBarB {};
	struct IoChrA {};
	struct IoChrB {};
#endif
}	// namespace sim_io
