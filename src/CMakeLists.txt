# app_core holds all logic that doesn't need to link the final executable's
# main(): the pure business logic plus the HTTP controller. Splitting it out
# like this is what lets the test binaries link the exact same code the real
# server runs, instead of re-declaring or duplicating anything.
#
# This MUST be an OBJECT library, not a STATIC one. Drogon controllers
# self-register via a static initializer in their .cpp file, and nothing
# else in main.cpp ever references ApiController's symbols directly -- so if
# app_core were a plain STATIC archive, the linker would see no reason to
# pull that .o out of it, the static initializer would never run, and every
# route would silently 404 with no error at build or link time. An OBJECT
# library's .o files are always included wholesale in anything that links
# it, which avoids that failure mode entirely.
add_library(app_core OBJECT
    GreetingService.cpp
    controllers/ApiController.cpp
)

target_include_directories(app_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(app_core PUBLIC drogon)
target_compile_features(app_core PUBLIC cxx_std_17)

add_executable(api_server main.cpp)
target_link_libraries(api_server PRIVATE app_core)
