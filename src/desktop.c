#include "base.h"
#include "io.h"
#include "func_defs.h"
#include "config.h"
#include "desktop.h"

void new_win(byte x, byte y, byte w, byte h, const byte* title)
{
    plot_line(x, y, x + w, y, 3);
    plot_line(x, y + h, x + w, y + h, 3);
    plot_line(x, y, x, y + h, 3);
    plot_line(x + w, y, x + w, y + h, 3);
    plot_line(x + 1, y + 1, x + w - 1, y + 1, 2);
    plot_line(x + 1, y + h - 1, x + w - 1, y + h - 1, 2);
    plot_line(x + 1, y + 1, x + 1, y + h - 1, 2);
    plot_line(x + w - 1, y + 1, x + w - 1, y + h - 1, 2);
    plot_line(x + 1, y + 17, x + w - 2, y + 17, 2);
    print(title, x + 2, y + 1, 2);
    render(vram1);
}
