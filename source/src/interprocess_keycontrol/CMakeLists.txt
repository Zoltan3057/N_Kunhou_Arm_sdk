
set(INTERPROCESS_KEYCONTROL_SRC

	main.cpp
)

IF(CMAKE_BUILD_TYPE MATCHES "release")

if(WIN32)
set(INTERPROCESS_KEYCONTROL_LIB
	libsensor	
	libcom 
	librobot
	libshared_pool  
	${Boost_LIBRARIES}
)
else()

link_directories(/lib)

set(INTERPROCESS_KEYCONTROL_LIB
	libsensor    	
	libcom 
	librobot
	libshared_pool 
	rt 
	${Boost_LIBRARIES}
)
endif()

add_executable(interprocess_keycontrol ${INTERPROCESS_KEYCONTROL_SRC})
target_link_libraries(interprocess_keycontrol ${INTERPROCESS_KEYCONTROL_LIB})

ELSE()

if(WIN32)
set(INTERPROCESS_KEYCONTROL_LIB
	libsensord	
	libcomd 
	librobotd
	libshared_poold  
	${Boost_LIBRARIES}
)
else()

link_directories(/lib)

set(INTERPROCESS_KEYCONTROL_LIB
	libsensord    	
	libcomd 
	librobotd
	libshared_poold 
	rt 
	${Boost_LIBRARIES}
)
endif()

add_executable(interprocess_keycontrol ${INTERPROCESS_KEYCONTROL_SRC})
target_link_libraries(interprocess_keycontrol ${INTERPROCESS_KEYCONTROL_LIB})
ENDIF()




