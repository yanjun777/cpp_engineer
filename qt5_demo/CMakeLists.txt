cmake_minimum_required(VERSION 3.5)

project(Qt5Demo)

set(CMAKE_CXX_STANDARD 11)

# 开启 Qt 自动处理 moc/uic/rcc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


# 查找 Qt5
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# 源文件
set(SOURCES
    main.cpp
    MainWindow.cpp
)

set(HEADERS
    MainWindow.h
)

# 生成可执行文件
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# 链接 Qt5 Widgets
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
