set(TESTS_FILES
  draft
  test_core
  test_range
)

foreach(FILE ${TESTS_FILES})
  add_executable(${FILE} ${FILE})
  add_test(${FILE} ${FILE})
endforeach(FILE)
