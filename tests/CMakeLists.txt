set(TESTS_FILES
  draft
  test_core
  test_range
  test_map
)

foreach(FILE ${TESTS_FILES})
  add_executable(${FILE} ${FILE})
  add_test(${FILE} ${FILE})
endforeach(FILE)
