#pragma once

#include <string>
#include <deque>
#include <variant>
#include <memory>

#include "common.h"


class Callback
{
public:
	Callback(std::shared_ptr<Amx> amx, const std::string& function, const char* format, cell* params, unsigned int param_offset);
	void setResult(int value);
	Amx* getAmx() const;
	bool call();

private:
	std::shared_ptr<Amx> amx_;
	std::string function_;
	std::deque<std::variant<int, std::string>> arguments_;
	int result_;
};
