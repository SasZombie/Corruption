set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)

# Use the env variable (CMake 3.15+ supports ENV{...})
set(RAYLIB_ROOT $ENV{RAYLIB_WINDOWS_ROOT})
include_directories("${RAYLIB_ROOT}/include")
link_directories("${RAYLIB_ROOT}/lib")

# Optional, helps CMake search paths
set(CMAKE_FIND_ROOT_PATH "${RAYLIB_ROOT}")
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
