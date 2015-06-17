#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(glxdirc)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Find ROOT (required package)
find_package(ROOT REQUIRED)

#----------------------------------------------------------------------------
# Generate dictionary for root
execute_process(COMMAND rootcint -f GlxDict.cxx -c ../src/GlxHit.h ../src/GlxEvent.h ../src/GlxLutNode.h ../src/GlxTrackInfo.h  ../src/GlxPhotonInfo.h ../src/GlxAmbiguityInfo.h ../src/GlxLinkDef.h)


#----------------------------------------------------------------------------
# Locate sources and headers for this project
include_directories(${PROJECT_SOURCE_DIR}/src 
                    ${Geant4_INCLUDE_DIR}
		    ${ROOT_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cxx)
file(GLOB headers ${PROJECT_SOURCE_DIR}/src/*.h)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
add_executable(glxdirc ../src/GlxDirc.cxx ${sources} GlxDict.cxx ${headers} GlxDict.h)
target_link_libraries(glxdirc -lGLU ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} -lSpectrum )
add_library(glxdirclib SHARED ../src/GlxHit.cxx ../src/GlxEvent.cxx ../src/GlxLutNode.cxx ../src/GlxTrackInfo.cxx  ../src/GlxPhotonInfo.cxx ../src/GlxAmbiguityInfo.cxx GlxDict.cxx ../src/GlxHit.h ../src/GlxEvent.h ../src/GlxLutNode.h ../src/GlxTrackInfo.h  ../src/GlxPhotonInfo.h ../src/GlxAmbiguityInfo.h GlxDict.h)


#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
install(TARGETS glxdirc DESTINATION bin)
