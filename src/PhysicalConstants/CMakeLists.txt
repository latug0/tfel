tfel_library(TFELPhysicalConstants
  PhysicalConstants.cxx)
target_include_directories(TFELPhysicalConstants
   PUBLIC 
   $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
   $<INSTALL_INTERFACE:include>)
