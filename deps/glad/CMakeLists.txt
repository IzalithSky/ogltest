cmake_minimum_required(VERSION 3.10)

project(glad)

include_directories(include)

add_library(glad src/glad.c)
