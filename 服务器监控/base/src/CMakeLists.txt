# 设置要cmake工具的最低版
cmake_minimum_required (VERSION 3.5.1)

# 设置安装位置
set(CMAKE_INSTALL_PREFIX ../../)

# 包含其它目录CMakeLists.txt脚本
add_subdirectory(./monitor/cmake/)
add_subdirectory(./socket/cmake/)
add_subdirectory(./tools/cmake/)
add_subdirectory(./json/cmake/)
add_subdirectory(./md5/cmake/)
add_subdirectory(./dsa/cmake/)
add_subdirectory(./log/cmake/)
add_subdirectory(./msg/cmake/)
add_subdirectory(./pool/cmake/)