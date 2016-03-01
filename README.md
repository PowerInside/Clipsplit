# Clipsplit
"Clipsplit" is a small program to split a clipboard selection (triggered via keyboard shortcuts). Linux only. 

## Requires 

* `xsel` and Linux with X11
*  GCC, GNU Make

## Usage

1. Compile (`make`) and run (`./a.out`) **with** elevated permissions.
2. Select a text anywhere by **using** Ctrl + C to copy.
3. Ctrl + V to paste. First paste will be normal. Subsequent pastes will do the magic.
4. Send a SIGINT or one of those signals to this program to stop.

Don't ask me why I made this.
