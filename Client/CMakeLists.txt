cmake_minimum_required(VERSION 3.16.3)
project(KNNClassifier)

set(CMAKE_CXX_STANDARD 14)

include_directories(..)

add_executable(Client
        clientMain.cpp
)