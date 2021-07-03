set(COMPONENT_SRCS "main.c"
    "gpio.c"
    "keyboard.c"
    "smb_put.c"
    "tcp_server.c"
    "udp_server.c"
    "web_server.c"
    "http_server.c")
set(COMPONENT_ADD_INCLUDEDIRS ".")

register_component()
