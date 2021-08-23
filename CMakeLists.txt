cmake_minimum_required(VERSION 3.15)
set(PROJECT_NAME libwinmedia)
set(VERSION 0.0.1)
set(CPPWINRT_VERSION 2.0.201102.2)
project(${PROJECT_NAME} LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

if (WIN32)
  message(
    "${PROJECT_NAME} - ${VERSION}\n"
    "-------------------\n"
    "Building on Windows against C++/WinRT ${CPPWINRT_VERSION}\n"
    "C++ Standard: 17"
  )

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
    COMMAND ${NUGET} install Microsoft.Windows.CppWinRT -Version ${CPPWINRT_VERSION} -ExcludeVersion -OutputDirectory ${CMAKE_CURRENT_SOURCE_DIR}/external
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
  if (DEFINED DART_VM)
    message(
      "Enabled Dart VM NativePorts callbacks."
    )
    target_include_directories(
      ${PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/external/dart_api
    )
    add_definitions(/DDART_VM)
  else()
    message(
      "Disabled Dart VM NativePorts callbacks."
    )
  endif()
  target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/external/Microsoft.Windows.CppWinRT/build/native/Microsoft.Windows.CppWinRT.targets
  )
endif()