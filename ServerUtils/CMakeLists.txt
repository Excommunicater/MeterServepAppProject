add_library(serverUtils serverUtils.c)

#add_subdirectory(../MeterHardware)

target_include_directories(${PROJECT_NAME} PUBLIC ../MeterHardware )