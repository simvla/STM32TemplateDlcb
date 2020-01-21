/*C Util functions lib by simvla*/

#include "Util.h"


char* uitoa(uint16_t n)   //convert unsigned int to string
{
  uint8_t dh,h,s,ds,j; //ten thousand,thousand,hundred,ten,one

  static char a[2];  //0-9 + null
  static char b[3];  //10-99 + null
  static char c[4];  //100-999 + null
  static char d[5];  //1000-9999 + null

  static char e[6];  //10000-65536 + null

  if(n<10) {
    a[0]=('0'+ n);
    return a;
  }
  if(n>=10 && n<100){
    ds=(n/10);
    j=(n-((n/10)*10));
    b[0]=('0'+ ds);
    b[1]=('0'+ j);
    return b;
  }
  if(n>=100 && n<999){
    s=(n/100);
    ds=((n-(100*s))/10);
    j=((n-(100*s))-(ds*10));
    c[0]=('0'+ s);
    c[1]=('0'+ ds);
    c[2]=('0'+ j);
    return c;
  }
  if(n>=1000 && n<9999){
    h=(n/1000);
    s=((n-(1000*h))/100);
    ds=(((n-(1000*h))-(s*100))/10);
    j=(((n-(1000*h))-(s*100))-(ds*10));
    d[0]=('0'+ h);
    d[1]=('0'+ s);
    d[2]=('0'+ ds);
    d[3]=('0'+ j);
    return d;
  }

  if(n>=10000 && n<65536){
    dh=(n/10000);
    h= ((n-(10000*dh))/1000);
    s= (((n-(10000*dh))-(h*1000))/100);
    ds= ((((n-(10000*dh))-(h*1000))-(s*100))/10);
    j= ((((n-(10000*dh))-(h*1000))-(s*100))-(ds*10));
    e[0]=('0'+ dh);
    e[1]=('0'+ h);
    e[2]=('0'+ s);
    e[3]=('0'+ ds);
    e[4]=('0'+ j);
    return e;
  }

  return 0;
}

//clear string
void strclr(char* str){

  uint8_t i;
  for(i=0;*str;i++){
    *str++=' ';
  }
}

//add char from left( "99.9" - "-99.9" )
uint8_t addchl(char* str,char c,uint8_t len){

  uint8_t n=0,i=1,j=0;
  char tmp;
  //get num of chars
  while(*str){
    n++;
    str++;
  }
  //check if char array is full
  if(n >(len-1)) return 1; //error char array full,exit
  //move chars one place right
  while(i<n){
    tmp=*(str-i);
    *(str-j)=tmp;
    j++;
    i++;
  }
  *(str-n)=c; //add char on zero index

  return 0;//OK
}

//unsigned 16 bit number to string convert with decimal point(0->0.0 to 999->99.9)
char* addDecPoint(uint16_t num){

  static char fmtstr[10];
  char tmp;
  //CONV
  strcpy(fmtstr,uitoa(num));
  //FORMAT
  if(strlen(fmtstr)==3){
    tmp=*(fmtstr+2);
    fmtstr[2]='.';
    fmtstr[3]=tmp;
    fmtstr[4]='\0';
  }
  if(strlen(fmtstr)==2){
    tmp=*(fmtstr+1);
    fmtstr[1]='.';
    fmtstr[2]=tmp;
    fmtstr[3]=' ';
    fmtstr[4]='\0';
  }
  if(strlen(fmtstr)==1){
    tmp=*fmtstr;
    fmtstr[0]='0';
    fmtstr[1]='.';
    fmtstr[2]=tmp;
    fmtstr[3]=' ';
    fmtstr[4]='\0';
  }
  //LIMIT
  if(num>999){
    fmtstr[0]='9';
    fmtstr[1]='9';
    fmtstr[2]='.';
    fmtstr[3]='9';
    fmtstr[4]='\0';
  }

  return fmtstr;
}

//expected input str(0.0-99.9),return int(0-999)
uint16_t strToInt(char* str){

  uint16_t num;

  num=str[0]-'0';
  str++;
  if(*str=='.'){
    str++;
    num=((num*10)+(*str-'0'));
  }else{
    num=((num*10)+(*str-'0'));
    str+=2;
    num=((num*10)+(*str-'0'));
  }
  return num;
}

//custom search string imlementation
uint8_t searchStr(char* str,const char* mstr ){//mstr characters match with str

  uint8_t len=strlen(mstr);
  uint8_t matchN=0;

  while(*mstr){
    while(*str){
      if(*str==*mstr){
         matchN++;
         break;
      }
      str++;
    }
    mstr++;
  }
  mstr-=len; //reset pointer
  if(matchN==strlen(mstr)) return 1;

  return 0;
}

