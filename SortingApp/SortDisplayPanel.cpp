#include "SortDisplayPanel.h"

SortDisplayPanel::SortDisplayPanel(wxWindow* parent, std::shared_ptr<std::vector<int>> data, std::shared_ptr<DrawData> drawData, std::shared_ptr<Gradient> gradient) :
    wxPanel(parent), data(data), drawData(drawData), gradient(gradient)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);

    Bind(wxEVT_PAINT, &SortDisplayPanel::OnPaint, this);
    Bind(wxEVT_SIZE, &SortDisplayPanel::OnSize, this);
}

void SortDisplayPanel::SetDrawData(std::shared_ptr<DrawData> drawData)
{
    this->drawData = drawData;

    Refresh();
}

void SortDisplayPanel::SetGradient(std::shared_ptr<Gradient> gradient)
{
    this->gradient = gradient;

    Refresh();
}

void SortDisplayPanel::OnSize(wxSizeEvent& event)
{
    Refresh();
}

void SortDisplayPanel::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);

    dc.SetBackground(*wxWHITE_BRUSH); 
    dc.SetPen(*wxTRANSPARENT_PEN);
    
    dc.Clear();

    wxSize sz = GetClientSize();

    double dx = (double)sz.x / data->size();
    double x = 0.0;

    std::function<double(int)> calculateH = GetCalculateHFunction();

    for (int number : *data)
    {
        double h = ceil(calculateH(number) * sz.y);

        wxRect recToDraw(x, sz.y - h, ceil(dx), h);

        wxColour color = gradient->GetValue(h / sz.y);
        wxBrush brush(color);

        dc.SetBrush(brush);
        dc.DrawRectangle(recToDraw);

        x += dx;
    }
}

std::function<double(int)> SortDisplayPanel::GetCalculateHFunction() const
{
    auto linearDraw = [this](const LinearDrawData& drawData) -> std::function<double(int)>
        {
            size_t size = data->size();
            return [size](int number) { return (double)number / size; };
        };

    auto sinDraw = [this](const SinDrawData& drawData) -> std::function<double(int)>
        {
            double scale = (1.0 / data->size()) * 2.0 * drawData.frequency * M_PI;
            double offset = drawData.offset;
            return [scale, offset](int number) { return 0.5 * (sin(number * scale + offset) + 1.0); };
        };

    return std::visit(overloaded{ linearDraw, sinDraw }, *drawData);
}
