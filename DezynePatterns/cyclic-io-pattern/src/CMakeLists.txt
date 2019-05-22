cmake_minimum_required(VERSION 3.0)

find_package (Threads)
set(CMAKE_CXX_STANDARD 14)


include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/runtime)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/cpp_impl)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dzn_code)

add_executable(DezynePattern main.cpp
  dzn_code/ITrigger.hh
  dzn_code/ITrigger.cc
  dzn_code/IOutput.hh
  dzn_code/IOutput.cc
  dzn_code/InputDezyne.hh
  dzn_code/InputDezyne.cc
  dzn_code/IInput.hh
  dzn_code/IInput.cc
  dzn_code/Controller.hh
  dzn_code/Controller.cc
  cpp_impl/Trigger.hh
  cpp_impl/InputCpp.hh
  runtime/runtime.cc
  runtime/pump.cc
  runtime/dzn/runtime.hh
  runtime/dzn/pump.hh
  runtime/dzn/meta.hh
  runtime/dzn/locator.hh
  runtime/dzn/coroutine.hh
  runtime/dzn/context.hh
  runtime/dzn/container.hh)

target_link_libraries (DezynePattern ${CMAKE_THREAD_LIBS_INIT})