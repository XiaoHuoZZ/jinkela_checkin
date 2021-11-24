#搜索头文件目录
include_directories(../include)
#搜索库文件目录
link_directories(../lib)

# 指定生成目标
add_executable(airport-signin main.cpp)

# 添加链接库
target_link_libraries(airport-signin ssl)
target_link_libraries(airport-signin crypto)
target_link_libraries(airport-signin dl)
target_link_libraries(airport-signin pthread)

