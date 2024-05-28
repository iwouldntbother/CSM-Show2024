# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/display_bounding_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/display_bounding_autogen.dir/ParseCache.txt"
  "CMakeFiles/form_scan_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/form_scan_autogen.dir/ParseCache.txt"
  "display_bounding_autogen"
  "form_scan_autogen"
  )
endif()
