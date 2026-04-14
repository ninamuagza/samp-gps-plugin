#pragma once
#include <memory>
#include "common.h"


namespace AmxHandler
{
	void LoadAmx(AMX* amx);
	void UnloadAmx(AMX* amx);
	std::shared_ptr<Amx> GetAmx(AMX* amx);
	void ProcessTick();
};
