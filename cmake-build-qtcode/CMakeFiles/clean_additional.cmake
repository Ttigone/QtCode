# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\QtCode_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\QtCode_autogen.dir\\ParseCache.txt"
  "QtCode_autogen"
  )
endif()
