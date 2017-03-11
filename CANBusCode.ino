#include <mcp_can.h>
#include <SPI.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string

#define CAN0_INT 2                              // Set INT to pin 2
MCP_CAN CAN0(10);                               // Set CS to pin 10


void setup()
{
  Serial.begin(9600);
  
  // Initialize MCP2515 running at 16MHz with a baudrate of 250kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_STDEXT/*MCP_STDEXT  MCP_ANY*/, CAN_250KBPS, MCP_16MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");
  
  CAN0.setMode(MCP_NORMAL);                     // Set operation mode to normal so the MCP2515 sends acks to received data.

  pinMode(CAN0_INT, INPUT);                            // Configuring pin for /INT input
  
  Serial.println("MCP2515 Library Receive Example...");
}

void loop()
{
  if(!digitalRead(CAN0_INT))                         // If CAN0_INT pin is low, read receive buffer
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
//    if((rxId & 0x80000000) == 0x80000000)     // Determine if ID is standard (11 bits) or extended (29 bits)
//    sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len   
//    Serial.println();
//    Serial.print(rxId);

//Throttle Position.
//Coolant temp.
//Oil temp.
//Lambda.
//Map.
//Frequency2.
//Rpm.
    Serial.println();
    if(rxId == 2365583432){ //Extended ID: 0x0CFFF048
      Serial.print("Throttle Percentage");
      double percentage=(rxBuf[3]*256+rxBuf[2])/10.0;
      Serial.print(percentage);
    }
    Serial.println();

      if(rxId == 2365583688){ //Extended ID: 0x0CFFF148
      Serial.print("Barometer");
      double barometer=(rxBuf[1]*256+rxBuf[0])/100.0;
      Serial.print(barometer);
      }
    Serial.println();
      if(rxId == 2365584712){ //Extended ID: 0x0CFFF548
        Serial.print("CoolantTemp");
        double coolanttemp=(rxBuf[5]*256+rxBuf[4])/10.0;
        Serial.print(coolanttemp);
      }
    Serial.println();
  }
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
