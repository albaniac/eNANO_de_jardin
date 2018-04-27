/*
 #       _\|/_   A ver..., ¿que tenemos por aqui?
 #       (O-O)
 # ---oOO-(_)-OOo---------------------------------

 ##########################################################
 # ****************************************************** #
 # *             DOMOTICA PARA PRINCIPIANTES            * #
 # *         eNANO, termometro solar  'nocturno'        * #
 # *            Autor: Eulogio López Cayuela            * #
 # *                                                    * #
 # *          Versión 1.0    Fecha: 21/04/2018          * #
 # ****************************************************** #
 ##########################################################



 ===== NOTAS IMPORTANTES / OPCIONES DISPONIBLES POR AHORA) ===== 

 Luz solar de jardin 'vitaminada'
 Su funcionamiento es el siguiente: 
 Se mantiene encendida durante 20 segundos,  se apaga un breve instante 
 y comienza a mostrar la temperatura mediante parpadeos de colores.
 Se separa la cifra de temperatura en dos digitos, decenas y unidades.
 Primero unos parpadeos verdes a intervalos de 1 segundo muestran las decenas
 despues con parpadeos rojos (si la temperatura es positiva) o
 parpadeos azules (si la temperatura es negativa), se muestran las unidades.
 Si la temperatura fuese de cero grados (0ºC) se mostraria simplemente un breve parpadeo del mismo color rosa 
 Si la temperatura es un multiplo de 10 (es decir 20ºC, o -10ºC...) al no existir unidades para mostrar,
 las temperaturas por debajo de cero se indicarian mostrando un breve parpadeo rosa tras los parpadeos verdes

 Ejemplos:
 +30 ºC --> 3 parpadeos verdes
 +19 ºC --> 1 parpadeo verde, 9 parpadeos rojos
  +6 ºC --> 6 parpadeos rojos
   0 ºC --> 4 parpadeos rapidos en color rosa
  -2 ºC --> 2 parpadeos azules
 -13 ºC --> 1 parpadeo verde, 3 parpadeos azules
 -20 ºC --> 2 parpadeos verdes, 2 parpadeos rapidos en color rosa


  (No se usan los datos de presion del barometro)
*/




/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//        SECCION DE DECLARACION DE CONSTANTES  Y  VARIABLES GLOBALES
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

//------------------------------------------------------
// Algunas definiciones muy personales para mi comodidad al escribir codigo
//------------------------------------------------------
#define AND &&
#define OR ||
#define NOT !
#define ANDbit &
#define ORbit |
#define NOTbit ~
#define XORbit ^



//------------------------------------------------------
// Otras definiciones para pines y variables
//------------------------------------------------------

#define ERROR_TEMPERATURA 1     //valor que se suma a la lectura de este BMP180
                                //en mi caso mide 1 grado menos que otros termometros que tengo calibrados

#define PIN_LED 13              //led OnBoard de Arduino UNO/NANO/MEGA

#define pinRojo    3
#define pinVerde   5
#define pinAzul    6



//------------------------------------------------------
// Importamos las librerias necesarias
//------------------------------------------------------
#include <Wire.h>           //libreria para comunicaciones I2C
#include <SFE_BMP180.h>     //libreria del sensor de presion y temperatura


//------------------------------------------------------
// Creamos las instancia de los objetos:
//------------------------------------------------------

//Creamos el objeto sensor BMP180
SFE_BMP180 sensorBMP180;   //Creamos el objeto 'sensorBMP180' como una instancia del tipo "SFE_BMP180"


//------------------------------------------------------
// Variables GLOBALES
//------------------------------------------------------

float ALTITUD = 407.0;              // Altitud de Sorbas en metros
float altitud_estimada;             //guarda el calculo de altitud estimada en funcion de la presion 
                                    //(Sin utilidad practica)
float PresionRelativaCotaCero = 0;  //(Sin utilidad practica)
float PresionABS = 0;               //variable para la presion leida del BMP180  //(Sin utilidad practica)
                           
float Temperatura = 0;              //variable para la temperatura leida del BMP180

unsigned long momento_para_mostrar_datos = 0;




//*******************************************************
//         FUNCION DE CONFIGURACION
//*******************************************************
void setup()
{
  Serial.begin(9600);   // --> para DEBUG
  sensorBMP180.begin();
  
  pinMode(pinRojo, OUTPUT);   //PIN conectado al rojo del LED
  pinMode(pinVerde, OUTPUT);  //PIN conectado al verde del LED
  pinMode(pinAzul, OUTPUT);   //PIN conectado al azul del LED
  setColor(255, 000, 255);    //rosa intenso
  delay(2000);
  pinMode(13, OUTPUT);        //PIN13 como salida
  digitalWrite(13, LOW);      //apagar PIN13
  
  leerDatosSensorBMP180();    //mirar la temperatura actual
  Temperatura = int(Temperatura) + ERROR_TEMPERATURA;      //eliminar decimales y corregir el error del bmp180
}




//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm 
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm 
//***************************************************************************************************
//  BUCLE PRINCIPAL DEL PROGRAMA   (SISTEMA VEGETATIVO)
//***************************************************************************************************
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm 
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm 

void loop()
{ 
  unsigned long tiempoActual = millis();  
  setColor(255, 000, 255);   //rosa intenso
  
  /* control del tiempo ("intervalo_refresco" milisegundos) entre lecturas de los sensores */
   
  if(tiempoActual > momento_para_mostrar_datos){
    /*LEER LOS SENSORES y ACTUALIZAR VARIABLES*/   
    leerDatosSensorBMP180();  //actualiza Temperatura (y la presion, que no se usa)

    //eliminar decimales y corregir el error del bmp180
    Temperatura = int(Temperatura) + ERROR_TEMPERATURA;
    
    mostar_temperatura_con_parpadeos(Temperatura);
    delay (2000); 
    momento_para_mostrar_datos = millis() + 20000; //actualizamos el momento de la proxima muestra
    setColor(255, 000, 255);   //rosa intenso
  }

}




//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm 
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm 
//***************************************************************************************************
//  BLOQUE DE FUNCIONES: LECTURA SENSORES, TOMA DE DECISIONES,...
//***************************************************************************************************
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm 
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm



/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//    TERMOMETRO   (CONSULTA DE SENSORES Y ACTUALIZACION DE VARIABLES)
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

//========================================================
//  TERMOMETRO / BAROMETRO, usando sensor BMP180
//========================================================
void leerDatosSensorBMP180()
{
  char estado;
  double T,P,p0,a;
  boolean FLAG_error_presion = false;         //banderas por si se desea detectar errores en las mediciones
  boolean FLAG_error_temperatura = false;     //y actuar en consecuencia. (No usado actualmente)

  /* Primero se debe hacer una lectura de la temepratura para poder hacer una medida de presion.
  Se inicia el proceso de lectura de la temperatura.
  Si se realiza sin errores, se devuelve un numero de (ms) de espera, si no, la funcion devuelve 0.
  */
  
  estado = sensorBMP180.startTemperature();
  if (estado != 0)
  {
    // pausa para que se complete la medicion en el sensor.
    delay(estado);

    // Obtencion de la medida de temperatura que se almacena en T:
    // Si la lectura el correcta la funcion devuelve 1, si se producen errores, devuelve 0.

    estado = sensorBMP180.getTemperature(T);
    if (estado != 0)
    {
      Temperatura = T;  //Asignacion a la variable global Temperatura
      
      /* Se inicia el proceso de lectura de la presion.
         El parametro para la resolucion de muestreo varia de 0 a 3 (a mas resolucion, mayor tiempo necesario).
         Si se realiza sin errores, se devuelve un numero de (ms) de espera, si no, la funcion devuelve 0.
      */

      estado = sensorBMP180.startPressure(3);
      if (estado != 0)
      { 
        delay(estado); // pausa para que se complete la medicion en el sensor.
        
        // Obtencion de la medida de Presion que se almacena en P:
        // Si la lectura el correcta la funcion devuelve 1, si se producen errores, devuelve 0.
        estado = sensorBMP180.getPressure(P,T);

        if (estado != 0)
        {
          PresionABS = P;  //Asignacion a variable global

          /* 
          El sensor devuelve presion absoluta. Para compensar el efecto de la altitud
          usamos la funcion interna de la libreria del sensor llamada: 'sealevel'
          P = presion absoluta en (mb) y ALTITUD = la altitud del punto en que estomos (m).
          Resultado: p0 = presion compensada a niveldel mar en (mb)
          */

          p0 = sensorBMP180.sealevel(P,ALTITUD); // 407 metros (SORBAS)
          PresionRelativaCotaCero= p0;  //Asignacion a variable global
        }
        else FLAG_error_presion = true;
      }
      else FLAG_error_presion = true;
    }
    else FLAG_error_temperatura = true;
  }
  else FLAG_error_temperatura = true;
}




/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//    LED RGB anodo comun
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

//========================================================
//   FUNCION PARA ACTIVAR Y GENERAR EL COLOR EN EL LED
//========================================================
void setColor(int red, int green, int blue)
{
  //led de anodo comun
  analogWrite(pinRojo, 255 - red);
  analogWrite(pinVerde, 255 - green);
  analogWrite(pinAzul, 255 - blue); 
}


//========================================================
//  FUNCION PARA MOSTAR LA TEMPERATURA CON UN LED rgb  :)
//========================================================

void mostar_temperatura_con_parpadeos(int cifra)
{
  int Num = abs(cifra);
  int8_t unidades = Num %10 ;               //unidades
  int8_t decenas = (Num % 100) / 10 ;       //decenas
  setColor(000, 000, 000);                  // apagar
  delay (3000);

  /* si la temperatura es 0ºC */
  if(Num==0){ //si hay cero grados
    for(int i=0;i<4;i++){
      setColor(255, 000, 255);              // rosa
      delay(300);
      setColor(000, 000, 000);              // apagar
      delay(300);
    }
    
    return;  // 4 parpadeos rosa rapidos y retorno
  }

  /* MOSTAR DECENAS */
  for(int i=0;i<decenas;i++){
    setColor(000, 255, 000);                // verde
    delay(1000);
    setColor(000, 000, 000);                // apagar
    delay(1000);
  }
 
  setColor(000, 000, 000);                  // apagar
  delay (2000);                             // pausa entre decenas y unidades

  /* MOSTAR UNIDADES */
  /* si las unidades son CERO... */
  if(unidades==0){ 
    if(cifra<0){                            //si la temperatura es bajo cero...
      for(int i=0;i<2;i++){                 //2 parpadeos rosa rapidos (-10 ºC o multiplos de -10)
        setColor(255, 000, 255);            // rosa
        delay(300);
        setColor(000, 000, 000);            // apagar
        delay(300);
      }
    }
    return;  
  }
 
  /*  si existe un valor de unidades --> el color indica si Temperatura es + o -   */
  if(cifra>0){ //temperatura positiva
    for(int i=0;i<unidades;i++){
      setColor(255, 000, 000);              // rojo
      delay(1000);
      setColor(000, 000, 000);              // apagar
      delay(1000);
    }
  }
  else{  //temperatura bajo cero
    for(int i=0;i<unidades;i++){
      setColor(000, 000, 255);              // azul
      delay(1000);
      setColor(000, 000, 000);              // apagar
      delay(1000);
    }
  } 
}


//*******************************************************
//                    FIN DE PROGRAMA
//*******************************************************

