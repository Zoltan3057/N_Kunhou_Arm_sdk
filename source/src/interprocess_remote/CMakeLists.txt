
set(INTERPROCESS_REMOTE_SRC
	remote_ctl.cpp
	key2speed.cpp
	main.cpp
)

IF(CMAKE_BUILD_TYPE MATCHES "release")

if(WIN32)
set(INTERPROCESS_REMOTE_LIB
	libtransfer
	libsensor	
	libcom 
	librobot
	libshared_pool  
	${Boost_LIBRARIES}
)
else()

link_directories(/lib)

set(INTERPROCESS_REMOTE_LIB
	libtransfer
	libsensor    	
	libcom 
	librobot
	libshared_pool 
	rt 
	${Boost_LIBRARIES}
)
endif()

add_executable(interprocess_remote ${INTERPROCESS_REMOTE_SRC})
target_link_libraries(interprocess_remote ${INTERPROCESS_REMOTE_LIB})

ELSE()

if(WIN32)
set(INTERPROCESS_REMOTE_LIB
	libtransferd
	libsensord	
	libcomd 
	librobotd
	libshared_poold  
	${Boost_LIBRARIES}
)
else()

link_directories(/lib)

set(INTERPROCESS_REMOTE_LIB
	libtransferd
	libsensord    	
	libcomd 
	librobotd
	libshared_poold 
	rt 
	${Boost_LIBRARIES}
)
endif()

add_executable(interprocess_remote ${INTERPROCESS_REMOTE_SRC})
target_link_libraries(interprocess_remote ${INTERPROCESS_REMOTE_LIB})
ENDIF()




