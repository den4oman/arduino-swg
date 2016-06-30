/*

    den4oman
    GNUGPLv3

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
/*
	
	Square Wave Generator
	Control Through Serial Port
	
*/
/////////// Control Pins
unsigned int SWGCtrl1       = 11; // Control Pin 1 // On Mega 11 // On Uno 9
unsigned int SWGCtrl2       = 12; // Control Pin 2 // On Mega 12 // On Uno 10
///////////
/////////// Parameters
unsigned long SWGFreq       = 10000;							// Default Frequency 
unsigned long SWGICRMath    = (long)(F_CPU/SWGFreq); 			// Default Timer Counter
unsigned int SWGDutyCycle11 = 50;								// Default Duty Cycle Chanel 1
unsigned int SWGDutyCycle12 = 50;								// Default Duty Cycle Chanel 2
unsigned int SWGOCRMath11   = SWGICRMath*SWGDutyCycle11/100;    // Default Timer Counter Chanel 1
unsigned int SWGOCRMath12   = SWGICRMath*SWGDutyCycle12/100;	// Default Timer Counter Chanel 2
unsigned int SWGPhase1      = 0; 								// Default Phase Chanel 1 
unsigned int SWGPhase2      = 1;								// Default Phase Chanel 2
///////////
/////////// Command Variables
char SWGCtrlCmm;	//Control Character
///////////

void setup() 
{
  
Serial.begin(9600);
Serial.setTimeout(5000);
pinMode(SWGCtrl1,OUTPUT);
pinMode(SWGCtrl2,OUTPUT);


while(!Serial){}
delay(1000);

}

void loop() 
{
  Serial.println("Working! \n Press 8 for Control Panel");
  if (Serial.read() == '8')
  {
    Serial.println("Entering Control Panel: \n");
    SWGControlPanel();
  }
  delay(1000);
}

void SWGControlPanel()												// Control Panel
{
    Serial.println("Press 8 for Info");
    SWGCtrlCmm = 0;
    while (SWGCtrlCmm != 57)
{
  if (Serial.available() >= 0)
    {
        SWGCtrlCmm = Serial.read();
        switch (SWGCtrlCmm)
        {
          case '1':													// Set Frequency
            Serial.flush();
            Serial.println("Enter Frequency in Hz (5s) \n");
            delay(1000);           
            SWGFreq = Serial.parseInt();
            SWGICRMath = (long)(16000000/SWGFreq);
            SWGOCRMath11 = (int)(SWGDutyCycle11*SWGICRMath/100);
            SWGOCRMath12 = (int)(SWGDutyCycle12*SWGICRMath/100);
            Serial.println(" Frequency Adjusted ");
          break;
          case '2':													// View Information
            Serial.print("The Frequency is:   ");
            Serial.println(SWGFreq);
            Serial.print("Duty Cycle 11:   ");
            Serial.println(SWGDutyCycle11);
            Serial.print("Duty Cycle 12:   ");
            Serial.println(SWGDutyCycle12);
            Serial.print("OCR11:   ");
            Serial.println(SWGOCRMath11);
            Serial.print("OCR12:   ");
            Serial.println(SWGOCRMath12);
            Serial.print("ICR:   ");
            Serial.println(SWGICRMath);
            Serial.print("Phase1:   ");
            Serial.println(SWGPhase1);
            Serial.print("Phase2:   ");
            Serial.println(SWGPhase2);
          break;
          case '4':													// Set Duty Cycle Chanel 1
          Serial.flush();
          Serial.println("Set Duty Cycle Chanel 11 in percentage:    ");
          SWGDutyCycle11  =  Serial.parseInt();
          SWGOCRMath11 = (int)(SWGDutyCycle11*SWGICRMath/100);
          Serial.println("Done!");         
          break;
          case '5':													// Set Duty Cycle Chanel 2
            Serial.flush();
            Serial.println("Set Duty Cycle Chanel 12 in percentage:    ");
            SWGDutyCycle12  =  Serial.parseInt();
            SWGOCRMath12 = (int)(SWGDutyCycle12*SWGICRMath/100);
            Serial.println("Done!"); 
          break;
          case '6':													// Set Phase For Chanel 1
            Serial.flush();
            Serial.println("Set Inverting 11: 0 - Not , 1 - Inverted    ");
            SWGPhase1 = Serial.parseInt();
            Serial.println("Done!");           
          break;
          case '7':													// Set Phase For Chanel 2
            Serial.flush();
            Serial.println("Set Inverting 12: 0 - Not , 1 - Inverted ");
            SWGPhase2 = Serial.parseInt();            
            Serial.println("Done!");         
            
          break;
          case '8':													// Print Information
            Serial.println("    Commands are: ");
            Serial.println(" 1: Adjust Frequency ");
            Serial.println(" 2: View Adjusted Parameters ");
            Serial.println(" 3: Set Scale Factor ");
            Serial.println(" 4: Set Duty Cycle Chanel 11 ");
            Serial.println(" 5: Set Duty Cycle Chanel 12 ");
            Serial.println(" 6: Set Invering 11  ");
            Serial.println(" 7: Set Invering 12  ");
            Serial.println(" 8: View This Help ");
            Serial.println(" 9: Write to Registers and Begin ");
            Serial.println(" 0: Stop Clock ");
            Serial.println("    Maximum Frequency is 1 MHz!   ");
          break;
          case '9':													// Write To Registers and Begin
            if(SWGPhase1 && SWGPhase2 == 0)
            {
              TCCR1A = 0xA2; // Both Non Inverted
            }
            else if(SWGPhase1 && SWGPhase2 == 1)
            {
              TCCR1A = 0xF2; // Both Inverted
            }
            else if (SWGPhase1 == 0 )
            {
              TCCR1A = 0xB2;
            }
            else
            {
              TCCR1A = 0xE2;
            }
            TCCR1B  = 0x19;
            ICR1    = (int)SWGICRMath;
            OCR1A   = (int)SWGOCRMath11;
            OCR1B   = (int)SWGOCRMath12;
            Serial.println("DONE");
          break;
          case '0':													// Stop Timer
            TCCR1A	= 0;
            TCCR1B	= 0;
            ICR1 	= 0;
            OCR1A	= 0;
            OCR1B	= 0;
          break;
          default:
          /////////////////////////////////////////////////////////
          break;
        }
    }
 
  delay(100);
}
}
