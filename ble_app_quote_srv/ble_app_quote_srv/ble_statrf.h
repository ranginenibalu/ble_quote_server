
#ifndef _BLE_STATRF_H_
#define _BLE_STATRF_H_

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"

#define BTS_UUID_BASE {0x23, 0xD1, 0xBC, 0xEA, 0x5F, 0x78, 0x23, 0x15, 0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00}
#define BTS_UUID_SERVICE 0x1523
#define BTS_UUID_TEMP_MEAS_CHAR 0x1525
#define BTS_UUID_DEL_UPD_CHAR 0X1524
#define BTS_UUID_THRES_MEAS_CHAR 0x1526
#define BTS_UUID_THRES_MIN_CHAR 0x1527
#define BTS_UUID_COUNT_CHAR 0x1528

// Forward declaration of the ble_lbs_t type. 
typedef struct ble_statrf_s ble_statrf_t;
typedef void (*ble_statrf_delay_write_handler_t) (ble_statrf_t * p_bts, uint8_t new_state);


typedef struct
{
    ble_statrf_delay_write_handler_t   delay_write_handler; 
					
} ble_statrf_init_t;

typedef struct ble_statrf_s
{         
    uint16_t                     service_handle;                 
    ble_gatts_char_handles_t     pedo_char_handles;   
		ble_gatts_char_handles_t     delay_char_handles;
		ble_gatts_char_handles_t		 count_char_handles;
		uint8_t                      uuid_type;
    uint16_t                     conn_handle;  
		ble_statrf_delay_write_handler_t  delay_write_handler;
		bool                         is_notifying;
} ble_statrf_t;



uint32_t ble_statrf_init(ble_statrf_t * p_statrf, const ble_statrf_init_t * p_statrf_init);


void ble_statrf_on_ble_evt(ble_statrf_t * p_statrf, ble_evt_t * p_ble_evt);


uint32_t ble_statrf_notify_data(ble_statrf_t * p_statrf, uint8_t *current_temp);
uint32_t ble_statrf_notify_count(ble_statrf_t * p_statrf, uint8_t *current_temp);
#endif 


