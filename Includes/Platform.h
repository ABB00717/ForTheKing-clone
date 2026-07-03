#ifndef PLATFORM_H
#define PLATFORM_H

// Thin cross-platform shim for the console operations the game relies on.
// The rest of the code was written against the Win32 console API and conio;
// this hides those behind a small interface so the same source builds on
// Linux. On POSIX the calls map to termios, ioctl and ANSI escape codes; on
// Windows they still fall back to the native console API.
namespace Platform {

    // Sleep for the given number of milliseconds.
    void sleepMs(int ms);

    // Read a single keypress without waiting for Enter and without echo.
    // Arrow keys are folded into the codes the input layer already expects
    // (72 up, 80 down, 75 left, 77 right) and Enter is normalised to 13, so
    // KeyBoard keeps working unchanged across platforms. Returns -1 when the
    // key is not recognised.
    int readKey();

    // Query the visible console size in character cells.
    void consoleSize(int& rows, int& cols);

    // Move the cursor to a zero-based (x, y) cell, matching the old
    // SetConsoleCursorPosition semantics.
    void moveCursor(int x, int y);

    // Clear the whole screen (replaces system("CLS")/system("clear")).
    void clearScreen();

    // Wait for any key, mimicking the old system("Pause").
    void pause();

}

#endif
