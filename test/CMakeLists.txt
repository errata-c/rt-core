
find_package(fmt CONFIG REQUIRED)
find_package(SFML CONFIG REQUIRED COMPONENTS system window)

find_package(rt-loader CONFIG REQUIRED COMPONENTS glew)

add_library(test_framework INTERFACE)
target_include_directories(test_framework
INTERFACE
	"${CMAKE_CURRENT_LIST_DIR}/include/"
)
target_link_libraries(test_framework
INTERFACE
	fmt::fmt 
	sfml-system 
	sfml-window 
	rt-core
	rt::loader-glew
)
target_compile_definitions(test_framework INTERFACE RENDER_TOOLS_ERROR_CHECKS)

configure_file("include/config.hpp.in" "${CMAKE_CURRENT_LIST_DIR}/include/config.hpp")

add_subdirectory("basic")

add_subdirectory("framebuffer")
add_subdirectory("textures")

add_executable(integer_test "integer_test.cpp")
target_link_libraries(integer_test PRIVATE test_framework)

add_executable(clone_test "clone_test.cpp")
target_link_libraries(clone_test PRIVATE test_framework)

add_executable(managed_program_test "managed_program_test.cpp")
target_link_libraries(managed_program_test PRIVATE test_framework)

