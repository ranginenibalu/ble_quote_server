

#include "ble_quote.h"
#include "nrf_delay.h"
#include <string.h>
#include "nordic_common.h"
#include "ble_srv_common.h"
#include "app_util.h"

#include "nrf_gpio.h"

#include "ble_quote.h"


static void on_connect(ble_quote_t * p_quote, ble_evt_t * p_ble_evt)
{
    p_quote->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}

static void on_disconnect(ble_quote_t * p_quote, ble_evt_t * p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_quote->conn_handle = BLE_CONN_HANDLE_INVALID;
}


/************************************/
// here you need to change where it needs to take care of writing and all
static void on_write(ble_quote_t * p_quote, ble_evt_t * p_ble_evt)
{
    ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;
   
	
	if ((p_evt_write->handle == p_quote->write_char_handles.value_handle) &&
       (p_evt_write->len == 1) &&
       (p_quote->write_read_handler != NULL))
   {
		   p_quote->write_read_handler(p_quote, p_evt_write->data[0]);
   }
	 if ((p_evt_write->handle == p_quote->erase_char_handles.value_handle) &&
       (p_evt_write->len == 1) &&
       (p_quote->erase_handler != NULL))
   {
		   p_quote->erase_handler(p_quote, p_evt_write->data[0]);
   }
	 
}



void ble_quote_on_ble_evt(ble_quote_t * p_quote, ble_evt_t * p_ble_evt)
{
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_quote, p_ble_evt);
            break;
            
        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_quote, p_ble_evt);
            break;
            
        case BLE_GATTS_EVT_WRITE:
					  on_write(p_quote, p_ble_evt);
            break;
            
        default:
            break;
    }
}



/*******************************/
/* one service
				2 characteristics
					- with write capability(when wrote from application)
					
*/
static uint32_t write_char_add(ble_quote_t * p_quote, const ble_quote_init_t * p_quote_init)
{
		ble_gatts_char_md_t char_md;
		ble_gatts_attr_md_t cccd_md;
	  ble_gatts_attr_t attr_char_value;
		ble_uuid_t          ble_uuid;
		ble_gatts_attr_md_t attr_md;
		
				
		memset(&cccd_md, 0, sizeof(cccd_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    
    cccd_md.vloc = BLE_GATTS_VLOC_STACK;     //Attribute Value is located in stack memory, no user memory is required.
    
    memset(&char_md, 0, sizeof(char_md));
	
	
	
	  char_md.char_props.read = 1;
		char_md.char_props.write = 1;
		char_md.char_props.notify = 1; // notification are supported for this characteristic
		char_md.p_char_user_desc = NULL;
		char_md.p_char_pf = NULL;
		char_md.p_user_desc_md = NULL;
		char_md.p_cccd_md = &cccd_md;
		char_md.p_sccd_md = NULL;
	
		ble_uuid.type = p_quote->uuid_type;
    ble_uuid.uuid = BTS_UUID_WRITE_CHAR;
		
		memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 1;
		
		memset(&attr_char_value, 0, sizeof(attr_char_value));
		
	  attr_char_value.p_uuid       = &ble_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = sizeof(uint8_t);
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = 25;//sizeof(uint8_t);
    attr_char_value.p_value      = NULL;
		
		
		
	return sd_ble_gatts_characteristic_add(p_quote->service_handle, &char_md,
                                               &attr_char_value,
                                               &p_quote->write_char_handles);
}

static uint32_t erase_char_add(ble_quote_t * p_quote, const ble_quote_init_t * p_quote_init)
{
	ble_gatts_char_md_t char_md;
		ble_gatts_attr_md_t cccd_md;
	  ble_gatts_attr_t attr_char_value;
		ble_uuid_t          ble_uuid;
		ble_gatts_attr_md_t attr_md;
		
				
		memset(&cccd_md, 0, sizeof(cccd_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    
    cccd_md.vloc = BLE_GATTS_VLOC_STACK;     //Attribute Value is located in stack memory, no user memory is required.
    
    memset(&char_md, 0, sizeof(char_md));
	
	
	
	  char_md.char_props.read = 1;
		char_md.char_props.write = 1;
		char_md.char_props.notify = 1; // notification are supported for this characteristic
		char_md.p_char_user_desc = NULL;
		char_md.p_char_pf = NULL;
		char_md.p_user_desc_md = NULL;
		char_md.p_cccd_md = &cccd_md;
		char_md.p_sccd_md = NULL;
	
		ble_uuid.type = p_quote->uuid_type;
    ble_uuid.uuid = BTS_UUID_ERASE_CHAR;
		
		memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 1;
		
		memset(&attr_char_value, 0, sizeof(attr_char_value));
		
	  attr_char_value.p_uuid       = &ble_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = sizeof(uint8_t);
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = 25;//sizeof(uint8_t);
    attr_char_value.p_value      = NULL;
		
		
		
	return sd_ble_gatts_characteristic_add(p_quote->service_handle, &char_md,
                                               &attr_char_value,
                                               &p_quote->erase_char_handles);
}

uint32_t ble_quote_init(ble_quote_t * p_quote, const ble_quote_init_t * p_quote_init)
{
    uint32_t   err_code;
    ble_uuid_t ble_uuid;

    // Initialize service structure
    p_quote->conn_handle       = BLE_CONN_HANDLE_INVALID;
    	
	
		p_quote->write_read_handler =  p_quote_init->write_read_handler;
	  p_quote->erase_handler =  p_quote_init->erase_handler;
	  
	
    // Add base UUID to softdevice's internal list. 
    ble_uuid128_t base_uuid = BTS_UUID_BASE;
    err_code = sd_ble_uuid_vs_add(&base_uuid, &p_quote->uuid_type);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    
    ble_uuid.type = p_quote->uuid_type;
    ble_uuid.uuid = BTS_UUID_SERVICE;

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_quote->service_handle);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    
		
		// accelerometer characteristic
    err_code = write_char_add(p_quote, p_quote_init);
    if (err_code != NRF_SUCCESS)
   {
        return err_code;
    }
		
		// magnetometer characteristic 
		err_code = erase_char_add(p_quote, p_quote_init);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
		
		
     
	
    return NRF_SUCCESS;
}

uint32_t ble_quote_notify_write(ble_quote_t * p_quote, uint8_t command_data)
{
	  ble_gatts_hvx_params_t params;
    uint16_t len = 6;			// this can be change later depending on the data obtained	
    memset(&params, 0, sizeof(params));
    params.type = BLE_GATT_HVX_NOTIFICATION;
    params.handle = p_quote->write_char_handles.value_handle;
    params.p_data = &command_data;
    params.p_len = &len;
	
	  
		return sd_ble_gatts_hvx(p_quote->conn_handle, &params);
}

uint32_t ble_quote_notify_erase(ble_quote_t * p_quote, uint8_t command_data)
{
	  ble_gatts_hvx_params_t params;
    uint16_t len = 1;			// this can be change later depending on the data obtained		
    memset(&params, 0, sizeof(params));
    params.type = BLE_GATT_HVX_NOTIFICATION;
    params.handle = p_quote->erase_char_handles.value_handle;
    params.p_data = &command_data;
    params.p_len = &len;
	
	  
		return sd_ble_gatts_hvx(p_quote->conn_handle, &params);
}



