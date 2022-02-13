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

#include <ctime>
#include <iostream>
#include <iomanip>
#include "tty.hpp"

//--------------------------------------------------------------
cTTY::cTTY(const bool debug) :
    bkg_next_task(0),
    char_count(0),
    char_next(0),
    debug_show_keys(debug)
{
    // configure the console for unbuffered input
    tcgetattr(STDIN_FILENO, &old_tio);  // fetch the current settings
    new_tio = old_tio;                  // keep the old settings for restore

    // disable canonical mode (buffered i/o) and local echo
    new_tio.c_cflag &= ~(CSIZE|PARENB);
    new_tio.c_cflag |= CS8;
    new_tio.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
    new_tio.c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
    new_tio.c_oflag &= ~OPOST;

    // read timeout
    new_tio.c_cc[VMIN]  = 0;
    new_tio.c_cc[VTIME] = 2; // in 1/10 seconds, 0 -> return immediately

    // set the new settings immediately
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

    // dump termios
    if(debug) {
        debugDisplayTermios(&old_tio);
        debugDisplayTermios(&new_tio);
        std::cout << cTTY::endl();
    }
}

//--------------------------------------------------------------
cTTY::~cTTY() {
    // restore the saved settings
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
}

//--------------------------------------------------------------
void cTTY::addBackgroundTask(std::shared_ptr<cBackgroundTask> bt) {
    bkg_tasks.push_back(bt);
}

//--------------------------------------------------------------
void cTTY::debugDecodeChar(const unsigned char c) {
    switch(c) {
    case KEY_ARROW_DOWN:  std::cout << " KEY_ARROW_DOWN"; break;
    case KEY_ARROW_LEFT:  std::cout << " KEY_ARROW_LEFT"; break;
    case KEY_ARROW_RIGHT: std::cout << " KEY_ARROW_RIGHT"; break;
    case KEY_ARROW_UP:    std::cout << " KEY_ARROW_UP"; break;
    case KEY_DELETE:      std::cout << " KEY_DELETE"; break;
    case KEY_END:         std::cout << " KEY_END"; break;
    case KEY_HOME:        std::cout << " KEY_HOME"; break;
    case KEY_INSERT:      std::cout << " KEY_INSERT"; break;
    case KEY_PAGE_DOWN:   std::cout << " KEY_PAGE_DOWN"; break;
    case KEY_PAGE_UP:     std::cout << " KEY_PAGE_UP"; break;
    case KEY_F1:          std::cout << " KEY_F1"; break;
    case KEY_F2:          std::cout << " KEY_F2"; break;
    case KEY_F3:          std::cout << " KEY_F3"; break;
    case KEY_F4:          std::cout << " KEY_F4"; break;
    case KEY_F5:          std::cout << " KEY_F5"; break;
    case KEY_F6:          std::cout << " KEY_F6"; break;
    case KEY_F7:          std::cout << " KEY_F7"; break;
    case KEY_F8:          std::cout << " KEY_F8"; break;
    case KEY_F9:          std::cout << " KEY_F9"; break;
    case KEY_F10:         std::cout << " KEY_F10"; break;
    case KEY_F11:         std::cout << " KEY_F11"; break;
    case KEY_F12:         std::cout << " KEY_F12"; break;
    default:
        if(c < ' ') { std::cout << " ^" << char(c+0x40); }
        else if(c >= DEL) std::cout << ' ' << fmthex(int(c), 2);
        else { std::cout << ' ' << c; }
        break;
    }
}

//--------------------------------------------------------------
void cTTY::debugDisplayTermios(struct termios* tio) {
    std::cout << cTTY::endl()
    << "sizeof(struct termios)=" << sizeof(struct termios)
    << " NCCS=" << NCCS << " VMIN=" << VMIN << " VTIME=" << VTIME << cTTY::endl()
    << "tcflag_t c_iflag;  " << fmthex(tio->c_iflag, 4)      << " /* input mode flags */" << cTTY::endl()
    << "tcflag_t c_oflag;  " << fmthex(tio->c_oflag, 4)      << " /* output mode flags */" << cTTY::endl()
    << "tcflag_t c_cflag;  " << fmthex(tio->c_cflag, 4)      << " /* control mode flags */" << cTTY::endl()
    << "tcflag_t c_lflag;  " << fmthex(tio->c_lflag, 4)      << " /* local mode flags */" << cTTY::endl()
    << "cc_t c_line;       " << fmthex(tio->c_line, 4)       << " /* line discipline */" << cTTY::endl()
    << "cc_t c_cc[VINTR];  " << fmthex(tio->c_cc[VINTR], 4)  << " /* control characters */" << cTTY::endl()
    << "cc_t c_cc[VQUIT];  " << fmthex(tio->c_cc[VQUIT], 4)  << " /* control characters */" << cTTY::endl()
    << "cc_t c_cc[VERASE]; " << fmthex(tio->c_cc[VERASE], 4) << " /* control characters */" << cTTY::endl()
    << "cc_t c_cc[VKILL];  " << fmthex(tio->c_cc[VKILL], 4)  << " /* control characters */" << cTTY::endl()
    << "cc_t c_cc[VEOF];   " << fmthex(tio->c_cc[VEOF], 4)   << " /* control characters */" << cTTY::endl()
    << "cc_t c_cc[VTIME];  " << fmthex(tio->c_cc[VTIME], 4)  << " /* control characters */" << cTTY::endl()
    << "cc_t c_cc[VMIN];   " << fmthex(tio->c_cc[VMIN], 4)   << " /* control characters */" << cTTY::endl()
    << "cc_t c_cc[VSWTC];  " << fmthex(tio->c_cc[VSWTC], 4)  << " /* control characters */" << cTTY::endl()
    << "speed_t c_ispeed;  " << fmthex(tio->c_ispeed, 4)     << " /* input speed */" << cTTY::endl()
    << "speed_t c_ospeed;  " << fmthex(tio->c_ospeed, 4)     << " /* output speed */" << cTTY::endl();
}

//--------------------------------------------------------------
// format hex digits
std::string cTTY::fmthex(const int n, const int w) {
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(w) << std::hex << n;
    return ss.str();
}

//--------------------------------------------------------------
unsigned char cTTY::getChar() {
    if(char_count > char_next) {
        return char_buf[char_next++]; // return buffered chars first
    }
    for(char_count = 0; char_count == 0; ) {
        char_count = isCharWaiting();
        runBackgroundTask();
    }
    char_next = 0;
    if(char_count > 1 && char_buf[0] == ESC) {
        mapEscapeSequences();
    }
    if(debug_show_keys) debugDecodeChar(char_buf[char_next]);
    return char_buf[char_next++];
}

//--------------------------------------------------------------
// returns: >0 -> number of characters waiting
//          =0 -> no characters waiting
//          <0 -> error
int cTTY::isCharWaiting() {
    char_buf[3] = 0; // for 3 character escape sequence compare
    int count = read(STDIN_FILENO, char_buf, sizeof(char_buf));
    if(count > 0) {
        return count;
    }
    else if(count < 0) {
        std::stringstream ss;
        ss << "cTTY() errno=" << errno;
        throw ss.str().c_str();
    }
    else {
        //std::cout << " [timeout] " << std::flush;
        return 0;
    }
}

//--------------------------------------------------------------
// Keys F1 F10 F11 are captured by the Xfce Terminal Emulator
// in my setup so translations are not available
void cTTY::mapEscapeSequences() {
    uint8_t c = 0;
    uint32_t seq = *(uint32_t*)char_buf;
    if(debug_show_keys) {
        std::cout << ' ' << fmthex(int(seq), 8)
                  << ' ' << fmthex(int(char_buf[0]), 2)
                  << ' ' << fmthex(int(char_buf[1]), 2)
                  << ' ' << fmthex(int(char_buf[2]), 2)
                  << ' ' << fmthex(int(char_buf[3]), 2)
                  << cTTY::endl();
    }

    // 3 character sequences
         if(seq == 0x00425b1b) c = KEY_ARROW_DOWN;  // ESC [ B
    else if(seq == 0x00445b1b) c = KEY_ARROW_LEFT;  // ESC [ D
    else if(seq == 0x00435b1b) c = KEY_ARROW_RIGHT; // ESC [ C
    else if(seq == 0x00415b1b) c = KEY_ARROW_UP;    // ESC [ A
    else if(seq == 0x00465b1b) c = KEY_END;         // ESC [ F
    else if(seq == 0x00485b1b) c = KEY_HOME;        // ESC [ H
    else if(seq == 0x00514f1b) c = KEY_F2;          // ESC [ P
    else if(seq == 0x00524f1b) c = KEY_F3;          // ESC [ Q
    else if(seq == 0x00534f1b) c = KEY_F4;          // ESC [ R
    if(c != 0) {
        char_next = 2;
        char_buf[char_next] = c;
        return;
    }

    // 4 character sequences
         if(seq == 0x7e335b1b) c = KEY_DELETE;      // ESC [ 3 ~
    else if(seq == 0x7e325b1b) c = KEY_INSERT;      // ESC [ 2 ~
    else if(seq == 0x7e365b1b) c = KEY_PAGE_DOWN;   // ESC [ 6 ~
    else if(seq == 0x7e355b1b) c = KEY_PAGE_UP;     // ESC [ 5 ~
    if(c != 0) {
        char_next = 3;
        char_buf[char_next] = c;
        return;
    }

    // 5 character sequences
         if(seq == 0x35315b1b) c = KEY_F5;          // ESC [ 1 5 ~
    else if(seq == 0x37315b1b) c = KEY_F6;          // ESC [ 1 7 ~
    else if(seq == 0x38315b1b) c = KEY_F7;          // ESC [ 1 8 ~
    else if(seq == 0x39315b1b) c = KEY_F8;          // ESC [ 1 9 ~
    else if(seq == 0x30325b1b) c = KEY_F9;          // ESC [ 1 0 ~
    else if(seq == 0x34325b1b) c = KEY_F12;         // ESC [ 1 4 ~
    if(c != 0 && char_buf[4] == '~') {
        char_next = 4;
        char_buf[char_next] = c;
        return;
    }

    // not an escape sequence
    return;
}

//--------------------------------------------------------------
// run background tasks
void cTTY::runBackgroundTask() {
    if(bkg_tasks.size() == 0) { return; }
    for(auto ptr : bkg_tasks) {
        ptr->task();
    }
}

//--------------------------------------------------------------
// Test Code
//--------------------------------------------------------------

#ifdef TEST_TTY
const int task_column = 50;
const char* const CSI = "\x1b["; // ESC [

std::string clreol() {
    std::ostringstream ss;
    ss << CSI << "0K";
    return ss.str();
}

std::string curpos(const int row, const int col) {
    std::ostringstream ss;
    ss << CSI << row << ";" << col << "H";
    return ss.str();
}

const char* currestore() { return "\x1b" "8"; }
const char* cursave() { return "\x1b" "7"; }

class cBT1: public cBackgroundTask {
public:
    cBT1() { }
    void task() {
        time_t t = time(NULL);
        struct tm* timeinfo = localtime(&t);
        std::cout << cursave()
                  << curpos(1, task_column)
                  << asctime(timeinfo) << clreol()
                  << currestore() << std::flush;
    }
};

class cBT2: public cBackgroundTask {
    int count;
public:
    cBT2() : count(0) { }
    void task() {
        ++count;
        std::cout << cursave()
                  << curpos(2, task_column) << count << clreol()
                  << currestore() << std::flush;
    }
};

int main(const int argc, const char* const argv[]) {
    std::unique_ptr<cTTY> tty(new cTTY());
    std::shared_ptr<cBT1> bt1(new cBT1); tty->addBackgroundTask(bt1);
    std::shared_ptr<cBT2> bt2(new cBT2); tty->addBackgroundTask(bt2);
    try {
        unsigned char c1, c2;
        while(c1 != '`') {
            c1 = c2 = tty->getChar();
            if(c1==EOT) throw "got EOT ^D";
            if(c1==ETX) throw "got ETX ^C";
            if(c1 <= ' ' || c1 >= DEL) c2 = '.';
            std::cout << " char: " << c2 << " "
                      << tty->fmthex(int(c1), 2) 
                      << cTTY::endl();
        }
    }
    catch(const char* message) {
        std::cout << cTTY::endl()
                  << "Exception! " << message
                  << cTTY::endl();
    }
    return 0;
}
#endif

