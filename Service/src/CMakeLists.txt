set(SOURCE_FILES 
    main.cpp
    )

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

if(WIN32 OR MINGW OR MSYS OR CYGWIN)
    target_link_libraries(${PROJECT_NAME} PRIVATE setupapi)
endif()

target_link_options(${PROJECT_NAME} PRIVATE "-static")

target_include_directories(${PROJECT_NAME} PRIVATE .)
target_link_libraries(${PROJECT_NAME} PRIVATE serial_lib)