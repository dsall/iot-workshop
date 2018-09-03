#include <math.h>

int TempC;
int TempF;
double Thermistor(int RawADC) {
 double Temp;
 Temp =log(10000.0/(1024.0/RawADC-1)); // for pull-up configuration
 Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
 Temp = Temp - 273.15;            // Convert Kelvin to Celcius
 return Temp;
}

void setup() {
 Serial.begin(9600);
}

void loop() {
  TempC = int(Thermistor(analogRead(A0)));
  TempF = int(TempC*1.8 + 32);
  String Text = "The Temperature in the room is: ";
  String Response = Text + TempC + "C" + "  and " + TempF +"F.";
  Serial.println(Response);
 delay(1000);
}
