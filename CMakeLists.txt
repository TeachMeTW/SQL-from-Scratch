project(barkeshli_s21 CXX)

# CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# ------------------------------------
# place binaries and libraries according to GNU standards
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif()
#. . . . . . . . . . . . . . . . . . . .
include(cmake/googletest.cmake)
fetch_googletest(
    ${PROJECT_SOURCE_DIR}/cmake
    ${PROJECT_BINARY_DIR}/googletest
    )



# ------------------------------------------------------------------------------------------
#                                    DO NOT EDIT 
#                               ANYTHING ABOVE THIS LINE
# ------------------------------------------------------------------------------------------


# ADD_SUBDIRECTORY(googletest)

ADD_EXECUTABLE(main
    main.cpp
)

ADD_EXECUTABLE(basic_test
    _tests/_test_files/basic_test.cpp
)

ADD_EXECUTABLE(testB
    _tests/_test_files/testB.cpp
)

TARGET_LINK_LIBRARIES(basic_test gtest)
TARGET_LINK_LIBRARIES(testB gtest)

