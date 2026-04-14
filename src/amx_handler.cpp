#include <unordered_map>
#include <mutex>
#include <vector>
#include <memory>

#include "amx.h"


namespace AmxHandler
{
	namespace
	{
		std::unordered_map<AMX*, std::shared_ptr<Amx>> amx_list;
		std::mutex amx_list_lock;
	}


	void LoadAmx(AMX* amx)
	{
		std::lock_guard<std::mutex> guard(amx_list_lock);
		amx_list[amx] = std::make_shared<Amx>(amx);
	}


	void UnloadAmx(AMX* amx)
	{
		std::lock_guard<std::mutex> guard(amx_list_lock);
		amx_list.erase(amx);
	}


	std::shared_ptr<Amx> GetAmx(AMX* amx)
	{
		std::lock_guard<std::mutex> guard(amx_list_lock);

		const auto it = amx_list.find(amx);

		if (it == amx_list.end())
		{
			return nullptr;
		}

		return it->second;
	}


	void ProcessTick()
	{
		std::vector<std::shared_ptr<Amx>> amx_instances;
		amx_instances.reserve(amx_list.size());

		{
			std::lock_guard<std::mutex> guard(amx_list_lock);

			for (const auto& amx : amx_list)
			{
				amx_instances.push_back(amx.second);
			}
		}

		for (const auto& amx : amx_instances)
		{
			amx->processCallbacks();
		}
	}
}
