AUX_SOURCE_DIRECTORY(. TEST_SOURCES)
INCLUDE_DIRECTORIES(
	googletest
	googletest/include
)

FOREACH(SRC ${TEST_SOURCES})
	STRING(REGEX REPLACE ".*/(.*)\\.cpp$" "\\1" TGT ${SRC})
	ADD_EXECUTABLE(${TGT} ${SRC} googletest/src/gtest-all.cc googletest/src/gtest_main.cc)
	ADD_TEST(NAME ${TGT} COMMAND ${TGT})
ENDFOREACH()
