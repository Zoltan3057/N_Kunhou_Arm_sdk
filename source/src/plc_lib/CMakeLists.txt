#plc Lib 
AUX_SOURCE_DIRECTORY(./ PLC_SRC_LIS)

IF(CMAKE_BUILD_TYPE MATCHES "release")

add_library(libplc ${PLC_SRC_LIS})

if(WIN32)
		target_link_libraries(libplc libtransfer libcom)
else()  
		link_directories(/lib)  
        #add_library(soplc SHARED ${PLC_SRC_LIS})		
		target_link_libraries(libplc libtransfer libcom)		
endif()

#IF(BUILD_SDK)
#	if(WIN32)
#		INSTALL(TARGETS libplc DESTINATION lib)
#	else()
#		INSTALL(TARGETS libplc DESTINATION lib)
#	endif()
#ENDIF(BUILD_SDK)
	
ELSE()

add_library(libplcd ${PLC_SRC_LIS})
if(WIN32)
		target_link_libraries(libplcd libtransferd libcomd)
else()  
		link_directories(/lib)
		#add_library(soplcd SHARED ${PLC_SRC_LIS} )     
		target_link_libraries(libplcd libtransferd libcomd)
endif()

#IF(BUILD_SDK)
#	if(WIN32)
#	INSTALL(TARGETS libplcd DESTINATION lib)
#	else()
#	#INSTALL(TARGETS libplcd soplcd DESTINATION lib)
#	INSTALL(TARGETS libplcd DESTINATION lib)
#	endif()
#ENDIF(BUILD_SDK)

ENDIF()
