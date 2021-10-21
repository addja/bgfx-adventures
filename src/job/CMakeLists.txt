cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(job LANGUAGES CXX)

add_library(job
    STATIC
     "Job.cpp"
     "Job.h"
     "Queue.cpp"
     "Queue.h"
     "Counter.h")
