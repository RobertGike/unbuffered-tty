# Unbuffered tty console input for Linux
This project presents the C++ class cTTY which encapsulates the code needed to configure the Linux console tty for unbuffered (single character) input. The class constructor saves the current tty configuration before configuring the tty for unbuffered input. The class destructor restores the saved configuration to restore normal tty operation on exit. The class also implements a feature where background tasks may be configured to execute in the (blocking) getChar() API call giving the appearance of a simple cooperative task scheduler. Console tty output is provided by the normal library API's (std::cout, printf, etc).

## Installation

## Public API

## Building
Compile the test program with the command: "make test_tty".
The output binary is named "test_tty".

## Test
Start the test program with command "./test_tty" (or "make run" which clears the screen first).

## License and Acknowledgements
- The project code is Copyright Robert I. Gike under the Apache 2.0 license.
