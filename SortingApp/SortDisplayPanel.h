#pragma once

#include <wx/wx.h>
#include <wx/dcbuffer.h>

#include <vector>
#include <cmath>
#include <variant>

#include "Overloaded.h"
#include "Gradient.h"
#include "wxColourExtensions.h"

struct LinearDrawData;
struct SinDrawData;

using DrawData = std::variant<LinearDrawData, SinDrawData>;

struct LinearDrawData {};

struct SinDrawData 
{
	double frequency = 1.0;
	double offset = -M_PI / 2.0;
};

class SortDisplayPanel : public wxPanel
{
private:
	std::shared_ptr<std::vector<int>> data;
	std::shared_ptr<DrawData> drawData;
	std::shared_ptr<Gradient> gradient;

public:
	SortDisplayPanel(wxWindow* parent, std::shared_ptr<std::vector<int>> data, std::shared_ptr<DrawData> drawData, std::shared_ptr<Gradient> gradient);

	void SetDrawData(std::shared_ptr<DrawData> drawData);

	void SetGradient(std::shared_ptr<Gradient> gradient);

private:
	void OnPaint(wxPaintEvent& event);

	void OnSize(wxSizeEvent& event);

	std::function<double(int)> GetCalculateHFunction() const;
};

