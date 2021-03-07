cmake_minimum_required(VERSION 3.10)

project(Mineweeper VERSION 0.1)
configure_file(appconfig.hpp.in appconfig.hpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(wxWidgets REQUIRED)
find_package(Boost REQUIRED)

include(${wxWidgets_USE_FILE})

add_executable(minesweeper cAPP.cpp
                           cMain.cpp
                           )
target_link_libraries(minesweeper
                      ${wxWidgets_LIBRARIES}
                      ${Boost_LIBRARIES}
                      )
target_include_directories(minesweeper PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}"
                           "${Boost_INCLUDE_DIRS}"
                           "${wxWidgets_INCLUDE_DIRS}"
                           )
install(TARGETS minesweeper DESTINATION bin)
