add_library(s STATIC stringify.cpp)
target_compile_features(s PUBLIC cxx_std_17)

add_executable(mem memory.cpp)

add_library(parts STATIC
        select.endl.cpp
    )
target_link_libraries(parts s)
