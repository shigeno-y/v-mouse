// SPDX-License-Identifier: Apache-2.0

#include "temporary_directory.hpp"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

void setup();
void loop();

namespace {
void
push_console()
{
    std::cout << "\x1B[s\x1B[?1049h";
}
void
pop_console()
{
    std::cout << "\x1B[?1049l\x1B[u";
}

void
clear_console()
{
    std::cout << "\x1B[2J\x1B[H";
}

void
show_cursor()
{
    std::cout << "\x1B[?25h";
}

void
hide_cursor()
{
    std::cout << "\x1B[?25l";
}

void
reset_console()
{
    show_cursor();
    std::cout << "\x1B[!P";
}
} // namespace

int
main(int argc, char* argv[])
{
    // console manip
    /*
    {
        push_console();
        hide_cursor();
        std::atexit(reset_console);
        std::atexit(pop_console);
    }
    // */

    usdRemoteViewer::messaging::tempfile::TemporaryDirectory tmp{ {}, {}, {}, false };
    usdRemoteViewer::messaging::tempfile::logfile = std::ofstream{ tmp.tmpfile() };

    setup();

    loop();

    return 0;
}