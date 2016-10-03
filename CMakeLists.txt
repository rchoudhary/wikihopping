set(CMAKE_BUILD_TYPE "RELEASE")

cmake_minimum_required(VERSION 3.2)
project(WikiHopping)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

add_executable(wikihopping main.cpp get_internal_links.cpp find_path.cpp link_count.cpp)

include_directories(/usr/local/include)
find_library(GUMBO gumbo /usr/local/lib)

target_link_libraries(wikihopping ${GUMBO} curl)
