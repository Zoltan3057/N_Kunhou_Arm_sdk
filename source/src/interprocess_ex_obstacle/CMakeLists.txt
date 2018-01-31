set(INTERPROCESS_EX_OBSTACLE_SRC
	main.cpp
	ultrasonic_ks106.cpp
)

IF(CMAKE_BUILD_TYPE MATCHES "release")

if(WIN32)
set(INTERPROCESS_EX_OBSTACLE_LIB
	libcom
	librobot
	libtransfer
	libshared_pool
	${Boost_LIBRARIES}
	WS2_32.lib
	setupapi.lib 
)
else()

link_directories(/lib)

set(INTERPROCESS_EX_OBSTACLE_LIB
    libtransfer	
    libcom 	
	librobot
	libshared_pool
	rt 
	${Boost_LIBRARIES}
)
endif()

add_executable(interprocess_ex_obstacle ${INTERPROCESS_EX_OBSTACLE_SRC})
target_link_libraries(interprocess_ex_obstacle ${INTERPROCESS_EX_OBSTACLE_LIB})

ELSE()

if(WIN32)
set(INTERPROCESS_EX_OBSTACLE_LIB
	libcomd
	librobotd
	libtransferd
	libshared_poold
	${Boost_LIBRARIES}
	WS2_32.lib
	setupapi.lib
)
else()

link_directories(/lib)

set(INTERPROCESS_EX_OBSTACLE_LIB
    libtransferd
    libcomd
	librobotd
	libshared_poold
	rt 
	${Boost_LIBRARIES}
)
endif()

add_executable(interprocess_ex_obstacle ${INTERPROCESS_EX_OBSTACLE_SRC})
target_link_libraries(interprocess_ex_obstacle ${INTERPROCESS_EX_OBSTACLE_LIB})
ENDIF()




