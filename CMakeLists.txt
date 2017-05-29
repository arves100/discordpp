cmake_minimum_required(VERSION 3.6)
project(discordpp)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES  src/Rest.cc discordpp/Rest.hh) # discordpp/discordpp.hh src/discordpp.cc discordpp/bot.hh src/bot.cc
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_include_directories (${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost REQUIRED coroutine filesystem program_options) # system
set(BOOST_INCLUDEDIR ${Boost_INCLUDE_DIRS})

add_subdirectory("lib/nlohmannjson")
add_subdirectory("lib/beast")

MESSAGE(STATUS ${json_INCLUDE_DIRS})
MESSAGE(STATUS "BOOP BOOP BOOP BOOP BOOP BOOP BOOP BOOP BOOP BOOP ")
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${json_INCLUDE_DIRS} "lib/beast/include" "lib/nlohmannjson/src") #SYSTEM PRIVATE
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${Boost_LIBRARIES} pthread ssl crypto)
