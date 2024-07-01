#include <filesystem>
#include <fstream>
#include <format>
#include <string>
#include <memory>
#include <sstream>
#include <cassert>
#include "io_barchr.hpp"
#include "object_manager.hpp"
#include "customer.hpp"
#include "barber.hpp"
#include "chair.hpp"
#include "factory.hpp"

using namespace std;
namespace fs = std::filesystem;

namespace simulation
{

const char* dir = "data";
const char* b_file = "./data/barber.sim";
const char* c_file = "./data/chair.sim";

//存储位置为相对地址，不设置修改功能
//如果打开文件失败，返回false
template<typename TyManager>
bool save_barber(const TyManager& objManager)
{
	if (!fs::exists(dir))
	{
		if (!fs::create_directory(dir))
			return false;
	}

	ofstream outFile { b_file };
	if (outFile.bad())
		return false;
	for (auto itr { objManager.template cbegin<Barber>() };
		 itr != objManager.template cend<Barber>();
		 ++itr)
	{
		auto [ idRef, pBarber ] { *itr };
		outFile << std::format(
			"{} {} {}\n",
			static_cast<int>(pBarber->get_level()),
			pBarber->get_time_factor(),
			pBarber->get_total_worktime()
		);
	}
	return true;
}

//如果无法读取预期数据，则忽略此行
template<typename TyManager>
bool load_barber(TyManager& objManager)
{
	if (!fs::exists(dir))
	{
		if (!fs::create_directory(dir))
			return false;
	}

	ifstream inFile { b_file };
	string eachLine;
	if (inFile.bad())
		return false;
	
	size_t successCounter {0};
	while(getline(inFile, eachLine))
	{
		istringstream issm { eachLine };
		int levelNum { -1 };
		double timeFactor { 0 };
		double totalTime { -1 };
		issm >> levelNum;
		issm >> timeFactor;
		issm >> totalTime;

		if (levelNum == -1 || timeFactor == 0 || totalTime == -1)
			continue;

		BarberFactory barberFactory { objManager };
		barberFactory.create_barber(static_cast<Level>(levelNum), timeFactor, totalTime);
		successCounter++;
	}

	if (successCounter <= 0)
		return false;

	return true;
}

template<typename TyManager>
bool save_chair(const TyManager& objManager)
{
	if (!fs::exists(dir))
	{
		if (!fs::create_directory(dir))
			return false;
	}

	ofstream outFile { c_file };
	if (outFile.bad())
		return false;
	long size { std::distance(objManager.template cbegin<Chair>(),
			objManager.template cend<Chair>()) };

	assert(size > 0);
	outFile << size;
	return true;
}

template<typename TyManager>
bool load_chair(TyManager& objManager)
{
	if (!fs::exists(dir))
	{
		if (!fs::create_directory(dir))
			return false;
	}

	ifstream inFile { c_file };
	if (inFile.bad())
		return false;

	long size { -1 };
	inFile >> size;
	if ( size <= 0 )
		return false;

	ChairFactory chairFactory{ objManager };
	for (long i {0}; i < size; i++)
	{
		chairFactory.create_chair();
	}
	return true;
}

using MainObjManager = ObjectManager<Customer, Barber, Chair>;

template bool save_barber<MainObjManager>(const MainObjManager&);
template bool load_barber<MainObjManager>(MainObjManager&);
template bool save_chair<MainObjManager>(const MainObjManager&);
template bool load_chair<MainObjManager>(MainObjManager&);


//工厂已经全特化ObjManager为具体类型，不适用其他类型
//#ifdef DEBUG
//using M1 = ObjectManager<Customer, Barber, Chair, IoBarA>;
//using M2 = ObjectManager<Customer, Barber, Chair, IoBarB>;
//using M3 = ObjectManager<Customer, Barber, Chair, IoChrA>;
//using M4 = ObjectManager<Customer, Barber, Chair, IoChrB>;
//
//template bool save_barber<M1>(const M1&);
//template bool load_barber<M1>(M1&);
//
//// 显式实例化模板函数 for M2
//template bool save_barber<M2>(const M2&);
//template bool load_barber<M2>(M2&);
//
//// 显式实例化模板函数 for M3
//template bool save_chair<M3>(const M3&);
//template bool load_chair<M3>(M3&);
//
//// 显式实例化模板函数 for M4
//template bool save_chair<M4>(const M4&);
//template bool load_chair<M4>(M4&);
//#endif

}	//namespace simulation
