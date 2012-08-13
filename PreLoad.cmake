#check out of source build
get_filename_component(_src_dir_real_path "${CMAKE_SOURCE_DIR}" REALPATH)
get_filename_component(_bin_dir_real_path "${CMAKE_BINARY_DIR}" REALPATH)

if(${_src_dir_real_path} STREQUAL ${_bin_dir_real_path})
	message(FATAL_ERROR "In-source builds are forbidden! Choose separate build directory.")
endif()