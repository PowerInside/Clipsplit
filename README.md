# Clipsplit
Program to split a clipboard selection (triggered via keyboard shortcuts). Linux only. 

## Requires 

* `xsel` and Linux with X11
*  GCC, kernel headers etc 

## Usage

1. Compile and run.
2. Select a text anywhere by **using** Ctrl + C to copy.
3. Ctrl + V to paste. First paste will be normal. Subsequent pastes will do the magic.
4. Send a SIGINT to this program to stop.

Don't ask me why I made this.
