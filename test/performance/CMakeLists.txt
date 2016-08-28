
ADD_EXECUTABLE (test-old-benchmark Benchmark.cpp)

ADD_EXECUTABLE (test-performance single_member.cpp multiple_fixed_handlers.cpp)
TARGET_LINK_LIBRARIES (test-performance hayai_main ${LIB_TIMING})
