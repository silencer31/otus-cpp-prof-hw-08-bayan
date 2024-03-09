cmake_minimum_required(VERSION 3.5)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(bayan VERSION ${PROJECT_VERSION})

find_package(Boost COMPONENTS REQUIRED program_options system regex filesystem)

include_directories(${Boost_INCLUDE_DIR})

add_definitions(-DBOOST_ALL_NO_LIB) # Without this string boost filesystem will not compile.

configure_file(version.h.in version.h)

#Main target 
add_executable(bayan main.cpp options_reader.cpp)
add_library(lib_bayan lib.cpp)

set_target_properties(bayan lib_bayan PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(lib_bayan 
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(bayan PRIVATE
    ${Boost_LIBRARIES}
    lib_bayan
)

if (MSVC)
    target_compile_options(bayan PRIVATE
        /W4
    )
    target_compile_options(lib_bayan PRIVATE
        /W4
    )
else ()
    target_compile_options(bayan PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(lib_bayan PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS bayan RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()