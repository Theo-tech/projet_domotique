/**
  ******************************************************************************
  * @file    LwIP/LwIP_HTTP_Server_Netconn_RTOS/Src/httpser-netconn.c 
  * @author  MCD Application Team
  * @brief   Basic http server implementation using LwIP netconn API  
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"

#include "lwip/apps/fs.h"
#include "string.h"
#include <stdio.h>
#include "httpserver-netconn.h"
#include "cmsis_os.h"
#include "lwip/apps/mqtt.h"
#include "lwip/apps/mqtt_priv.h"
#include "lwip/apps/mqtt_opts.h"
#include "lcd_log.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MQTT_THREAD_PRIO osPriorityAboveNormal
#define LWIP_ALTCP 0

void example_do_connect(mqtt_client_t *client);
static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status);
static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len);
static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags);


static void mqtt_sub_request_cb(void *arg, err_t result)
{
	/* Just print the result code here for simplicity,
     normal behaviour would be to take some action if subscribe fails like
     notifying user, retry subscribe or disconnect from server */
	LCD_UsrLog((char *)"  mqtt_sub_request_cb %d\n", result);
}

void example_do_connect(mqtt_client_t *client)
{
	struct mqtt_connect_client_info_t ci;
	err_t err;

	/* Setup an empty client info structure */
	memset(&ci, 0, sizeof(ci));

	/* Minimal amount of information required is client identifier, so set it here */
	ci.client_id = "lwip_RT";


	ip_addr_t ip_addr;
	IP4_ADDR(&ip_addr, 192,168,1,94);


	err = mqtt_client_connect(client, &ip_addr, MQTT_PORT, mqtt_connection_cb, 0, &ci);
	LCD_UsrLog((char *)"mqtt_client_connect err=%d\n", err);

	for ( ;; ) {
		osDelay(200);
	}
}

void mqtt(void const *argument)
{
	mqtt_client_t static_client;
	memset(&static_client, 0, sizeof(mqtt_client_t));

	example_do_connect(&static_client);
}


/**
  * @brief  Initialize the MQTT server (start its thread) 
  * @param  none
  * @retval None
  */
void mqtt_client_init()
{

	/* Start DHCPClient */
	LCD_UsrLog((char *)"Waiting 5s before start MQTT\n");
	osDelay(5000);
	LCD_UsrLog((char *)"Starting MQTT\n");

	osThreadDef(MQTT, mqtt, osPriorityLow, 0, configMINIMAL_STACK_SIZE*4);
	osThreadCreate(osThread(MQTT), NULL);
}

static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
	err_t err;
	LCD_UsrLog((char *)"mqtt_connection_cb: %d\n", status);

	if (status == MQTT_CONNECT_ACCEPTED)
	{
		LCD_UsrLog((char *)"mqtt_connection_cb: Successfully connected\n");

		/* Subscribe to a topic named "subtopic" with QoS level 1, call mqtt_sub_request_cb with result */
		err = mqtt_subscribe(client, "TR_temperature", 0, mqtt_sub_request_cb, arg);
		LCD_UsrLog((char *)"subscribe topic TR_temperature: %d\n", err);
		err = mqtt_subscribe(client, "TR_humidity", 0, mqtt_sub_request_cb, arg);
		LCD_UsrLog((char *)"subscribe topic TR_humidity: %d\n", err);
		mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, arg);

		if (err != ERR_OK)
		{
			LCD_UsrLog((char *)"mqtt_subscribe return: %d\n", err);
		}
	}
	else
	{
		LCD_UsrLog((char *)"mqtt_connection_cb: Disconnected, reason: %d\n", status);

		/* Its more nice to be connected, so try to reconnect */
		example_do_connect(client);
	}
}





enum {
	TOPIC_TEMPERATURE,
	TOPIC_HUMIDITY
};


/* 
-----------------------------------------------------------------
3. Implementing callbacks for incoming publish and data
The idea is to demultiplex topic and create some reference to be used in data callbacks
Example here uses a global variable, better would be to use a member in arg
If RAM and CPU budget allows it, the easiest implementation might be to just take a copy of
the topic string and use it in mqtt_incoming_data_cb
*/
static int inpub_id;
static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len)
{

  /* Decode topic string into a user defined reference */
  if(strcmp(topic, "TR_temperature") == 0) {
    inpub_id = TOPIC_TEMPERATURE;
  }
    /* Decode topic string into a user defined reference */
  if(strcmp(topic, "TR_humidity") == 0) {
    inpub_id = TOPIC_HUMIDITY;
  }  
}

static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags)
{

  if(flags & MQTT_DATA_FLAG_LAST) {

	  if(inpub_id == TOPIC_HUMIDITY) {
		  char str[50];
		  for(int i = 0; i < len; i++) {
			  str[i] = (char) data[i];
		  }
		  LCD_UsrLog((char *)"Humidity %s\n", str);
	  }

	  if(inpub_id == TOPIC_TEMPERATURE) {
		  char str[50];
		  for(int i = 0; i < len; i++) {
			  str[i] = (char) data[i];
		  }
		  LCD_UsrLog((char *)"Temperature %s\n", str);
	  }

  }
}
