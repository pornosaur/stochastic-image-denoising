cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(ui-image-denoising)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

include_directories(
        ui-widgets/include
        models/include
        utils/include
        controllers/include
        ../core-doc-viewer/include
)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package( OpenCV REQUIRED )

set(LIBS Qt5::Core Qt5::Gui Qt5::Widgets ${OpenCV_LIBS} stdc++fs core-image-denoising_lib)

set(SOURCE
        # MODELS
        models/src/image_model.cpp

        # CONTROLLERS
        controllers/src/image_controller.cpp
        controllers/src/toolbox_controller.cpp

        # WIDGETS
        ui-widgets/src/image_renderer.cpp
        ui-widgets/src/imager_tab_widget.cpp
        ui-widgets/src/mainwindow.cpp

        # MAIN
        main.cpp
        )

set(INCLUDE
        # MODELS
        models/include/image_model.h

        #CONTROLLERS
        controllers/include/image_controller.h
        controllers/include/toolbox_controller.h

        # WIDGETS
        ui-widgets/include/image_renderer.h
        ui-widgets/include/image_tab_widget.h
        ui-widgets/include/mainwindow.h

        )

set(UI  # FORE WIDGETS .ui FILES
        ui-widgets/views/mainwindow.ui
        )

qt5_wrap_ui(UI_GEN ${UI})
qt5_wrap_cpp(CPP_GEN ${INCLUDE})

#file(COPY images DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${SOURCE} ${INCLUDE} ${UI_GEN} ${CPP_GEN})

target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBS})