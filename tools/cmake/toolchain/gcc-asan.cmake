# Set normal toolchain variables
set(CMAKE_SYSTEM_NAME ...)
set(CMAKE_SYSTEM_PROCESSOR ...)
set(CMAKE_CROSSCOMPILING OFF)
set(CMAKE_C_FLAGS_INIT "...etc...")

# Set VCPKG-specific variables
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE dynamic)
set(VCPKG_BUILD_TYPE release)
# Pick a triplet name
set(VCPKG_TARGET_TRIPLET "project")
set(VCPKG_OVERLAY_TRIPLETS "${CMAKE_BINARY_DIR}/tools/vcpkg/triplets")

# Generate triplet file
if(CMAKE_BINARY_DIR AND NOT EXISTS "${VCPKG_OVERLAY_TRIPLETS}/${VCPKG_TARGET_TRIPLET}.cmake")
  make_directory("${VCPKG_OVERLAY_TRIPLETS}")
  file(
      CONFIGURE
      OUTPUT "${VCPKG_OVERLAY_TRIPLETS}/${VCPKG_TARGET_TRIPLET}.cmake"
      CONTENT
          [=[
  set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_FILE}")
  include("${VCPKG_CHAINLOAD_TOOLCHAIN_FILE}")
  ]=]
  )
endif()
