# 设置要cmake工具的最低版
cmake_minimum_required (VERSION 3.5.1)

# 设置编译器
set(CMAKE_C_COMPILER "gcc")

# 设置语法标准
set(CMAKE_C_STANDARD 99)

# 设置编译参数
set(CMAKE_C_FLAGS "-Wall -Werror")

# 设置要链接的库
link_libraries(m)

# 要编译的文件
file(GLOB CJSON_SRC_LIST ../*.c)

# 设置要编译的目标
add_library(cjson SHARED ${CJSON_SRC_LIST})

# 设置安装位置
install(TARGETS cjson
	RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

# 设置要安装头文件  
file(GLOB CJSON_HEAD_LIST ../*.h)
install(FILES ${CJSON_HEAD_LIST} DESTINATION include)


