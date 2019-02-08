if (OpenMP_C_FOUND)
    add_executable (HuffmanArchiveParallel Main.c)
    target_link_libraries (HuffmanArchiveParallel HuffmanCoding)
    add_compile_definitions()
    set_target_properties (HuffmanArchiveParallel PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
            COMPILE_FLAGS -fopenmp LINK_FLAGS -fopenmp)
endif ()
