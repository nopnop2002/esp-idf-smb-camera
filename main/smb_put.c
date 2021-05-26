/* SMB Client Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "smb2.h"
#include "libsmb2.h"
#include "libsmb2-raw.h"

#include "cmd.h"

extern QueueHandle_t xQueueRequest;
extern QueueHandle_t xQueueResponse;

static const char *TAG = "SMB";

void smb_put_task(void *pvParameter)
{
	ESP_LOGI(TAG, "smb_post start");

	struct smb2_context *smb2;
	struct smb2_url *url;
	struct smb2fh *fh;

	ESP_LOGI(TAG, "CONFIG_ESP_SMB_USER=[%s]",CONFIG_ESP_SMB_USER);
	ESP_LOGI(TAG, "CONFIG_ESP_SMB_HOST=[%s]",CONFIG_ESP_SMB_HOST);
	ESP_LOGI(TAG, "CONFIG_ESP_SMB_PATH=[%s]",CONFIG_ESP_SMB_PATH);

	REQUEST_t requestBuf;
	RESPONSE_t responseBuf;
	responseBuf.taskHandle = xTaskGetCurrentTaskHandle();

	while(1) {
		ESP_LOGI(TAG,"Waitting....");
		xQueueReceive(xQueueRequest, &requestBuf, portMAX_DELAY);
		ESP_LOGI(TAG,"requestBuf.command=%d", requestBuf.command);
		if (requestBuf.command == CMD_HALT) break;

		ESP_LOGI(TAG,"requestBuf.localFileName=%s", requestBuf.localFileName);
		ESP_LOGI(TAG,"requestBuf.localFileSize=%d", requestBuf.localFileSize);
		ESP_LOGI(TAG,"requestBuf.remoteFileName=%s", requestBuf.remoteFileName);

		smb2 = smb2_init_context();
		if (smb2 == NULL) {
			ESP_LOGE(TAG, "Failed to init context");
			while(1){ vTaskDelay(1); }
		}

		char smburl[128];
		sprintf(smburl, "smb://%s@%s/%s/%s", CONFIG_ESP_SMB_USER, CONFIG_ESP_SMB_HOST, CONFIG_ESP_SMB_PATH, requestBuf.remoteFileName);
		ESP_LOGI(TAG, "smburl=%s", smburl);

#if CONFIG_ESP_NEED_PASSWORD
		smb2_set_password(smb2, CONFIG_ESP_SMB_PASSWORD);
#endif

		url = smb2_parse_url(smb2, smburl);
		if (url == NULL) {
			ESP_LOGE(TAG, "Failed to parse url: %s", smb2_get_error(smb2));
			while(1){ vTaskDelay(1); }
		}

		smb2_set_security_mode(smb2, SMB2_NEGOTIATE_SIGNING_ENABLED);

		if (smb2_connect_share(smb2, url->server, url->share, url->user) < 0) {
			ESP_LOGE(TAG, "smb2_connect_share failed. %s", smb2_get_error(smb2));
			while(1){ vTaskDelay(1); }
		}
				
		FILE *local = fopen(requestBuf.localFileName, "rb");
		if (local == NULL) {
			ESP_LOGE(TAG, "Failed to open file for reading");
			while(1){ vTaskDelay(1); }
		}


		ESP_LOGI(TAG, "url->path=%s", url->path);
		fh = smb2_open(smb2, url->path, O_WRONLY|O_CREAT);
		if (fh == NULL) {
			ESP_LOGE(TAG, "smb2_open failed. %s", smb2_get_error(smb2));
			while(1){ vTaskDelay(1); }
		}

		char buf[128];
		int totalRead = 0;
		while(!feof(local)) {
			size_t readed = fread(&buf,1,sizeof(buf),local);
			totalRead = totalRead + readed;
			ESP_LOGD(TAG, "readed=%d totalRead=%d FileSize=%d", readed, totalRead, requestBuf.localFileSize);
			size_t writed = smb2_write(smb2, fh, (uint8_t *)buf, readed);
			ESP_LOGD(TAG, "writed=%d", writed);
			if (readed != writed) {
				ESP_LOGE(TAG, "smb2_write failed.");
				while(1){ vTaskDelay(1); }
			}
		}
		fclose(local);

																
		smb2_close(smb2, fh);
		smb2_disconnect_share(smb2);
		smb2_destroy_url(url);
		smb2_destroy_context(smb2);

		responseBuf.command = CMD_OK;
		strcpy(responseBuf.response, smburl);
		if (xQueueSend(xQueueResponse, &responseBuf, 10) != pdPASS) {
			ESP_LOGE(TAG, "xQueueSend fail");
		}

	}

	//smb2_closedir(smb2, dir);
	//smb2_disconnect_share(smb2);
	//smb2_destroy_url(url);
	//smb2_destroy_context(smb2);
}
