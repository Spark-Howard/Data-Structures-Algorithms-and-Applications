cmake_minimum_required(VERSION 3.31)
project(Data-Structures-Algorithms-and-Applications)

set(CMAKE_CXX_STANDARD 20)

add_executable(HelloWorld HelloWorld.cpp)
add_executable(Sorting Sorting.cpp)
add_executable(LinearList LinearList.h test_of_chain.cpp )
add_executable(Matrix Matrix.cpp)
add_executable(AlternativeMatrix AlternativeMatrix.h test_of_alternativeMatrix.cpp)
add_executable(Stack Stack.cpp)