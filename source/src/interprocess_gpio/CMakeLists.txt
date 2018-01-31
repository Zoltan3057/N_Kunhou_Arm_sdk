set(INTERPROCESS_GPIO_SRC
	main.cpp
)

IF(CMAKE_BUILD_TYPE MATCHES "release")

if(WIN32)
set(INTERPROCESS_GPIO_LIB
	libcom
	libsensor 
	librobot
	libtransfer
	libshared_pool
	liblinuxdriver  
	${Boost_LIBRARIES}
	WS2_32.lib
	setupapi.lib 
)
else()

link_directories(/lib)

set(INTERPROCESS_GPIO_LIB
    	libcom 
	libsensor	
	librobot
	libtransfer
	libshared_pool
	liblinuxdriver 
	rt 
	${Boost_LIBRARIES}
)
endif()

add_executable(interprocess_gpio ${INTERPROCESS_GPIO_SRC})
target_link_libraries(interprocess_gpio ${INTERPROCESS_GPIO_LIB})

ELSE()

if(WIN32)
set(INTERPROCESS_GPIO_LIB
	libcomd
	libsensord 
	librobotd
	libtransferd
	libshared_poold
	liblinuxdriverd  
	${Boost_LIBRARIES}
	WS2_32.lib
	setupapi.lib
)
else()

link_directories(/lib)

set(INTERPROCESS_GPIO_LIB
    	libcomd
	libsensord 
	librobotd
	libtransferd
	libshared_poold
	liblinuxdriverd 
	rt 
	${Boost_LIBRARIES}
)
endif()

add_executable(interprocess_gpio ${INTERPROCESS_GPIO_SRC})
target_link_libraries(interprocess_gpio ${INTERPROCESS_GPIO_LIB})
ENDIF()




