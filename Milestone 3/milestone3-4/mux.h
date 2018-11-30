#define MUX_OUT A0
#define MUX_0 A1
#define MUX_1 A2
#define MUX_2 A3



void set_mux_select(byte device)
{
  switch(device) {

    case 0: // MICROPHONE
      digitalWrite(MUX_0,LOW);
      digitalWrite(MUX_1,LOW);
      digitalWrite(MUX_2,LOW);
      //Serial.println("Set mux: microphone");
      break; 

    case 1: // IR SENSOR
      digitalWrite(MUX_0,HIGH);
      digitalWrite(MUX_1,LOW);
      digitalWrite(MUX_2,LOW);
      //Serial.println("Set mux: IR sensor");
      break;

    case 2: // WALL SENSOR FRONT
      digitalWrite(MUX_0,LOW);
      digitalWrite(MUX_1,HIGH);
      digitalWrite(MUX_2,LOW);
      //Serial.println("Set mux: front wall sensor");
      break;

    case 3: // WALL SENSOR LEFT
      digitalWrite(MUX_0,HIGH);
      digitalWrite(MUX_1,HIGH);
      digitalWrite(MUX_2,LOW);
      //Serial.println("Set mux: left wall sensor");
      break;

    case 4: // WALL SENSOR RIGHT
      digitalWrite(MUX_0,LOW);
      digitalWrite(MUX_1,LOW);
      digitalWrite(MUX_2,HIGH);
      //Serial.println("Set mux: right wall sensor");
      break;

    default:
      Serial.println("Mux not set");
      break;
  }
  delay(1); //1 to be safe? (0.5 = 500ns)
}


void mux_init() {
    pinMode(MUX_0, OUTPUT);
    pinMode(MUX_1, OUTPUT);
    pinMode(MUX_2, OUTPUT);
    set_mux_select(0);
}
