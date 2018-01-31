set(SERIAL_TEST_SRC
	main.cpp
)

IF(CMAKE_BUILD_TYPE MATCHES "release")

if(WIN32)
set(SERIAL_TEST_LIB
	libtransfer
	libcom
	librobot
	libshared_pool
	${Boost_LIBRARIES}
	WS2_32.lib
	setupapi.lib 
)
else()

link_directories(/lib)

set(SERIAL_TEST_LIB
    libtransfer
    libcom 	
	librobot
	libshared_pool
	rt 
	${Boost_LIBRARIES}
)
endif()

add_executable(serial_test ${SERIAL_TEST_SRC})
target_link_libraries(serial_test ${SERIAL_TEST_LIB})

ELSE()

if(WIN32)
set(SERIAL_TEST_LIB
	libtransferd
	libcomd
	librobotd
	libshared_poold
	${Boost_LIBRARIES}
	WS2_32.lib
	setupapi.lib
)
else()

link_directories(/lib)

set(SERIAL_TEST_LIB
    libtransferd
    libcomd
	librobotd
	libshared_poold
	rt 
	${Boost_LIBRARIES}
)
endif()

add_executable(serial_test ${SERIAL_TEST_SRC})
target_link_libraries(serial_test ${SERIAL_TEST_LIB})
ENDIF()




