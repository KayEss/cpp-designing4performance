add_executable(memory memory.cpp)
target_compile_features(memory PRIVATE cxx_std_17)
install(TARGETS memory EXPORT memory RUNTIME DESTINATION bin)


add_executable(writes writes.cpp)
target_compile_features(writes PRIVATE cxx_std_17)
install(TARGETS writes EXPORT writes RUNTIME DESTINATION bin)


add_library(j STATIC json.cpp)
target_link_libraries(j fost-core)
target_compile_features(j PUBLIC cxx_std_17)

# Most obvious implementation
add_executable(select.endl select.endl.cpp)
target_link_libraries(select.endl j fost-postgres)
install(TARGETS select.endl EXPORT select.endl RUNTIME DESTINATION bin)

# Fix the std::endl bit, and see only a tiny improvement
add_executable(select.simple select.simple.cpp)
target_link_libraries(select.simple j fost-postgres)
install(TARGETS select.simple EXPORT select.simple RUNTIME DESTINATION bin)

# Turn off buffering
add_executable(select.unbuffered select.unbuffered.cpp)
target_link_libraries(select.unbuffered j fost-postgres)
install(TARGETS select.unbuffered EXPORT select.unbuffered RUNTIME DESTINATION bin)

# Use a string buffer
add_executable(select.buffer select.buffer.cpp)
target_link_libraries(select.buffer j fost-postgres)
install(TARGETS select.buffer EXPORT select.buffer RUNTIME DESTINATION bin)

# # Use std::to_chars
# add_executable(select.to_chars select.to_chars.cpp)
# target_link_libraries(select.to_chars j fost-postgres)
# install(TARGETS select.to_chars EXPORT select.to_chars RUNTIME DESTINATION bin)

# Use pgasio
add_executable(select.pgasio select.pgasio.cpp)
target_link_libraries(select.pgasio j f5-pgasio)
install(TARGETS select.pgasio EXPORT select.pgasio RUNTIME DESTINATION bin)
