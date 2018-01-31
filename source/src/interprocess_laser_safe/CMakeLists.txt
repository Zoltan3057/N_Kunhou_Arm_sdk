set(INTERPROCESS_LASER_SAFE_SRC
	main.cpp
	Obstacle_Detector.cpp
)

IF(CMAKE_BUILD_TYPE MATCHES "release")

if(WIN32)
set(INTERPROCESS_LASER_SAFE_LIB
	librobot
	libsensor
	libtransfer
	libcom
	libplc 
	libshared_pool
	${Boost_LIBRARIES}
	WS2_32.lib
	setupapi.lib 
)
else()

link_directories(/lib)

set(INTERPROCESS_LASER_SAFE_LIB
    
	librobot
	libsensor
	libtransfer
	libcom
	libplc 
	libshared_pool
	rt 
	${Boost_LIBRARIES}
)
endif()

add_executable(interprocess_laser_safe ${INTERPROCESS_LASER_SAFE_SRC})
target_link_libraries(interprocess_laser_safe ${INTERPROCESS_LASER_SAFE_LIB})

ELSE()

if(WIN32)
set(INTERPROCESS_LASER_SAFE_LIB
	librobotd
	libsensord
	libtransferd
	libcomd
	libplcd
	libshared_poold
	${Boost_LIBRARIES}
	WS2_32.lib
	setupapi.lib
)
else()

link_directories(/lib)

set(INTERPROCESS_LASER_SAFE_LIB
    librobotd
	libsensord
	libtransferd
	libcomd
	libplcd
	libshared_poold
	rt 
	${Boost_LIBRARIES}
)
endif()

add_executable(interprocess_laser_safe ${INTERPROCESS_LASER_SAFE_SRC})
target_link_libraries(interprocess_laser_safe ${INTERPROCESS_LASER_SAFE_LIB})
ENDIF()




