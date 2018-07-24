/*

sergio rivera lavado
mayo 2017

la cnc se mueve a 64 pulsos por 1/10 mm (decima de mm)
(calibracion experimental manual)

API

implementar funcion que pare el movimiento al detectar posicion limite
por interrupcion!

1) longitud maxima < 30cm = 192kpulsos -> 18 bits -> 3 bytes
3 variables de 3 bytes para codificar la posicion absoluta en pulsos
una funcion del api ha de devolver esta info al usuario.
Long variables are extended size variables for number storage, 
and store 32 bits (4 bytes), from -2,147,483,648 to 2,147,483,647.
 
2) otra funcion que diga si se ha acabado de procesar la info mandada
esto es, si se ha recorrido todo el camino

3) funcion que recibe una coordenada relativa 3d y se dirige a ella *IMPLEMENTADO*

4) funcion que recibe una coordenada absoluta 3d y se dirige a ella

5) sigue circulo de radio x, centro y, sentido cw o ccw a cota z

6) funcion calibra recorridos maximos
(calibracion experimental automatica)

7) funcion calibra origen de coordenadas

8) funcion ir a posicion pone broca

9) funcion calibra posicion de la broca

10) funcion de vuelta al origen *IMPLEMENTADO*


*/

void posiciona(char CK, char CW, char EN, char dir, unsigned long k, int V1, int V2);
int tiempo;
char inChar  = 0;        // incoming serial byte
char outChar = 0;        // outcoming serial byte
char outChartemp = 0;
int  resta = 0;

//utilizo los pines digitales 2 al 10
//del arduino mega para controlar las señales
//CK, CW y EN de los tres ejes x, y, z.
char CKx  = 2; // pin reloj x
char CWx  = 3; // pin sentido x
char ENx  = 4; // pin enable x
char dirx = 0; // avanza o retrocede
volatile  unsigned long kx = 1000;  // 1mm?
char CK  = 5; 
char CKy  = 5; 
char CWy  = 6; 
char CW  = 6; 
char ENy  = 7; 
char EN  = 7; 
char diry = 0;
char dir = 0;
char   diry1 = 0;
char   diry2 = 0;
volatile  unsigned long ky = 1000;
char CKz  = 8; 
char CWz  = 9; 
char ENz  = 10; 
char dirz = 0;
volatile  unsigned long kz = 1000;

volatile  unsigned long AbsPosX = 100000;
volatile  unsigned long AbsPosY = 100000;
volatile  unsigned long AbsPosZ = 100000;

volatile  long AbsRelX = 0;
volatile  long AbsRelY = 0;
volatile  long AbsRelZ = 0;

void setup() 
{
  Serial.begin(230400);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

//  configuración de los puertos del AVR
  pinMode(CKx, OUTPUT);
  pinMode(CWx, OUTPUT);
  pinMode(ENx, OUTPUT);
  digitalWrite(CKx, 0);  
  digitalWrite(CWx, 0);  
  digitalWrite(ENx, 0); 
  pinMode(CKy, OUTPUT);
  pinMode(CWy, OUTPUT);
  pinMode(ENy, OUTPUT);
  digitalWrite(CKy, 0);  
  digitalWrite(CWy, 0);  
  digitalWrite(ENy, 0); 
  pinMode(CKz, OUTPUT);
  pinMode(CWz, OUTPUT);
  pinMode(ENz, OUTPUT);
  digitalWrite(CKz, 0);  
  digitalWrite(CWz, 0);  
  digitalWrite(ENz, 0); 
}

void loop() {
 delay(1500);
 byte serialError=0;
 setup();
 while(1)
 {
 serialError=1;
 while(!Serial.available()){
  ;
 }
 if (Serial.available()){
  inChar = Serial.read();
  if (inChar=='x'){
   CK=CKx;
   CW=CWx;
   EN=ENx;
   } else if (inChar=='y'){
   CK=CKy;
   CW=CWy;
   EN=ENy;
   } else if (inChar=='z'){
   CK=CKz;
   CW=CWz;
   EN=ENz;
   } else {
   serialError=0;
   continue;
   }
 }

 while(!Serial.available()){
  ;
 }
 if (Serial.available()){
  inChar = Serial.read();
  if (inChar==','){
   } else {
   serialError=0;
   continue;
   }
 }
 
 while(!Serial.available()){
  ;
 }
 if (Serial.available()){
  inChar = Serial.read();
  if (inChar=='1'){
   dir=1;
   } else if (inChar=='0'){
   dir=0;
   } else {
   serialError=0;
   continue;
   }
 }

 while(!Serial.available()){
  ;
 }
 if (Serial.available()){
  inChar = Serial.read();
  if (inChar==','){
   } else {
   serialError=0;
   continue;
   }
 }
 
 int j=0;
 double kx=0;
 while(j<5){
  while(!Serial.available()){
   ;
  }
  if (Serial.available()){
   inChar = Serial.read();
   kx+=pow(10,4-j)*(inChar-48);
  }
  j++;
 }
 kx=round(kx);

 while(!Serial.available()){
  ;
 }
 if (Serial.available()){
  inChar = Serial.read();
  if (inChar==','){
   } else {
   serialError=0;
   continue;
   }
 }

 j=0;
 double t1=0;
 while(j<5){
  while(!Serial.available()){
   ;
  }
  if (Serial.available()){
   inChar = Serial.read();
   t1+=pow(10,4-j)*(inChar-48);
  }
  j++;
 }
 t1=round(t1);

 while(!Serial.available()){
  ;
 }
 if (Serial.available()){
  inChar = Serial.read();
  if (inChar==','){
   } else {
   serialError=0;
   continue;
   }
 }

 j=0;
 double t2=0;
 while(j<5){
  while(!Serial.available()){
   ;
  }
  if (Serial.available()){
   inChar = Serial.read();
   t2+=pow(10,4-j)*(inChar-48);
  }
  j++;
 }
 t2=round(t2);

 while(!Serial.available()){
  ;
 }
 if (Serial.available()){
  inChar = Serial.read();
  if (inChar==','){
   } else {
   serialError=0;
   continue;
   }
 }
 if(serialError=1){
  posiciona(CK, CW, EN, dir, (long) kx, (long) t1, (long) t2);
 }
 }








//          xyz,         10, 00000,00000,00000
//posiciona(CK, CW, EN, dirx, kx, 1, 160);

// regresa_origen();
   
  while(1)
  {
    delay(100);
  }
}





















//------------------------------------------------
//desplazamiento relativo
//en linea recta
//dx,dy,dz en mm/10
void recta(long dx, long dy, long dz, long k)
 // long k=20; //N saltos entre ejes
              // no más de 10/mm
              // cada eje se mueve un poquito
{
  byte del1 = 0; //
  byte del2 = 310; // sin un delay
                // los motores no responden
  kx=(abs(dx))*64/k; //64 pulsos por 1/10 mm
  ky=(abs(dy))*64/k;
  kz=(abs(dz))*64/k;
  
  dirx = 0;
  diry1 = 0;
  diry2 = 1;
  dirz = 0;
  AbsRelX = AbsRelX + dx*64;
  AbsRelY = AbsRelY + dy*64;
  AbsRelZ = AbsRelZ + dz*64;
  Serial.print("1 ");
  Serial.println(AbsRelZ);
  if(dx<0)
  {
    dirx = 1;
  }
  if(dy<0)
  {
    diry1 = 1;
    diry2 = 0;
  }
  if(dz<0)
  {
    dirz = 1;
  }
  
  while(k>0)
  {
    //        pin, pin, pin, bool, int
    if(dx |= 0)
      posiciona(CKx, CWx, ENx, dirx, kx, 1, 160);
      delay(100);
    if(dy |= 0){      
      posiciona(CKy, CWy, ENy, diry1, ky, 1, 150);

      
    //  posiciona(CKy, CWy, ENy, diry1, ky+10, 1, 56);
      delay(100);
     // posiciona(CKy, CWy, ENy, diry2, 10, 100, 560);
    }
    if(dz |= 0)
      posiciona(CKz, CWz, ENz, dirz, kz, 1, 160);
    k--;
  delay(del1);
  }
//  delay((1/(kx+1)+1/(ky+1)+1/(kz+1))*del2);
}
//------------------------------------------------

//------------------------------------------------
//esta función mueve la cnc en 3 ejes en cierto sentido
//un número de pasos k
//                 pin,     pin,     pin,     bool,   int
void posiciona3d( char CKx, char CWx, char ENx, char dirx, unsigned long kx,
                  char CKy, char CWy, char ENy, char diry, unsigned long ky,
                  char CKz, char CWz, char ENz, char dirz, unsigned long kz)
{
//  volatile byte i=0;
//  digitalWrite(CWx, dirx);  
//  digitalWrite(CWy, diry);  
//  digitalWrite(CWz, dirz);  
//  
//  long kprod = kx*ky*kz;
//     
//  while (kprod>0)
//  {
//
//      digitalWrite(CKx, 1);
//      digitalWrite(ENx, 1);
////      digitalWrite(CKy, 1);  
////      digitalWrite(ENy, 1);
////      digitalWrite(CKz, 1);  
////      digitalWrite(ENz, 1);
//      delayMicroseconds(1);
//      
//    if(1){
//      digitalWrite(CKx, 0);  
//      digitalWrite(ENx, 0);
////      digitalWrite(CKy, 0);  
////      digitalWrite(ENy, 0);
////      digitalWrite(CKz, 0);  
////      digitalWrite(ENz, 0);
//      delayMicroseconds(163330);
//      kprod--;
//    }
//        
//  }
  
}
//------------------------------------------------

//------------------------------------------------
//esta función mueve la cnc en cierto eje en cierto sentido
//un número de pasos k
//                 pin,     pin,     pin,     bool,   int
void posiciona(char CK, char CW, char EN, char dir,  unsigned long k, int V1, int V2)
{
  volatile byte i=0;
  digitalWrite(CW, dir);     
  while (k>0)
  {
  
    tiempo = micros();
    
    digitalWrite(CK, 1);  
    digitalWrite(EN, 1);  
  //  i = 1;
    i = V1;
    while(i>0)
    {
      i--;
    }  

    digitalWrite(CK, 0);
    digitalWrite(EN, 0);
    i = V2;
   // i = 160;
    while(i>0)
    {
      i--;
    }  
    k--;
  }
  Serial.println(1);
}
//------------------------------------------------

void regresa_origen()
{
  byte del = 1; // sin un delay
                // los motores no responden
  int k=1;
  kx=(abs(AbsRelX)); //64 pulsos por 1/10 mm
  ky=(abs(AbsRelY));
  kz=(abs(AbsRelZ));
  
  dirx = 1;
  diry = 1;
  dirz = 1;
  
  if(AbsRelX<0)
    dirx = 0;
  if(AbsRelY<0)
    diry = 0;
  if(AbsRelZ<0)
    dirz = 0;
    
  
  Serial.print(" 2 ");
  Serial.println(AbsRelZ);
  
  if(AbsRelZ |= 0)
    posiciona(CKz, CWz, ENz, dirz, kz,1,160);
    
    posiciona(CKz, CWz, ENz, 1, 6400,1,160); //////////////////
    
  delay(del);
      
  while(k>0)
  {
    //        pin, pin, pin, bool, int
    if(AbsRelX |= 0)
      posiciona(CKx, CWx, ENx, dirx, kx,1,160);
    if(AbsRelY |= 0)
      posiciona(CKy, CWy, ENy, diry, ky,1,160);
    delay(del);
    k--;
  }

    posiciona(CKz, CWz, ENz, 0, 6400,1,160); //////////////////
    
  //if(AbsRelZ |= 0)
  //  posiciona(CKz, CWz, ENz, not dirz, kz);
  
  pinMode(CKx, INPUT);
  pinMode(CWx, INPUT);
  pinMode(ENx, INPUT);
  pinMode(CKy, INPUT);
  pinMode(CWy, INPUT);
  pinMode(ENy, INPUT);
  pinMode(CKz, INPUT);
  pinMode(CWz, INPUT);
  pinMode(ENz, INPUT);
}


char leeSensores()
{
  u8 maxX   = digitalRead(30);
  u8 minX   = digitalRead(32);
  u8 maxY   = digitalRead(34);
  u8 minY   = digitalRead(36);
  u8 maxZ   = digitalRead(38);
  u8 minZ   = digitalRead(40);
  char medida = maxX + 2 * minX + 4 * maxY + 8 * minY + 16 * maxZ + 32 * minZ + 48;
  return medida;
}
