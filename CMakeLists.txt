cmake_minimum_required(VERSION 3.15...3.26)

project(nanobind_example LANGUAGES CXX)

if (NOT SKBUILD)
  message(WARNING "\
  This CMake file is meant to be executed using 'scikit-build'. Running
  it directly will almost certainly not produce the desired result. If
  you are a user trying to install this package, please use the command
  below, which will install all necessary build dependencies, compile
  the package in an isolated environment, and then install it.
  =====================================================================
   $ pip install .
  =====================================================================
  If you are a software developer, and this is your own package, then
  it is usually much more efficient to install the build dependencies
  in your environment once and use the following command that avoids
  a costly creation of a new virtual environment at every compilation:
  =====================================================================
   $ pip install nanobind scikit-build-core[pyproject]
   $ pip install --no-build-isolation -ve .
  =====================================================================
  You may optionally add -Ceditable.rebuild=true to auto-rebuild when
  the package is imported. Otherwise, you need to re-run the above
  after editing C++ files.")
endif()

# Try to import all Python components potentially needed by nanobind
find_package(Python 3.10
  REQUIRED COMPONENTS Interpreter Development.Module
  OPTIONAL_COMPONENTS Development.SABIModule)

# Import nanobind through CMake's find_package mechanism
find_package(nanobind CONFIG REQUIRED)

# Set extension name
set(EXT_NAME "nanobind_example_ext")

# We are now ready to compile the actual extension module
nanobind_add_module(
  # Name of the extension
  ${EXT_NAME}

  # This extension is free-threaded. Remove this flag if your
  # code is not thread-safe. See the documentation for details:
  # https://nanobind.readthedocs.io/en/latest/free_threaded.html
  FREE_THREADED

  # Target the stable ABI for Python 3.12+, which reduces
  # the number of binary wheels that must be built. This
  # does nothing on older Python versions
  STABLE_ABI

  # Build libnanobind statically and merge it into the
  # extension (which itself remains a shared library)
  #
  # If your project builds multiple extensions, you can
  # replace this flag by NB_SHARED to conserve space by
  # reusing a shared libnanobind across libraries
  NB_STATIC

  # Source code goes here
  src/nanobind_example_ext.cpp
)

# Generate a stub file for the extension, required for type checking
nanobind_add_stub(
  ${EXT_NAME}_stub
  MODULE ${EXT_NAME}
  OUTPUT ${EXT_NAME}.pyi
  PYTHON_PATH $<TARGET_FILE_DIR:${EXT_NAME}>
  MARKER_FILE py.typed
  DEPENDS ${EXT_NAME}
)

# Keep generated typing artifacts visible to language servers during local/editable
# development. VS Code/Pylance resolves this workspace package from source.
add_custom_command(
  TARGET ${EXT_NAME}_stub POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_BINARY_DIR}/${EXT_NAME}.pyi
    ${CMAKE_CURRENT_SOURCE_DIR}/src/${PROJECT_NAME}/${EXT_NAME}.pyi
)


# Install directive for scikit-build-core
install(TARGETS ${EXT_NAME} LIBRARY DESTINATION ${PROJECT_NAME})
