cmake_minimum_required(VERSION 3.16.3)
project(KNNClassifier)

set(CMAKE_CXX_STANDARD 14)

include_directories(..)

add_executable(Server
        Source/main.cpp
        Source/srcKnnClassifier/NPoint.hpp
        Source/srcKnnClassifier/NPoint.cpp
        Source/srcKnnClassifier/Flower.hpp
        Source/srcKnnClassifier/Flower.cpp
        Source/srcKnnClassifier/Classifier.hpp
        Source/srcKnnClassifier/Classifier.cpp
        Source/srcIO/FileConverter.hpp
        Source/srcIO/FileConverter.cpp
        Source/srcDistances/DistancesData.hpp
        Source/srcDistances/DistancesData.cpp
        Source/srcDistances/DistanceCalc.hpp
        Source/srcDistances/DistanceCalc.cpp
        Source/srcDistances/srcDistanceCalculators/ManhattanDistance.hpp
        Source/srcDistances/srcDistanceCalculators/ManhattanDistance.cpp
        Source/srcDistances/srcDistanceCalculators/EuclideanDistance.hpp
        Source/srcDistances/srcDistanceCalculators/EuclideanDistance.cpp
        Source/srcDistances/srcDistanceCalculators/ChebyshevDistance.hpp
        Source/srcDistances/srcDistanceCalculators/ChebyshevDistance.cpp
)
