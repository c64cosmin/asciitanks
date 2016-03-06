cmake_minimum_required (VERSION 2.6)

project (AsciiTanksServer)
set(SERVER_SOURCES common/listening.c)
set(SERVER_SOURCES ${SERVER_SOURCES} server/main.c)
add_executable (AsciiTanksServer ${SERVER_SOURCES})
