#set(COMPONENT_SRCS main.c keyboard.c gpio.c smb_put.c tcp_server.c udp_server.c web_server.c)
#set(COMPONENT_ADD_INCLUDEDIRS ".")

#register_component()
idf_component_register(SRCS main.c keyboard.c gpio.c smb_put.c tcp_server.c udp_server.c web_server.c
	INCLUDE_DIRS ".")
