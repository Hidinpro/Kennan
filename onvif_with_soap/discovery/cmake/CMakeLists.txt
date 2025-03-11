# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (Demo2)

# 取消默认链接参数
string(REGEX REPLACE "-rdynamic" "" CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_C_FLAGS}")
message("CMAKE_SHARED_LIBRARY_LINK_C_FLAGS = " ${CMAKE_SHARED_LIBRARY_LINK_C_FLAGS})

# 编译器参数
add_compile_options(-DWITH_OPENSSL -DWITH_DOM -DWITH_ZLIB -DWITH_NOIDREF -Os -ffunction-sections -fdata-sections)

# 链接器参数 
add_link_options(-Wl,-exclude-libs=ALL,-gc-sections -s)

# 指定生成目标
add_executable(Demo ../main.c ../duration.c ../struct_timeval.c ../stdsoap2.c ../dom.c ../soapC.c ../threads.c ../wsaapi.c ../soapClient.c ../onvif_comm.c ../wsseapi.c ../smdevp.c ../mecevp.c)

# 指定要链接的动态库的路径
#link_directories(${PROJECT_SOURCE_DIR}/lib)

#link_libraries(Demo libcrypto.a libssl.a libz.a)

# 指定要链接的动态库
target_link_libraries(Demo libcrypto.so libssl.so libz.so)