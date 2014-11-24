ble_quote_server
================


I wrote this application in SDK - nrf sdk v5_2_0_39364
and tested with the softdevice S110 - s110_nrf51822_6.0.0_softdevice

Quote_server
  |_  Service 1
      |__ characteristic 1 ( write/read/notify)
          writing and reading of flash can be done on this characteristic
          
      |__ characteristic 2 ( write/read/notify)
          erasing of flash can be done on this characteristic 


I use some default UUID for both characteristics, if you want you can change them

I tested the application, you can find images of screen shots of - scanning, connection enabling services


void write_read_handler(ble_quote_t *p_bts, uint8_t new_state)
{
uint32_t err_code =0;
}

void erase_handler(ble_quote_t *p_bts, uint8_t new_state)
{
	uint32_t err_code =0;
}

These two functions are the call backs, whenever you wrote some date from the central/Mater control panel 
You can implement your logic here, writing and erasing flash depending on commands sent from the Central/MCP



