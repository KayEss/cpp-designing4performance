add_library(s STATIC stringify.cpp)
target_compile_features(s PUBLIC cxx_std_17)

add_executable(slide1 memory.cpp)
add_executable(slide2 writes.cpp)

add_library(parts STATIC
        barrier.cpp
        row.cpp
        select.endl.cpp
        select.buffer.cpp
    )
target_link_libraries(parts s)
