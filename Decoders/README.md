# JavaScript Decoders for TTN and Chirpstack

Se facilitan dos ficheros javascript para facilitar la tarea de descodificación de información en el lado del servidor tanto para _The Things Network_ como para _Chirpstack_.

Adicionalmente también pueden indicarse funciones de codificación, pero no son abordadas en el presente proyecto.

Descodificar los datos recibidos sirve para poder visualizar de la forma deseada los datos desde el servidor (o para enviarlos a otras aplicaciones gracias a las integraciones que ofrece tanto Chirpstack como The Things Network).

### ¿Donde se añaden los _Decoders_?

Los decoders, tal y como indica la documentación general del presente proyecto, se indican en:
* The Things Network: Pestaña aplicaciones de la consola de la aplicación y accedemos al _End Device_ al que buscamos indicarle la función. En la pestaña _Payload Formatters_ seleccionamos _Javascript_ y escribimos el programa para descodificar la información.
* Chirpstack: Para cada perfil de dispositivo (_Device-profiles_) accedemos a la pestaña _codec_ y seleccionamos _Custom JavaScript codec functions_ para escribir el programa que descodificará la información.

## Texto plano

Si se desea enviar **texto plano** (no datos de sensores codificados) basta con usar las siguientes funciones:

* En The Things Network:
```
function Decoder(bytes, port) {
  // Decode plain text; for testing only 
  return {
      myTestValue: String.fromCharCode.apply(null, bytes)
  };
}
```
* En Chirpstack:
```
function Decode(fPort, bytes) {
    var tempObj = new Object();
  	tempObj.data=bytes;
    tempObj.decodedData = String.fromCharCode.apply(null, bytes);
    tempObj.message = "Informacion recibida del nodo";
    return tempObj;
}
```



## Arduino (LoRa Serialization)

Estos ficheros de descodificación (``arduino_ttn.js`` y ``arduino_chirpstark.js``) funcionan para el metodo de codificación utilizado por la liberia [**LoRa Serialization**](https://github.com/thesolarnomad/lora-serialization) para Arduino. Los metodos de descodificación han sido obtenido de los ejemplos de la misma libreria, consultar su documentación para más información.

En resumen, cada tipo de medición hace uso de un número determinado de bytes y se utiliza una función especifica de la libreria para tipo. Después, en el lado servidor se indica que tipo de mediciones se han realizado y este las descodifica y retorna un objeto JSON con los datos.