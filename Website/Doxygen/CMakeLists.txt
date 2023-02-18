# https://github.com/llvm/llvm-project/blob/main/llvm/docs/CMakeLists.txt
cmake_minimum_required(VERSION 3.18.4)
project(DoxygenDocs)

find_package(Doxygen REQUIRED dot)

if (DOXYGEN_FOUND)
        set(abs_top_srcdir ${CMAKE_CURRENT_SOURCE_DIR}/../..)
        set(DOT ${DOXYGEN_DOT_EXECUTABLE})
        set(enable_searchengine "NO")
        set(searchengine_url "")
        set(enable_server_based_search "NO")
        set(enable_external_search "NO")
        set(extra_search_mappings "")
        set(llvm_doxygen_generate_qhp "NO")
        set(llvm_doxygen_qch_filename "")
        set(llvm_doxygen_qhp_namespace "")
        set(llvm_doxygen_qhelpgenerator_path "")
        set(llvm_doxygen_qhp_cust_filter_name "")
        set(llvm_doxygen_qhp_cust_filter_attrs "")
        set(DOT_IMAGE_FORMAT "png")

        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doxygen.cfg.in
                       ${CMAKE_CURRENT_BINARY_DIR}/doxygen.cfg @ONLY)

        add_custom_target(Name ALL COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxygen.cfg
                                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()
