add_executable(memory memory.cpp)
target_compile_features(memory PRIVATE cxx_std_17)
install(TARGETS memory EXPORT memory RUNTIME DESTINATION bin)


add_executable(writes writes.cpp)
target_compile_features(writes PRIVATE cxx_std_17)
install(TARGETS writes EXPORT writes RUNTIME DESTINATION bin)

