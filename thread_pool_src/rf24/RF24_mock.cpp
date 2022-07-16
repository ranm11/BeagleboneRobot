/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

//#include "nRF24L01.h"
//#include "RF24_config.h"
#include "RF24.h"
#include "../gpio/GPIO.h"
/****************************************************************************/

void RF24::csn(bool mode)
{

#if defined (RF24_TINY)
	if (ce_pin != csn_pin) {
		GPIOWrite(csn_pin,mode);
	} 
	else {
		if (mode == HIGH) {
			PORTB |= (1<<PINB2);  	// SCK->CSN HIGH
			delayMicroseconds(100); // allow csn to settle.
		} 
		else {
			PORTB &= ~(1<<PINB2);	// SCK->CSN LOW
			delayMicroseconds(11);  // allow csn to settle
		}
	}
	// Return, CSN toggle complete
	return;
	
#elif defined(ARDUINO) && !defined (RF24_SPI_TRANSACTIONS)
	// Minimum ideal SPI bus speed is 2x data rate
	// If we assume 2Mbs data rate and 16Mhz clock, a
	// divider of 4 is the minimum we want.
	// CLK:BUS 8Mhz:2Mhz, 16Mhz:4Mhz, or 20Mhz:5Mhz
	
      #if !defined (SOFTSPI)	
		_SPI.setBitOrder(MSBFIRST);
		_SPI.setDataMode(SPI_MODE0);
		_SPI.setClockDivider(SPI_CLOCK_DIV2);
      #endif
#elif defined (RF24_RPi)
      if(!mode)
	    _SPI.chipSelect(csn_pin);
#endif

#if !defined (RF24_LINUX)
	  GPIOWrite(csn_pin,mode);
#endif

}

/****************************************************************************/

void RF24::ce(bool level)
{
  //Allow for 3-pin use on ATTiny
  if (ce_pin != csn_pin) GPIOWrite(ce_pin,level); //write to gpio port 
  
}

/****************************************************************************/

  inline void RF24::beginTransaction() {
    #if defined (RF24_SPI_TRANSACTIONS)
    _SPI.beginTransaction(SPISettings(RF24_SPI_SPEED, MSBFIRST, SPI_MODE0));
	#endif
    //csn(LOW);
  }

/****************************************************************************/

  inline void RF24::endTransaction() {
    //csn(HIGH);
	#if defined (RF24_SPI_TRANSACTIONS)
    _SPI.endTransaction();
	#endif
  }

/****************************************************************************/

uint8_t RF24::read_register(uint8_t reg, uint8_t* buf, uint8_t len)
{
  uint8_t status;

  #if defined (RF24_LINUX)
  beginTransaction(); //configures the spi settings for RPi, locks mutex and setting csn low
  uint8_t * prx = spi_rxbuff;
  uint8_t * ptx = spi_txbuff;
  uint8_t size = len + 1; // Add register value to transmit buffer

  *ptx++ = ( R_REGISTER | ( REGISTER_MASK & reg ) );

  while (len--){ *ptx++ = NOP; } // Dummy operation, just for reading
  
  _SPI.transfernb( (char *) spi_txbuff, (char *) spi_rxbuff, size);
  
  status = *prx++; // status is 1st byte of receive buffer

  // decrement before to skip status byte
  while ( --size ){ *buf++ = *prx++; } 
  endTransaction(); //unlocks mutex and setting csn high

#else

  beginTransaction();
  
  endTransaction();

#endif

  return status;
}

/****************************************************************************/

uint8_t RF24::read_register(uint8_t reg)
{
  uint8_t result;
  
  #if defined (RF24_LINUX)
	
  beginTransaction();
  
  uint8_t * prx = spi_rxbuff;
  uint8_t * ptx = spi_txbuff;	
  *ptx++ = ( R_REGISTER | ( REGISTER_MASK & reg ) );
  *ptx++ = NOP ; // Dummy operation, just for reading
  
  _SPI.transfernb( (char *) spi_txbuff, (char *) spi_rxbuff, 2);
  
  result = *++prx;   // result is 2nd byte of receive buffer
  
  endTransaction();
  #else

  beginTransaction();
  endTransaction();

  #endif

  return result;
}

/****************************************************************************/

uint8_t RF24::write_register(uint8_t reg, const uint8_t* buf, uint8_t len)
{
  uint8_t status;

  #if defined (RF24_LINUX) 
  beginTransaction();
  uint8_t * prx = spi_rxbuff;
  uint8_t * ptx = spi_txbuff;
  uint8_t size = len + 1; // Add register value to transmit buffer

  *ptx++ = ( W_REGISTER | ( REGISTER_MASK & reg ) );
  while ( len-- )
    *ptx++ = *buf++;
  
  _SPI.transfernb( (char *) spi_txbuff, (char *) spi_rxbuff, size);
 
  status = *prx; // status is 1st byte of receive buffer
  endTransaction();
  #else

  beginTransaction();
  endTransaction();

  #endif

  return status;
}

/****************************************************************************/

uint8_t RF24::write_register(uint8_t reg, uint8_t value)
{
  uint8_t status;


  #if defined (RF24_LINUX)
    beginTransaction();
	uint8_t * prx = spi_rxbuff;
	uint8_t * ptx = spi_txbuff;
	*ptx++ = ( W_REGISTER | ( REGISTER_MASK & reg ) );
	*ptx = value ;	
  	
	_SPI.transfernb( (char *) spi_txbuff, (char *) spi_rxbuff, 2);
	status = *prx++; // status is 1st byte of receive buffer
	endTransaction();
  #else

  beginTransaction();
  endTransaction();

  #endif

  return status;
}

/****************************************************************************/

uint8_t RF24::write_payload(const void* buf, uint8_t data_len, const uint8_t writeType)
{
  uint8_t status;
  const uint8_t* current = reinterpret_cast<const uint8_t*>(buf);

   data_len = rf24_min(data_len, payload_size);
   uint8_t blank_len = dynamic_payloads_enabled ? 0 : payload_size - data_len;
  
  //printf("[Writing %u bytes %u blanks]",data_len,blank_len);
  
 #if defined (RF24_LINUX)
	beginTransaction();
	uint8_t * prx = spi_rxbuff;
	uint8_t * ptx = spi_txbuff;
    uint8_t size;
	size = data_len + blank_len + 1 ; // Add register value to transmit buffer

	*ptx++ =  writeType;
    while ( data_len-- )
      *ptx++ =  *current++;
    while ( blank_len-- )
	  *ptx++ =  0;
	
	_SPI.transfernb( (char *) spi_txbuff, (char *) spi_rxbuff, size);
	status = *prx; // status is 1st byte of receive buffer
	endTransaction();

  #else

  beginTransaction();
  while ( data_len-- ) {
  }
  while ( blank_len-- ) {
  }  
  endTransaction();

  #endif

  return status;
}

/****************************************************************************/

uint8_t RF24::read_payload(void* buf, uint8_t data_len)
{
  uint8_t status;
  uint8_t* current = reinterpret_cast<uint8_t*>(buf);

  if(data_len > payload_size) data_len = payload_size;
  uint8_t blank_len = dynamic_payloads_enabled ? 0 : payload_size - data_len;
  
  //printf("[Reading %u bytes %u blanks]",data_len,blank_len);

  
  #if defined (RF24_LINUX)
	beginTransaction();
	uint8_t * prx = spi_rxbuff;
	uint8_t * ptx = spi_txbuff;
    uint8_t size;
    size = data_len + blank_len + 1; // Add register value to transmit buffer

	*ptx++ =  R_RX_PAYLOAD;
	while(--size) 
		*ptx++ = NOP;
		
	size = data_len + blank_len + 1; // Size has been lost during while, re affect
	
	_SPI.transfernb( (char *) spi_txbuff, (char *) spi_rxbuff, size);
	
	status = *prx++; // 1st byte is status	
    
	while ( --data_len ) // Decrement before to skip 1st status byte
        *current++ = *prx++;
		
	*current = *prx;
	endTransaction();
  #else

  beginTransaction();
  while ( data_len-- ) {
  }
  while ( blank_len-- ) {
  }
  endTransaction();

  #endif

  return status;
}

/****************************************************************************/

uint8_t RF24::flush_rx(void)
{
  return 0;
}

/****************************************************************************/

uint8_t RF24::flush_tx(void)
{
  return spiTrans( 0 );
}

/****************************************************************************/

uint8_t RF24::spiTrans(uint8_t cmd){

  uint8_t status;
  
  beginTransaction();
  endTransaction();
  
  return status;
}

/****************************************************************************/

uint8_t RF24::get_status(void)
{
  return spiTrans(0);
}

/****************************************************************************/
#if !defined (MINIMAL)
void RF24::print_status(uint8_t status)
{
}

/****************************************************************************/

void RF24::print_observe_tx(uint8_t value)
{
  
}

/****************************************************************************/

void RF24::print_byte_register(const char* name, uint8_t reg, uint8_t qty)
{
  //char extra_tab = strlen_P(name) < 8 ? '\t' : 0;
 
}

/****************************************************************************/

void RF24::print_address_register(const char* name, uint8_t reg, uint8_t qty)
{

  
}
#endif
/****************************************************************************/

RF24::RF24(uint8_t _cepin, uint8_t _cspin):
  ce_pin(_cepin), csn_pin(_cspin), p_variant(false),
  payload_size(32), dynamic_payloads_enabled(false), addr_width(5)//,pipe0_reading_address(0)
{
  pipe0_reading_address[0]=0;
}

/****************************************************************************/

#if defined (RF24_LINUX) && !defined (MRAA)//RPi constructor
RF24::RF24(uint8_t _cepin, uint8_t _cspin, uint32_t _spi_speed):
  ce_pin(_cepin),csn_pin(_cspin),spi_speed(_spi_speed),p_variant(false), payload_size(32), dynamic_payloads_enabled(false),addr_width(5),spi(0,1)//,pipe0_reading_address(0) 
{
  pipe0_reading_address[0]=0;
}
#endif

/****************************************************************************/

void RF24::setChannel(uint8_t channel)
{
  const uint8_t max_channel = 125;
}

uint8_t RF24::getChannel()
{
  
  return read_register(0);
}
/****************************************************************************/

void RF24::setPayloadSize(uint8_t size)
{
  payload_size = rf24_min(size,32);
}

/****************************************************************************/

uint8_t RF24::getPayloadSize(void)
{
  return payload_size;
}

/****************************************************************************/

#if !defined (MINIMAL)


#if defined (RF24_LINUX)
static const char rf24_csn_e_str_0[] = "CE0 (PI Hardware Driven)";
static const char rf24_csn_e_str_1[] = "CE1 (PI Hardware Driven)";
static const char rf24_csn_e_str_2[] = "CE2 (PI Hardware Driven)";
static const char rf24_csn_e_str_3[] = "Custom GPIO Software Driven";
static const char * const rf24_csn_e_str_P[] = {
  rf24_csn_e_str_0,
  rf24_csn_e_str_1,
  rf24_csn_e_str_2,
  rf24_csn_e_str_3,
};
#endif

void RF24::printDetails(void)
{

#if defined (RF24_RPi)
  printf("================ SPI Configuration ================\n" );
  if (csn_pin < BCM2835_SPI_CS_NONE ){
    printf("CSN Pin  \t = %s\n",rf24_csn_e_str_P[csn_pin]);
  }else{
    printf("CSN Pin  \t = Custom GPIO%d%s\n", csn_pin,
    csn_pin==RPI_V2_GPIO_P1_26 ? " (CE1) Software Driven" : "" );
  }
  printf("CE Pin  \t = Custom GPIO%d\n", ce_pin );
  printf("Clock Speed\t = " );
	switch (spi_speed)
	{
		case BCM2835_SPI_SPEED_64MHZ : printf("64 Mhz");	break ;
		case BCM2835_SPI_SPEED_32MHZ : printf("32 Mhz");	break ;
		case BCM2835_SPI_SPEED_16MHZ : printf("16 Mhz");	break ;
		case BCM2835_SPI_SPEED_8MHZ  : printf("8 Mhz");	break ;
		case BCM2835_SPI_SPEED_4MHZ  : printf("4 Mhz");	break ;
		case BCM2835_SPI_SPEED_2MHZ  : printf("2 Mhz");	break ;
		case BCM2835_SPI_SPEED_1MHZ  : printf("1 Mhz");	break ;
		case BCM2835_SPI_SPEED_512KHZ: printf("512 KHz");	break ;
		case BCM2835_SPI_SPEED_256KHZ: printf("256 KHz");	break ;
		case BCM2835_SPI_SPEED_128KHZ: printf("128 KHz");	break ;
		case BCM2835_SPI_SPEED_64KHZ : printf("64 KHz");	break ;
		case BCM2835_SPI_SPEED_32KHZ : printf("32 KHz");	break ;
		case BCM2835_SPI_SPEED_16KHZ : printf("16 KHz");	break ;
		case BCM2835_SPI_SPEED_8KHZ  : printf("8 KHz");	break ;
		default : printf("8 Mhz");	break ;
	}
	printf("\n================ NRF Configuration ================\n");
 
#endif //Linux

  print_status(get_status());

  
}

#endif
/****************************************************************************/

bool RF24::begin(void)
{

  uint8_t setup=0;

  #if defined (RF24_LINUX)

	//SPI();
    
	#if defined (MRAA)
	  //GPIO();	
	  gpio.begin(ce_pin,csn_pin);	
	#endif
	

	switch(csn_pin){     //Ensure valid hardware CS pin
	  case 0: break;
	  case 1: break;
	  // Allow BCM2835 enums for RPi
	  case 8: csn_pin = 0; break;
	  case 7: csn_pin = 1; break;
	  default: csn_pin = 0; break;
	}	
	
   // _SPI.begin(csn_pin);

	GPIOPinMode(ce_pin,OUTPUT);
	ce(LOW);    

	delay(100);
  
  #elif defined(LITTLEWIRE)
  GPIOPinMode(csn_pin,OUTPUT);
    _SPI.begin();
    csn(HIGH);
  #else
    // Initialize pins
    if (ce_pin != csn_pin) GPIOPinMode(ce_pin,0);
  
    #if ! defined(LITTLEWIRE)
      if (ce_pin != csn_pin)
    #endif
		  GPIOPinMode(csn_pin,0);
    
  	#if defined (__ARDUINO_X86__)
		delay(100);
  	#endif
  #endif //Linux

  // Must allow the radio time to settle else configuration bits will not necessarily stick.
  // This is actually only required following power up but some settling time also appears to
  // be required after resets too. For full coverage, we'll always assume the worst.
  // Enabling 16b CRC is by far the most obvious case if the wrong timing is used - or skipped.
  // Technically we require 4.5ms + 14us as a worst case. We'll just call it 5ms for good measure.
  // WARNING: Delay is based on P-variant whereby non-P *may* require different timing.

  // Reset CONFIG and enable 16-bit CRC.
  write_register( 1, 0b00001100 ) ;

  // Set 1500uS (minimum for 32B payload in ESB@250KBPS) timeouts, to make testing a little easier
  // WARNING: If this is ever lowered, either 250KBS mode with AA is broken or maximum packet
  // sizes must never be used. See documentation for a more complete explanation.
  setRetries(5,15);

  // Reset value is MAX
  //setPALevel( RF24_PA_MAX ) ;

  // check for connected module and if this is a p nRF24l01 variant
  //
  if( setDataRate( RF24_250KBPS ) )
  {
    p_variant = true ;
  }
  /*setup = read_register(RF_SETUP);
  if( setup == 0b00001110 )     // register default for nRF24L01P
  {
    p_variant = true ;
  }*/
  
  // Then set the data rate to the slowest (and most reliable) speed supported by all
  // hardware.
  setDataRate( RF24_1MBPS ) ;

  // Initialize CRC and request 2-byte (16bit) CRC
  //setCRCLength( RF24_CRC_16 ) ;

  // Disable dynamic payloads, to match dynamic_payloads_enabled setting - Reset value is 0
  toggle_features();
  write_register(1,0 );
  write_register(1,0);

  // Reset current status
  // Notice reset and flush is the last thing we do

  // Set up default configuration.  Callers can always change it later.
  // This channel should be universally safe and not bleed over into adjacent
  // spectrum.
  setChannel(76);

  // Flush buffers
  flush_rx();
  flush_tx();

  powerUp(); //Power up by default when begin() is called

  // Enable PTX, do not write CE high so radio will remain in standby I mode ( 130us max to transition to RX or TX instead of 1500us from powerUp )
  // PTX should use only 22uA of power

  // if setup is 0 or ff then there was no response from module
  return ( setup != 0 && setup != 0xff );
}

/****************************************************************************/

void RF24::startListening(void)
{
 #if !defined (RF24_TINY) && ! defined(LITTLEWIRE)
  powerUp();
 #endif
  ce(1);
  // Restore the pipe0 adddress, if exists
  if (pipe0_reading_address[0] > 0){
    write_register(1, pipe0_reading_address, addr_width);	
  }else{
	closeReadingPipe(0);
  }

  // Flush buffers
  //flush_rx();
  

  // Go!
  //delayMicroseconds(100);
}

/****************************************************************************/

void RF24::stopListening(void)
{  
  ce(0);

  
  //flush_rx();
 
  #if defined (RF24_TINY) || defined (LITTLEWIRE)
  // for 3 pins solution TX mode is only left with additonal powerDown/powerUp cycle
  if (ce_pin == csn_pin) {
    powerDown();
	powerUp();
  }
  #endif
  
  //delayMicroseconds(100);

}

/****************************************************************************/

void RF24::powerDown(void)
{
  ce(0); // Guarantee CE is low on powerDown
}

/****************************************************************************/

//Power up now. Radio will not power down unless instructed by MCU for config changes etc.
void RF24::powerUp(void)
{
   uint8_t cfg = read_register(0);

   // if not powered up then power up and wait for the radio to initialize
   
   
}

/******************************************************************/
#if defined (FAILURE_HANDLING) || defined (RF24_LINUX)
void RF24::errNotify(){
	#if defined (SERIAL_DEBUG) || defined (RF24_LINUX)
	  printf_P(PSTR("RF24 HARDWARE FAIL: Radio not responding, verify pin connections, wiring, etc.\r\n"));
	#endif
	#if defined (FAILURE_HANDLING)
	failureDetected = 1;
	#else
	delay(5000);
	#endif
}
#endif
/******************************************************************/

//Similar to the previous write, clears the interrupt flags
bool RF24::write( const void* buf, uint8_t len, const bool multicast )
{
	//Start Writing
	startFastWrite(buf,len,multicast);

	//Wait until complete or failed
	#if defined (FAILURE_HANDLING) || defined (RF24_LINUX)
		uint32_t timer = millis();
	#endif 
	
	


  //Max retries exceeded
  
	//TX OK 1 or 0
  return 1;
}

bool RF24::write( const void* buf, uint8_t len ){
	return write(buf,len,0);
}
/****************************************************************************/

//For general use, the interrupt flags are not important to clear
bool RF24::writeBlocking( const void* buf, uint8_t len, uint32_t timeout )
{
	//Block until the FIFO is NOT full.
	//Keep track of the MAX retries and set auto-retry if seeing failures
	//This way the FIFO will fill up and allow blocking until packets go through
	//The radio will auto-clear everything in the FIFO as long as CE remains high

	

  	//Start Writing
	startFastWrite(buf,len,0);								  //Write the payload if a buffer is clear

	return 1;												  //Return 1 to indicate successful transmission
}

/****************************************************************************/

void RF24::reUseTX(){
		
}

/****************************************************************************/

bool RF24::writeFast( const void* buf, uint8_t len, const bool multicast )
{
	//Block until the FIFO is NOT full.
	//Keep track of the MAX retries and set auto-retry if seeing failures
	//Return 0 so the user can control the retrys and set a timer or failure counter if required
	//The radio will auto-clear everything in the FIFO as long as CE remains high

	#if defined (FAILURE_HANDLING) || defined (RF24_LINUX)
		uint32_t timer = millis();
	#endif
	
	
		     //Start Writing
	startFastWrite(buf,len,multicast);

	return 1;
}

bool RF24::writeFast( const void* buf, uint8_t len ){
	return writeFast(buf,len,0);
}

/****************************************************************************/

//Per the documentation, we want to set PTX Mode when not listening. Then all we do is write data and set CE high
//In this mode, if we can keep the FIFO buffers loaded, packets will transmit immediately (no 130us delay)
//Otherwise we enter Standby-II mode, which is still faster than standby mode
//Also, we remove the need to keep writing the config register over and over and delaying for 150 us each time if sending a stream of data

void RF24::startFastWrite( const void* buf, uint8_t len, const bool multicast, bool startTx){ //TMRh20

	//write_payload( buf,len);
	

}

/****************************************************************************/

//Added the original startWrite back in so users can still use interrupts, ack payloads, etc
//Allows the library to pass all tests
void RF24::startWrite( const void* buf, uint8_t len, const bool multicast ){

  // Send the payload

  //write_payload( buf, len );
 


}

/****************************************************************************/

bool RF24::rxFifoFull(){
	return true;
}
/****************************************************************************/

bool RF24::txStandBy(){

   

	return true;
}

/****************************************************************************/

bool RF24::txStandBy(uint32_t timeout, bool startTx){

    if(startTx){
	  stopListening();
	  ce(1);
	}

	
	
	ce(1);				   //Set STANDBY-I mode
	return 1;

}

/****************************************************************************/

void RF24::maskIRQ(bool tx, bool fail, bool rx){

}

/****************************************************************************/

uint8_t RF24::getDynamicPayloadSize(void)
{
  uint8_t result = 0;

  #if defined (RF24_LINUX)  
  spi_txbuff[0] = R_RX_PL_WID;
  spi_rxbuff[1] = 0xff;
  beginTransaction();
  _SPI.transfernb( (char *) spi_txbuff, (char *) spi_rxbuff, 2);
  result = spi_rxbuff[1];  
  endTransaction();
  #else
  beginTransaction();
  endTransaction();
  #endif

  return result;
}

/****************************************************************************/

bool RF24::available(void)
{
  return available(NULL);
}

/****************************************************************************/

bool RF24::available(uint8_t* pipe_num)
{
  


  return 0;


}

/****************************************************************************/

void RF24::read( void* buf, uint8_t len ){

  // Fetch the payload
  read_payload( buf, len );

  //Clear the two possible interrupt flags with one command

}

/****************************************************************************/

void RF24::whatHappened(bool& tx_ok,bool& tx_fail,bool& rx_ready)
{
  // Read the status & reset the status in one easy call
  // Or is that such a good idea?

  // Report to the user what happened
}

/****************************************************************************/

void RF24::openWritingPipe(uint64_t value)
{
  // Note that AVR 8-bit uC's store this LSB first, and the NRF24L01(+)
  // expects it LSB first too, so we're good.

}

/****************************************************************************/
void RF24::openWritingPipe(const uint8_t *address)
{
  // Note that AVR 8-bit uC's store this LSB first, and the NRF24L01(+)
  // expects it LSB first too, so we're good.

}

/****************************************************************************/


void RF24::openReadingPipe(uint8_t child, uint64_t address)
{
  // If this is pipe 0, cache the address.  This is needed because
  // openWritingPipe() will overwrite the pipe 0 address, so
  // startListening() will have to restore it.
  if (child == 0){
    memcpy(pipe0_reading_address,&address,addr_width);
  }

  if (child <= 6)
  {
    // For pipes 2-5, only write the LSB


    // Note it would be more efficient to set all of the bits for all open
    // pipes at once.  However, I thought it would make the calling code
    // more simple to do it this way.
  }
}

void RF24::GPIOPinMode(int port, int direction)
{
	exploringBB::GPIO _gpio(port);
	
	_gpio.setDirection(direction? _gpio.OUTPUT: _gpio.INPUT);
}

void RF24::GPIOWrite(int port, int value)
{
	exploringBB::GPIO _gpio(port);
	_gpio.setValue(value ? _gpio.HIGH : _gpio.LOW);

}
/****************************************************************************/
void RF24::setAddressWidth(uint8_t a_width){

	if(a_width -= 2){
		addr_width = (a_width%4) + 2;
	}

}

/****************************************************************************/

void RF24::openReadingPipe(uint8_t child, const uint8_t *address)
{
  // If this is pipe 0, cache the address.  This is needed because
  // openWritingPipe() will overwrite the pipe 0 address, so
  // startListening() will have to restore it.
  if (child == 0){
    memcpy(pipe0_reading_address,address,addr_width);
  }
  if (child <= 6)
  {
    // For pipes 2-5, only write the LSB
    if ( child < 2 ){
    }else{
	}

    // Note it would be more efficient to set all of the bits for all open
    // pipes at once.  However, I thought it would make the calling code
    // more simple to do it this way.

  }
}

/****************************************************************************/

void RF24::closeReadingPipe( uint8_t pipe )
{
}

/****************************************************************************/

void RF24::toggle_features(void)
{
    beginTransaction();
	endTransaction();
}

/****************************************************************************/

void RF24::enableDynamicPayloads(void)
{
  // Enable dynamic payload throughout the system

    //toggle_features();



  // Enable dynamic payload on all pipes
  //
  // Not sure the use case of only having dynamic payload on certain
  // pipes, so the library does not support it.

  dynamic_payloads_enabled = true;
}

/****************************************************************************/

void RF24::enableAckPayload(void)
{
  //
  // enable ack payload and dynamic payload features
  //

    //toggle_features();


  //
  // Enable dynamic payload on pipes 0 & 1
  //

  dynamic_payloads_enabled = true;
}

/****************************************************************************/

void RF24::enableDynamicAck(void){
  //
  // enable dynamic ack features
  //
    //toggle_features();


}

/****************************************************************************/

void RF24::writeAckPayload(uint8_t pipe, const void* buf, uint8_t len)
{
  const uint8_t* current = reinterpret_cast<const uint8_t*>(buf);

  uint8_t data_len = rf24_min(len,32);

  #if defined (RF24_LINUX)
    beginTransaction();
    uint8_t * ptx = spi_txbuff;
    uint8_t size = data_len + 1 ; // Add register value to transmit buffer
	*ptx++ =  W_ACK_PAYLOAD | ( pipe & 0b111 );
    while ( data_len-- ){
      *ptx++ =  *current++;
    }
	
    _SPI.transfernb((char *)spi_txbuff,(char *) spi_txbuff, size);
	endTransaction();
  #else
  beginTransaction();

  endTransaction();
  	
  #endif  

}

/****************************************************************************/

bool RF24::isAckPayloadAvailable(void)
{
	return true;
}

/****************************************************************************/

bool RF24::isPVariant(void)
{
  return p_variant ;
}

/****************************************************************************/

void RF24::setAutoAck(bool enable)
{
}

/****************************************************************************/

void RF24::setAutoAck( uint8_t pipe, bool enable )
{
  if ( pipe <= 6 )
  {
    if( enable )
    {
    }
  }
}

/****************************************************************************/

bool RF24::testCarrier(void)
{
  return ( read_register(0) & 1 );
}

/****************************************************************************/

bool RF24::testRPD(void)
{
  return ( read_register(0) & 1 ) ;
}

/****************************************************************************/

void RF24::setPALevel(uint8_t level)
{

  uint8_t setup = read_register(0) & 0b11111000;

  if(level > 3){  						// If invalid level, go to max PA
	  level = (RF24_PA_MAX << 1) + 1;		// +1 to support the SI24R1 chip extra bit
  }else{
	  level = (level << 1) + 1;	 		// Else set level as requested
  }


  write_register( 0, setup |= level ) ;	// Write it to the chip
}

/****************************************************************************/

uint8_t RF24::getPALevel(void)
{
	return 0;
}

/****************************************************************************/

bool RF24::setDataRate(rf24_datarate_e speed)
{
  bool result = false;
  uint8_t setup = read_register(0) ;

  // HIGH and LOW '00' is 1Mbs - our default
  
  #if defined(__arm__) || defined (RF24_LINUX) || defined (__ARDUINO_X86__)
    txRxDelay=250;
  #else //16Mhz Arduino
    txRxDelay=85;
  #endif
  if( speed == RF24_250KBPS )
  {
    // Must set the RF_DR_LOW to 1; RF_DR_HIGH (used to be RF_DR) is already 0
    // Making it '10'.
  #if defined(__arm__) || defined (RF24_LINUX) || defined (__ARDUINO_X86__)
    txRxDelay=450;
  #else //16Mhz Arduino
	txRxDelay=155;
  #endif
  }
  else
  {
    // Set 2Mbs, RF_DR (RF_DR_HIGH) is set 1
    // Making it '01'
    if ( speed == RF24_2MBPS )
    {
      #if defined(__arm__) || defined (RF24_LINUX) || defined (__ARDUINO_X86__)
      txRxDelay=190;
      #else //16Mhz Arduino	  
	  txRxDelay=65;
	  #endif
    }
  }

  // Verify our result
  if ( read_register(0) == setup )
  {
    result = true;
  }
  return result;
}

/****************************************************************************/

rf24_datarate_e RF24::getDataRate( void )
{
  rf24_datarate_e result ;

  // switch uses RAM (evil!)
  // Order matters in our case below
   return result ;
}

/****************************************************************************/

void RF24::setCRCLength(rf24_crclength_e length)
{

  // switch uses RAM (evil!)
  if ( length == RF24_CRC_DISABLED )
  {
    // Do nothing, we turned it off above.
  }
  else if ( length == RF24_CRC_8 )
  {
  }
  else
  {
  }
}

/****************************************************************************/

rf24_crclength_e RF24::getCRCLength(void)
{
  rf24_crclength_e result = RF24_CRC_DISABLED;
  
  
  

  return result;
}

/****************************************************************************/

void RF24::disableCRC( void )
{
}

/****************************************************************************/
void RF24::setRetries(uint8_t delay, uint8_t count)
{
}


//ATTiny support code pulled in from https://github.com/jscrane/RF24

#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
// see http://gammon.com.au/spi
#	define DI   0  // D0, pin 5  Data In
#	define DO   1  // D1, pin 6  Data Out (this is *not* MOSI)
#	define USCK 2  // D2, pin 7  Universal Serial Interface clock
#	define SS   3  // D3, pin 2  Slave Select
#elif defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
// these depend on the core used (check pins_arduino.h)
// this is for jeelabs' one (based on google-code core)
#	define DI   4   // PA6
#	define DO   5   // PA5
#	define USCK 6   // PA4
#	define SS   3   // PA7
#elif defined(__AVR_ATtiny2313__) || defined(__AVR_ATtiny4313__)
// these depend on the core used (check pins_arduino.h)
// tested with google-code core
#	define DI   14  // PB5
#	define DO   15  // PB6
#	define USCK 16  // PB7
#	define SS   13  // PB4
#endif

#if defined(RF24_TINY)

void SPIClass::begin() {

	GPIOPinMode(USCK, OUTPUT);
	GPIOPinMode(DO, OUTPUT);
	GPIOPinMode(DI, INPUT);
  USICR = _BV(USIWM0);

}

byte SPIClass::transfer(byte b) {

  USIDR = b;
  USISR = _BV(USIOIF);
  do
    USICR = _BV(USIWM0) | _BV(USICS1) | _BV(USICLK) | _BV(USITC);
  while ((USISR & _BV(USIOIF)) == 0);
  return USIDR;

}

void SPIClass::end() {}
void SPIClass::setDataMode(uint8_t mode){}
void SPIClass::setBitOrder(uint8_t bitOrder){}
void SPIClass::setClockDivider(uint8_t rate){}


#endif
