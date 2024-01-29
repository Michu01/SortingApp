#include "GradientLoader.h"

void to_json(nlohmann::json& j, const std::pair<std::string, Gradient>& p) 
{
    auto colorsHex = p.second.GetColors() |
        std::ranges::views::transform(wxColourToHex) |
        std::ranges::to<std::vector>();

    j = nlohmann::json
    {
        { "name", p.first },
        { "colors", colorsHex }
    };
}

void from_json(const nlohmann::json& j, std::pair<std::string, Gradient>& p)
{
    j["name"].get_to(p.first);

    auto colors = j["colors"].get<std::vector<std::string>>() |
        std::ranges::views::transform(wxColourFromHex) |
        std::ranges::to<std::vector>();

    p.second = Gradient(colors);
}

std::vector<std::pair<std::string, Gradient>> GradientLoader::Load() const
{
    std::ifstream file(Path);

    auto j = nlohmann::json::parse(file);

    auto gradients = j.template get<std::vector<std::pair<std::string, Gradient>>>();

    return gradients;
}
