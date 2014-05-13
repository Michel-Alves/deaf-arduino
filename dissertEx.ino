/**
 * LIMIT(x)
 * Limita ‘x‘ ao ntervalo [-127,127].
 */
#define LIMIT(x) if (x < -127) x = -127; if (x > 127)  x = 127;

/**
 * TMOD(x, index, len)
 * Leitura circular do indice ‘index‘ de um vetor ‘x‘ de tamanho ‘len‘,
 * potencia de 2.
 */
#define TMOD(x, index, len) x[(index)&(len-1)]
boolean dsp_block = false; //flag existe novo bloco a ser processado?
uint8_t readind = 0, writeind = 0; //posiço atual do indice
int BLOCK_SIZE = 255; //o tamanho total do bloco
int BUFFER_SIZE = 255 ; //limite do buffer
int16_t x[255], y[255]; //buffer das amostras
int count = 0;
//int y


void setup(){}

void loop(){
  /* aguarda um novo bloco de amostras */
  while (!dsp_block);
  /* variaveis para contagem de tempo */
  static unsigned long elapsed_time = 0;
  unsigned long start_time = micros();

  /* processamento do bloco */
  uint16_t maxind = readind+BLOCK_SIZE;
  for (uint8_t n = readind; n < maxind; n++) {
    int16_t yn = 0;
    uint8_t order = 0;
    for (uint8_t i = 0; i <= order; i++) {
      yn += TMOD(x, n, BUFFER_SIZE) * 0.33;
    }
    LIMIT(yn);
    TMOD(y, n, BUFFER_SIZE) = yn;
  }
  elapsed_time += micros() - start_time;
  count++;
  dsp_block = false;
}

ISR(TIMER2_OVF_vect){
  /*
   * Divisao da frequencia de amostragem por 2.
   */
  static boolean div = false;
  div = !div;
  if (div){

    /*
     * Leitura da entrada com conversao de tipo. ADCH e’ o registrador que
     * contem o resultado da ultima conversao ADC.
     */
    x[writeind] = (int16_t) 127 - ADCH;

    /*
     * Escrita da amostra de saida no registrador PWM, com ajuste de
     * offset e conversao de tipo.
     */
    OCR2A = (uint8_t) 127 + y[(writeind-BLOCK_SIZE)];
    /*
     * Verifica se um novo bloco de amostras esta cheio, calculando a
     * seguinte condicao: (writeind mod BLOCK_SIZE) == 0.
     */
    if ((writeind & (BLOCK_SIZE - 1)) == 0) {
      readind = writeind - BLOCK_SIZE;
      dsp_block = true;
    }
   /** Incremento do indice de escrita.*/
    writeind++;
   /** Inicia a proxima conversao.*/
    //sbi(ADCSRA,ADSC);
  }
}

