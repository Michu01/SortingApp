#pragma once

#include <wx/wx.h>
#include <wx/spinctrl.h>

#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <ranges>

#include "SortDisplayPanel.h"
#include "SortingAlgorithms.h"
#include "GradientLoader.h"

using SortingFunction = std::function<std::experimental::generator<bool>(std::vector<int>::iterator, std::vector<int>::iterator)>;

class MainFrame : public wxFrame
{
private:
	const int DefaultDataCount = 128;
	const int MinDataCount = 8;
	const int MaxDataCount = 1024;

	const int DefaultDelay = 10;
	const int MinDelay = 0;
	const int MaxDelay = 100;

	std::shared_ptr<std::vector<int>> data;

	SortDisplayPanel* sortDisplayPanel;

	wxButton* sortButton;
	wxChoice* algorithmChoice;
	wxSpinCtrl* dataCountSpinCtrl;
	wxSpinCtrl* delaySpinCtrl;
	wxChoice* drawTypeChoice;
	wxChoice* gradientChoice;

	wxStopWatch stopWatch;

	std::unique_ptr<std::experimental::generator<bool>> sort;

	std::mt19937 random{ std::random_device{}() };

	std::vector<std::pair<wxString, SortingFunction>> sortVec;
	std::unordered_map<wxString, SortingFunction> sortMap;

	std::vector<std::pair<wxString, std::shared_ptr<DrawData>>> drawTypes;
	std::unordered_map<wxString, std::shared_ptr<DrawData>> drawTypesMap;

	std::vector<std::pair<wxString, std::shared_ptr<Gradient>>> gradients;
	std::unordered_map<wxString, std::shared_ptr<Gradient>> gradientsMap;

public:
	MainFrame();

private:
	void RandomizeData();

	wxPanel* CreateSettingsPanel();

	void OnIdle(wxIdleEvent& event);

	void OnRandomizeButtonClick(wxEvent& event);

	void OnSortButtonClick(wxEvent& event);

	void OnDataCountSpinCtrlSpin(wxSpinEvent& event);

	void OnDataCountSpinCtrlText(wxCommandEvent& event);

	void OnDrawTypeChoice(wxCommandEvent& event);

	void OnGradientChoice(wxCommandEvent& event);
};
