file(GLOB CONTRIBS ${PROJECT_SOURCE_DIR}/contrib/*/CMakeLists.txt)
list(SORT CONTRIBS)
foreach(contrib ${CONTRIBS})
  include(${contrib})
endforeach()
