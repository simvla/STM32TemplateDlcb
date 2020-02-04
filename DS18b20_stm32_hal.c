/*stm32 DS18b20 lib by simvla*/

#include "DS18b20_stm32_hal.h"
#include "delay_stm32_hal.h"
#include "Util.h"
#include <string.h>

// Private definitions:
#define DS_HIGH HAL_GPIO_WritePin(DS_PORT,DS_PIN,GPIO_PIN_SET)
#define DS_LOW  HAL_GPIO_WritePin(DS_PORT,DS_PIN,GPIO_PIN_RESET)
// commands
#define CONVERTTEMP 0x44
#define RSCRATCHPAD 0xbe
#define READROM 0x33
#define MATCHROM 0x55
#define SKIPROM 0xcc
// constant
#define DECIMAL_STEPS_12BIT 625 //.0625

// Private func:
uint8_t onewire_reset(void);
void onewire_write_1(void);
void onewire_write_0(void);
uint8_t onewire_read_bit(void);
void onewire_write_bit(uint8_t bit);
uint8_t onewire_read_byte(void);
void onewire_write_byte(uint8_t data);
void ds18b20_matchRom(uint8_t rom[]);

// Private vars:
static GPIO_InitTypeDef DS_gpio;


void ds18b20_init(void){
  //Set gpio clock
  if(DS_PORT==GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE();
  else if(DS_PORT==GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
  else if(DS_PORT==GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();
  else while(1) {} //not implemented
  //Set parm
  DS_gpio.Mode=GPIO_MODE_OUTPUT_OD;
  DS_gpio.Pin=DS_PIN;
  DS_gpio.Pull=GPIO_NOPULL;
  DS_gpio.Speed=GPIO_SPEED_FREQ_MEDIUM;

  HAL_GPIO_Init(DS_PORT,&DS_gpio);

}

uint8_t onewire_reset(void){

  uint8_t i;

  DS_LOW;
  Delay_us(500);
  DS_HIGH; // relase bus
  Delay_us(70); //wait for ds18b20 pull bus low
  i=HAL_GPIO_ReadPin(DS_PORT,DS_PIN);
  Delay_us(430);
  //i=0-OK,i=1-ERROR
  return i;

}

void onewire_write_1(void){

  DS_LOW;
  Delay_us(15);
  DS_HIGH;
  //wait until end of timeslot (60 us) + 5 us for recovery
  Delay_us(50);

}

void onewire_write_0(void){

  DS_LOW;
  Delay_us(60);
  DS_HIGH;
  //wait until end of timeslot (60 us) + 5 us for recovery
  Delay_us(5);

}

void onewire_write_bit(uint8_t bit){

  if(bit) onewire_write_1();
  else onewire_write_0();

}

uint8_t onewire_read_bit(void){

  uint8_t bit=0;

  DS_LOW;
  Delay_us(5);
  DS_HIGH;
  //wait 5us and check bus state
  Delay_us(5);
  if(HAL_GPIO_ReadPin(DS_PORT,DS_PIN)) bit=1;
  //wait until end of timeslot (60 us) + 5 us for recovery
  Delay_us(55);

  return bit;

}

void onewire_write_byte(uint8_t data){

  uint8_t i=8;

  while(i--){
    onewire_write_bit(data&1); // send lsb first
    data>>=1; //shift all right

  }

}

uint8_t onewire_read_byte(void){

  uint8_t i=8 ,data=0;

  while(i--){
    data>>=1; //shift all right
    data|=(onewire_read_bit()<<7); //lsb read first

  }

  return data;

}

void ds18b20_matchRom(uint8_t rom[]){

  uint8_t i;
  onewire_write_byte(MATCHROM);
  for(i=0;i<8;i++){
    onewire_write_byte(rom[i]);
  }

}

void ds18b20_readAddr(uint8_t addr[]){ //*(addr+0)

  uint8_t i;

  onewire_reset();
  onewire_write_byte(READROM);
  for(i=0;i<8;i++){
    addr[i]=onewire_read_byte(); //*(addr+i)
  }

}

uint8_t ds18b20_read(char* tstr,uint8_t id[]){

  uint8_t t0,t1;
  uint8_t digit=0;
  uint16_t decimal=0;
  uint8_t minus=0;
  static char tempStr[10]="";
  uint16_t tempInt=0;
  uint8_t decToOne;

  if(onewire_reset()) return 1; // error no resp from sensor
  if(*id) ds18b20_matchRom(id);
  else onewire_write_byte(SKIPROM);
  onewire_write_byte(CONVERTTEMP);
  Delay_ms(750);
  onewire_reset();
  if(*id) ds18b20_matchRom(id);
  else onewire_write_byte(SKIPROM);
  onewire_write_byte(RSCRATCHPAD);
  //read Scratchpad(first 2 bytes)
  t0=onewire_read_byte();
  t1=onewire_read_byte();
  //check if negative and convert to positive
  if(t1&0xf0) { //sign bits t1 (b3-b7) 0b 11111000
    t0=(~t0)+1;
    t1=(~t1);
    minus=1; //minus marker
  }
  //store whole part
  digit=t0>>4; //num 10 -> t0(b4..7)0b10100000
  digit|=(t1&0x7)<<4; //t1(b0..2)0b00000000
  //store decimal part
  decimal=t0&0xf; // (b0..3) hex -> 0x0-0xf,dec -> 0-15,bin -> 0b00000000-0b00001111
  decimal*=DECIMAL_STEPS_12BIT; // 0*625=0 - 15*625=9375
  decToOne=decimal/1000; //strip to one num precision (9375/1000 -> 9)
  //format
  tempInt=(decToOne+(digit*10)); // 10.9 -> (10*10)+9=109
  if(minus) {
    strcpy(tempStr,addDecPoint(tempInt));
    addchl(tempStr,'-',sizeof(tempStr));
    strcpy(tstr,tempStr);
  }else {
    strcpy(tempStr,addDecPoint(tempInt));
    strcat(tempStr," ");
    strcpy(tstr,tempStr);
  }

 return 0; // Read OK

}





















//this software using STM32 HAL libraries by STMicroelectronics :
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
