project(rime-phraser)
cmake_minimum_required(VERSION 2.8)

aux_source_directory(src RIME_PHRASER_SRC)

add_library(rime-phraser-objs OBJECT ${RIME_PHRASER_SRC})
if(BUILD_SHARED_LIBS)
  set_target_properties(rime-phraser-objs
    PROPERTIES 
    POSITION_INDEPENDENT_CODE ON)
endif()

set(plugin_name "rime-phraser" PARENT_SCOPE)
set(plugin_objs $<TARGET_OBJECTS:rime-phraser-objs> PARENT_SCOPE)
set(plugin_deps ${rime_library} PARENT_SCOPE)
set(plugin_modules "grammar" PARENT_SCOPE)
