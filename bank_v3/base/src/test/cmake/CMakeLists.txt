# 设置要cmake工具的最低版
cmake_minimum_required (VERSION 3.5.1)

# 设置编译器
set(CMAKE_C_COMPILER "gcc")

# 设置语法标准
set(CMAKE_C_STANDARD 99)

# 设置编译参数
set(CMAKE_C_FLAGS "-Wall -Werror")

# 指定头文件路径
include_directories(../../../include/)

# 指定库文件的路径
link_directories(../../../lib/)

# 设置要链接的库名
link_libraries(m cjson dsa md5 tools)

# 设置要编译可执行文件名，要编译的源文件
add_executable(json_test ../cJSON_test.c)
add_executable(list_test ../list_test.c)
add_executable(md5_test ../md5_test.c)
add_executable(sort_test ../sort_test.c)
add_executable(tools_test ../tools_test.c)
add_executable(tree_test ../tree_test.c)

# 设置安装位置
install(TARGETS json_test list_test md5_test sort_test tools_test tree_test
	RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)


