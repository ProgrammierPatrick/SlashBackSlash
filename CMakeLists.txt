cmake_minimum_required(VERSION 3.10)

project(SlashBackSlash)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# enable warnings
if(MSVC)
	add_compile_options(/W4 /WX)
else()
	add_compile_options(-Wall -Wextra -pedantic)
endif()

file(GLOB_RECURSE sbs_src_files "src/*.cpp" "src/*.h")
list(REMOVE_ITEM sbs_src_files "src/main.cpp")

add_library(sbs_lib ${sbs_src_files})

add_executable(sbs "src/main.cpp")
target_link_libraries(sbs sbs_lib)

file(GLOB sbs_lib_files "lib/*.sbs")
file(COPY ${sbs_lib_files} DESTINATION "lib")

enable_testing()

file(GLOB_RECURSE sbs_unit_test_files "unit_tests/*.cpp")
foreach(Y ${sbs_unit_test_files})
	get_filename_component(Y_NAME ${Y} NAME)
	add_executable(${Y_NAME} ${Y})
	target_link_libraries(${Y_NAME} sbs_lib)
	target_include_directories(${Y_NAME} PRIVATE "src/")
	add_test(NAME ${Y_NAME} COMMAND ${Y_NAME})
endforeach()

file(GLOB_RECURSE sbs_test_files "tests/*.sbst")
foreach(X ${sbs_test_files})
	get_filename_component(X_NAME ${X} NAME)
	add_test(NAME ${X_NAME} COMMAND sbs -T ${X})
endforeach()

