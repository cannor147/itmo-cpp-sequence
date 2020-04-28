cmake_minimum_required(VERSION 3.12)
project(sequence)

set(CMAKE_CXX_STANDARD 17)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -fsanitize=address -D_GLIBCXX_DEBUG -Wextra")


set(StrangeSequence
        sequence.h
        )
set(Tests
        doctest.h
        seq_tests.cpp
        )

add_executable(main main.cpp ${StrangeSequence})
add_executable(tests ${StrangeSequence} ${Tests})