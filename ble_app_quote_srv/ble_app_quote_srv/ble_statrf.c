

#include "ble_statrf.h"
#include <string.h>
#include "nordic_common.h"
#include "ble_srv_common.h"
#include "app_util.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"

#define EVAL_BOARD_LED_0           18   
#define LEDBUTTON_LED_PIN_NO            EVAL_BOARD_LED_0

static void on_connect(ble_statrf_t * p_statrf, ble_evt_t * p_ble_evt)
{
    p_statrf->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}


/**@brief Disconnect event handler.
 *
 * @param[in]   p_lbs       LEDButton Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_disconnect(ble_statrf_t * p_statrf, ble_evt_t * p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_statrf->conn_handle = BLE_CONN_HANDLE_INVALID;
}

static void on_write(ble_statrf_t * p_statrf, ble_evt_t * p_ble_evt)
{
    ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;
   
	
	if ((p_evt_write->handle == p_statrf->delay_char_handles.value_handle) &&
       (p_evt_write->len == 1) &&
       (p_statrf->delay_write_handler != NULL))
   {
		   p_statrf->delay_write_handler(p_statrf, p_evt_write->data[0]);
   }
	 
}
void ble_statrf_on_ble_evt(ble_statrf_t * p_statrf, ble_evt_t * p_ble_evt)
{
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_statrf, p_ble_evt);
            break;
            
        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_statrf, p_ble_evt);
            break;
            
        case BLE_GATTS_EVT_WRITE:
					  on_write(p_statrf, p_ble_evt);
            break;
            
        default:
            break;
    }
}

static uint32_t pedo_char_add(ble_statrf_t * p_statrf, const ble_statrf_init_t * p_statrf_init)
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
	
		ble_uuid.type = p_statrf->uuid_type;
    ble_uuid.uuid = BTS_UUID_TEMP_MEAS_CHAR;
		
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
		
		
		
	return sd_ble_gatts_characteristic_add(p_statrf->service_handle, &char_md,
                                               &attr_char_value,
                                               &p_statrf->pedo_char_handles);
}
static uint32_t count_char_add(ble_statrf_t * p_statrf, const ble_statrf_init_t * p_statrf_init)
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
	
		ble_uuid.type = p_statrf->uuid_type;
    ble_uuid.uuid = BTS_UUID_COUNT_CHAR;
		
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
		
		
		
	return sd_ble_gatts_characteristic_add(p_statrf->service_handle, &char_md,
                                               &attr_char_value,
                                               &p_statrf->count_char_handles);
}
uint32_t ble_statrf_init(ble_statrf_t * p_statrf, const ble_statrf_init_t * p_statrf_init)
{
    uint32_t   err_code;
    ble_uuid_t ble_uuid;

    // Initialize service structure
    p_statrf->conn_handle       = BLE_CONN_HANDLE_INVALID;
    	
    // Add base UUID to softdevice's internal list. 
    ble_uuid128_t base_uuid = BTS_UUID_BASE;
    err_code = sd_ble_uuid_vs_add(&base_uuid, &p_statrf->uuid_type);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    
    ble_uuid.type = p_statrf->uuid_type;
    ble_uuid.uuid = BTS_UUID_SERVICE;

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_statrf->service_handle);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    
    err_code = pedo_char_add(p_statrf, p_statrf_init);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
     
	//	err_code = count_char_add(p_statrf, p_statrf_init);
  //  if (err_code != NRF_SUCCESS)
  //  {
  //      return err_code;
  //  }
    return NRF_SUCCESS;
}

uint32_t ble_statrf_notify_data(ble_statrf_t * p_statrf, uint8_t *current_temp)
{
	  ble_gatts_hvx_params_t params;
    //uint16_t len = 10;			
		uint16_t len = 5;			
    memset(&params, 0, sizeof(params));
    params.type = BLE_GATT_HVX_NOTIFICATION;
    params.handle = p_statrf->pedo_char_handles.value_handle;
    params.p_data = current_temp;
    params.p_len = &len;
	
	  
		return sd_ble_gatts_hvx(p_statrf->conn_handle, &params); 
}

uint32_t ble_statrf_notify_count(ble_statrf_t * p_statrf, uint8_t *current_temp)
{
	  ble_gatts_hvx_params_t params;
    uint16_t len = 5;			
    memset(&params, 0, sizeof(params));
    params.type = BLE_GATT_HVX_NOTIFICATION;
    params.handle = p_statrf->count_char_handles.value_handle;
    params.p_data = current_temp;
    params.p_len = &len;
	
	  
		return sd_ble_gatts_hvx(p_statrf->conn_handle, &params); 
}




