include_directories(
  ${KUNHOU_ARM_THIRD_PARTY_PATH}/include/canalyst2/
)

IF(ARM)
link_directories(${KUNHOU_ARM_THIRD_PARTY_PATH}/arm/canalyst2)
ELSE()
link_directories(${KUNHOU_ARM_THIRD_PARTY_PATH}/x64/canalyst2)
ENDIF(ARM)

set(INTERPROCESS_CHASSIS_SRC
	#diff_pos_multi.cpp
	copley_driver.cpp
	sim_driver.cpp
	main.cpp
)

IF(CMAKE_BUILD_TYPE MATCHES "release")

if(WIN32)
set(INTERPROCESS_CHASSIS_LIB
	libtransfer
	libchassis
	libplc
	libcom
	librobot
	liblinuxdriver
	libshared_pool    
	liblog4cpp  
	${Boost_LIBRARIES}
)
else()

set(INTERPROCESS_CHASSIS_SRC 
	${INTERPROCESS_CHASSIS_SRC}
)

link_directories(/lib)

set(INTERPROCESS_CHASSIS_LIB
	libtransfer
	libchassis
	libplc
	libcom
	librobot
	liblinuxdriver
	libshared_pool    
	liblog4cpp
	controlcan
	pthread
	rt 
	${Boost_LIBRARIES}
)
endif()

add_executable(interprocess_chassis ${INTERPROCESS_CHASSIS_SRC})
target_link_libraries(interprocess_chassis ${INTERPROCESS_CHASSIS_LIB})

ELSE()

if(WIN32)
set(INTERPROCESS_CHASSIS_LIB
	libtransferd
	libchassisd
	libplcd
	libcomd
	librobotd
	liblinuxdriverd
	libshared_poold    
	liblog4cppd
	${Boost_LIBRARIES}
)
else()

set(INTERPROCESS_CHASSIS_SRC 
	${INTERPROCESS_CHASSIS_SRC}
	can_socket.cpp
)

link_directories(/lib)

set(INTERPROCESS_CHASSIS_LIB
	libtransferd
	libchassisd
	libplcd
	libcomd
	librobotd
	liblinuxdriverd
	libshared_poold    
	liblog4cppd
	controlcan
	pthread
	rt 
	${Boost_LIBRARIES}
)
endif()

add_executable(interprocess_chassis ${INTERPROCESS_CHASSIS_SRC})
target_link_libraries(interprocess_chassis ${INTERPROCESS_CHASSIS_LIB})
ENDIF()




