#include "smallsh.h"


static bool fg_only = false;


bool get_fg_only() {
    return fg_only;
}


void toggle_fg_only() {
    fg_only = !fg_only;
}

