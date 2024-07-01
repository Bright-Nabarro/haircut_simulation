#pragma once

namespace simulation
{

	template<typename TyManager>
	bool save_barber(const TyManager& objManager);
	template<typename TyManager>
	bool load_barber(TyManager& objManager);
	template<typename TyManager>
	bool save_chair(const TyManager& objManager);
	template<typename TyManager>
	bool load_chair(TyManager& objManager);

#ifdef DEBUG
	struct IoBarA {};
	struct IoBarB {};
	struct IoChrA {};
	struct IoChrB {};
#endif
}	// namespace sim_io
