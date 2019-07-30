set(TESTS_FILES
  draft
)

foreach(FILE ${TESTS_FILES})
  add_executable(${FILE} ${FILE})
  add_test(${FILE} ${FILE})
endforeach(FILE)
