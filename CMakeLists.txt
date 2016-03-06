cmake_minimum_required (VERSION 2.6)

project (AsciiTanksServer)
set(SERVER_SOURCES "common/connection.c")
add_executable (AsciiTanksServer ${SERVER_SOURCES})
