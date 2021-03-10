#include <BT_Flash_Commands.h>
#include <BT_Flash_Data.h>

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern uint8_t buf[128];


uint8_t BT_WriteBuffer[147];
uint8_t BT_ReadBuffer[147];
HAL_StatusTypeDef Status;

//////////////////////////////////
////////Flash Commands////////////
//////////////////////////////////

//OPEN FLASH MEMORY
uint8_t open_Transmit[17] = {0x01,0x05,0x04,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t open_Response[21] = {0x04,0x0F,0x04,0x00,0x01,0x05,0x04,0x04,0x03,0x0B,0x00,0xFF,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00};

//READ COMMAND
uint8_t read_Transmit[19] = {0x02,0xFF,0x0F,0x0E,0x00,0x10,0x01,0x0A,0x00,0x03,0x00,0x00,0x40,0x03,0x00,0x80,0x00,0x00,0x00};
uint8_t read_Response[19] = {0x04,0x13,0x05,0x01,0xFF,0x0F,0x01,0x00,0x02,0xFF,0x0F,0x86,0x00,0x10,0x01,0x82,0x00,0x00,0x00};

//ERASE ALL FLASH
uint8_t erase_Transmit[19] = {0x02,0xFF,0x0F,0x0E,0x00,0x12,0x01,0x0A,0x00,0x03,0x00,0x00,0x40,0x03,0x00,0x00,0x20,0x00,0x00};
uint8_t erase_Response[27] = {0x04,0x13,0x05,0x01,0xFF,0x0F,0x01,0x00,0x02,0xFF,0x0F,0x0E,0x00,0x12,0x01,0x0A,0x00,0x00,0x00,0x00,0x40,0x03,0x00,0x00,0x20,0x00,0x00};

//WRITE COMMAND
uint8_t write_Transmit[19] = {0x02,0xFF,0x0F,0x8E,0x00,0x11,0x01,0x8A,0x00,0x03,0x00,0x00,0x40,0x03,0x00,0x80,0x00,0x00,0x00};
uint8_t write_Response[19] = {0x04,0x13,0x05,0x01,0xFF,0x0F,0x01,0x00,0x02,0xFF,0x0F,0x06,0x00,0x11,0x01,0x02,0x00,0x00,0x00};

//CLOSE FLASH MEMORY
uint8_t close_Transmit[7]  = {0x01,0x06,0x04,0x03,0xFF,0x0F,0x00};
uint8_t close_Response[14] = {0x04,0x0F,0x04,0x00,0x01,0x06,0x04,0x04,0x05,0x04,0x00,0xFF,0x0F,0x00};


////////////////////////////////////
////////Supporting Functions////////
////////////////////////////////////

//Reset the BM71
void BT_Reset(void){
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);	//Set the reset pin low
	  HAL_Delay(50);											//A reset pulse must be greater than 1ms
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);		//Pull the reset pin back up
}

//Put the BM71 into test mode
void BT_EnterTestMode(void){

	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);	//Set configuration pin low
	  BT_Reset();
	  HAL_Delay(100);

}

//Put the BM71 into application mode
void BT_EnterAppMode(void){

	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);		//Set configuration pin high
	  BT_Reset();
	  HAL_Delay(100);

}


//Open configuration for flash memory of the BM71
void BT_OpenFlashMemory(void){

	  //Reset read and write buffers
	  memset(BT_WriteBuffer, 0x00 , sizeof(BT_WriteBuffer) );
	  memset(BT_ReadBuffer,  0x00 , sizeof(BT_ReadBuffer)  );

	  //Send command to open flash memory
	  HAL_UART_Transmit(&huart1, open_Transmit, sizeof(open_Transmit), 200 );

	  //Read response message
	  Status = HAL_UART_Receive(&huart1, BT_ReadBuffer, sizeof(open_Response), 200);
	  if(Status == HAL_OK){
		  _Bool TestFlag = true;
	  	 for(int i=0; i<sizeof(open_Response); i++){
	  		 if(BT_ReadBuffer[i] != open_Response[i] ){
	  			TestFlag = false;
	  		 }
	  	 }
	  	 if(TestFlag == true){
			 sprintf((char*)buf, "BT Open Flash - Successful\r\n");
		  	 HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 50 );
	  	 }else{
			 sprintf((char*)buf, "BT Open Flash - Failed (Mismatch)\r\n");
		  	 HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 50 );
	  	 }
	  }else{
	  	 sprintf((char*)buf, "BT Open Flash - Failed (Timeout)\r\n");
	  	 HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 50 );
	  }
	  HAL_Delay(50);
}


//Close configuration for flash memory of the BM71
void BT_CloseFlashMemory(void){

	  //Reset read and write buffers
	  memset(BT_WriteBuffer, 0x00 , sizeof(BT_WriteBuffer) );
	  memset(BT_ReadBuffer,  0x00 , sizeof(BT_ReadBuffer)  );

	  //Send command to open flash memory
	  HAL_UART_Transmit(&huart1, close_Transmit, sizeof(close_Transmit), 200 );

	  //Read response message
	  Status = HAL_UART_Receive(&huart1, BT_ReadBuffer, sizeof(close_Response), 200);
	  if(Status == HAL_OK){
		  _Bool TestFlag = true;
	  	 for(int i=0; i<sizeof(close_Response); i++){
	  		 if(BT_ReadBuffer[i] != close_Response[i] ){
	  			TestFlag = false;
	  		 }
	  	 }
	  	 if(TestFlag == true){
			 sprintf((char*)buf, "BT Close Flash - Successful\r\n");
		  	 HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 50 );
	  	 }else{
			 sprintf((char*)buf, "BT Close Flash - Failed (Mismatch)\r\n");
		  	 HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 50 );
	  	 }
	  }else{
	  	 sprintf((char*)buf, "BT Close Flash - Failed (Timeout)\r\n");
	  	 HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 50 );
	  }
}


//Erase flash memory of the BM71
void BT_EraseFlashMemory(uint16_t address){

	  //Reset read and write buffers
	  memset(BT_WriteBuffer, 0x00 , sizeof(BT_WriteBuffer) );
	  memset(BT_ReadBuffer,  0x00 , sizeof(BT_ReadBuffer)  );

		//Fill beginning of write buffer with read_transmit information
		for(int i=0; i<19; i++ ){
			BT_WriteBuffer[i]=erase_Transmit[i];
		}

		//Set read address
	    BT_WriteBuffer[11] = (uint8_t) address;					//Add lower address byte to base address low byte (0x00) already in packet
	    BT_WriteBuffer[12] = (uint8_t) (address >> 8);			//Add upper address byte to base address high byte (0x40) already in packet


	  //Send command to erase flash memory
	  HAL_UART_Transmit(&huart1, BT_WriteBuffer, sizeof(erase_Transmit), 200 );

	  //Read response message
	  Status = HAL_UART_Receive(&huart1, BT_ReadBuffer, sizeof(erase_Response), 200);
	  if(Status == HAL_OK){
		  _Bool TestFlag = true;
	  	 for(int i=0; i<sizeof(erase_Response); i++){

	  		  HAL_Delay(5);

	  		 if(BT_ReadBuffer[i] != erase_Response[i] && i!=20 ){
	  			TestFlag = false;
	  		 }
	  	 }
	  	 if(TestFlag == false){
			 sprintf((char*)buf, "BT Erase Flash - Failed (Mismatch)\r\n");
		  	 HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 50 );
	  	 }
	  }else{
	  	 sprintf((char*)buf, "BT Erase Flash - Failed (Timeout)\r\n");
	  	 HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 50 );
	  }

	  HAL_Delay(10);

}



//Read 128 bytes from the flash memory according to an address
void BT_Read128Bytes(uint16_t address){

	//Reset read and write buffers
	memset(BT_WriteBuffer, 0x00 , sizeof(BT_WriteBuffer) );
	memset(BT_ReadBuffer,  0x00 , sizeof(BT_ReadBuffer)  );


	//Fill beginning of write buffer with read_transmit information
	for(int i=0; i<19; i++ ){
		BT_WriteBuffer[i]=read_Transmit[i];
	}

	//Set read address
    BT_WriteBuffer[11] = (uint8_t) address;					//Add lower address byte to base address low byte (0x00) already in packet
    BT_WriteBuffer[12] = (uint8_t) (address >> 8);			//Add upper address byte to base address high byte (0x40) already in packet


    //Send command over uart1
    HAL_UART_Transmit(&huart1, BT_WriteBuffer, sizeof(read_Transmit), 200 );

    //Receive data over uart1
    Status = HAL_UART_Receive(&huart1, BT_ReadBuffer, sizeof(BT_ReadBuffer), 400);

	  if(Status == HAL_OK){
		  _Bool TestFlag = true;
	  	 for(int i=0; i<sizeof(read_Response); i++){
	  		 if(BT_ReadBuffer[i] != read_Response[i] ){
	  			TestFlag = false;
	  		 }
	  	 }
	  	 if(TestFlag == false){
			 sprintf((char*)buf, "BT Read Flash - Failed (Mismatch)\r\n");
		  	 HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 50 );
	  	 }
	  }else{
	  	 sprintf((char*)buf, "BT Read Flash - Failed (Timeout)\r\n");
	  	 HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 50 );
	  }

	  HAL_Delay(10);

}





//Read 128 bytes from the flash memory according to an address
void BT_PrintRead128Bytes(uint16_t address){

	//Reset read and write buffers
	memset(BT_WriteBuffer, 0x00 , sizeof(BT_WriteBuffer) );
	memset(BT_ReadBuffer,  0x00 , sizeof(BT_ReadBuffer)  );


	//Fill beginning of write buffer with read_transmit information
	for(int i=0; i<19; i++ ){
		BT_WriteBuffer[i]=read_Transmit[i];
	}

	//Set read address
    BT_WriteBuffer[11] = (uint8_t) address;					//Add lower address byte to base address low byte (0x00) already in packet
    BT_WriteBuffer[12] = (uint8_t) (address >> 8);			//Add upper address byte to base address high byte (0x40) already in packet


    //Send command over uart1
    HAL_UART_Transmit(&huart1, BT_WriteBuffer, sizeof(read_Transmit), 200 );

    //Receive data over uart1
    Status = HAL_UART_Receive(&huart1, BT_ReadBuffer, sizeof(BT_ReadBuffer), 400);

	  if(Status == HAL_OK){
		  _Bool TestFlag = true;
	  	 for(int i=0; i<sizeof(read_Response); i++){
	  		 if(BT_ReadBuffer[i] != read_Response[i] ){
	  			TestFlag = false;
	  		 }
	  	 }

	  	 if(TestFlag == true){

	  		 int i=0;
	  		 while(i<128){

				 sprintf((char*)buf, "%.4X    ", address+i );
				 HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 50 );

				 for(int j=0; j<16; j++){
					 sprintf((char*)buf, "%.2X ", BT_ReadBuffer[i+19] );
					 HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 50 );
					 if(j==7){
						 sprintf((char*)buf, "    " );
						 HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 50 );
					 }
					 if(j==15){
						 sprintf((char*)buf, "   ;\r\n" );
						 HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 50 );
					 }
					 i++;
				 }
	  		 }
		  }else{
			 sprintf((char*)buf, "BT Read Flash - Failed (Mismatch)\r\n");
		  	 HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 50 );
	  	 }
	  }else{
	  	 sprintf((char*)buf, "BT Read Flash - Failed (Timeout)\r\n");
	  	 HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 50 );
	  }

	  HAL_Delay(10);

}





//Write 128 bytes from the flash memory according to an address
void BT_Write128Bytes(uint16_t address, uint8_t *InputDataArray){

	//Reset read and write buffers
	memset(BT_WriteBuffer, 0x00 , sizeof(BT_WriteBuffer) );
	memset(BT_ReadBuffer,  0x00 , sizeof(BT_ReadBuffer)  );


	//Fill beginning of write buffer with read_transmit information
	for(int i=0; i<19; i++ ){
		BT_WriteBuffer[i]=write_Transmit[i];
	}

	//Fill the rest of the write buffer with data from the input data array
	for(int i=19 ; i<147; i++){
		BT_WriteBuffer[i]=InputDataArray[i-19];
	}


	//Set read address
    BT_WriteBuffer[11] = (uint8_t) address;					//Add lower address byte to base address low byte (0x00) already in packet
    BT_WriteBuffer[12] = (uint8_t) (address >> 8);			//Add upper address byte to base address high byte (0x40) already in packet


    //Send command over uart1
    HAL_UART_Transmit(&huart1, BT_WriteBuffer, sizeof(BT_WriteBuffer), 200 );

    //Receive data over uart1
    Status = HAL_UART_Receive(&huart1, BT_ReadBuffer, sizeof(write_Response), 400);

	  if(Status == HAL_OK){
		 _Bool TestFlag = true;
	  	 for(int i=0; i<sizeof(write_Response); i++){
	  		 if(BT_ReadBuffer[i] != write_Response[i] ){
	  			TestFlag = false;
	  		 }
	  	 }
	  	 if(TestFlag == false){
			 sprintf((char*)buf, "\nBT Write Flash - Failed (Mismatch)\r\n");
		  	 HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 50 );
	  	 }
	  }else{
	  	 sprintf((char*)buf, "BT Write Flash - Failed (Timeout)\r\n");
	  	 HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 50 );
	  }

	  HAL_Delay(10);

}


_Bool BT_CompareData(uint8_t *InputDataArray){

	for(int i=0; i<128; i++){
		if( InputDataArray[i] != BT_ReadBuffer[19+i] ){
			return false;
		}
	}

	return true;
}



void BT_MemoryDump(void){


	 sprintf((char*)buf, "\n\n\n");
	 HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 50 );


	for(int addr=0x0000; addr<0x2000; addr=addr+0x0100){

		 sprintf((char*)buf, "\n;;;      0  1  2  3  4  5  6  7     8  9  A  B  C  D  E  F\r\n");
		 HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 50 );
		 BT_PrintRead128Bytes(addr);
		 BT_PrintRead128Bytes(addr+0x0080);

	}


	 sprintf((char*)buf, "\n\n\n");
	 HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 50 );





}





void BT_ConfigRun(void){

		/*
		BT_EnterTestMode();			//Enter test mode
		BT_OpenFlashMemory();		//Connect to flash memory
		BT_MemoryDump();			//Print memory
		BT_CloseFlashMemory();		//Disconnect to flash memory
		BT_EnterAppMode();			//Enter test mode
		*/

	_Bool TestFlag = true;

	BT_EnterTestMode();			//Enter test mode

	BT_OpenFlashMemory();		//Connect to flash memory


	//Check to see if firmware versions are the same
	 sprintf((char*)buf, "\nChecking firmware...\r\n");
	 HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 50 );

	 for(int i=0; i<63 ; i++){
	 	 BT_Read128Bytes( i*0x080 );
		 if( !BT_CompareData( memoryBlock[i].data) ){
			 TestFlag=false;
		 }
	 }

	 sprintf((char*)buf, "Firmware Check Completed\r\n");
	 HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 50 );
 	 sprintf((char*)buf, "Status of Compare is %d \r\n\n", TestFlag);
 	 HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 50 );


 	 if(TestFlag!=true){

  		//Erase memory
 		 sprintf((char*)buf, "Erasing BM71 Flash Memory...\r\n");
 		 HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 50 );
 		 for(int addr=0x0000; addr<0x2000; addr=addr+0x080){
 			BT_EraseFlashMemory(addr);
 		 }

 		//Write firmware to memory
 		sprintf((char*)buf, "Writing new firmware to BM71...\r\n");
 		HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 50 );
 		for(int i=0; i<63; i++){
 			BT_Write128Bytes( memoryBlock[i].address, memoryBlock[i].data );
 		}

 		//Check to see if firmware versions are the same
 		sprintf((char*)buf, "\nRechecking firmware...\r\n");
 		HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 50 );
 		TestFlag = true;
 		for(int i=0; i<63 ; i++){
 			 BT_Read128Bytes( i*0x080 );
 			 if( !BT_CompareData( memoryBlock[i].data) ){
 				 TestFlag=false;
 			 }
 		 }
 		 sprintf((char*)buf, "Firmware Recheck Completed\r\n");
 		 HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 50 );
 	 	 sprintf((char*)buf, "Status of Compare is %d \r\n\n", TestFlag);
 	 	 HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), 50 );



 	 }


	BT_CloseFlashMemory();		//Disconnect to flash memory


	BT_EnterAppMode();			//Enter test mode



}





