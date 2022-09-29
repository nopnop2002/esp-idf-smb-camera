#define CMD_TAKE	100
#define CMD_PUT		200
#define CMD_OK		300
#define CMD_NG		400
#define CMD_HALT	900

typedef struct {
    uint16_t command;
    TaskHandle_t taskHandle;
} CMD_t;

// Request to SMB Client
typedef struct {
    uint16_t command;
    char localFileName[64];
	size_t localFileSize;
    char remoteFileName[64];
    TaskHandle_t taskHandle;
} REQUEST_t;

// Responce from SMB Client
typedef struct {
    uint16_t command;
    char response[256];
    TaskHandle_t taskHandle;
} RESPONSE_t;

// Message to HTTP Server
typedef struct {
    char localFileName[64];
    TaskHandle_t taskHandle;
} HTTP_t;
