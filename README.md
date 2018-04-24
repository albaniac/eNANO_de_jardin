# eNANO de jardin
Luz de jardin 'vitaminada' con arduino nano

 Su funcionamiento es el siguiente: 
 Se mantiene encendida durante 20 segundos,  se apaga un breve instante 
 y comienza a mostrar la temperatura mediante parpadeos de colores.
 Se separa la cifra de temperatura en dos digitos, decenas y unidades.
 Primero unos parpadeos verdes a intervalos de 1 segundo muestran las decenas
 despues con parpadeos rojos (si la temperatura es positiva) o
 parpadeos azules (si la temperatura es negativa), se muestran las unidades.
 Si la temperaturafuese de cero grados (0ºC) se postraria simplemente un breve parpadeo del mismo color rosa 
 Si la temperatura es un multiplo de 10 (es decir 20ºC, o -10ºC...) al no existir unidades para mostrar,
 las temperaturas por debajo de cero se indicarian mostrando un breve parpadeo rosa tras los parpadeos verdes

 Ejemplos:
 +30 ºC --> 3 parpadeos verdes
 
 +19 ºC --> 1 parpadeo verde, 9 parpadeos rojos
 
  +8 ºC --> 6 parpadeos rojos
 
   0 ºC --> 4 parpadeos rapidos en color rosa
 
  -2 ºC --> 2 parpadeos azules
 
 -13 ºC --> 1 parpadeo verde, 3 parpadeos azules
 
 -20 ºC --> 2 parpadeos verdes, 2 parpadeos rapidos en color rosa
 


  (No se usan los datos de presion del barometro)
  
  
  Como bateria se ha aprovechado una celda de una bateria de portatil estropeada a la que se le ha añadido
  un circuito protector para carga/descarga dado que va a estar sometida a carga mediante 
  un panel solar de 6v (-0'6 de caida en el diodo).
  
  Arduino se alimetna  mediante sus pines 5V y GND.
  
  Ciertamente no son 5v los que alimentan a arduino (por no añadir mas elementos al montaje)
  pero los 3'7v de la bateria son más que suficientes para hacer funcionar el montaje  
  
