# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/lucas/esp/esp-idf/components/bootloader/subproject"
  "/home/lucas/Documents/UnB/9Semestre/FSE/fse-trabalho-3-2023-1/trabalho-3/build/bootloader"
  "/home/lucas/Documents/UnB/9Semestre/FSE/fse-trabalho-3-2023-1/trabalho-3/build/bootloader-prefix"
  "/home/lucas/Documents/UnB/9Semestre/FSE/fse-trabalho-3-2023-1/trabalho-3/build/bootloader-prefix/tmp"
  "/home/lucas/Documents/UnB/9Semestre/FSE/fse-trabalho-3-2023-1/trabalho-3/build/bootloader-prefix/src/bootloader-stamp"
  "/home/lucas/Documents/UnB/9Semestre/FSE/fse-trabalho-3-2023-1/trabalho-3/build/bootloader-prefix/src"
  "/home/lucas/Documents/UnB/9Semestre/FSE/fse-trabalho-3-2023-1/trabalho-3/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/lucas/Documents/UnB/9Semestre/FSE/fse-trabalho-3-2023-1/trabalho-3/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/lucas/Documents/UnB/9Semestre/FSE/fse-trabalho-3-2023-1/trabalho-3/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
