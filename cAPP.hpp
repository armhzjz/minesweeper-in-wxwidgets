#pragma once
#include "wx/wx.h"
#include "cMain.hpp"

class cAPP : public wxApp
{
public:
    cAPP();
    ~cAPP();
    virtual bool OnInit();

private:
    cMain* m_frame1 = nullptr;
};