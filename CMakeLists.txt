cmake_minimum_required(VERSION 3.16.3)
project(KNNClassifier)

set(CMAKE_CXX_STANDARD 14)
SET(GCC_COVERAGE_LINK_FLAGS "-lWs2_32")
SET(CMAKE_CXX_FLAGS  "${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${GCC_COVERAGE_LINK_FLAGS}")

include_directories(.)

add_executable(Server
        Server/Source/main.cpp
        Server/Source/srcKnnClassifier/NPoint.hpp
        Server/Source/srcKnnClassifier/NPoint.cpp
        Server/Source/srcKnnClassifier/Flower.hpp
        Server/Source/srcKnnClassifier/Flower.cpp
        Server/Source/srcKnnClassifier/Classifier.hpp
        Server/Source/srcKnnClassifier/Classifier.cpp
        Server/Source/srcIO/FileConverter.hpp
        Server/Source/srcIO/FileConverter.cpp
        Server/Source/srcDistances/DistancesData.hpp
        Server/Source/srcDistances/DistancesData.cpp
        Server/Source/srcDistances/DistanceCalc.hpp
        Server/Source/srcDistances/DistanceCalc.cpp
        Server/Source/srcDistances/srcDistanceCalculators/ManhattanDistance.hpp
        Server/Source/srcDistances/srcDistanceCalculators/ManhattanDistance.cpp
        Server/Source/srcDistances/srcDistanceCalculators/EuclideanDistance.hpp
        Server/Source/srcDistances/srcDistanceCalculators/EuclideanDistance.cpp
        Server/Source/srcDistances/srcDistanceCalculators/ChebyshevDistance.hpp
        Server/Source/srcDistances/srcDistanceCalculators/ChebyshevDistance.cpp
)

add_executable(Client
        Client/clientMain.cpp
)
