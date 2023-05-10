cmake_minimum_required(VERSION 3.17.3)
set(CMAKE_CXX_STANDARD 17)#cpp standard to be used for all executables
project (TDD_Raytracer)#sln name

SET(SOURCE_DIR_ProductionCode ../TDD_raytracer/TDD_Raytracer)
#todo: dynamically fill these lists by globbing
SET(SOURCE_ProductionCode
  ${SOURCE_DIR_ProductionCode}/TDD_Raytracer.cpp
  ${SOURCE_DIR_ProductionCode}/ArithmeticStructures.cpp
  ${SOURCE_DIR_ProductionCode}/Canvas.cpp
  ${SOURCE_DIR_ProductionCode}/GeometricStructures.cpp
  ${SOURCE_DIR_ProductionCode}/Material.cpp
  ${SOURCE_DIR_ProductionCode}/PPMWriter.cpp
  ${SOURCE_DIR_ProductionCode}/Ray.cpp
  ${SOURCE_DIR_ProductionCode}/SceneObject.cpp
)

SET(HEADERS_ProductionCode
  ${SOURCE_DIR_ProductionCode}/TDD_Raytracer.h
  ${SOURCE_DIR_ProductionCode}/ArithmeticStructures.h
  ${SOURCE_DIR_ProductionCode}/Canvas.h
  ${SOURCE_DIR_ProductionCode}/GeometricStructures.h
  ${SOURCE_DIR_ProductionCode}/Material.h
  ${SOURCE_DIR_ProductionCode}/PPMWriter.h
  ${SOURCE_DIR_ProductionCode}/Ray.h
  ${SOURCE_DIR_ProductionCode}/SceneObject.h
)
add_executable(Raytracer ${SOURCE_ProductionCode} ${HEADERS_ProductionCode})#name of the projects in the sln

SET(SOURCE_DIR_TestCode ../TDD_raytracer/TDDRaytracer_UnitTests)
SET(SOURCE_TestCode
  ${SOURCE_DIR_TestCode}/TDDRaytracer_UnitTests.cpp
  ${SOURCE_DIR_TestCode}/pch.cpp
)

SET(HEADERS_TestCode
  ${SOURCE_DIR_TestCode}/pch.h
)

#todo: settings for eg additional include directories and related obj files are missing
#todo: by default obj files are not put in the CONFIGURATION folder, but in a CMAKE defined folder, see e.g. here: https://stackoverflow.com/questions/46330056/set-output-directory-for-cmake-object-libraries
add_executable(Raytracer_Test ${SOURCE_TestCode} ${HEADERS_TestCode})#name of the projects in the sln

