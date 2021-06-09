# ***********************************************************************************
# * Project     : WinMediaLib                                                       *
# * Description : A media playback, metadata, recording & broadcast library in C++. *
# * License     : MIT                                                               *
# *                                                                                 *
# * Author      : Hitesh Kumar Saini                                                *
# * Email       : saini123hitesh@gmail.com; alexmercerind@gmail.com                 *
# * GitHub      : https://github.com/alexmercerind                                  *
# ***********************************************************************************


cmake_minimum_required(VERSION 3.15)
set(PROJECT_NAME WinMediaLib)

project(${PROJECT_NAME} LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(NUGET_URL https://dist.nuget.org/win-x86-commandline/latest/nuget.exe)
set(NUGET ${CMAKE_CURRENT_SOURCE_DIR}/nuget.exe)
if (NOT EXISTS ${NUGET})
  file(DOWNLOAD ${NUGET_URL} ${NUGET})
endif()

set(LIBS ${CMAKE_CURRENT_SOURCE_DIR}/external)
if (NOT EXISTS ${LIBS})
add_custom_target(CPP_WINRT_DOWNLOAD ALL)
add_custom_command(
  TARGET CPP_WINRT_DOWNLOAD PRE_BUILD
  COMMAND ${NUGET} install Microsoft.Windows.CppWinRT -Version 2.0.201102.2 -ExcludeVersion -OutputDirectory ${CMAKE_CURRENT_SOURCE_DIR}/external
  DEPENDS ${NUGET}
)
endif()

add_library(${PROJECT_NAME} SHARED
  ${PROJECT_NAME}.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden)
set_target_properties(
  ${PROJECT_NAME} PROPERTIES
  VS_PROJECT_IMPORT ${CMAKE_CURRENT_SOURCE_DIR}/external/Microsoft.Windows.CppWinRT/build/native/Microsoft.Windows.CppWinRT.props
)
target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/external/Microsoft.Windows.CppWinRT/build/native/Microsoft.Windows.CppWinRT.targets
)
