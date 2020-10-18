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


add_executable(sbs ${sbs_src_files})

file(GLOB sbs_lib_files "lib/*.sbs")
file(COPY ${sbs_lib_files} DESTINATION "lib")

enable_testing()
file(GLOB_RECURSE sbs_test_files "tests/*.sbst")
foreach(X ${sbs_test_files})
	get_filename_component(X_NAME ${X} NAME)
	add_test(NAME ${X_NAME} COMMAND sbs -T ${X})
endforeach()

#add_test(NAME no_rebinding COMMAND sbs -T ../tests/no_rebinding.sbs)
#add_test(NAME id_id COMMAND sbs -T ../tests/id_id.sbst)
#add_test(NAME alpha_equiv COMMAND sbs -T ../tests/alpha_equiv.sbst)
