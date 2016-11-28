cmake_minimum_required (VERSION 3.1)
project (FinalProject)

#########################################################
# SET COMPILER 
#########################################################
set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)


###
# opencv #
###
find_package(OpenCV)
if (OpenCV_FOUND)
   include_directories(${OpenCV_INCLUDE_DIRS})
   include_directories(/opt/X11/include)
   #ADD_EXECUTABLE(webcam_face_pose_ex webcam_face_pose_ex.cpp)
   #TARGET_LINK_LIBRARIES(webcam_face_pose_ex dlib ${OpenCV_LIBS} )
endif()

####
# Dlib #
###
include(/Users/sunohyoo/Downloads/dlib-18.18/dlib/cmake)
#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)

#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)


#########################################################
# CREATE WORLD FILE VARIABLES
#########################################################
SET(CW_SOURCE
  #createWorld.cpp
)
SET(CW_HEADERS
  #createWorld.h #createWorld.cpp doesn't have header
)

#########################################################
# JELLO EXE VARIABLE
#########################################################
SET(JELLO_SOURCE
  jello.cpp
  input.cpp
  physics.cpp
  ppm.cpp
  showCube.cpp
  pic.cpp
  Webcam.cpp
  Face.cpp
)
SET(JELLO_HEADERS
  jello.h
  input.h
  physics.h
  #ppm.h #ppm doesn't have a header
  showCube.h
  pic.h
  Face.h
  Webcam.h
)

#########################################################
# ADD SUB DIRECTORIES
#########################################################
#add_subdirectory(<name>)

#########################################################
# ADD EXECUTABLES
#########################################################
add_executable(jello ${JELLO_SOURCE} ${JELLO_HEADERS})
#add_executable(createWorld ${CW_SOURCE} ${CW_HEADERS})

#########################################################
# LINK LIBRARIES
#########################################################
target_link_libraries(jello dlib ${OpenCV_LIBS} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
