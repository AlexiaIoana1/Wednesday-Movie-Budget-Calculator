cmake_minimum_required(VERSION 3.23)
project(Tema_Alexia)

set(CMAKE_CXX_STANDARD 14)

include_directories(Source)

add_executable(Tema_Alexia
        Source/Constants.hpp
        Source/CSV.cpp
        Source/CSV.hpp
        Source/Currency.cpp
        Source/Currency.hpp
        Source/Exceptions.hpp
        Source/Instance.cpp
        Source/Instance.hpp
        Source/Person.cpp
        Source/Person.hpp
        Main.cpp)
