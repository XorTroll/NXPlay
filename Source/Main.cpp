#include "UI.h"

int main()
{
    UI::Init();
    while(appletMainLoop())
    {
        UI::Loop();
    }
    UI::Exit();
    return 0;
}