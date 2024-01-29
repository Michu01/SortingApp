#pragma once

#include <wx/wx.h>

wxColour interpolate(wxColour left, wxColour right, double x);

class Gradient
{
private:
	std::vector<wxColour> colors;

public:
	Gradient();

	template<class T>
	Gradient(const T& ts);

	Gradient(std::initializer_list<wxColour> colors);

	wxColour GetValue(double x) const;

	const std::vector<wxColour>& GetColors() const;
};

template<class T>
inline Gradient::Gradient(const T& ts) : colors(ts.cbegin(), ts.cend())
{
}