//variable to store incoming audio sample
byte incomingAudio;

//clipping indicator variables
boolean clipping = 0, dsp_block = 0;
int16_t incomingBuffer[255], actualPosition = 0;

void setup(){
   cli();//disable interrupts
  
  //set up continuous sampling of analog pin 0
  
  //clear ADCSRA and ADCSRB registers
  ADCSRA = 0;
  ADCSRB = 0;
  
  ADMUX |= (1 << REFS0); //set reference voltage
  ADMUX |= (1 << ADLAR); //left align the ADC value- so we can read highest 8 bits from ADCH register only
  
  ADCSRA |= (1 << ADPS2) | (1 << ADPS0); //set ADC clock with 32 prescaler- 16mHz/32=500kHz
  ADCSRA |= (1 << ADATE); //enabble auto trigger
  ADCSRA |= (1 << ADIE); //enable interrupts when measurement complete
  ADCSRA |= (1 << ADEN); //enable ADC
  ADCSRA |= (1 << ADSC); //start ADC measurements
  
  sei();//enable interrupts

  //if you want to add other things to setup(), do it here
  Serial.begin(115200);
  Serial.println("is fine!");
  pinMode(13,OUTPUT);//led indicator pin
  

}

ISR(ADC_vect) {//when new ADC value ready
  if(dsp_block){
    incomingBuffer[actualPosition] = (int16_t) ADCH;
    actualPosition++;
  }
  
  if(actualPosition > 254)
    dsp_block = false;
}

void loop(){
  /* aguarda um novo bloco de amostras */
  while (!dsp_block);
  writeBlock(incomingBuffer);
  dsp_block = true;
}

void writeBlock(int16_t incomingBuffer[]){}
