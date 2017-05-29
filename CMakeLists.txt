cmake_minimum_required(VERSION 3.6)
project(discordpp)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES  src/Rest.cc discordpp/Rest.hh) # discordpp/discordpp.hh src/discordpp.cc discordpp/bot.hh src/bot.cc
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_include_directories (${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/lib/beast/include ${CMAKE_CURRENT_SOURCE_DIR}/lib/nlohmannjson/src)

set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost REQUIRED system) # coroutine filesystem program_options
set(BOOST_INCLUDEDIR ${Boost_INCLUDE_DIRS})

find_package(OpenSSL)


INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR}) #SYSTEM PRIVATE
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} pthread ssl crypto)
