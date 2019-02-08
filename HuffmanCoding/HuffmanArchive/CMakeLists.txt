add_executable (HuffmanArchive Main.c)
target_link_libraries (HuffmanArchive HuffmanCoding)
set_target_properties (HuffmanArchive PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
