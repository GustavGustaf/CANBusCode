#include <mcp_can.h>
#include <SPI.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string

#define CAN0_INT 2                              // Set INT to pin 2
MCP_CAN CAN0(10);                               // Set CS to pin 10

int scaleIt(double value){ //This function will scale all values to an integer between 100 and 999.
  int scaledValue=(value*899)+100;
  return scaledValue;
}

//Initialize variables to 0 since they are sent from the CANBus at different rates.
double rpm=0;
double tps=0;
double mabsp=0;
double lambda=0;
double oilTemp=0;
double frequency2=0;
double coolantTemp=0;

void setup()
{
  Serial.begin(9600);
  
  // Initialize MCP2515 running at 16MHz with a baudrate of 250kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_STDEXT, CAN_500KBPS, MCP_16MHZ) == CAN_OK) //Changed from MCP_ANY to MCP_STDEXT
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
    String outString=""; //This will store the fixed-length values to be sent to LabView

    //All readings are turned into a percentage of their max values.
    CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
    if(rxId == 2365583432){ //Extended ID: 0x0CFFF048
      rpm=(rxBuf[1]*256+rxBuf[0])/30000.0; //Rotations per minute
      tps=((rxBuf[3]*256+rxBuf[2])/10.0)/100; //Throttle position as a percentage
    }
    else if(rxId == 2365583688){ //Extended ID: 0x0CFFF148
      mabsp=((rxBuf[3]*256+rxBuf[2])/100.0)/300; //Manifold absolute pressure
      lambda=((rxBuf[5]*256+rxBuf[4])/100.0)/10; //Oxygen
    }
    else if(rxId == 2365583944){ //Extended ID: 0CFFF248
      oilTemp=((rxBuf[1]*256+rxBuf[0])/1000.0)/5; //Oil temperature from Analog Input #1
    }
    else if(rxId == 2365584456){ //Extended ID: 0CFFF448
      frequency2=((rxBuf[3]*256+rxBuf[2])/10.0)/6000; //Reading from Frequency2
    }
    else if(rxId == 2365584712){ //Extended ID: 0x0CFFF548
      coolantTemp=(((rxBuf[5]*256+rxBuf[4])/10.0)+1000)/2000; //Coolant temperature
    }
    else {
      //Use the following to test only
      //sprintf(msgString, "rxId: %.8lX not recognized", rxId);
      //Serial.println(msgString);
    }

    outString=scaleIt(rpm);
    outString+=scaleIt(tps);
    outString+=scaleIt(mabsp);
    outString+=scaleIt(lambda);
    outString+=scaleIt(oilTemp);
    outString+=scaleIt(frequency2);
    outString+=scaleIt(coolantTemp);
    Serial.println(outString);
  }
}
