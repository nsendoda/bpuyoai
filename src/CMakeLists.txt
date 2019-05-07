cmake_minimum_required(VERSION 3.10)

add_library(BpuyoAI STATIC
  kumipuyo.cpp
  field.cpp
  puttype.cpp
  chain.cpp
  state.cpp
  simulator.cpp
  search_until_next.cpp
  fin.cpp
  tomlhelper.cpp
  padsearch.cpp
)
