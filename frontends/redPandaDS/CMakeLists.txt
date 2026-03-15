cmake_minimum_required(VERSION 3.10)

# ==================================================================================================
# Pull in Web Resources
# ==================================================================================================
file(DOWNLOAD
   "https://github.com/qwerasd205/PixelCode/blob/main/dist/ttf/PixelCode.ttf"
   ${CMAKE_BINARY_DIR}/resources/PixelCodeFont/PixelCode.ttf
   SHOW_PROGRESS
)
file(DOWNLOAD
   "https://github.com/qwerasd205/PixelCode/blob/main/LICENSE"
   ${CMAKE_BINARY_DIR}/resources/PixelCodeFont/LICENSE
   SHOW_PROGRESS
)

# ==================================================================================================
# Source / Include List
# ==================================================================================================
set(RED_PANDA_DS_INCLUDES
   ${GLOBAL_INCLUDES}
   ${CMAKE_CURRENT_SOURCE_DIR}/
)
set(RED_PANDA_DS_SOURCES
   ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/app/app.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/mainWindow/mainWindow.cpp
)
set(RED_PANDA_DS_QT_UI_SOURCES
   ${CMAKE_CURRENT_SOURCE_DIR}/resources.qrc
   ${CMAKE_CURRENT_SOURCE_DIR}/mainWindow/mainWindow.ui
)

# ==================================================================================================
# QT
# ==================================================================================================
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)
qt_standard_project_setup()

# ==================================================================================================
# Binary Definition
# ==================================================================================================
set(BINARY_NAME redPandaDS)
add_executable(${BINARY_NAME}
   ${RED_PANDA_DS_SOURCES}
   ${RED_PANDA_DS_QT_UI_SOURCES}
)
target_include_directories(${BINARY_NAME} PRIVATE ${RED_PANDA_DS_INCLUDES})

target_link_libraries(${BINARY_NAME} PRIVATE ${EMU_CORE_LIB}
   Qt6::Core
   Qt6::Widgets
   Qt6::Gui
)
set_target_properties(${BINARY_NAME} PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/
)
