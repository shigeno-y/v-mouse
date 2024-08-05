// SPDX-License-Identifier: Apache-2.0

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

    std::this_thread::sleep_for(std::chrono::seconds{ 5 });
    setup();

    loop();

    return 0;
}