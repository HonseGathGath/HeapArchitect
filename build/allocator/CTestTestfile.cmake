# CMake generated Testfile for 
# Source directory: /home/gathman/main/dev/HeapArchitect/allocator
# Build directory: /home/gathman/main/dev/HeapArchitect/build/allocator
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test("test_basic" "/home/gathman/main/dev/HeapArchitect/build/bin/test_basic")
set_tests_properties("test_basic" PROPERTIES  _BACKTRACE_TRIPLES "/home/gathman/main/dev/HeapArchitect/allocator/CMakeLists.txt;37;add_test;/home/gathman/main/dev/HeapArchitect/allocator/CMakeLists.txt;0;")
add_test("test_stress" "/home/gathman/main/dev/HeapArchitect/build/bin/test_stress")
set_tests_properties("test_stress" PROPERTIES  _BACKTRACE_TRIPLES "/home/gathman/main/dev/HeapArchitect/allocator/CMakeLists.txt;41;add_test;/home/gathman/main/dev/HeapArchitect/allocator/CMakeLists.txt;0;")
add_test("test_fragmentation" "/home/gathman/main/dev/HeapArchitect/build/bin/test_fragmentation")
set_tests_properties("test_fragmentation" PROPERTIES  _BACKTRACE_TRIPLES "/home/gathman/main/dev/HeapArchitect/allocator/CMakeLists.txt;45;add_test;/home/gathman/main/dev/HeapArchitect/allocator/CMakeLists.txt;0;")
