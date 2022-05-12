#include "qr_code.h"

int main(void)
{
    Gui gui = Gui();
    while (!gui.WindowShouldClose())
    {
        gui.Draw();
    }

    return 0;
}