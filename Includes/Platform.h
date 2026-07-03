#ifndef PLATFORM_H
#define PLATFORM_H

// Console I/O for Linux and Windows: sleep, key input, cursor movement,
// screen size and clearing. POSIX backs these with termios/ioctl/ANSI
// escapes; Windows falls back to the native console API under _WIN32.
namespace Platform {

    // Sleep for the given number of milliseconds.
    void sleepMs(int ms);

    // Read a single keypress without waiting for Enter and without echo.
    // Arrow keys map to the codes KeyBoard expects (72 up, 80 down, 75 left,
    // 77 right); Enter normalises to 13. Returns -1 for an unrecognised key.
    int readKey();

    // Query the visible console size in character cells.
    void consoleSize(int& rows, int& cols);

    // Move the cursor to a zero-based (x, y) cell.
    void moveCursor(int x, int y);

    // Clear the whole screen.
    void clearScreen();

    // Wait for a keypress before continuing.
    void pause();

}

#endif
