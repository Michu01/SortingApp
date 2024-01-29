#include "MainFrame.h"

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "Sorting App")
{
	sortVec = std::vector<std::pair<wxString, SortingFunction>>
	{
		{ "Bubble sort", bubbleSort<std::vector<int>::iterator> },
		{ "Selection sort", selectionSort<std::vector<int>::iterator> },
		{ "Insertion sort", insertionSort<std::vector<int>::iterator> },
		{ "Merge sort", mergeSort<std::vector<int>::iterator> },
		{ "Quick sort", quickSort<std::vector<int>::iterator> },
		{ "Heap sort", heapSort<std::vector<int>::iterator> },
		{ "Radix sort (LSD)", LSD_RadixSort<std::vector<int>::iterator> },
		{ "Hybrid sort (quick + insertion)", hybridSort<std::vector<int>::iterator> }
	};

	drawTypes = std::vector<std::pair<wxString, std::shared_ptr<DrawData>>>
	{
		{ "Linear", std::make_shared<DrawData>(LinearDrawData{}) },
		{ "Sin", std::make_shared<DrawData>(SinDrawData{}) }
	};

	GradientLoader loader;

	gradients = loader.Load() | 
		std::ranges::views::transform([](auto&& p) -> std::pair<wxString, std::shared_ptr<Gradient>> {
			return { p.first, std::make_shared<Gradient>(p.second) };
		}) |
		std::ranges::to<std::vector>();

	data = std::make_shared<std::vector<int>>(DefaultDataCount);

	std::transform(sortVec.cbegin(), sortVec.cend(), std::inserter(sortMap, sortMap.end()), [](const auto& pair) { return pair; });
	std::transform(drawTypes.cbegin(), drawTypes.cend(), std::inserter(drawTypesMap, drawTypesMap.end()), [](const auto& pair) { return pair; });
	std::transform(gradients.cbegin(), gradients.cend(), std::inserter(gradientsMap, gradientsMap.end()), [](const auto& pair) { return pair; });

	SetFont(GetFont().Scale(1.3f));
	SetBackgroundColour(*wxWHITE);

	Bind(wxEVT_IDLE, &MainFrame::OnIdle, this);

	RandomizeData();

	sortDisplayPanel = new SortDisplayPanel(this, data, drawTypes.front().second, gradients.front().second);

	wxPanel* settingsPanel = CreateSettingsPanel();
	
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	sizer->Add(sortDisplayPanel, 1, wxEXPAND | wxLEFT | wxTOP | wxBOTTOM, 5);
	sizer->Add(settingsPanel, 0, wxEXPAND | wxALL, 5);

	SetSizerAndFit(sizer);
}

void MainFrame::RandomizeData()
{
	std::iota(data->begin(), data->end(), 1);
	std::shuffle(data->begin(), data->end(), random);
}

wxPanel* MainFrame::CreateSettingsPanel()
{
	wxPanel* panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 0));

	wxButton* randomizeButton = new wxButton(panel, wxID_ANY, "Randomize");
	randomizeButton->Bind(wxEVT_BUTTON, &MainFrame::OnRandomizeButtonClick, this);

	sortButton = new wxButton(panel, wxID_ANY, "Sort");
	sortButton->Bind(wxEVT_BUTTON, &MainFrame::OnSortButtonClick, this);

	wxStaticText* algorithmLabel = new wxStaticText(panel, wxID_ANY, "Algorithm");

	wxArrayString algorithms;

	std::transform(sortVec.cbegin(), sortVec.cend(), std::back_inserter(algorithms), [](const auto& pair) { return std::get<0>(pair); });

	algorithmChoice = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, algorithms);
	algorithmChoice->SetSelection(0);

	wxStaticText* dataCountLabel = new wxStaticText(panel, wxID_ANY, "Size");

	dataCountSpinCtrl = new wxSpinCtrl(panel);
	dataCountSpinCtrl->SetValue(DefaultDataCount);
	dataCountSpinCtrl->SetMin(MinDataCount);
	dataCountSpinCtrl->SetMax(MaxDataCount);
	dataCountSpinCtrl->Bind(wxEVT_SPINCTRL, &MainFrame::OnDataCountSpinCtrlSpin, this);
	dataCountSpinCtrl->Bind(wxEVT_TEXT, &MainFrame::OnDataCountSpinCtrlText, this);

	wxStaticText* delayLabel = new wxStaticText(panel, wxID_ANY, "Delay [ms]");

	delaySpinCtrl = new wxSpinCtrl(panel);
	delaySpinCtrl->SetValue(DefaultDelay);
	delaySpinCtrl->SetMin(MinDelay);
	delaySpinCtrl->SetMax(MaxDelay);

	wxStaticText* drawTypeLabel = new wxStaticText(panel, wxID_ANY, "Draw type");

	wxArrayString drawTypeNames;

	std::transform(drawTypes.cbegin(), drawTypes.cend(), std::back_inserter(drawTypeNames), [](const auto& pair) { return std::get<0>(pair); });

	drawTypeChoice = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, drawTypeNames);
	drawTypeChoice->SetSelection(0);
	drawTypeChoice->Bind(wxEVT_CHOICE, &MainFrame::OnDrawTypeChoice, this);

	wxStaticText* gradientLabel = new wxStaticText(panel, wxID_ANY, "Color palette");

	wxArrayString gradientNames;

	std::transform(gradients.cbegin(), gradients.cend(), std::back_inserter(gradientNames), [](const auto& pair) { return std::get<0>(pair); });

	gradientChoice = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, gradientNames);
	gradientChoice->SetSelection(0);
	gradientChoice->Bind(wxEVT_CHOICE, &MainFrame::OnGradientChoice, this);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxSizerFlags sizerFlags = wxSizerFlags().Expand().Border(wxBOTTOM, 3);

	sizer->Add(randomizeButton, sizerFlags);
	sizer->Add(sortButton, sizerFlags);
	sizer->Add(algorithmLabel, sizerFlags);
	sizer->Add(algorithmChoice, sizerFlags);
	sizer->Add(dataCountLabel, sizerFlags);
	sizer->Add(dataCountSpinCtrl, sizerFlags);
	sizer->Add(delayLabel, sizerFlags);
	sizer->Add(delaySpinCtrl, sizerFlags);
	sizer->Add(drawTypeLabel, sizerFlags);
	sizer->Add(drawTypeChoice, sizerFlags);
	sizer->Add(gradientLabel, sizerFlags);
	sizer->Add(gradientChoice, sizerFlags);

	panel->SetSizer(sizer);

	return panel;
}

void MainFrame::OnRandomizeButtonClick(wxEvent& event)
{
	RandomizeData();

	sortDisplayPanel->Refresh();
}

void MainFrame::OnSortButtonClick(wxEvent& event)
{
	if (sort)
	{
		sort = {};
		sortButton->SetLabelText("Sort");
		return;
	}

	wxString algorithmName = algorithmChoice->GetStringSelection();
	SortingFunction algorithm = sortMap.at(algorithmName);

	sort = std::make_unique<std::experimental::generator<bool>>(algorithm(data->begin(), data->end()));

	sortButton->SetLabelText("Stop");
}

void MainFrame::OnDataCountSpinCtrlSpin(wxSpinEvent& event)
{
	sort = {};
	sortButton->SetLabelText("Sort");

	data->resize(event.GetValue());
	RandomizeData();

	sortDisplayPanel->Refresh();
}

void MainFrame::OnDataCountSpinCtrlText(wxCommandEvent& event)
{
	sort = {};
	sortButton->SetLabelText("Sort");

	data->resize(dataCountSpinCtrl->GetValue());
	RandomizeData();

	sortDisplayPanel->Refresh();
}

void MainFrame::OnDrawTypeChoice(wxCommandEvent& event)
{
	std::shared_ptr<DrawData> drawData = drawTypesMap.at(event.GetString());

	sortDisplayPanel->SetDrawData(drawData);
}

void MainFrame::OnGradientChoice(wxCommandEvent& event)
{
	std::shared_ptr<Gradient> gradient = gradientsMap.at(event.GetString());

	sortDisplayPanel->SetGradient(gradient);
}

void MainFrame::OnIdle(wxIdleEvent& event)
{
	if (!sort)
	{
		return;
	}

	long elapsed = stopWatch.Time();

	if (elapsed < delaySpinCtrl->GetValue())
	{
		event.RequestMore();
		return;
	}

	stopWatch.Start();

	bool isFinished = true;

	for (auto e : *sort)
	{
		isFinished = false;
		break;
	}

	sortDisplayPanel->Refresh();

	if (isFinished)
	{
		sort = {};
		sortButton->SetLabelText("Sort");
		return;
	}

	event.RequestMore();
}
