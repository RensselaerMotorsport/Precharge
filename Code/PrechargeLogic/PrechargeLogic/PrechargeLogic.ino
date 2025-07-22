/*****************************************************

                Precharge V1 Firmware
  
*****************************************************/
  
  int Vinvdig = 0;        // Tracks the inverter voltage (direct value from ADC)
  int Vbattdig = 0;       // Tracks the battery voltage (direct value from ADC)
  int VinvPIN = A2;       // Analog Pin for reading Inverter Voltage 
  int VbattPIN = A7;      // Analog Pin for reading Battery Voltage
  int RelayPIN = 3;       // Digital Pin for driving Precharge Relay
  int AIRPIN = 2;         // Digital Pin for driving AIR+
  int State=0;            // Tracks the State of the Precharge board, State 0 is ready to precharge, State 1 is Precharged, State 3 is fault
  int Vdiff;              // Tracks the difference between inverter and battery voltage
  int Vthresh = 40;       // Minimum battery voltage to initate precharge



void setup() {
  Serial.begin(9600);     // For reading through Serial Monitor
  // Set digital pins as outputs and write them to be low
  pinMode(RelayPIN,OUTPUT);
  pinMode(AIRPIN,OUTPUT);
  digitalWrite(AIRPIN, LOW);
  digitalWrite(RelayPIN, LOW);
}

void loop() {
  // Read Inverter and Battery Votlages
  Vinvdig = analogRead(VinvPIN);
  float Vinv = Vinvdig;             // Convert Inverter voltage to a float for precharge logic
  Vbattdig = analogRead(VbattPIN);
  Vdiff = abs(Vinvdig-Vbattdig);

  // Detects if battery voltage is present (if no battery voltage dont precharge)
  if(Vbattdig<=Vthresh){
    digitalWrite(AIRPIN, LOW);
    digitalWrite(RelayPIN, LOW);
    State=3;
  }
  
  // If inverter voltlage is <95% of battery voltage, do not close positive contactor
  if (Vdiff>=0.05*Vinv && Vbattdig>Vthresh){
    digitalWrite(AIRPIN, LOW);
    digitalWrite(RelayPIN, HIGH);
    State=0;
  }

  // If inverter voltage is >95% of battery voltage, close positive contactor
  if (Vdiff<=0.05*Vinv && Vinvdig>Vthresh){
    digitalWrite(AIRPIN, HIGH);
    State=1;
  }

  // delay between logic (late a bufer will be added that averages analog readings and delay will be less)
  delay(200);
}
