cmake_minimum_required(VERSION 3.14)

project(RT_CORE
VERSION 0.4
DESCRIPTION "OpenGL 4.5 helper classes"
LANGUAGES CXX)

include(GNUInstallDirs)
include("cmake/ez-cmake/install_package.cmake")

option(BUILD_TESTS "Build the test executables" ON)
option(SEPARATE_DIRECTORY "Install the package into its own directory" ON)
set(CONFIG_DIR "${CMAKE_INSTALL_DATAROOTDIR}/ez-gui" CACHE STRING "The relative directory to install package config files.")



find_package(fmt CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(rt-loader CONFIG REQUIRED)
find_package(ez-bitflags CONFIG REQUIRED)

# create main render-tools target
add_library(rt-core INTERFACE)

# adds the include directory to the target.
target_include_directories(rt-core 
INTERFACE # Define these as being required interface components for our library, meaning projects depending on this will need these.
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> # Only substituted when installing (Relative directory)
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> # Only substituted when building (Absolute directory)
)

# Love c++ 17
target_compile_features(rt-core INTERFACE cxx_std_17)

# vector maths and opengl support
target_link_libraries(rt-core INTERFACE glm::glm rt::loader ez::bitflags fmt::fmt)

# Quiet certain unnecessary warnings.
target_compile_options(rt-core INTERFACE "$<BUILD_INTERFACE:$<$<CXX_COMPILER_ID:MSVC>:/permissive->>")

set_target_properties(rt-core PROPERTIES EXPORT_NAME "core")

# Build tests, if requested.
if(${BUILD_TESTS})
	add_subdirectory("test")
endif()

if(SEPARATE_DIRECTORY)
	set(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/rt-core)
endif()

install(
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/rt"
	TYPE INCLUDE
    FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
)

# installs the library as well as putting targets into render-tools-targets
install(
    TARGETS rt-core EXPORT rt-core-targets
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}/$<CONFIG>"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}/$<CONFIG>"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/$<CONFIG>"
)

install_package(
	NAME "rt-core"
	EXPORT rt-core-targets
	DESTINATION "${CONFIG_DIR}"
	VERSION ${RT_CORE_VERSION}
	NAMESPACE "rt::"
	COMPATIBILITY SameMajorVersion
	PRECONFIG "${CMAKE_CURRENT_SOURCE_DIR}/cmake/preconfig.cmake"
	ARCH_INDEPENDENT
)

