AUX_SOURCE_DIRECTORY(. TEST_SOURCES)
INCLUDE_DIRECTORIES(
  ${BHVR_TREE_SOURCE_DIR}/contrib/googletest
  ${BHVR_TREE_SOURCE_DIR}/contrib/googletest/include
)

FOREACH(SRC ${TEST_SOURCES})
  STRING(REGEX REPLACE ".*/(.*)\\.cpp$" "\\1" TGT ${SRC})
  ADD_EXECUTABLE(
    ${TGT} ${SRC}
    ${BHVR_TREE_SOURCE_DIR}/contrib/googletest/src/gtest-all.cc
    ${BHVR_TREE_SOURCE_DIR}/contrib/googletest/src/gtest_main.cc
  )
  TARGET_LINK_LIBRARIES(${TGT} pthread)
  ADD_TEST(NAME ${TGT} COMMAND ${TGT})
ENDFOREACH()
