include_directories(include)


#ADD_SUBDIRECTORY(map_test)
#ADD_SUBDIRECTORY(shared_test)
#ADD_SUBDIRECTORY(chassis_test)
#ADD_SUBDIRECTORY(laser_test)
#ADD_SUBDIRECTORY(gmapping_test)
#ADD_SUBDIRECTORY(irq_test)
#ADD_SUBDIRECTORY(particle_filter_test)
#ADD_SUBDIRECTORY(xmlrpc_test)
#ADD_SUBDIRECTORY(interprocess_test)
#ADD_SUBDIRECTORY(polygon_test)

IF(BUILD_GMAP)
ADD_SUBDIRECTORY(appOdomFilterbyLaser)
ADD_SUBDIRECTORY(ref_laser_loc_test)
ADD_SUBDIRECTORY(interprocess_amcl)
ADD_SUBDIRECTORY(interprocess_slam)
ADD_SUBDIRECTORY(interprocess_movement)
ENDIF(BUILD_GMAP)

IF(BUILD_SDK)
#ADD_SUBDIRECTORY(log_test)
#ADD_SUBDIRECTORY(sqlite3_test)
#ADD_SUBDIRECTORY(cordic_test)
ADD_SUBDIRECTORY(interprocess_core)
ADD_SUBDIRECTORY(interprocess_laser)
ADD_SUBDIRECTORY(interprocess_laser_rp)
ADD_SUBDIRECTORY(interprocess_urg)
ADD_SUBDIRECTORY(interprocess_tim561)
ADD_SUBDIRECTORY(interprocess_tim)
ADD_SUBDIRECTORY(interprocess_R2000)
#ADD_SUBDIRECTORY(interprocess_tianhe)
ADD_SUBDIRECTORY(interprocess_bag_player)
ADD_SUBDIRECTORY(interprocess_data_server)
ADD_SUBDIRECTORY(interprocess_map_amcl)
ADD_SUBDIRECTORY(interprocess_map_global_inf)
ADD_SUBDIRECTORY(interprocess_map_laser_inf)
#ADD_SUBDIRECTORY(interprocess_map_goal)
ADD_SUBDIRECTORY(interprocess_map_path)
ADD_SUBDIRECTORY(interprocess_map_prohibit)
ADD_SUBDIRECTORY(interprocess_recoder)
#ADD_SUBDIRECTORY(interprocess_chassis_omi)
#ADD_SUBDIRECTORY(interprocess_chassis_p3dx)
ADD_SUBDIRECTORY(interprocess_planner_tray)
ADD_SUBDIRECTORY(interprocess_gpio)
ADD_SUBDIRECTORY(interprocess_dio)
ADD_SUBDIRECTORY(interprocess_singleton)
ADD_SUBDIRECTORY(interprocess_simulater)
ADD_SUBDIRECTORY(interprocess_plc)
ADD_SUBDIRECTORY(interprocess_sicks300)
ADD_SUBDIRECTORY(plc_dio_test)

ADD_SUBDIRECTORY(project_lib)


ADD_SUBDIRECTORY(appWebInterface)
ADD_SUBDIRECTORY(appWebInterfaceTest)
ADD_SUBDIRECTORY(appCfgServer)
#ADD_SUBDIRECTORY(appCfgClient)
#ADD_SUBDIRECTORY(appQREncode)
#ADD_SUBDIRECTORY(appPngGrayTest)
ADD_SUBDIRECTORY(appCgiUpload)


ADD_SUBDIRECTORY(test_img_src)
ADD_SUBDIRECTORY(tcp2serial)
ADD_SUBDIRECTORY(calibration_angle)
ADD_SUBDIRECTORY(plugins)

ADD_SUBDIRECTORY(Ed_test)
ADD_SUBDIRECTORY(A_Star_Test)
ADD_SUBDIRECTORY(Pluma_lib)
ADD_SUBDIRECTORY(KInterface_lib)
ADD_SUBDIRECTORY(MapConvert_lib)
ADD_SUBDIRECTORY(vpp_lib)
ADD_SUBDIRECTORY(vpp_test)
ADD_SUBDIRECTORY(t20_calibration)
ADD_SUBDIRECTORY(canalyst2_test)
ENDIF(BUILD_SDK)


ADD_SUBDIRECTORY(interprocess_keycontrol)
ADD_SUBDIRECTORY(interprocess_remote)
ADD_SUBDIRECTORY(interprocess_chassis)
ADD_SUBDIRECTORY(appNavikitInterface)
ADD_SUBDIRECTORY(user_client_test)

ADD_SUBDIRECTORY(interprocess_laser_safe)

#ADD_SUBDIRECTORY(interprocess_data_server)
ADD_SUBDIRECTORY(interprocess_ex_obstacle)


#ADD_SUBDIRECTORY(log_test)


#ADD_SUBDIRECTORY(plugins)


ADD_SUBDIRECTORY(plc_lib)

ADD_SUBDIRECTORY(serial_test)

if(WIN32)
else()
ADD_SUBDIRECTORY(can_test)
endif(WIN32)

IF(BUILD_SDK)
	INSTALL(PROGRAMS CMakeLists.txt DESTINATION source/src)
	
	INSTALL(DIRECTORY
			include
			#plugins
			#interprocess_core
			#interprocess_amcl
			#interprocess_bag_player
			#interprocess_data_server
			interprocess_keycontrol
			interprocess_remote
			#interprocess_map_amcl
			#interprocess_map_global_inf
			#interprocess_map_laser_inf
			#interprocess_map_path
			#interprocess_map_prohibit
			#interprocess_laser
			#interprocess_urg
			interprocess_chassis
			#interprocess_planner_tray
			interprocess_gpio
			interprocess_laser_safe
            interprocess_ex_obstacle
			appNavikitInterface
			user_client_test
			#plc_dio_test
			#Ed_test
			#interprocess_plc
			#interprocess_singleton
			#interprocess_data_server
			#log_test
			#MapConvert_lib
			#KInterface_lib
			#Pluma_lib
			#project_lib
			plc_lib
			#vpp_lib
			#vpp_test
			#A_Star_Test
            can_test
			serial_test
			
			  DESTINATION source/src
	          FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.inl" PATTERN "*.cpp" PATTERN "*.hh" PATTERN "CMakeLists.txt"
	          PATTERN ".svn" EXCLUDE 
	          )
ENDIF(BUILD_SDK)
