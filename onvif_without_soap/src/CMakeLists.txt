# CMake 最低版本号要求
cmake_minimum_required(VERSION 2.6)

# 项目信息
project (yt_onvif)

# 取消默认链接参数
string(REGEX REPLACE "-rdynamic" "" CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_C_FLAGS}")
message("CMAKE_SHARED_LIBRARY_LINK_C_FLAGS = " ${CMAKE_SHARED_LIBRARY_LINK_C_FLAGS})

# 编译器参数
add_compile_options(-DWITH_OPENSSL -DWITH_DOM -DWITH_ZLIB -DWITH_NOIDREF -Os -ffunction-sections -fdata-sections)

# 链接器参数 
set(LINK_FLAGS "-Wl,-exclude-libs=ALL,-gc-sections -s")
set(CMAKE_SHARED_LINKER_FLAGS "${LINK_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS    "${LINK_FLAGS}")

# 指定生成目标
add_executable(${PROJECT_NAME})

# 添加头文件路径
INCLUDE_DIRECTORIES(./src)

# 添加所有源文件源文件路径
aux_source_directory(./src sources)
target_sources(${PROJECT_NAME} PUBLIC ${sources})

# 指定要链接的动态库
TARGET_LINK_LIBRARIES(${PROJECT_NAME} z ssl crypto curl ${LIBS})

# 设置安装路径
INSTALL(TARGETS ${PROJECT_NAME}
                ARCHIVE DESTINATION lib
                LIBRARY DESTINATION lib
                RUNTIME DESTINATION sbin
        )

