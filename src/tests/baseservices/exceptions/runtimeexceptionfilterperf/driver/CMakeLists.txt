project(RuntimeExceptionFilterDriver)

if(WIN32)
    include_directories(
        ${CLR_REPO_ROOT_DIR}/src/coreclr/inc
        ${CLR_REPO_ROOT_DIR}/src/coreclr/pal/prebuilt/inc)

    add_executable(RuntimeExceptionFilterDriver
        runtimeexceptionfilterdriver.cpp
        ${CLR_REPO_ROOT_DIR}/src/coreclr/pal/prebuilt/idl/cordebug_i.cpp)

    target_link_libraries(RuntimeExceptionFilterDriver PRIVATE Ole32.lib)
    install(TARGETS RuntimeExceptionFilterDriver DESTINATION bin)
    install(FILES $<TARGET_PDB_FILE:RuntimeExceptionFilterDriver> DESTINATION bin OPTIONAL)
endif()
