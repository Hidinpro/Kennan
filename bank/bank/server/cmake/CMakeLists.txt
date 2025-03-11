# 设置要cmake工具的最低版
cmake_minimum_required (VERSION 3.5.1)

# 设置编译器
set(CMAKE_C_COMPILER "gcc")

# 设置语法标准
set(CMAKE_C_STANDARD 99)

# 设置编译参数
set(CMAKE_C_FLAGS "-Wall -Werror -DDEBUG")

# 设置要链接的库
link_libraries(msg tools)

# 设置安装位置
set(CMAKE_INSTALL_PREFIX ../)

# 设置要编译的目标
add_executable(server ../src/server.c)
add_executable(create ../src/create.c)
add_executable(destroy ../src/destroy.c)
add_executable(login ../src/login.c)
add_executable(unlock ../src/unlock.c)
add_executable(save ../src/save.c)
add_executable(take ../src/take.c)
add_executable(transf ../src/transf.c)
add_executable(query ../src/query.c)
add_executable(modify ../src/modify.c)

# 设置安装位置
install(TARGETS server create destroy login unlock save take transf query modify
	RUNTIME DESTINATION bin)


