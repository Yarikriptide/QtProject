# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\MedicalRecordsSystem_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\MedicalRecordsSystem_autogen.dir\\ParseCache.txt"
  "MedicalRecordsSystem_autogen"
  )
endif()
