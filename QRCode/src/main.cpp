#include "main.h"

int main(void)
{
    Gui gui = Gui();
    while (!gui.WindowShouldClose())
    {
        gui.Draw();
    }

    return 0;
}