include_directories(
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/mfront/include"
  "${CMAKE_CURRENT_BINARY_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/mfront/src"
                 "${PROJECT_BINARY_DIR}/src/Material"
                 "${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception")

set(mfront_tests_SOURCES
  Elasticity
  Norton
  OrthotropicElastic
  OrthotropicCreep
  EllipticCreep)

mfront_behaviour_check_library(MFrontCyranoBehaviours cyrano  ${mfront_tests_SOURCES})
target_link_libraries(MFrontCyranoBehaviours
  CyranoInterface
  TFELMaterial
  TFELMath
  TFELUtilities
  TFELException)
if(WIN32)
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    set_target_properties(MFrontCyranoBehaviours
      PROPERTIES LINK_FLAGS "-Wl,--kill-at -Wl,--no-undefined")
  endif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
endif(WIN32)

get_property(MFrontCyranoBehavioursBuildPath TARGET MFrontCyranoBehaviours PROPERTY LOCATION)
get_property(MFrontMaterialPropertiesCyranoBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION)

macro(test_cyrano test_arg)
  configure_file(cyrano${test_arg}.mtest.in
    cyrano${test_arg}.mtest @ONLY)
  add_test(NAME cyrano${test_arg}_mtest
    COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false cyrano${test_arg}.mtest)
  set_tests_properties(cyrano${test_arg}_mtest
    PROPERTIES DEPENDS MFrontCyranoBehaviours)
  set_tests_properties(cyrano${test_arg}_mtest
    PROPERTIES DEPENDS mtest)
  add_custom_target(cyrano${test_arg}.mtest-install
    ${CMAKE_COMMAND} -E copy cyrano${test_arg}.mtest
    ${CMAKE_INSTALL_PREFIX}/share/mfront/tests/behaviours/cyrano/cyrano${test_arg}.mtest)
  add_dependencies(tests-install
    cyrano${test_arg}.mtest-install)
endmacro(test_cyrano)

test_cyrano(elasticity)
test_cyrano(elasticity10)
test_cyrano(norton)
test_cyrano(norton2)
test_cyrano(orthotropiccreep)
test_cyrano(ellipticcreep)