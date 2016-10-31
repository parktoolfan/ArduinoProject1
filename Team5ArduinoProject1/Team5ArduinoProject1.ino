/*     ---------------------------------------------------------
 *     |  Arduino Experimentation Kit Temp Sensor             |
 *     |  CIRC-10 .: Temperature :. (TMP36 Temperature Sensor) |
 *     ---------------------------------------------------------
 *   
 *  A simple program to output the current temperature to the IDE's debug window 
 * 
 *  For more details on this circuit: http://tinyurl.com/c89tvd 
 */
\
//TMP36 Pin Variables
int temperaturePin = 0; //the analog pin the TMP36's Vout (sense) pin is connected to
                        //the resolution is 10 mV / degree centigrade 
                        //(500 mV offset) to make negative temperatures an option

int ledDigitalOne[] = {9, 10, 11}; //the three digital pins of the digital LED 
                                   //9 = redPin, 10 = greenPin, 11 = bluePin

const boolean ON = LOW;     //Define on as LOW (this is because we use a common 
                            //Anode RGB LED (common pin is connected to +5 volts))
const boolean OFF = HIGH;   //Define off as HIGH

//Predefined Colors
const boolean RED[] = {ON, OFF, OFF};    
const boolean GREEN[] = {OFF, ON, OFF}; 
const boolean YELLOW[] = {ON, ON, OFF}; 
const boolean BLACK[] = {OFF, OFF, OFF};

//An Array that stores the predefined colors (allows us to later randomly display a color)
const boolean* COLORS[] = {RED, GREEN, YELLOW, BLACK};


// Initialize the speaker pin. ???
int speakerPin = 13;

int length = 1; //Only one note
char notes[] = "c"; // notes and beats are kept as arrays in case the user wants to adjust the alarm for other purposes.
int beats[] = {1};
int tempo = 20;


void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = {'c'};
  int tones[] = {1915};
// play the tone corresponding to the note name
  for (int i = 0; i < 1; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

void setup()
{
  Serial.begin(9600);  //Start the serial connection with the copmuter
                       //The temperature can be monitored by opening the serial monitor.

   for(int i = 0; i < 3; i++){
   pinMode(ledDigitalOne[i], OUTPUT);   //Set the three LED pins as outputs
  }
  // Set up output pin for the piezo element
  pinMode(speakerPin, OUTPUT);
}
 
void loop()                     // run over and over again
{
   float temperature = getVoltage(temperaturePin);  //getting the voltage reading from the temperature sensor
   temperature = (temperature - .5) * 100;          //converting from 10 mv per degree with 500 mV offset;to degrees ((volatge - 500mV) times 100)
   Serial.println(temperature);                     //printing the result
   delay(100); // this provides ample time for . . .


/*
 * The following branching statements are used by the program to execute a certain task
 * base on the current temperature.
 */
   if ((temperature > 21.0) && (temperature < 29.0)) { //This range is the "safe range" for temperature.
    setColor(ledDigitalOne, GREEN);
   }
   else if (((temperature > 20.0) && (temperature < 20.9)) || ((temperature > 29.1) && (temperature < 30.0))) {
    // The following four lines set the RGB LED to YELLOW for 500 milliseconds, then to BLACK for 500 milliseconds.
    // This essentially gives the effect of a flashing yellow light
    setColor(ledDigitalOne, YELLOW);
    delay(500);
    setColor(ledDigitalOne, BLACK); // Essentially, the LED is off
    delay(500);
   }
   else {
    setColor(ledDigitalOne, RED);
    // An alarm will sound at the current temperature ((temperature < 20.0) || (temperature > 30.0)).
    playNote(notes[0], beats[0] * tempo);
   }
}


// getVoltage() - returns the voltage on the analog input defined by
float getVoltage(int pin){
   return (analogRead(pin) * .004882814); //converting from a 0 to 1023 digital range
                                          // to 0 to 5 volts (each 1 reading equals ~ 5 millivolts
}

void setColor(int* led, boolean* color){
   for(int i = 0; i < 3; i++){
     digitalWrite(led[i], color[i]);
   }
}

