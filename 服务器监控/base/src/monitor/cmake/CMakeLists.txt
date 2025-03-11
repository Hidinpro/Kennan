# 设置要cmake工具的最低版
cmake_minimum_required (VERSION 3.5.1)

# 设置编译器
set(CMAKE_C_COMPILER "gcc")

# 设置语法标准
set(CMAKE_C_STANDARD 99)

# 设置编译参数
set(CMAKE_C_FLAGS "-Wall -Werror")

# 要编译的文件
file(GLOB MONITORED_SRC_LIST ../monitored.c)
file(GLOB MONITOR_SRC_LIST ../monitor.c)

# 设置要编译的目标
add_library(monitored SHARED ${MONITORED_SRC_LIST})
add_library(monitor SHARED ${MONITOR_SRC_LIST})

# 设置安装位置
install(TARGETS monitored monitor
	RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

# 设置要安装头文件  
file(GLOB HEAD_LIST ../*.h)
install(FILES ${HEAD_LIST} DESTINATION include)


