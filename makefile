#------------------------------------------------------------------------------
# Makefile for TTY interface class
#
# Copyright (c) 2022 Robert I. Gike
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#------------------------------------------------------------------------------

SHELL := /bin/bash

#---------------------------------------------
# shell commands
#---------------------------------------------
CDBACK=cd - 2>&1 >/dev/null
GCC=gcc -Wall -Werror -std=gnu11
GPP=g++ -Wall -Werror -std=c++14
LD=ld
LL=ls -al --color
NASM=nasm -f elf64
NASMDEBUG=nasm -f elf64 -F dwarf -g
PYTHON=python3
SQLITE=sqlite3
TAGS=ctags

#---------------------------------------------
# default target: help
#---------------------------------------------
.PHONY: help
help:
	@echo ""
	@echo "TTY Interface Targets:"
	@echo ""
	@echo "edit      - edit source files"
	@echo "run       - run the test program"
	@echo "test_tty  - compile tty test code"
	@echo ""

#---------------------------------------------
# target: edit
#---------------------------------------------
.PHONY: edit
edit:
	@vi tty.cpp tty.hpp makefile

#---------------------------------------------
# target: run
#---------------------------------------------
.PHONY: run
run:
	@./test_tty

#---------------------------------------------
# target: test_tty
#---------------------------------------------
.PHONY: test_tty
test_tty: tty.cpp
	@$(GPP) -I. -D TEST_TTY -o test_tty $^

