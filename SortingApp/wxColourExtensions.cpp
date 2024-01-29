#include "wxColourExtensions.h"

wxColour wxColourFromHex(wxString hex)
{
    unsigned value{};
    char c{};

    std::istringstream iss(hex.ToStdString());

    iss >> c >> std::hex >> value;

    wxByte r = (value & 0xFF0000) >> 16;
    wxByte g = (value & 0xFF00) >> 8;
    wxByte b = value & 0xFF;

    return wxColour(r, g, b);
}

wxString wxColourToHex(wxColour color)
{
    std::ostringstream oss;

    oss << "#" << std::hex << color.Red() << color.Green() << color.Blue();

    return oss.str();
}
