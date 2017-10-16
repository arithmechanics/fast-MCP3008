//Example of fast sampling with MCP3008 and Arduino Uno
//Calculate Min, Max, Avg over 100,000 samples
//Created by Dmitriy L. (arithmechanics.com), 2017

#include "FastMCP3008.h"

/*
MCP3008   Atmega328P
V_DD      5v
V_REF     5v
AGND      GND
CLK       D13
D_OUT     D12
D_IN      D11
CS        D10
DGND      GND
 */

FastMCP3008 adc;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("sizeof(int) = "); Serial.println(sizeof(int)); 
  Serial.print("sizeof(long) = "); Serial.println(sizeof(long)); 
  Serial.print("sizeof(size_t) = "); Serial.println(sizeof(size_t)); 

  adc.init();
}

#define DO_CALC_AVG

void loop() {
    
  Serial.print("Starting experiment... ");
  Serial.flush();
  const unsigned long N = 100000; //must be bigger than 1023
  unsigned int valMin = 1023;
  unsigned int valMax = 0;
  
#ifdef DO_CALC_AVG 
  unsigned int valAvgIntPart = 0;
  unsigned long valAvgRemPart = 0;
#endif
  
  const byte channelMask = adc.getChannelMask(0);
  int curValue;
  
  const unsigned long start_time = millis();
  for(unsigned long i=0; i<N; ++i) {
    curValue = adc.read(channelMask);
    
    if(curValue > valMax) valMax = curValue;
    if(curValue < valMin) valMin = curValue;

#ifdef DO_CALC_AVG
    if(valAvgRemPart >= N-curValue) {
      ++valAvgIntPart;
      valAvgRemPart -= N-curValue;
    } else 
      valAvgRemPart += curValue;
#endif

  }

  Serial.print(millis() - start_time);
  Serial.println("ms");
  Serial.print("  Min = ");
  Serial.print(valMin);
#ifdef DO_CALC_AVG
  float valAvg = valAvgIntPart + (float)valAvgRemPart / N;
  Serial.print("; Avg = ");
  Serial.print(valAvg, 3);
#endif
  Serial.print("; Max = ");
  Serial.println(valMax);
  Serial.flush();
  
  delay(2000);
}


