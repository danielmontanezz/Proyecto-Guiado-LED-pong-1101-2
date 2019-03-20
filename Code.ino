#include <LedControl.h>


//Tenemos que incluir una librería
 #include "LedControl.h"
 byte rightscore; //Puntuación Derecha
 byte leftscore; //Puntuación izquierda
 int toneloop=1000;
  byte pongdisplay[]={       // Se usa en el inicio para mostrar "Pong"
     
      B11111111,  B00000000,
      B00001001,  B00000000,
      B00001001,  B00000000,
      B00000110,  B00000000,  
      B00000000,  B00000000,
      B00000000,  B00000000,
      B01110000,  B00000000,
      B10001000,  B00000000,
      B10001000,  B00000000,
      B01110000,  B00000000,
      B00000000,  B00000000,
      B00000000,  B00000000,   
      B11110000,  B00000000,
      B00001000,  B00000000,
      B00001000,  B00000000,
      B11110000,  B00000000,
      B00000000,  B00000000,
      B00000000,  B00000000,
      B11001111,  B00000000,
      B10001001,  B00000000,  
      B10001001,  B00000000,
      B11111111,  B00000000,
      B00000000,  B00000000,
      B00000000,  B00000000,
 };
 
 byte zero[]={      // Se usa para mostrar un '0' para cuando se muestra la puntuación
   B00000000,
   B00000000,
   B00111100,
   B01000010,
   B01000010,
   B01000010,
   B00111100,
   B00000000  
 };
 byte one[]={       // Se usa para mostrar un '1' para cuando se muestra la puntuación
  B00000000,
  B00000000,
  B10001000,
  B10000100,
  B11111110,
  B10000000,
  B10000000,
  B00000000
 };
 
byte two[]={        // Se usa para mostrar un '2' para cuando se muestra la puntuación
  B00000000,
  B01100010,
  B01010001,
  B01010001,
  B01001001,
  B01000110,
  B00000000,
  B00000000
};
byte three[]={      // Se usa para mostrar un '3' para cuando se muestra la puntuación
  B00000000,
  B00100110,
  B01000001,
  B01001001,
  B00110110,
  B00000000,
  B00000000,
  B00000000
};

byte four[]={        // Se usa para mostrar un '4' para cuando se muestra la puntuación
  B00000000,
  B00000000,
  B00001111,
  B00001000,
  B00001000,
  B11111111,
  B00000000,
  B00000000
};
byte five[]={        // Se usa para mostrar un '5' para cuando se muestra la puntuación
  B00000000,
  B00000000,
  B10011110,
  B10010010,
  B10010010,
  B11110010,
  B00000000,
  B00000000
};


byte displayevens=0;
byte displayrow=0;
byte displayindex=0;
byte displayodds=1;
unsigned long time;
unsigned long currenttime;
/*

Ahora necesitamos un LedControl para trabajar.
 pin 12 está conectado a la DataIn
 pin 11 está conectado a la CLK
 el pin 10 está conectado a LOAD
 Tenemos 6 MAX72XX.
 */
byte mode;
byte deflect;
byte carry;
int line;
LedControl lc=LedControl(12,11,10,6);
long randnumber;
byte dir;
int startpos;
byte row;
byte column;
long randNumber;
byte start;
byte diry;
int pass;
byte limit;
#define leftpaddle 0  //La perilla izquierda del pong está conectada a la entrada analógica 0
#define rightpaddle 1 // perilla derecha pong está conectada a la entrada analógica 1
int directionpong;
int rightpongval;
int leftpongval;
byte blinkx;

byte pong[]={            // Almacena la posición de la bola y las paletas.
 B00000000, B00000000,
 B00000000, B00000000,
 B00000000, B00000000,
 B00000000, B00000000,
 B00000000, B00000000,
 B00000000, B00000000,
 B00000000, B00000000,
 B00000000, B00000000,
 
 B00000000, B00000000,
 B00000000, B00000000,
 B00000000, B00000000,
 B00000000, B00000000,
 B00000000, B00000000,
 B00000000, B00000000,
 B00000000, B00000000,
 B00000000, B00000000,
 
 B00000000, B00000000,
 B00000000, B00000000,
 B00000000, B00000000,
 B00000000, B00000000,
 B00000000, B00000000,
 B00000000, B00000000,
 B00000000, B00000000,
 B00000000, B00000000};

void setup(){
  pinMode(leftpaddle, INPUT);   //La paleta izquierda es una entrada.
  pinMode(rightpaddle, INPUT);  // La paleta derecha es una entrada.
   
  Serial.begin(9600);  // Comunicación en serie para la depuración. Establecer a 9600 baudios
  
  // Estas declaraciones a continuación pasan por y configuran las matrices para la configuración dada
  lc.shutdown(0,false);
  lc.setIntensity(0,1);
  lc.clearDisplay(0);
 
  lc.shutdown(1,false);
  lc.setIntensity(1,1);
  lc.clearDisplay(1);
  
  lc.shutdown(2,false);
  lc.setIntensity(2,1);
  lc.clearDisplay(2);
  
  lc.shutdown(3,false);
  lc.setIntensity(3,1);
  lc.clearDisplay(3);
  
  lc.shutdown(4,false);
  lc.setIntensity(4,1);
  lc.clearDisplay(4);
  
  lc.shutdown(5,false);
  lc.setIntensity(5,1);
  lc.clearDisplay(5);
  
  while(displayevens<5){
    while(displayrow<=7){
      lc.setRow(displayevens,displayrow,pongdisplay[displayindex]);
      displayrow++;
      displayindex+=2;
    }
    displayrow=0;
    displayevens+=2;
  }
  
  displayrow=0;
  displayindex=1;
  randomSeed(analogRead(4));
  start=1;
  time=millis();
  currenttime=millis();
  
  while(currenttime-time<3000){
    leftpongval=analogRead(leftpaddle);
    leftpongval=map(leftpongval,1023,20,2,0);  
    if(leftpongval==0){
      lc.setLed(3,2,7,true);
      lc.setLed(3,3,7,false);
      lc.setLed(3,4,7,false);
      mode=1;
      limit=2;
    }
    if(leftpongval==1){
      lc.setLed(3,2,7,true);
      lc.setLed(3,3,7,true);
      lc.setLed(3,4,7,false);
      mode=2;
      limit=1;
    }
    if(leftpongval==2){
      lc.setLed(3,2,7,true);
      lc.setLed(3,3,7,true);
      lc.setLed(3,4,7,true);
      mode=3;
      limit=2;
    }
    delay(50);
    lc.clearDisplay(3);
    currenttime=millis();
  }
  
}
 
void loop(){
  paddles();
  pongsim();
  displayscreen();
}

void paddles(){  //lee datos de las palas y muestra que en la matriz
  int searchbit; 
  int carry=0;
  // Serial.print("LeftPaddle:");
  // Serial.println(analogRead(leftpaddle));
  leftpongval=analogRead(leftpaddle);
  leftpongval=map(leftpongval,0,1010,0,13);
  rightpongval=analogRead(rightpaddle);
  rightpongval=map(rightpongval,1023,20,0,13);

  //  Serial.print("RightPaddle:");
  //  Serial.println(rightpongval);
 
  //clears the previous paddle inorder to display the next one
  pong[0]=B00000000;
  pong[1]=B00000000;
  pong[46]=B00000000;
  pong[47]=B00000000;
  //-------------------------------paleta izquierda
  if(mode!=3){
    if(rightpongval<=7){
      rightpongval=map(rightpongval,0,7,7,0);
      bitSet(pong[46], rightpongval);
      if(rightpongval-1>=0){
        bitSet(pong[46],rightpongval-1);
      }
      else{
        bitSet(pong[47], 7);
        if(mode!=2){
          bitSet(pong[47],6);
          carry=1;
        }
      }
      if(mode!=2){
        if(carry==0){
          if(rightpongval-2>=0){
            bitSet(pong[46], rightpongval-2);
          }
          else{
            bitSet(pong[47], 7);
            carry=1;
          }
        }
      }
    }
 
     if(rightpongval>7){
       rightpongval=map(rightpongval,8,13,7,limit);
       bitSet(pong[47],rightpongval);
       bitSet(pong[47],rightpongval-1);
       if(mode!=2){
         bitSet(pong[47],rightpongval-2);
       }
     }
   }
   else{
     pong[46]=B11111111;
     pong[47]=B11111111;
     lc.setRow(4,7,pong[46]);
     lc.setRow(5,7,pong[47]);
   }
   
 
//----------------------------------paleta derecha 
  carry=0;
  
  if(leftpongval<=7){
    leftpongval=map(leftpongval,0,7,7,0);
    bitSet(pong[0], leftpongval);   
    if(leftpongval-1>=0){
      bitSet(pong[0],leftpongval-1);
    }
    else{
      bitSet(pong[1], 7);
      if(mode!=2){
        bitSet(pong[1],6);
        carry=1;
      }
    }
    if(mode!=2){
      if(carry==0){
        if(leftpongval-2>=0){
          bitSet(pong[0], leftpongval-2);
        }
        else{
          bitSet(pong[1], 7);
        }
      }
    }
  }
 
  if(leftpongval>7){
    leftpongval=map(leftpongval,8,13,7,limit);
    bitSet(pong[1],leftpongval);
    bitSet(pong[1],leftpongval-1);
    if(mode!=2){
      bitSet(pong[1],leftpongval-2);
    }
  }
 
}

void pongsim(){

  if( ((line==2 || line==3) && diry == 0) || ((line==44 || line==45) && diry==1)){
    ball_meets_paddle();
  }

  if(start==1){                      // iniciar nuevo juego
     randnumber=random(1,7);          // Sube con un bit de inicio aleatorio
    dir=random(2);                   // // Crea una dirección de inicio y movimiento aleatoria.
    diry=random(2);                  // Si la dirección y es 0. Movimiento desde abajo hacia arriba
    if(diry==0){                     //  Si la dirección y es 1. Movimiento desde arriba hacia abajo
      line=random(30,34);            // Sube con una línea de inicio al azar entre 30-34
      bitSet(pong[line],randnumber); // Establezca el bit en la línea aleatoria y el bit aleatorio igual a 1 en la matriz pong

    }
    if(diry==1){                     // Si la dirección y es 1. Movimiento de arriba a abajo
      line=random(12,16);           // Sube con una línea de inicio al azar entre 12-16
      bitSet(pong[line],randnumber);  // Establecer el bit en la línea aleatoria y el bit aleatorio igual a 1 en la matriz pong
    }
    start=0;                         // Establecer el inicio variable de vuelta igual a 0
  }

 
  if(diry==0){                        // Si la pelota se mueve de abajo hacia arriba
    if(dir==0){                       // Si la pelota se mueve de derecha a izquierda.
      if(deflect==0 && carry==0){    // Si la bola no se está desviando o trasladándose a otra matriz
        line-=2;                      // Progresa la pelota hacia el otro lado restando 2 de la línea
        pong[line]= pong[line+2] << 1;// Desplaza la bola a la izquierda y ajústala a la nueva línea
        pong[line+2]=B00000000;       // Borrar la antigua línea de la pelota.
      }
      if(carry==1){                   // Si la pelota tiene que trasladarse a una nueva matriz.
        line-=3;                      // // Progresa la pelota hacia el otro lado restando 3 de la línea
        pong[line]=B00000001;         // Establecer la nueva línea
        pong[line+3]=B00000000;       // Borrar la antigua línea de la pelota.
        carry=0;                      // Establece la variable carry de nuevo a 0
        return;                       // terminamos
      }
      if(deflect==1){                // Si la pelota tiene que desviarse de la pared.
        line-=2;                      // Progresa la pelota hacia el otro lado restando 2 de la línea
        pong[line]=B01000000;         // Establecer la nueva línea
        pong[line+2]=B00000000;       // Borrar la antigua línea de la pelota.
        deflect=0;                    // Establecer la variable deflect de nuevo a 0
        dir=1;                        // Cambiar la dirección de izquierda a derecha
        return;                       // terminamos 
      }
      if(bitRead(pong[line],7)==1){   //Si la pelota está en el séptimo bit de la línea.
        if(line==3|| line==5 || line==7 || line==9 || line==11 || line==13 || line==15 ||    // Si la línea es una línea impar
        line==17 || line==19 || line==21 || line==23 || line==25 || line==27 ||
        line==29 || line==31 || line==33 || line==35 || line==37 || line==39 || line==41 ||line==43 || line==45){
          carry=1;              // La pelota necesita ser transferida a otra matriz.
          return;                     // terminamos
        }
        else{                         //  Else la pelota está en una línea uniforme
          if(line!=2){                // La línea no puede ser 2 porque la bola se encuentra con la paleta en 2
            deflect=1;                // La pelota necesita desviarse de la pared.
            return;                   // terminamos
          }
        }
      }
    }
    
    // Los comentarios anteriores deben explicar todas las cosas a continuación. Algunas cosas, diferentes parámetros.
    if(dir==1){
      if(deflect==0 && carry==0){
        line-=2;
        pong[line]= pong[line+2] >> 1;
        pong[line+2]=B00000000;
      }
      if(carry==1){
        line--;
        pong[line]=B10000000;
        pong[line+1]=B00000000;
        carry=0;
        return;
      }
      if(deflect==1){
        line-=2;
        pong[line]=B00000010;
        pong[line+2]=B0000000;
        deflect=0;
        dir=0;
        return;
      }
      if(bitRead(pong[line],0)==1){
        if( line==2 || line==4 || line==6 || line==8 || line==10 || line==12 || line==14 ||
        line==16 || line==18 || line==20 || line==22 || line==24 || line==26 ||
        line==28 || line==30 || line==32 || line==34 || line==36|| line==38 ||  line==40 ||line==42 || line==44 ){
          carry=1;
          return;
        }      
        else{
          if(line!=3){
            deflect=1;
            return;
          }
        }
      }
    }
  }

//--------------------------------------------diry = 1 abajo
  if(diry==1){  
    if(dir==0){
      if(deflect==0 && carry==0){
        line+=2;
        pong[line]= pong[line-2] << 1;
        pong[line-2]=B00000000;
      }
      if(carry==1){
        line+=1;
        pong[line]=B00000001;
        pong[line-1]=B00000000;
        carry=0;
        return;
      }
      if(deflect==1){
        line+=2;
        pong[line]=B01000000;
        pong[line-2]=B00000000;
        deflect=0;
        dir=1;
        return;
      }
      if(bitRead(pong[line],7)==1){
        if( line==5 || line==7 || line==9 || line==11 || line==13 || line==15 ||
        line==17 || line==19 || line==21 || line==23 || line==25 || line==27 ||
        line==29 || line==31 || line==33 || line==35 || line==37 || line==39|| line==41 ||line==43 || line==45){
          carry=1;
          return;
        }
        else{
          if(line!=44){
            deflect=1;
            return;
          }
        }
      }
    }
    
    if(dir==1){
      if(deflect==0 && carry==0){
        line+=2;
        pong[line]= pong[line-2] >> 1;
        pong[line-2]=B00000000;
      }
      if(carry==1){
        line+=3;
        pong[line]=B10000000;
        pong[line-3]=B00000000;
        carry=0;
        return;
      }
      if(deflect==1){
        line+=2;
        pong[line]=B00000010;
        pong[line-2]=B0000000;
        deflect=0;
        dir=0;
        return;
      }
      if(bitRead(pong[line],0)==1){
        if(line==2|| line==4 || line==6 || line==8 || line==10 || line==12 || line==14 ||
        line==16 || line==18 || line==20 || line==22 || line==24 || line==26 ||
        line==28 || line==30 || line==32 || line==34 || line==36|| line==38 || line==40 ||line==42 || line==44  ){
          carry=1;
          return;
        }
        else{
          if(line!=45){
            deflect=1;
            return;
          }
        }
      }
    }
  }
}

void displayscreen(){
  displayevens=0;
  displayrow=0;
  displayindex=0;
  displayodds=1;
  
  while(displayevens<5){
    while(displayrow<=7){
      lc.setRow(displayevens,displayrow,pong[displayindex]);
      displayrow++;
      displayindex+=2;
    }
    displayrow=0;
    displayevens+=2;
  }
  
  displayrow=0;
  displayindex=1;
   
  while(displayodds<6){
    while(displayrow<8){
      lc.setRow(displayodds,displayrow,pong[displayindex]);
      displayrow++;
      displayindex+=2;
    }
    displayrow=0;
    displayodds+=2;
  }
} 

void clearscreen(){
 int clearing=0;
 
  while(clearing<49){
    pong[clearing]=B00000000;
    clearing++;
  }
}

void ball_meets_paddle(){
  byte search1=0;
    
  if(diry==0){ // bola hacia arriva
    if(dir==0){ //bola de  izquierda a derecha 
      while(search1<8){ // encuentra el bit en el que está la pelota
        if(bitRead(pong[line],search1)==1){  // encontró el bit en el que está la pelota
          if(line==2){
            if( search1!=6 && search1!=7){ //si está en la línea 2, asegúrese de que no esté en esos bits especiales
              if( bitRead(pong[0], search1)==1){ //Compruebe que hay una paleta allí
                diry=1; // cambia la dirección y así se dirige hacia otra paleta
                tone(9,500,100);
              }
              else if( bitRead(pong[0], search1+1)==1){
                diry=1;
                dir=1;
                tone(9,500,100);
                if(search1==0){
                  carry=1;
                }
              }
              else { // no remo bola perdida 
                tone(9,500,300);
                clearscreen();
                start=1;
                delay(1000);
                scoreleft();
             }
           }
          
          if(search1==6){// bit especial, necesita desviar el mismo camino en el que viene
            if(bitRead(pong[0],7)==1){ // hay una paleta allí
              pong[2]=B01000000;
              dir=1;
              diry=1;
              tone(9,500,100);
            }
            else{//perdido, no paleta 
              tone(9,500,300);
              clearscreen();
              start=1;
              delay(1000);
              scoreleft();
            }
          }
          
          if(search1==7){// bit especial, primero tiene que rebotar en la pared
            if(bitRead(pong[0],6)==1){// encontré la paleta
              pong[2]=B00100000;
              diry=1;
              dir=1;
              line=2;
              tone(9,500,100);
              displayscreen();
            }
            else{//perdido no paleta
              tone(9,500,300);
              clearscreen();
              start=1;
              delay(1000);
              scoreleft();
            }
          }
        }  
          
        if(line==3){
          if( search1!=7){ // si está en la línea 3, asegúrese de que no esté en esos bits especiales
            if( bitRead(pong[1], search1)==1){ //Compruebe que hay una paleta allí
              diry=1; // cambia la dirección y así se dirige hacia otra paleta
              tone(9,500,100);
            }  
            else if( bitRead(pong[1], search1+1)==1){
              diry=1;
              dir=1;
              tone(9,500,100);
            }
            else { // no paddle, missed the ball
              tone(9,500,300);
              clearscreen();
              start=1;
              delay(1000);
              scoreleft();
            }
          }
          
         
          if(search1==7){// bit especial, necesita ser transferido a la siguiente matriz
            if(bitRead(pong[1],7)==1){ // encontré una paleta allí
              //pong[2]=B10000000;  // limpia la fila en la que estaba
              dir=0;  //misma direccion
              diry=1;  // diferente direccion
              tone(9,500,100);
            }
            else if(bitRead(pong[0],0)==1){
              dir=1;
              diry=1;
              tone(9,500,100);
              carry=0; 
            }
            else{// no remo bola perdida
              tone(9,500,300);
              clearscreen();
              start=1;
              delay(1000);
              scoreleft();
            }    
          }            
         }
        }
      search1++;
      }
    }
    
    
    if(dir==1){// la pelota se mueve de izquierda a derecha
      while(search1<8){ // encuentra el bit en el que está la pelota
        if(bitRead(pong[line],search1)==1){  // encontró el bit en el que está la pelota
          if(line==3){
            if( search1!=1 && search1!=0){ // si está en la línea 3, asegúrese de que no esté en un bit especial
              if(bitRead(pong[1], search1)==1){ // comprueba si hay una paleta
                diry=1; // cambia la dirección y para que la bola se dirija hacia la otra paleta
                tone(9,500,100);
              }
              else if(bitRead(pong[1], search1-1)==1){
                diry=1;
                dir=0;
                tone(9,500,100);
                if(search1==7){
                  carry=1;
                }
              }
              else{ // no remo bola perdida
                tone(9,500,300);
                clearscreen();
                start=1;
                delay(1000);
                scoreleft();
              }
            }
            
           if(search1==1){
             if(bitRead(pong[1],0)==1){
               pong[3]=B00000010;
               dir=0;
               diry=1;
               tone(9,500,100);
             }
             else{
               tone(9,500,300);
               clearscreen();
               start=1;
               delay(1000);
               scoreleft();
             }
           }
        
          if(search1==0){
            if(bitRead(pong[1],0)==1){
              pong[3]=B00000100;
              diry=1;
              dir=0;
              line=3;
              tone(9,500,100);
              displayscreen();
            }
            else{
              tone(9,500,300);
              clearscreen();
              start=1;
              delay(1000);
              scoreleft();
            } 
          }
        }
        
        if(line==2){
          if( search1!=0){
            if(bitRead(pong[0], search1)==1){ // comprueba si hay una paleta
              diry=1; // cambia la dirección y para que la bola se dirija hacia la otra paleta

              tone(9,500,100);
            }  
            else if(bitRead(pong[0], search1-1)==1){
              diry=1;
              dir=0;
              tone(9,500,100);
            }
            else{ // no remo, bola perdida
              tone(9,500,300);
              clearscreen();
              start=1;
              delay(1000);
              scoreleft();
            }
          }
        
          if(search1==0){
            if(bitRead(pong[0],0)==1){
             // pong[2]=B00000001;
              line=2;
              dir=1;
              diry=1;
              tone(9,500,100);
              return;
            }
            else if(bitRead(pong[1],7)==1){
              diry=1;
              dir=0;
              tone(9,500,100);
              carry=0;
            }
            else{
              tone(9,500,300);
              clearscreen();
              start=1;
              delay(1000);
              scoreleft();
            }
          }
         }
       }
       search1++;
      } 
    }
  }
//----------------------------------------------------------------------

 if(diry==1){ // bola hacia arriva
    if(dir==0){ //bola de izquierda a derecha 
      while(search1<8){ // encuentra el bit en el que está la pelota
        if(bitRead(pong[line],search1)==1){  // encontró el bit en el que está la pelota
          if(line==44){
            if( search1!=6 && search1!=7){ // si está en la línea 2, asegúrate de que no esté en esos bits especiales
              if( bitRead(pong[46], search1)==1){// Comprueba que hay una paleta allí.
                diry=0; // cambia la dirección y así se dirige hacia otra paleta
                tone(9,500,100);
              }  
              else if( bitRead(pong[46], search1+1)==1){
                diry=0;
                dir=1;
                tone(9,500,100);
                if(search1==0){
                  carry=1;
                }
              }
              else { // no remo, bola perdida
                tone(9,500,300);
                clearscreen();
                start=1;
                delay(1000);
                scoreright();
               }
            }
            
            if(search1==6){// bit especial, necesita desviar el mismo camino en el que viene
              if(bitRead(pong[46],7)==1){ // hay una paleta allí
                dir=1;
                diry=0;
                tone(9,500,100);
              }
              else{//perdido, no remo
                tone(9,500,300);
                clearscreen();
                start=1;
                delay(1000);
                scoreright();
              }
            }
          
            if(search1==7){// bit especial, primero tiene que rebotar en la pared
              if(bitRead(pong[46],6)==1){// encontré la paleta
                pong[44]=B00100000;
                diry=0;
                dir=1;
                tone(9,500,100);
                displayscreen();
                line=44;
                return;
              }
            }
          }  
          
          if(line==45){
            if( search1!=7){ // si está en la línea 3, asegúrate de que no esté en esos bits especiales
              if( bitRead(pong[47], search1)==1){ // Comprueba que hay una paleta allí.
                diry=0; // cambia la dirección y así se dirige hacia otra paleta
                tone(9,500,100);
              }
              else if( bitRead(pong[47], search1+1)==1){
                diry=0;
                dir=1;
                tone(9,500,100);
              }
              else { // no remo,bola perdida
                tone(9,500,300);
                clearscreen();
                start=1;
                delay(1000);
                scoreright();
              }
            }
          
          if(search1==7){// bit especial, necesita ser transferido a la siguiente matriz
            if(bitRead(pong[47],7)==1){ // encontré una paleta allí
             // pong[45]=B10000000; // limpia la fila en la que estaba
              line=45;
              dir=0;  //misma direccion
              diry=0;  // diferente direccion
              tone(9,500,100);
            }
            else if(bitRead(pong[46],0)==1){
              dir=1;
              diry=0;
              tone(9,500,100);
              carry=0;
            }
            else{// no remo bola perdida
              tone(9,500,300);
              clearscreen();
              start=1;
              delay(1000);
              scoreright();
            } 
           }   
          }         
        }
        search1++;
      }
    }
    
    if(dir==1){//bola hacia la izquierda 
      while(search1<8){ // encuentra el bit en el que está la pelota
        if(bitRead(pong[line],search1)==1){  // encontró el bit en el que está la pelota
          if(line==45){
            if( search1!=1 && search1!=0){ // si está en la línea 3, asegúrese de que no esté en un bit especial
              if(bitRead(pong[47], search1)==1){ // Comprobar si hay una paleta
                diry=0; // cambia la dirección y para que la bola se dirija hacia la otra paleta
                tone(9,500,100);
              }
              else if(bitRead(pong[47], search1-1)==1){
                diry=0;
                dir=0;
                tone(9,500,100);
                if(search1==7){
                  carry=1;
                }
              }          
              else{ // no remo,bola perdida
                tone(9,500,300);
                clearscreen();
                start=1;
                delay(1000);
                scoreright();
              }
            }
           
            if(search1==1){
              if(bitRead(pong[47],0)==1){
                pong[43]=B00000010;
                dir=0;
                diry=0;
                tone(9,500,100);
              }
              else{
                tone(9,500,300);
                clearscreen();
                start=1;
                delay(1000);
                scoreright();
              } 
            }
        
            if(search1==0){
              if(bitRead(pong[47],1)==1){
                pong[45]=B00000100;
                diry=0;
                dir=0;
                line=45;
                tone(9,500,100);
                displayscreen();
              }
              else{
                tone(9,500,300);
                clearscreen();
                start=1;
                delay(1000);
                scoreright();
              }        
            }
          }
          
          if(line==44){ 
            if( search1!=0){
              if(bitRead(pong[46], search1)==1){ // comprueba si hay una paleta
                diry=0; // cambia la dirección y para que la bola se dirija hacia la otra paleta
                tone(9,500,100);
              } 
              else if(bitRead(pong[46], search1-1)==1){
                diry=0;
                dir=0;
                tone(9,500,100);
              }
              else{ // no remo,bola perdida
                tone(9,500,300);
                clearscreen();
                start=1;
                delay(1000);
                scoreright();
              }
            }
        
            if(search1==0){
              if(bitRead(pong[46],0)==1){
                pong[44]=B00000001;
                line=44;
                dir=1;
                diry=0;
                tone(9,500,100);
              }
              else if(bitRead(pong[47],7)==1){
                dir=0;
                diry=0;
                tone(9,500,100);
                carry=0;
              }
              else{
                tone(9,500,300);
                clearscreen();
                start=1;
                delay(1000);
                scoreright();
              }
            }
          }
        }
        search1++;
      }
    }
  }
  pass=1;
  search1=0;
}





void scoreleft(){
  clearscreen();
  rightscore++;
  setscore();
  displayscreen();

  if(rightscore==5){
    while(blinkx++<4){
      clearscreen();
      displayscreen();
      delay(500);
      setscore();
      displayscreen();
      delay(500);
    }
    blinkx=0;
    rightscore=0;
    leftscore=0;
  }
  else{
    delay(2000);
  }
  
  clearscreen();

}

void scoreright(){
  clearscreen();
  leftscore++;
  setscore();
  displayscreen();
   
  if(leftscore==5){
    while(blinkx++<4){
      clearscreen();
      displayscreen();
      delay(500);
      setscore();
      displayscreen();
      delay(500);
    }
    blinkx=0;
    rightscore=0;
    leftscore=0;
  }
  else{
    delay(2000);
  }

  clearscreen();

}

void setscore(){
  
byte setScoreLoop=0;

  while(setScoreLoop<=7){
    if(leftscore==0){
      pong[setScoreLoop*2]=zero[setScoreLoop];
    }  
    if(leftscore==1){
      pong[setScoreLoop*2]=one[setScoreLoop];
    }
    if(leftscore==2){
      pong[setScoreLoop*2]=two[setScoreLoop];
    }  
    if(leftscore==3){
      pong[setScoreLoop*2]=three[setScoreLoop];
    }
    if(leftscore==4){
      pong[setScoreLoop*2]=four[setScoreLoop];
    }  
    if(leftscore==5){
      pong[setScoreLoop*2]=five[setScoreLoop];
    }
    
    if(rightscore==0){
      pong[32+(setScoreLoop*2)]=zero[setScoreLoop];
    }
    if(rightscore==1){
      pong[32+(setScoreLoop*2)]=one[setScoreLoop];
    }
    if(rightscore==2){
      pong[32+(setScoreLoop*2)]=two[setScoreLoop];
    }
    if(rightscore==3){
      pong[32+(setScoreLoop*2)]=three[setScoreLoop];
    }
    if(rightscore==4){
      pong[32+(setScoreLoop*2)]=four[setScoreLoop];
    }
    if(rightscore==5){
      pong[32+(setScoreLoop*2)]=five[setScoreLoop];
    }
    setScoreLoop++;
  }
  
  pong[20]=B00011000;
  pong[22]=B00011000;
  pong[24]=B00011000;
 
}
