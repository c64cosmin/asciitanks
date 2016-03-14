cmake_minimum_required (VERSION 2.6)

project (AsciiTanksServer C)
set(SERVER_SOURCES)
set(SERVER_SOURCES ${SERVER_SOURCES} common/listening.c)
set(SERVER_SOURCES ${SERVER_SOURCES} common/messaging.c)
set(SERVER_SOURCES ${SERVER_SOURCES} common/game/map.c)
set(SERVER_SOURCES ${SERVER_SOURCES} server/main.c)

set(SERVER_LIBS)
set(SERVER_LIBS ${SERVER_LIBS} "pthread")
set(SERVER_LIBS ${SERVER_LIBS} "m")
add_executable (AsciiTanksServer ${SERVER_SOURCES})

target_link_libraries (AsciiTanksServer ${SERVER_LIBS}) 



project (AsciiTanks C)
set(CLIENT_SOURCES)
set(CLIENT_SOURCES ${CLIENT_SOURCES} common/messaging.c)
set(CLIENT_SOURCES ${CLIENT_SOURCES} common/game/map.c)
set(CLIENT_SOURCES ${CLIENT_SOURCES} client/gfx.c)
set(CLIENT_SOURCES ${CLIENT_SOURCES} client/kbd.c)
set(CLIENT_SOURCES ${CLIENT_SOURCES} client/map_draw.c)
set(CLIENT_SOURCES ${CLIENT_SOURCES} client/main.c)

set(CLIENT_LIBS)
set(CLIENT_LIBS ${CLIENT_LIBS} "pthread")
set(CLIENT_LIBS ${CLIENT_LIBS} "ncurses")
set(CLIENT_LIBS ${CLIENT_LIBS} "m")
add_executable (AsciiTanks ${CLIENT_SOURCES})

target_link_libraries (AsciiTanks ${CLIENT_LIBS}) 
