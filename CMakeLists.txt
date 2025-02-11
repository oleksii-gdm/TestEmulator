cmake_minimum_required(VERSION 3.10)

project(TestEmulator)

add_executable(PromptProgramm src/prompt_programm.c)

add_executable(EmulatorFork src/emulator_fork.cpp
                            src/utils.hpp)

add_executable(EmulatorSocket src/bonus_emulator_socket.cpp
                              src/utils.hpp)
