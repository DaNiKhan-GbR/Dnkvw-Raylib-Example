cmake_minimum_required(VERSION 3.15)
project(DnkvwRaylibExample C)
set(CMAKE_C_STANDARD 11)


# Import Raylib
find_package(raylib 3.0.0 QUIET)
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)

    FetchContent_Declare(
        raylib
        URL https://github.com/raysan5/raylib/archive/master.tar.gz
    )

    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)

        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples

        # build raylib
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()

# Import Dnkvw
find_package(dnkvw 1.0.0 QUIET)
if (NOT dnkvw_FOUND) # If there's none, fetch dnkvw
    if(MSVC)
        set(dnkvw_url "https://github.com/DaNiKhan-GbR/DaNiKhan-V-Window/releases/latest/download/Dnkvw-Windows-MSVC.zip")
    elseif(APPLE)
        set(dnkvw_url "https://github.com/DaNiKhan-GbR/DaNiKhan-V-Window/releases/latest/download/Dnkvw-macOS.zip")
    else()
        message( FATAL_ERROR "There is no precompiled dnkvw library for your system." )
    endif()

    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/dnkvw")
    file(DOWNLOAD "${dnkvw_url}" "${CMAKE_BINARY_DIR}/dnkvw/dnkvw.zip" SHOW_PROGRESS)
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xvf "${CMAKE_BINARY_DIR}/dnkvw/dnkvw.zip" 
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/dnkvw"
    )
    set(dnkvw_DIR "${CMAKE_BINARY_DIR}/dnkvw/lib/cmake/dnkvw" CACHE PATH "" FORCE)
    find_package(dnkvw 1.0.0 REQUIRED)
endif()

add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} raylib)
target_link_libraries(${PROJECT_NAME} dnkvw)
