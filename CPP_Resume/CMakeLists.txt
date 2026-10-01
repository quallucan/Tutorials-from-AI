cmake_minimum_required(VERSION 3.21)
project(CPP_Resume LANGUAGES CXX)

# Keep every example on the same teaching baseline, without compiler extensions.
if(DEFINED CMAKE_CXX_STANDARD AND NOT CMAKE_CXX_STANDARD EQUAL 20)
    message(FATAL_ERROR "This tutorial uses C++20. Set CMAKE_CXX_STANDARD=20.")
endif()
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)
include(CheckCXXSourceCompiles)
set(CMAKE_REQUIRED_LIBRARIES Threads::Threads)
check_cxx_source_compiles("
    #include <format>
    #include <stop_token>
    #include <thread>
    int main() {
        auto text = std::format(\"{}\", 20);
        std::jthread worker([](std::stop_token) {});
        worker.request_stop();
        return text.empty();
    }
" TUTORIAL_HAS_CXX20_LIBRARY)
unset(CMAKE_REQUIRED_LIBRARIES)
if(NOT TUTORIAL_HAS_CXX20_LIBRARY)
    message(FATAL_ERROR
        "C++20 std::format and std::jthread are required. Upgrade both the compiler "
        "and its standard library; see README.md. C++17 fallback is not supported.")
endif()

include(CTest)
set(TUTORIAL_EXAMPLES
    01_syntax_review/examples/basic_demo.cpp
    01_syntax_review/examples/pointer_demo.cpp
    01_syntax_review/examples/class_demo.cpp
    01_syntax_review/examples/template_demo.cpp
    01_syntax_review/examples/stl_demo.cpp
    01_syntax_review/examples/modern_features_demo.cpp
    01_syntax_review/examples/coroutine_demo.cpp
    02_modern_paradigms/examples/raii_demo.cpp
    02_modern_paradigms/examples/smart_ptr_demo.cpp
    02_modern_paradigms/examples/move_demo.cpp
    02_modern_paradigms/examples/lambda_demo.cpp
    02_modern_paradigms/examples/type_deduction_demo.cpp
    02_modern_paradigms/examples/constexpr_demo.cpp
    02_modern_paradigms/examples/error_demo.cpp
    02_modern_paradigms/examples/thread_demo.cpp
    03_algorithm_design/examples/complexity_demo.cpp
    03_algorithm_design/examples/stl_algo_demo.cpp
    03_algorithm_design/examples/sort_search_demo.cpp
    03_algorithm_design/examples/recursion_demo.cpp
    03_algorithm_design/examples/dp_demo.cpp
    03_algorithm_design/examples/data_struct_demo.cpp
)

foreach(source IN LISTS TUTORIAL_EXAMPLES)
    get_filename_component(example "${source}" NAME_WE)
    add_executable(${example} "${source}")
    target_link_libraries(${example} PRIVATE Threads::Threads)
    if(MSVC)
        target_compile_options(${example} PRIVATE /W4 /permissive- /utf-8 /Zc:__cplusplus)
        # These examples also teach the portable C file APIs.
        target_compile_definitions(${example} PRIVATE _CRT_SECURE_NO_WARNINGS)
    else()
        target_compile_options(${example} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
    if(BUILD_TESTING)
        file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/test-work/${example}")
        add_test(NAME ${example} COMMAND ${example})
        set_tests_properties(${example} PROPERTIES
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/test-work/${example}"
            TIMEOUT 60)
    endif()
endforeach()
