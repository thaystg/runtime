project(mono)

set(subdirs mini profiler mscordbi)

if(ENABLE_PERFTRACING)
  list(APPEND subdirs
    eventpipe/test
  )
endif(ENABLE_PERFTRACING)

foreach(dir ${subdirs})
  add_subdirectory(${dir})
endforeach()
