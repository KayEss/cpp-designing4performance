add_executable(memory memory.cpp)
target_compile_features(memory PRIVATE cxx_std_17)
install(TARGETS memory EXPORT memory RUNTIME DESTINATION bin)


add_executable(writes writes.cpp)
target_compile_features(writes PRIVATE cxx_std_17)
install(TARGETS writes EXPORT writes RUNTIME DESTINATION bin)


add_library(j STATIC json.cpp)
target_link_libraries(j fost-core)
target_compile_features(j PUBLIC cxx_std_17)

add_executable(select.endl select.endl.cpp)
target_link_libraries(select.endl j fost-postgres)
install(TARGETS select.endl EXPORT select.endl RUNTIME DESTINATION bin)

add_executable(select select.cpp)
target_link_libraries(select j fost-postgres)
install(TARGETS select EXPORT select RUNTIME DESTINATION bin)

add_executable(select.buffer select.buffer.cpp)
target_link_libraries(select.buffer j fost-postgres)
install(TARGETS select.buffer EXPORT select.buffer RUNTIME DESTINATION bin)

