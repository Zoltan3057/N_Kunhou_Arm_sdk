set(appNavikitInterface_SRCS
    main.cpp
)

IF(CMAKE_BUILD_TYPE MATCHES "release")

add_executable(appNavikitInterface  ${appNavikitInterface_SRCS})

if(WIN32)
target_link_libraries(appNavikitInterface  libxmlrpc libtransfer libcom libjsoncpp libwebinterface libxmlrpc librobot ${Boost_LIBRARIES})
else()
target_link_libraries(appNavikitInterface  libxmlrpc libtransfer libcom libjsoncpp libwebinterface libxmlrpc librobot rt ${Boost_LIBRARIES})
endif()

ELSE()

add_executable(appNavikitInterfaced  ${appNavikitInterface_SRCS})
if(WIN32)
target_link_libraries(appNavikitInterfaced  libxmlrpcd libtransferd libcomd libjsoncppd libwebinterfaced libxmlrpcd librobotd ${Boost_LIBRARIES})
else()
target_link_libraries(appNavikitInterfaced  libxmlrpcd libtransferd libcomd libjsoncppd libwebinterfaced libxmlrpcd librobotd rt ${Boost_LIBRARIES})
endif()

ENDIF()

