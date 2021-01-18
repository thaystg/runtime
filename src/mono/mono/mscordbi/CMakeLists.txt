project(mscordbi)


include_directories(
  ${PROJECT_BINARY_DIR}/
  ${PROJECT_BINARY_DIR}/../..
  ${PROJECT_BINARY_DIR}/../../mono/eglib
  ${CMAKE_CURRENT_SOURCE_DIR}/../..
  ${PROJECT_SOURCE_DIR}/../mini/  
  ${PROJECT_SOURCE_DIR}/../
  ${PROJECT_SOURCE_DIR}/../eglib
  ${PROJECT_SOURCE_DIR}/../mscordbi
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/enc
  ${PROJECT_SOURCE_DIR}/../../../coreclr/inc
  ${PROJECT_SOURCE_DIR}/../../../coreclr/pal/prebuilt/inc
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/inc
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/compiler)


include(../eglib/CMakeLists.txt)


set(UTILCODE_COMMON_SOURCES_BASE
  clrhost_nodependencies.cpp
  ccomprc.cpp
  ex.cpp
  sbuffer.cpp
  sstring_com.cpp
  fstring.cpp
  namespaceutil.cpp
  makepath.cpp
  splitpath.cpp
  clrconfig.cpp
  configuration.cpp
  collections.cpp
  posterror.cpp
  fstream.cpp
  clrhelpers.cpp
  stgpool.cpp
  stgpooli.cpp
  stgpoolreadonly.cpp
  utsem.cpp
  peinformation.cpp
  check.cpp
  log.cpp
  arraylist.cpp
  bitvector.cpp
  comex.cpp
  guidfromname.cpp
  memorypool.cpp
  iallocator.cpp
  loaderheap.cpp
  outstring.cpp
  ilformatter.cpp
  opinfo.cpp
  corimage.cpp
  format1.cpp
  prettyprintsig.cpp
  regutil.cpp
  sha1.cpp
  sigbuilder.cpp
  sigparser.cpp
  sstring.cpp
  util_nodependencies.cpp
  utilmessagebox.cpp
  safewrap.cpp
  clrhost.cpp
  cycletimer.cpp
  md5.cpp
  util.cpp
  stresslog.cpp
  debug.cpp
  pedecoder.cpp
  winfix.cpp
  longfilepathwrappers.cpp
  yieldprocessornormalized.cpp
  hostimpl.cpp
)

set(MDRUNTIMERW_SOURCES
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/enc/liteweightstgdbrw.cpp
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/enc/metamodelenc.cpp
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/enc/metamodelrw.cpp
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/enc/peparse.cpp
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/enc/rwutil.cpp
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/enc/stgio.cpp
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/enc/stgtiggerstorage.cpp
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/enc/stgtiggerstream.cpp
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/enc/mdinternalrw.cpp
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/enc/pdbheap.cpp
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/compiler/importhelper.cpp
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/runtime/metamodel.cpp
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/runtime/mdcolumndescriptors.cpp
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/runtime/recordpool.cpp
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/runtime/mdfileformat.cpp
)

set(MDRUNTIMERW_HEADERS
  ${PROJECT_SOURCE_DIR}/../../../coreclr/inc/corhdr.h  
  ${PROJECT_SOURCE_DIR}/../../../coreclr/inc/metadata.h
  ${PROJECT_SOURCE_DIR}/../../../coreclr/inc/mdfileformat.h
  ${PROJECT_SOURCE_DIR}/../../../coreclr/inc/pedecoder.h
  ${PROJECT_SOURCE_DIR}/../../../coreclr/inc/pedecoder.inl
  ${PROJECT_SOURCE_DIR}/../../../coreclr/inc/posterror.h
  ${PROJECT_SOURCE_DIR}/../../../coreclr/inc/sstring.h
  ${PROJECT_SOURCE_DIR}/../../../coreclr/inc/sstring.inl
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/compiler/importhelper.h
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/compiler/regmeta.h
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/hotdata/hotdataformat.h
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/inc/liteweightstgdb.h
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/inc/mdinternalrw.h
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/inc/mdlog.h
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/inc/metadatahash.h
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/inc/metamodel.h
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/inc/metamodelro.h
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/inc/metamodelrw.h
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/inc/pdbheap.h
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/inc/portablepdbmdds.h
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/inc/portablepdbmdi.h
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/inc/rwutil.h
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/inc/stgio.h
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/inc/stgtiggerstorage.h
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/inc/stgtiggerstream.h
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/inc/streamutil.h
  ${PROJECT_SOURCE_DIR}/../../../coreclr/md/runtime/mdinternalro.h
  ${PROJECT_SOURCE_DIR}/../../../coreclr/inc/debugmacros.h
)

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


if(HOST_WIN32)
  list(APPEND UTILCODE_COMMON_SOURCES_BASE
    dacutil.cpp
    dlwrap.cpp
    securitywrapper.cpp
    securityutil.cpp
    stacktrace.cpp
  )
endif(HOST_WIN32)

addprefix(mscorbi_sources ../mscordbi/ "${mscorbi_sources_base}")
addprefix(UTILCODE_COMMON_SOURCES ${PROJECT_SOURCE_DIR}/../../../coreclr/utilcode/ "${UTILCODE_COMMON_SOURCES_BASE}")

add_library(mscordbi SHARED "${eglib_sources};${mscorbi_sources};${PROJECT_SOURCE_DIR}/../mini/debugger-protocol.c;${PROJECT_SOURCE_DIR}/../../../coreclr/pal/prebuilt/idl/xcordebug_i.cpp;${PROJECT_SOURCE_DIR}/../../../coreclr/pal/prebuilt/idl/cordebug_i.cpp")

add_library(mdruntimerw-dbi STATIC "${MDRUNTIMERW_SOURCES};${MDRUNTIMERW_HEADERS}")
add_library(utilcode STATIC "${UTILCODE_COMMON_SOURCES}")


target_link_libraries(mscordbi mdruntimerw-dbi utilcode ${OS_LIBS})

set(TARGET_AMD64 1)
add_compile_definitions(TARGET_AMD64)
add_compile_definitions(DBI_COMPONENT)
add_compile_definitions(HOST_WINDOWS)
add_compile_definitions(HOST_64BIT)
add_compile_definitions(FEATURE_COMINTEROP)
add_compile_definitions(SELF_NO_HOST)


target_precompile_headers(mdruntimerw-dbi PRIVATE ${PROJECT_SOURCE_DIR}/../../../coreclr/md/enc/stdafx.h)
target_precompile_headers(utilcode PRIVATE ${PROJECT_SOURCE_DIR}/../../../coreclr/utilcode/stdafx.h)


if(HOST_WIN32)
target_compile_definitions(utilcode PRIVATE _CRTIMP=)
endif(HOST_WIN32)

SET(CMAKE_C_COMPILER ${CMAKE_CXX_COMPILER})
SET_TARGET_PROPERTIES(mdruntimerw-dbi PROPERTIES LINKER_LANGUAGE CXX)
SET_TARGET_PROPERTIES(utilcode PROPERTIES LINKER_LANGUAGE CXX)

set_target_properties(mdruntimerw-dbi PROPERTIES DBI_COMPONENT TRUE)

set_source_files_properties(${PROJECT_SOURCE_DIR}/../mini/debugger-protocol.c PROPERTIES LANGUAGE CXX)
