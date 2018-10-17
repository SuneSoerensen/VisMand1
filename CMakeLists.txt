cmake_minimum_required(VERSION 2.8)
project(vision_mandatory_1)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

#image1
#add_executable(image1 "image1.cpp")
#target_link_libraries(image1 ${OpenCV_LIBS})

#image2
#add_executable(image2 "image2.cpp")
#target_link_libraries(image2 ${OpenCV_LIBS})

#image3
#add_executable(image3 "image3.cpp")
#target_link_libraries(image3 ${OpenCV_LIBS})

#image4_1
add_executable(image4_1 "image4_1.cpp")
target_link_libraries(image4_1 ${OpenCV_LIBS})

#image4_2
#add_executable(image4_2 "image4_2.cpp")
#target_link_libraries(image4_2 ${OpenCV_LIBS})

#image5
#add_executable(image5 "image5.cpp")
#target_link_libraries(image5 ${OpenCV_LIBS})
