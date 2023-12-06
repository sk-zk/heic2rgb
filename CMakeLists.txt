cmake_minimum_required(VERSION 3.14...3.22)
project(heic2rgb VERSION "0.2.0")

if (UNIX AND NOT APPLE)
  set(CMAKE_CXX_STANDARD 14)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -std=c++14 -stdlib=libc++")
endif()

set(Headers "${CMAKE_CURRENT_SOURCE_DIR}/lib/headers/")

if (WIN32)
  set(AvcodecPath "${CMAKE_CURRENT_SOURCE_DIR}/lib/avcodec.lib")
  set(AvutilPath "${CMAKE_CURRENT_SOURCE_DIR}/lib/avutil.lib")
  set(SwscalePath "${CMAKE_CURRENT_SOURCE_DIR}/lib/swscale.lib")
  set(ISOBMFFPath "${CMAKE_CURRENT_SOURCE_DIR}/lib/ISOBMFF.lib")
endif()
if (UNIX AND NOT APPLE)
  find_library(AVCODEC_LIBRARY avcodec)
  find_library(AVUTIL_LIBRARY avutil)
  find_library(SWSCALE_LIBRARY swscale)
  set(ISOBMFFPath "${CMAKE_CURRENT_SOURCE_DIR}/lib/libISOBMFF.a")
endif()


include(FetchContent)
FetchContent_Declare(
  pybind11
  URL https://github.com/pybind/pybind11/archive/refs/tags/v2.10.0.zip
  #URL_HASH
  #  SHA256=6bd528c4dbe2276635dc787b6b1f2e5316cf6b49ee3e150264e455a0d68d19c1
)
FetchContent_MakeAvailable(pybind11)


pybind11_add_module(_core MODULE
  src/main.cpp
  src/heif.cpp
  src/decoder.cpp
)
target_include_directories(_core PRIVATE ${Headers})
if (WIN32)
  target_link_libraries(_core PRIVATE 
    ${ISOBMFFPath}
    ${AvcodecPath}
    ${AvutilPath}
    ${SwscalePath}
  )
endif()
if (UNIX AND NOT APPLE)
  target_link_libraries(_core PRIVATE
    ${ISOBMFFPath}
    ${AVCODEC_LIBRARY}
    ${AVUTIL_LIBRARY}
    ${SWSCALE_LIBRARY}
)
endif()
install(TARGETS _core DESTINATION .)
