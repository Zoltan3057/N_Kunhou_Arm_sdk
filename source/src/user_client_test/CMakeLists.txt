set(USER_CLIENT_TEST_SRCS
    main.cpp
)

IF(CMAKE_BUILD_TYPE MATCHES "release")

add_executable(user_client_test  ${USER_CLIENT_TEST_SRCS})

if(WIN32)
	target_link_libraries(user_client_test  libxmlrpc libtransfer libcom libjsoncpp libwebinterface libxmlrpc librobot ${Boost_LIBRARIES})
else()
	target_link_libraries(user_client_test  libxmlrpc libtransfer libcom libjsoncpp libwebinterface libxmlrpc librobot rt ${Boost_LIBRARIES})
endif()

ELSE()

add_executable(user_client_test  ${USER_CLIENT_TEST_SRCS})

if(WIN32)
	target_link_libraries(user_client_test libxmlrpcd libtransferd libcomd libjsoncppd libwebinterfaced libxmlrpcd librobotd ${Boost_LIBRARIES})
else()
	target_link_libraries(user_client_test libxmlrpcd libtransferd libcomd libjsoncppd libwebinterfaced libxmlrpcd librobotd rt ${Boost_LIBRARIES})
endif()

ENDIF()