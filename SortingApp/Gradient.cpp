#include "Gradient.h"

Gradient::Gradient()
{
}

Gradient::Gradient(std::initializer_list<wxColour> colors) :
	colors(colors)
{
	assert(colors.size() != 0);
}

wxColour Gradient::GetValue(double x) const
{
	if (colors.size() == 1)
	{
		return colors.front();
	}

	size_t i = std::min((size_t)(x * (colors.size() - 1)), colors.size() - 2);

	double sx = x * (colors.size() - 1) - i;

	wxColour color = interpolate(colors[i], colors[i + 1], sx);

	return color;
}

const std::vector<wxColour>& Gradient::GetColors() const
{
	return colors;
}

wxColour interpolate(wxColour left, wxColour right, double x)
{
	auto red = left.Red() * (1.0 - x) + right.Red() * x;
	auto green = left.Green() * (1.0 - x) + right.Green() * x;
	auto blue = left.Blue() * (1.0 - x) + right.Blue() * x;

	return wxColour(red, green, blue);
}