/*
The input for the tacho pulse is intended to come from the injection or spark plug control circuit. 

Effectively this means that, for example(s):
 - For a 6 or 8 cylinder four stroke engine car, each cylinder fires once for every 2 engine cycles, meaning the measured frequency would need to be doubled
 - If we received a quantity of TOTPULSES over a period of TOTTIME, the RPM would be calculated as follows
     - Convert TOTPULSES to Revolutions REVOLS = 2 x TOTPULSES
     - Scale TOTTIME (in ms) to one minute in conjunction with REVOLS. => RPM = (2 x TOTPULSES) x (1000 x 60)/(TOTTIME)

     Generally don't want the system to operate the LED light strip at less than 1000RPM, but operating up to 7000RPM
     This is equivalent to starting operation at 17 revolutions per second, operating up to 120 revolutions per second
     As we only see a pulse on every second revolution, this means we want operation circa:
     8 pulses per second to 60 pulses per second

     */




volatile byte REV;       //  VOLATILE DATA TYPE TO STORE REVOLUTIONS
 
unsigned long rpm, maxRPM;  //  DEFINE RPM AND MAXIMUM RPM
 
unsigned long eltime;         //  DEFINE TIME TAKEN TO COVER ONE REVOLUTION

unsigned long prevtime;       //  STORE IDLE TIME TO TOGGLE MENU

 void ICACHE_RAM_ATTR RPMCount()                                // EVERYTIME WHEN THE SENSOR GOES FROM LOW TO HIGH , THIS FUNCTION WILL BE INVOKED 
 {
   REV++;                                         // INCREASE REVOLUTIONS
      
 }
    
 void setup()
 {
     Serial.begin(9600);   // GET VALUES USING SERIAL MONITOR
     
     attachInterrupt(4, RPMCount, RISING);     //  ADD A HIGH PRIORITY ACTION ( AN INTERRUPT)  WHEN THE SENSOR GOES FROM LOW TO HIGH
     
     REV = 0;      //  START ALL THE VARIABLES FROM 0
     eltime = 0;
     rpm = 0;
     prevtime = 0;
     
    
 }
 
 void loop()
 {
  unsigned long currtime = millis();                 // GET CURRENT TIME
  
  unsigned long idletime = currtime - prevtime;        //  CALCULATE IDLE TIME
    
    if(REV >= 5 )                  //  IT WILL UPDATE AFTER EVERY 5 READINGS
   {
     
     
    rpm = 2*REV*1000*60/(millis()-eltime);            //  RPM = (2 x TOTPULSES) x (1000 x 60)/(TOTTIME)

     if(rpm > maxRPM)
     maxRPM = rpm;                             //  GET THE MAX RPM THROUGHOUT THE RUN
    
     eltime = millis();                            
     
     REV = 0;
        
     prevtime = currtime;
     Serial.print("RPM = ");
     Serial.print(rpm);
     Serial.print("     Max RPM = ");
     Serial.print(maxRPM);
     Serial.println();

   }
     
 }
 

//////////////////////////////////////////////////////////////  END OF THE PROGRAM  ///////////////////////////////////////////////////////////////////////
