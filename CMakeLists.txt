cmake_minimum_required (VERSION 2.6)

project (AsciiTanksServer C)
set(SERVER_SOURCES)
set(SERVER_SOURCES ${SERVER_SOURCES} common/listening.c)
set(SERVER_SOURCES ${SERVER_SOURCES} common/messaging.c)
set(SERVER_SOURCES ${SERVER_SOURCES} server/main.c)

set(LIBS)
set(LIBS ${LIBS} "pthread")
add_executable (AsciiTanksServer ${SERVER_SOURCES})

target_link_libraries (AsciiTanksServer ${LIBS}) 



project (AsciiTanks C)
set(CLIENT_SOURCES)
set(CLIENT_SOURCES ${CLIENT_SOURCES} common/messaging.c)
set(CLIENT_SOURCES ${CLIENT_SOURCES} client/gfx.c)
set(CLIENT_SOURCES ${CLIENT_SOURCES} client/main.c)

set(LIBS)
set(LIBS ${LIBS} "pthread")
add_executable (AsciiTanks ${CLIENT_SOURCES})

target_link_libraries (AsciiTanks ${LIBS}) 
