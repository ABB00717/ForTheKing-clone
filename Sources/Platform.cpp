#include <Platform.h>

#include <iostream>

#ifdef _WIN32

#include <windows.h>
#include <conio.h>

namespace Platform {

    void sleepMs(int ms) { Sleep(ms); }

    int readKey() { return _getch(); }

    void consoleSize(int& rows, int& cols) {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
            rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
            cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        } else {
            rows = 0;
            cols = 0;
        }
    }

    void moveCursor(int x, int y) {
        COORD coord;
        coord.X = static_cast<SHORT>(x);
        coord.Y = static_cast<SHORT>(y);
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    void clearScreen() { system("cls"); }

    void pause() {
        std::cout << "Press any key to continue . . ." << std::flush;
        _getch();
    }

}

#else // POSIX

#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

namespace Platform {

    void sleepMs(int ms) { usleep(ms * 1000); }

    // Put the terminal into raw mode just long enough to read one key, then
    // restore the previous settings. Keeping the raw state local avoids a
    // global side effect.
    int readKey() {
        struct termios oldTerm;
        struct termios rawTerm;
        tcgetattr(STDIN_FILENO, &oldTerm);
        rawTerm = oldTerm;
        rawTerm.c_lflag &= ~(ICANON | ECHO);
        rawTerm.c_cc[VMIN] = 1;
        rawTerm.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &rawTerm);

        int result = -1;
        unsigned char ch;
        if (read(STDIN_FILENO, &ch, 1) == 1) {
            if (ch == 27) {
                // Could be a lone ESC or the head of an arrow-key sequence
                // (ESC '[' 'A'..'D'). Peek with a short timeout so a bare ESC
                // still returns promptly instead of blocking.
                rawTerm.c_cc[VMIN] = 0;
                rawTerm.c_cc[VTIME] = 1; // 0.1s
                tcsetattr(STDIN_FILENO, TCSANOW, &rawTerm);
                unsigned char seq[2];
                if (read(STDIN_FILENO, &seq[0], 1) == 1 && seq[0] == '[' &&
                    read(STDIN_FILENO, &seq[1], 1) == 1) {
                    switch (seq[1]) {
                        case 'A': result = 72; break; // up
                        case 'B': result = 80; break; // down
                        case 'C': result = 77; break; // right
                        case 'D': result = 75; break; // left
                        default:  result = 27; break;
                    }
                } else {
                    result = 27;
                }
            } else if (ch == '\n') {
                result = 13; // normalise Enter to the CR code KeyBoard expects
            } else {
                result = ch;
            }
        }

        tcsetattr(STDIN_FILENO, TCSANOW, &oldTerm);
        return result;
    }

    void consoleSize(int& rows, int& cols) {
        struct winsize w;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
            rows = w.ws_row;
            cols = w.ws_col;
        } else {
            rows = 0;
            cols = 0;
        }
    }

    // ANSI cursor addressing is 1-based; callers pass 0-based coordinates,
    // so shift by one here.
    void moveCursor(int x, int y) {
        std::cout << "\033[" << (y + 1) << ';' << (x + 1) << 'H' << std::flush;
    }

    void clearScreen() {
        std::cout << "\033[2J\033[H" << std::flush;
    }

    void pause() {
        std::cout << "Press any key to continue . . ." << std::flush;
        readKey();
        std::cout << '\n';
    }

}

#endif
