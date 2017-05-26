#include <Brain.h>
#include <SoftwareSerial.h>

#define RED_LED 9
#define YELLOW_LED 8
#define GREEN_LED 12

SoftwareSerial btSerial(10,11);

Brain brain(Serial);

class Values
{
    byte sigStrength,attention,meditation;
    unsigned long delta,theta,lowAlpha,highAlpha,lowBeta,highBeta,lowGamma,midGamma;        
public:
    void print();
    void read(Brain brain);
    char getWristAngle();
    char getFingersAngle();
    void sendSignal();
    void set();
}values;

char wrist,fingers;
const int btState = 14;
int state;

void setFingers(char );
void setWrist(char );

void setup() 
{
    Serial.begin(9600);
    wrist = 0;
    fingers = 0;
    btSerial.begin(38400);
    pinMode(RED_LED,OUTPUT);
    pinMode(YELLOW_LED,OUTPUT);
    pinMode(GREEN_LED,OUTPUT);
    digitalWrite(RED_LED,HIGH);
}

void loop()
{    
   if(brain.update())
    {
        Serial.println(brain.readErrors());
        Serial.println(brain.readCSV());
        values.read(brain);
        values.set();
        wrist = values.getWristAngle();
        fingers = values.getFingersAngle();

        if(wrist == 'b' && fingers == '1')
        {
            setFingers(fingers);     
        }
        if(wrist == 'b')
        {
            setWrist(wrist);
        }
        else
        {
            setFingers(fingers);
        }
      
    }
}

void Values :: print()
{
     Serial.print("Signal Strength : ");   
     Serial.println(this->sigStrength);
     Serial.print("Attention Value : ");   
     
     Serial.println(this->attention);
     Serial.print("Mediation Value : ");   
     Serial.println(this->meditation);
     Serial.print("Delta Value     : "); 
     Serial.println(this->delta);
     Serial.print("Theta Value     : ");  
     Serial.println(this->theta);
     Serial.print("Low Alpha       : ");
     Serial.println(this->lowAlpha);
     Serial.print("High Alpha      : ");
     Serial.println(this->highAlpha);
     Serial.print("Low Beta        : ");
     Serial.println(this->lowBeta);
     Serial.print("High Beta       : ");
     Serial.println(this->highBeta);
     Serial.print("Low Gamma       : ");
     Serial.println(this->lowGamma);
     Serial.print("High Gamma      : ");   
     Serial.println(this->midGamma);
     Serial.print("\n");
}

void Values :: read(Brain brain)
{
     this->sigStrength = brain.readSignalQuality();
     this->attention = brain.readAttention();
     this->meditation = brain.readMeditation();
     this->delta = brain.readDelta();
     this->theta = brain.readTheta();
     this->lowAlpha = brain.readLowAlpha();
     this->highAlpha = brain.readHighAlpha();
     this->lowBeta = brain.readLowBeta();
     this->highBeta = brain.readHighBeta();
     this->lowGamma = brain.readLowGamma();
     this->midGamma = brain.readMidGamma();
}

char Values :: getWristAngle()
{
    if(this->sigStrength)
        return '0';
    if(this->meditation > 70)
        return 'b';
    return '0';
}

char Values :: getFingersAngle()
{
    if(this->sigStrength)
        return '0';
    if(this->attention > 50)
        return '1';
    else
        return '0';     
}

void setFingers(char angle)
{
    btSerial.write(angle);
}

void setWrist(char angle)
{
    btSerial.write(angle);   
}

void Values :: sendSignal()
{
     btSerial.write(this->sigStrength);
     btSerial.write(this->attention);
     btSerial.write(this->meditation);
     btSerial.write(this->delta);
     btSerial.write(this->theta);
}

void Values :: set()
{
    if(this->sigStrength == 0)
    {
        digitalWrite(GREEN_LED,HIGH);
        digitalWrite(RED_LED,LOW);
        digitalWrite(YELLOW_LED,LOW);
    }
    else if(this->sigStrength == 200)
    {
        digitalWrite(GREEN_LED,LOW);
        digitalWrite(RED_LED,HIGH);
        digitalWrite(YELLOW_LED,LOW);
    }
    else
    {
        digitalWrite(GREEN_LED,LOW);
        digitalWrite(RED_LED,LOW);
        digitalWrite(YELLOW_LED,HIGH);
    }
}

