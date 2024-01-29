#pragma once

#include <wx/wx.h>

#include <fstream>
#include <ranges>
#include <vector>

#include "json.hpp"

#include "Gradient.h"
#include "wxColourExtensions.h"

void to_json(nlohmann::json& j, const std::pair<std::string, Gradient>& p);

void from_json(const nlohmann::json& j, std::pair<std::string, Gradient>& p);

class GradientLoader
{
private:
	const std::string Path = "gradients.json";

public:
	std::vector<std::pair<std::string, Gradient>> Load() const;
};

