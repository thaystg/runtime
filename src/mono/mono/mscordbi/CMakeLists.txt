project(mscordbi)

function(addprefix var prefix list)
  set(f)
  foreach(i ${list})
    set(f ${f} ${prefix}/${i})
  endforeach()
  set(${var} ${f} PARENT_SCOPE)
endfunction()

include_directories(
  ${PROJECT_BINARY_DIR}/
  ${PROJECT_BINARY_DIR}/../..
  ${PROJECT_BINARY_DIR}/../../mono/eglib
  ${CMAKE_CURRENT_SOURCE_DIR}/../..
  ${PROJECT_SOURCE_DIR}/../mini/  
  ${PROJECT_SOURCE_DIR}/../
  ${PROJECT_SOURCE_DIR}/../eglib
  ${PROJECT_SOURCE_DIR}/../mscordbi
  ${PROJECT_SOURCE_DIR}/../../../coreclr/inc
  ${PROJECT_SOURCE_DIR}/../../../coreclr/pal/prebuilt/inc)

include(../eglib/CMakeLists.txt)

set(mscorbi_sources_base
    cordb.cpp
    cordb.hpp
    cordb_appdomain.cpp
    cordb_appdomain.hpp
    cordb_assembly.cpp
    cordb_assembly.hpp
    cordb_blocking_obj.cpp
    cordb_blocking_obj.hpp
    cordb_breakpoint.cpp
    cordb_breakpoint.hpp
    cordb_chain.cpp
    cordb_chain.hpp
    cordb_class.cpp
    cordb_class.hpp
    cordb_code.cpp
    cordb_code.hpp
    cordb_frame.cpp
    cordb_frame.hpp
    cordb_function.cpp
    cordb_function.hpp
    cordb_process.cpp
    cordb_process.hpp
    cordb_register.cpp
    cordb_register.hpp
    cordb_stepper.cpp
    cordb_stepper.hpp
    cordb_symbol.cpp
    cordb_symbol.hpp
    cordb_thread.cpp
    cordb_thread.hpp
    cordb_type.cpp
    cordb_type.hpp
    cordb_value.cpp
    cordb_value.hpp
)

if(HOST_WIN32)
set(OS_LIBS bcrypt.lib Mswsock.lib ws2_32.lib psapi.lib version.lib advapi32.lib winmm.lib kernel32.lib)
endif()

set_source_files_properties(${PROJECT_SOURCE_DIR}/../mini/debugger-protocol.c PROPERTIES LANGUAGE CXX)

addprefix(mscorbi_sources ../mscordbi/ "${mscorbi_sources_base}")

add_library(mscordbi SHARED "${eglib_sources};${mscorbi_sources};${PROJECT_SOURCE_DIR}/../mini/debugger-protocol.c;${PROJECT_SOURCE_DIR}/../../../coreclr/pal/prebuilt/idl/xcordebug_i.cpp;${PROJECT_SOURCE_DIR}/../../../coreclr/pal/prebuilt/idl/cordebug_i.cpp")

target_link_libraries(mscordbi ${OS_LIBS})

