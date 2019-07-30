set(EXAMPLES_FILES
  example_core
  example_range_uniform
  example_view
)

foreach(FILE ${EXAMPLES_FILES})
  add_executable(${FILE} ${FILE})
endforeach(FILE)
