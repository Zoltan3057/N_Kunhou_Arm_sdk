set(CAN_TEST_SRC

	main.cpp
)


IF(CMAKE_BUILD_TYPE MATCHES "release")
add_executable(test_can ${CAN_TEST_SRC})
target_link_libraries(test_can libcom liblog4cpp ${Boost_LIBRARIES} rt)
ELSE()
add_executable(test_cand ${CAN_TEST_SRC})
target_link_libraries(test_cand libcomd liblog4cppd ${Boost_LIBRARIES} rt)
ENDIF()



