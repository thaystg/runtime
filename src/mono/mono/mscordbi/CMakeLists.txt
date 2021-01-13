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
    cordb-appdomain.cpp
    cordb-appdomain.hpp
    cordb-assembly.cpp
    cordb-assembly.hpp
    cordb-blocking-obj.cpp
    cordb-blocking-obj.hpp
    cordb-breakpoint.cpp
    cordb-breakpoint.hpp
    cordb-chain.cpp
    cordb-chain.hpp
    cordb-class.cpp
    cordb-class.hpp
    cordb-code.cpp
    cordb-code.hpp
    cordb-eval.cpp
    cordb-eval.hpp
    cordb-frame.cpp
    cordb-frame.hpp
    cordb-function.cpp
    cordb-function.hpp
    cordb-process.cpp
    cordb-process.hpp
    cordb-register.cpp
    cordb-register.hpp
    cordb-stepper.cpp
    cordb-stepper.hpp
    cordb-symbol.cpp
    cordb-symbol.hpp
    cordb-thread.cpp
    cordb-thread.hpp
    cordb-type.cpp
    cordb-type.hpp
    cordb-value.cpp
    cordb-value.hpp
)

if(HOST_WIN32)
set(OS_LIBS bcrypt.lib Mswsock.lib ws2_32.lib psapi.lib version.lib advapi32.lib winmm.lib kernel32.lib)
endif()

set_source_files_properties(${PROJECT_SOURCE_DIR}/../mini/debugger-protocol.c PROPERTIES LANGUAGE CXX)

addprefix(mscorbi_sources ../mscordbi/ "${mscorbi_sources_base}")

add_library(mscordbi SHARED "${eglib_sources};${mscorbi_sources};${PROJECT_SOURCE_DIR}/../mini/debugger-protocol.c;${PROJECT_SOURCE_DIR}/../../../coreclr/pal/prebuilt/idl/xcordebug_i.cpp;${PROJECT_SOURCE_DIR}/../../../coreclr/pal/prebuilt/idl/cordebug_i.cpp")

target_link_libraries(mscordbi ${OS_LIBS})

