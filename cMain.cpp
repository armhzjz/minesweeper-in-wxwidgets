#include "cMain.hpp"
#include <boost/range/irange.hpp>


cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Mineweeper on wxWidgets!", wxPoint(30, 30), wxSize(800, 600))
{
    btn = new wxButton*[nFieldWidth * nFieldHeight];
    wxGridSizer *grid = new wxGridSizer(nFieldWidth, nFieldHeight, 0, 0);

    nField = new int[nFieldWidth * nFieldHeight];

    wxFont font(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

    for (auto x : boost::irange(0,nFieldWidth))
        for (auto y : boost::irange(0, nFieldHeight))
        {
            btn[y*nFieldWidth + x] = new wxButton(this, 10000 + (y * nFieldWidth +x));
            btn[y*nFieldWidth + x]->SetFont(font);
            btn[y*nFieldWidth + x]->SetForegroundColour(*wxBLUE);
            btn[y*nFieldWidth + x]->SetBackgroundColour(*wxWHITE);
            grid->Add(btn[y*nFieldWidth + x], 1, wxEXPAND | wxALL);

            // bind this button to an event (in this case they are all binded to the same events)
            btn[y*nFieldWidth + x]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::OnButtonClicked, this);
            // set the default value for each location in the mine field array
            nField[y*nFieldWidth + x] = 0;
        }

    this->SetSizer(grid);
    grid->Layout();
}

cMain::~cMain()
{
    /* I don't want memory leaks */
    delete[]btn;
}

void cMain::OnButtonClicked(wxCommandEvent &evt)
{
    // Get coordinate of button in field array
    int x = (evt.GetId() - 10000) % nFieldWidth;
    int y = (evt.GetId() - 10000) / nFieldWidth;

    // generate the mine field on the very first click of the user
    if (bFirstClick)
    {
        // total number of mines
        int mines = 30;

        while (mines)
        {
            int rx = rand() % nFieldWidth;
            int ry = rand() % nFieldHeight;

            if (nField[ry * nFieldWidth + rx] == 0 && rx != x && ry != y)
            {
                nField[ry * nFieldWidth + rx] = -1;
                mines--;
            }
        }

        // do not generate the mine field again
        bFirstClick = false;
    }

    // disable button, preventing it being pressed again
    btn[y*nFieldWidth + x]->Enable(false);
    btn[y*nFieldWidth + x]->SetBackgroundColour(*wxLIGHT_GREY);

    // check if player hit a mine
    if (nField[y*nFieldWidth + x] == -1)
    {
        btn[y*nFieldWidth + x]->SetBackgroundColour(*wxBLACK);
        btn[y*nFieldWidth + x]->SetForegroundColour(*wxRED);
        btn[y*nFieldWidth + x]->SetLabel("X");

        wxMessageBox("Bomb went off - Game Over !!");

        // reset game
        bFirstClick = true;
        for (auto x : boost::irange(0,nFieldWidth))
            for (auto y : boost::irange(0, nFieldHeight))
            {
                nField[y*nFieldWidth + x] = 0;
                btn[y*nFieldWidth + x]->SetLabel("");
                btn[y*nFieldWidth + x]->Enable(true);
                btn[y*nFieldWidth + x]->SetBackgroundColour(*wxWHITE);
                btn[y*nFieldWidth + x]->SetForegroundColour(*wxBLUE);
            }
    }
    else
    {
        // count neighbouring mines
        int mine_cntr = 0;
        for (auto i : boost::irange(-1, 2))
            for (auto j : boost::irange(-1, 2))
            {
                if (x + i >= 0 && x + i < nFieldWidth && y + j >= 0 && y + j < nFieldHeight)
                    if (nField[(y + j)*nFieldWidth + (x + i)] == -1)
                        mine_cntr++;
            }
        
        // update button's label to show mine count if > 0
        if (mine_cntr > 0)
            btn[y*nFieldWidth + x]->SetLabel(std::to_string(mine_cntr));
    }

    evt.Skip();
}