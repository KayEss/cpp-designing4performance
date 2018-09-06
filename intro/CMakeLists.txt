add_executable(memory memory.cpp)
target_compile_features(memory PRIVATE cxx_std_17)
install(TARGETS memory EXPORT memory RUNTIME DESTINATION bin)


add_executable(writes writes.cpp)
target_compile_features(writes PRIVATE cxx_std_17)
install(TARGETS writes EXPORT writes RUNTIME DESTINATION bin)


add_executable(select select.cpp)
target_compile_features(select PRIVATE cxx_std_17)
target_link_libraries(select fost-postgres)
install(TARGETS select EXPORT select RUNTIME DESTINATION bin)


add_executable(select.buffer select.buffer.cpp)
target_compile_features(select.buffer PRIVATE cxx_std_17)
target_link_libraries(select.buffer fost-postgres)
install(TARGETS select.buffer EXPORT select.buffer RUNTIME DESTINATION bin)

