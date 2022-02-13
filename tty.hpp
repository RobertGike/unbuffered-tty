//------------------------------------------------------------------
// TTY class for unbuffer character input
//
// Copyright (c) 2022 Robert I. Gike
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//------------------------------------------------------------------

#include <memory>
#include <vector>
#include <termios.h>
#include <unistd.h>

// control characters
const unsigned char BS  = 0x08;
const unsigned char CAN = 0x18;
const unsigned char CR  = 0x0d;
const unsigned char DEL = 0x7f;
const unsigned char EOT = 0x04;
const unsigned char ESC = 0x1b;
const unsigned char ETX = 0x03;
const unsigned char LF  = 0x0a;
const unsigned char SP  = 0x20;
const unsigned char TAB = 0x09;

// extended keys
const unsigned char KEY_ARROW_DOWN  = 0x81;
const unsigned char KEY_ARROW_LEFT  = 0x82;
const unsigned char KEY_ARROW_RIGHT = 0x83;
const unsigned char KEY_ARROW_UP    = 0x84;
const unsigned char KEY_BACKSPACE   = 0x7f;
const unsigned char KEY_CANCEL      = CAN;
const unsigned char KEY_DELETE      = 0x85;
const unsigned char KEY_END         = 0x86;
const unsigned char KEY_ENTER       = CR;
const unsigned char KEY_HOME        = 0x87;
const unsigned char KEY_INSERT      = 0x88;
const unsigned char KEY_PAGE_DOWN   = 0x89;
const unsigned char KEY_PAGE_UP     = 0x8a;

// function keys
const unsigned char KEY_F1          = 0x91;
const unsigned char KEY_F2          = 0x92;
const unsigned char KEY_F3          = 0x93;
const unsigned char KEY_F4          = 0x94;
const unsigned char KEY_F5          = 0x95;
const unsigned char KEY_F6          = 0x96;
const unsigned char KEY_F7          = 0x97;
const unsigned char KEY_F8          = 0x98;
const unsigned char KEY_F9          = 0x99;
const unsigned char KEY_F10         = 0x9a;
const unsigned char KEY_F11         = 0x9b;
const unsigned char KEY_F12         = 0x9c;

//------------------------------------------------------------------
// Background task base class
//------------------------------------------------------------------
class cBackgroundTask {
public:
    virtual ~cBackgroundTask() { }
    virtual void task() = 0;
};

//------------------------------------------------------------------
// TTY interface class
//------------------------------------------------------------------
class cTTY {
private:
    struct termios old_tio, new_tio; // tty configuration storage
    std::vector<std::shared_ptr<cBackgroundTask>> bkg_tasks; // background tasks
    unsigned int bkg_next_task;

    unsigned char char_buf[16];
    unsigned int  char_count;
    unsigned int  char_next;

    const bool debug_show_keys;

protected:
    void debugDecodeChar(const unsigned char c);
    void debugDisplayTermios(struct termios* tio_ptr);
    void mapEscapeSequences();
    void runBackgroundTask();

public:
    cTTY(const bool debug=false);
    ~cTTY();

    void addBackgroundTask(std::shared_ptr<cBackgroundTask> bt);
    std::string fmthex(const int n, const int w);
    unsigned char getChar();
    int isCharWaiting();

    static const char* const endl() { return "\r\n"; }
};

