#include <Wire.h>
 
void Read_MLX90614( char addr, char cmd, char *arry );
float Temperature( char addr, char type );
 
 
/* Slave address of MLX90614 (7bit) */
const char thermo_addr = 0x5A;    // default slave address
 
 
void setup(){  
  Wire.begin();  
  Serial.begin( 115200 );
}
 
void loop(){
  Serial.println( Temperature( thermo_addr, 'O' ) ); 
  delay(1000);
}
 
 
/* Function that recieves 3 bytes from resister of MLX90614 *
*    arguments                                                *
*     addr   : slave address                                  *
*     cmd    : command                                        *
*     *array : data array                                     *
**************************************************************/
void Read_MLX90614( char addr, char cmd, char *arry ){
  char i;
  Wire.beginTransmission( addr );     // start recieve process 
  Wire.write( cmd );                  // write command
  Wire.endTransmission(false);        // set repeated start condition
  Wire.requestFrom( addr, 3 );        // request data+pec byte
  while( Wire.available() < 3 );      // wait receive complete
  for( i = 0; i < 2; i++ ){
    arry[i] = Wire.read();            // read data
  }
  Wire.read();                        // pec byte
  Wire.endTransmission();             // end recieve process
}
 
/* Function that reads temperature from MLX90614 *
*      arguments                                 *
*        addr : slave address                    *
*        type : type of temperature              *
*                  'O' or 'o' : reads object     *
*                  other      : reads ambient    *
*      return                                    *
*        Degrees Celsius of temperature          *
*************************************************/
float Temperature( char addr, char type ){
  int tmp;
  char cmd;
  char dat[2];
  float temp;
   
  cmd = ( type == 'O' || type == 'o' )?( 0x07 ):( 0x06 ); // object or ambient
  if(cmd == 0x07){
    Serial.print( "object" ); 
  } else{
    Serial.print( "ambient" ); 
  }
  Serial.print( "\t" ); 
  Read_MLX90614( addr, cmd, dat );                  // recieve from MLX90614
  tmp = ( (dat[1] & 0x007F) << 8 ) + dat[0];        // to temperature in marge two bytes
  temp =  ( (float)tmp *0.02 -0.01 ) - 273.15;      // convert Absolute temperature to Degrees Celsius of temperature
   
  return temp; 
}
