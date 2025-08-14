cmake_minimum_required(VERSION 3.20)
project(KeyShield VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

find_package(YARA QUIET)

file(GLOB SOURCES "src/*.cpp" "src/gui/*.cpp" "src/gui/*.ui")
include_directories(
  include
)
if(YARA_FOUND)
  include_directories(${YARA_INCLUDE_DIRS})
endif()

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Qt6::Core 
    Qt6::Gui 
    Qt6::Widgets
)
if(YARA_FOUND)
  target_link_libraries(${PROJECT_NAME} PRIVATE ${YARA_LIBRARIES})
  target_compile_definitions(${PROJECT_NAME} PRIVATE KS_HAVE_YARA)
endif()

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE user32.lib advapi32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE KS_PLATFORM_WINDOWS)
endif()

if(UNIX AND NOT APPLE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE KS_PLATFORM_LINUX)
endif()
