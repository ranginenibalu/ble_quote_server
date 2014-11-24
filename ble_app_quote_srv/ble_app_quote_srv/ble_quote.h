

#ifndef _BLE_QUOTE_H_
#define _BLE_QUOTE_H_


#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"



#define BTS_UUID_BASE {0x23, 0xD1, 0xBC, 0xEA, 0x5F, 0x78, 0x23, 0x15, 0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00}
#define BTS_UUID_SERVICE 0x1522


#define BTS_UUID_WRITE_CHAR 0x1523

#define BTS_UUID_ERASE_CHAR 0x1524






// Forward declaration of the ble_lbs_t type. 
typedef struct ble_quote_s ble_quote_t;
typedef void (*ble_quote_write_handler_t) (ble_quote_t * p_bts, uint8_t new_state);

//Accelerometer;


typedef struct
{
    ble_quote_write_handler_t   write_read_handler; 
		ble_quote_write_handler_t   erase_handler; 
	 					
} ble_quote_init_t;

typedef struct ble_quote_s
{         
    uint16_t                     service_handle;                 
    ble_gatts_char_handles_t     write_char_handles;   
		ble_gatts_char_handles_t     erase_char_handles;
		//ble_gatts_char_handles_t		 gyro_char_handles;
		uint8_t                      uuid_type;
    uint16_t                     conn_handle;  
		ble_quote_write_handler_t  write_read_handler;
	  ble_quote_write_handler_t  erase_handler;
	 	bool                         is_notifying;
} ble_quote_t;



uint32_t ble_quote_init(ble_quote_t * p_inert, const ble_quote_init_t * p_inert_init);


void ble_quote_on_ble_evt(ble_quote_t * p_inert, ble_evt_t * p_ble_evt);

//uint32_t ble_quote_notify_accel(ble_quote_t * , struct accelerometer *);
//uint32_t ble_quote_notify_magn(ble_quote_t * p_inert, uint8_t *accel_data);
//uint32_t ble_quote_notify_gyro(ble_quote_t * p_inert, uint8_t *accel_data);
void quote_server_init(void);


#endif 


