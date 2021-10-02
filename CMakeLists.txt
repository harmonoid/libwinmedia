cmake_minimum_required(VERSION 3.15)
set(PROJECT_NAME libwinmedia)
set(VERSION 0.0.3)
set(CPPWINRT_VERSION 2.0.210806.1)
project(${PROJECT_NAME} LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
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
  set(CPP_WINRT ${LIBS}/Microsoft.Windows.CppWinRT)
  if (NOT EXISTS ${CPP_WINRT})
    add_custom_target(CPP_WINRT_DOWNLOAD ALL)
    add_custom_command(
      TARGET CPP_WINRT_DOWNLOAD PRE_BUILD
      COMMAND ${NUGET} install Microsoft.Windows.CppWinRT -Version ${CPPWINRT_VERSION} -ExcludeVersion -OutputDirectory ${LIBS}
      DEPENDS ${NUGET}
    )
  endif()

  add_library(${PROJECT_NAME} SHARED
    ${PROJECT_NAME}.cpp
    ${PROJECT_NAME}.rc
  )

  set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_VISIBILITY_PRESET hidden)
  set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    VS_PROJECT_IMPORT ${CPP_WINRT}/build/native/Microsoft.Windows.CppWinRT.props
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
    ${CPP_WINRT}/build/native/Microsoft.Windows.CppWinRT.targets
  )
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  message(
    "${PROJECT_NAME} - ${VERSION}\n"
    "-------------------\n"
    "Building on Linux against GTK 3.0 & WebKit2 4.0\n"
    "C++ Standard: 17"
  )
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(GTK REQUIRED IMPORTED_TARGET gtk+-3.0)
  pkg_check_modules(WebKit REQUIRED IMPORTED_TARGET webkit2gtk-4.0)
  add_library(${PROJECT_NAME} SHARED
    ${PROJECT_NAME}.cpp
  )
  set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME winmedia)
  target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::GTK)
  target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::WebKit)
  if (DEFINED DART_VM)
    message(
      "Enabled Dart VM NativePorts callbacks."
    )
    target_include_directories(
      ${PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/linux
      ${CMAKE_CURRENT_SOURCE_DIR}/external/dart_api
      ${CMAKE_CURRENT_SOURCE_DIR}/external/webview
    )
    add_definitions(-DDART_VM)
  else()
    message(
      "Disabled Dart VM NativePorts callbacks."
    )
  endif()
endif()