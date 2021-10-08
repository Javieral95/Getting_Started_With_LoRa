# LORA_TEST for CTIC

El presente proyecto busca realizar una pequeña prueba de concepto de la tecnologia de transmisión LoRa para recabar información que podrá ser usada por la [Fundación CTIC](https://www.fundacionctic.org/es/home). Se enviará información desde un dispositivo final (Nodo o cliente), esta información será recogida por un dispositivo Gateway y este, a su vez, la reenviará a un servidor para almacenarla.

## Tabla de contenido
- [Comenzando](##Comenzando)
- [Pre-requisitos](##Pre-requisitos)
- [LoRaMAC](#LoRaMAC)
- [LoRaWAN](#LoRaWAN)
  * [Servidor](##Servidor) 
    * [The Things Network](###TheThingsNetwork)
    * [ChirpStack (LoRa Server)](###Chirpstack(LoRaServer))
    * [ChirpStack privado en local](###LanzarServidorChirpstackPrivadoEnLocal)
  * [PyCom Gateway](##PycomGateway)
  * [Arduino End-Device](##ArduinoEnd-device)
- [Problemática](#Problemática)
- [Fin](#Fin)

_________________

## Comenzando 

### Primero un poco de información acerca de LoRa 📄

LoRa es una técnología inalambrica (como puede ser WiFi, Bluetooth, LTE, SigFox o Zigbee) que emplea un tipo de modulacion en radiofrecuencia patentada por [Semtech](https://www.semtech.com/).

Actualmente la tecnología esta administrada por la [LoRa Alliance](https://lora-alliance.org/) que certifica a todo fabricanete Hardware que busque trabajar con dicha tecnología.

LoRa tiene una **alta tolerancia a las interferencias**, una alta sensibilidad para recibir datos (-168db), un **MUY bajo consumo** (el dispositivo puede durar una decada con una sola bateria) y un **alcance de 10 a 20km** (dependiendo de si se dispone de visión directa o de la topologia del terreno). Por otro lado, nos encontramos una **muy baja transferencia de datos** (hasta 255 bytes).

Todo esto hace que esta técnologia de transferencia sea util para **grandes distancias** o redes IoT que no necesiten o no disponga de acceso a la corriente eléctrica.

LoRa funciona en las frecuencias libres de trabajo (estas son **868Mhz** en Europa, 916Mhz en América o 433Mhz en Asia, por ejemplo) lo que, juntando a la no necesidad de disponer de un proveedor de red electrica y de un proveedor de telecomunicaciones (usamos radiofrecuencia), hace que se pueda emitir y enviar información sin riesgo.

### Diferentes técnologias LoRa 📌

Podemos comunicar a los dispositivos mediante LoRaMAC (también conocido mediante LoRa) o mediante LoRaWAN.

* **LoRaMAC**: Utilizado para comunicar dos dispositivos entre sí con una topología punto a punto, uno envía información y otro la recoge (o viceversa).

* **LoRaWAN**: Utilizado para comunicar una red de dispositivos. En este enfoque aparece la figura del Gateway (dispositivo que recoge la información de uno o varios dispositivos y la reenvía al servidor). En LoRaWAN la información debe ir filtrada para conocer que dispositivos debe escuchar el Gateway y cuales ignorar, pero esto se verá más adelante.
Este enfoque utiliza una topología en estrella.

Por otro lado, en **LoRaWAN**, también se tienen **tres tipos de dispositivos finales o nodos**:
* De clase **A**: La más soportada y la que supone mayor ahorro de energia (el dispositivo pasa a modo escuchar o Ventana RX después de enviar datos al gateway), esto hace que solo pueda escuchar algo después de enviar información: Utiles para enviar información en intervalos separados de tiempo o cuando se reciba un evento (Ej: La temperatura bajo de 21º).
* De clase **B**: Tiene soporte para recibir paquetes del gateway, permite más espacios entre los ascendentes (enviados) y descendentes (recibidos). Se reduce la latencia de mensajes pero consume más energia.
* De clase **C**: Se dispone de recepción de paquetes continua, el dispositivo solo deja de escuchar en el momento que envía alguna información.

_*En los ejemplos solo se tiene soporte para nodos de clase A y B (soportados por la libreria utilizada), pero solo se implementa el de tipo A._
_____________________________________
## Pre-requisitos 📋

Se precisa de **minimo dos dispositivos** (una para el nodo y otro para el gateway, que debe tener además conectividad WiFi) y de una cuenta asociada en TTS (**The Things Network**) o **Chirpstack** (pudiendo usar un servidor propio en local).
En este ejemplo resultan funcionales tanto los ejemplos LoRaMAC como LoRaWAN que hacen uso de un nodo Arduino y un Gateway Pycom.

### Instalación 🔧

_Para comenzar basta con clonar el repositorio:_


```
git clone https://github.com/Javieral95/LoRa_Test.git
```

_Y después subir los proyectos pertinentes a los dispositivos_
* Para el caso del dispositivo Arduino se ha utilizado **Visual Studio Code** con la Extensión **PlatformIO**.
* Para el caso del dispositivo PyCom se ha utilizado **Atom** con la Extensión **Pymakr**.

¿Por qué dos IDEs? Simple, la extensión Pymakr apenas funciona en Visual Studio Code. Sientete libre de usar el espacio de trabajo que te resulte más comodo.

### Los dispositivos utilizados 🛠️

* El dispositivo PyCom se trata de una placa de desarrollo Fipy con conectividad WiFi, blueetooth, LoRa y Sigfox que se conecta a una placa Pytrack.
* El dispositivo Arduino se trata de una placa WiFi LoRa 32 que emula a la placa Heltec WiFi LoRa 32 (V1), el diagrama de entrada y salida de pines se encuentra en el repositorio con el nombre [_WiFi LoRa 32 Pinout Diagram.jpg_](https://github.com/Javieral95/LoRa_Test/blob/main/WiFi%20LoRa%2032%20Pinout%20Diagram.jpg), que se muestra a continuación.
   * Adicionalmente se ha conectado un sensor de temperatura y humedad AM2315, aunque por el momento no funciona.

Ambos dispositivos disponen de una antera LoRa conectada a ellos.

!["WiFi LoRa 32 Pinout Diagram.jpg"](https://github.com/Javieral95/LoRa_Test/blob/main/WiFi%20LoRa%2032%20Pinout%20Diagram.jpg?raw=true)

_____________________________________

# LoRaMAC 🔩

Los ejemplo LoRaMAC (se encuentran en la carpeta homónima) resultan funcionales haciendo uso de un dispositivo final Arduino y un Gateway PyCom. 

El nodo tan solo envía información hardcodeada y el Gateway tan solo se conecta a LoRa y a WiFi, recibe la información Pycom e imprime los datos leidos (aunque tiene implementada la función de enviar los datos a la red).

Se prescinde del uso de un servidor en red.
# LoRaWAN ⚙️

Para el uso de estos ejemplos (que resultan funcionales haciendo uso de un dispositivo final Arduino y de un Gateway Pycom) se precisa de un servidor para visualizar los datos. En este ejemplo se ha abordado el uso de **The Things Network** y de **Chirpstack** (anteriormente conocido como LoRaServer).
* **Funciona para las versiones LoRa 1.0.2 y 1.0.3.**

### Tipos de autenticación LoRaWAN

Existen dos tipos de autenticación en LoRaWAN:
* **ABP**: "Activation by Personalization", se hardcodea tanto la dirección del dispositivo como las claves de seguridad de este. Es más simple por prescindir del paso previo de autenticación, pero no es seguro.
* **OTAA**: "Over-The-Air Activation", es el metodo seguro de conectarse con el servidor. El dispositivo en primer lugar realiza una operación de autenticación en la red (la petición pasa por el gateway), la dirección del dispositivo es dinamica y las claves de seguridad se negocian con el dispositivo.

_*En los ejemplos por el momento solo se hace uso de OTAA._
_____________________________________
## Servidor 📦

Como ya se ha comentado anteriormente, precisaremos de un servidor. Para este ejemplo se ha usado una versión gratuita de The Things Network y un servidor Chirpstack propiedad de Pycom (próximamente se desplegará uno local).

### The Things Network

Se trata de la alternativa más confiable, segura y mejor documentada. No obstante todo apunta a que dejará de ser abierta (ahora mismo se tiene un limite de 50 nodos por aplicación).

* Debemos dirigirnos a su página oficial: https://www.thethingsnetwork.org/, una vez en ella nos registramos en el botón _Sign Up_.
* Cuando nos registremos y accedamos a nuestra cuenta de usuario, podremos acceder a nuestra [consola](https://console.cloud.thethings.network/) donde elegiremos el cluster que mejor nos convenga (en este caso el Europeo).
* La consola se encuentra dividida en dos grandes apartados: _Aplicaciones_ y _Gateways_, en el primero definiremos las aplicaciones y sus dispositivos finales y en la segunda, como bien indica, los gateways.

#### **Aplicaciones**

Crear una aplicación es sencillo, accedemos al menú y clicamos en el botón +. Una vez ahi indicamos el nombre de la aplicación, un ID unico y una descripción.

Cuando la aplicación sea creada, podremos añadir dispositivos finales (nodos) clicando en el botón +.
* La oferta de dispositivos no es muy amplia, si tu dispositivo no se encuentra en la lista añadelo manualmente. 
   * Indica una versión LoRaWAN 1.0.2 o 1.0.3 y una configuración regional de tipo REV A.
   * Escoge la frecuencia que vayas a utilizar, en este caso la Europea: 863-870MHz.
* Los siguientes IDs son importantes (si no dispones de ellos puedes hacer que TTS los genere por ti):
   * **DevEUI**: 64 bits que funcionan como identificador único de tu dispositivo final.
   * **AppEUI**: 64 bits que funcionan como identificador único de la aplicación, puedes rellenarlo con 0s.
   * **AppKey**: Clave que utilizará el dispositivo en el proceso OTAA de autenticación.

#### **Gateways**

Los gateways son dispositivos que se encargan de enviar el tráfico que llega desde varios dispositivos finales (pertenecientes a varias aplicaciones) y las reenvia al servidor.
Crear un Gateway también es sencillo, clica en el botón + y rellena el formulario prestando atención a los siguientes conceptos:
* **Gateway EUI**: 64 bits que funcionan como identificador único del gateway en hexadecimal, normalmente se introduce la MAC del dispositivo. Si no la conoces en el apartado que explica el gateway PyCom se detallará como obtenerlo.
* **Gateway server address**: Dirección del servidor al que enviar los datos, en este caso _eu1.cloud.thethings.network_
* **Frecuency Plan**: Una vez más escoge el que mas te convenga.

#### **Payload:**

Para poder leer los datos que ha enviado el nodo al servidor se necesita descodificar el payload, en el caso de TTN lo haremos para cada dispositivo, en la pestaña _Payload Formatters_. En este caso como solo mandamos un hola mundo tenemos la siguiente funcion:

```
function Decoder(bytes, port) {
  // Decode plain text; for testing only 
  return {
      myTestValue: String.fromCharCode.apply(null, bytes)
  };
}
```

#### **Nota:**

Fijese en que todas las direcciones hexadecimales de The Things Network se encuentran en mayusculas, es importante a la hora de programar los dispositivos.

### Chirpstack (LoRa Server)

Se trata de la alternativa open source, aún se encuentra en desarollo y su documentación no es tan buena. No obstante funciona y permite lanzar el servidor en local.

#### **Servidor Pycom**

PyCom ofrece un servidor Chirpstack para conectar su dispositivo gateway.

* Accede a https://pybytes.pycom.io/settings/lora-settings y crea tu cuenta.
   * Una vez accedido al servicio, dirigete a la pestaña settings y selecciona LORA SETTINGS.
   * Escoge PyCom Chirpstack Server (fijese que tambié se tiene soporte para The Things Network), escoge LoRa OTAA, la frecuencia deseada (en este caso EU868) y clica en Save.
   * Si es la primera vez que realizas este proceso, recibirás en tu correo electronico tu clave para acceder al servidor: https://loraserver.pycom.io/

#### **Aplicaciones**

La aplicación es similar a la detallada en el apartado de The Things Network. 
* Para crear una aplicacion nos dirigimos al menu con el mismo nombre y clicamos en el boton +.
* Una vez creada, puedes acceder a ella y añadir nodos clicando en el botón +.
   * Introduce un nombre al dispositivo y una descripción.
   * Deberás indicar un **Device EUI**: Se trata de 64 bits en hexadecimal que funcionan como identificador único (puedes hacer que ChirpStack lo genere por ti clicando en la flecha en bucle)
   * Puedes mantener el resto de valores por defecto.
* Necesitarás conocer la **Application Key** del dispositivo para hacer uso de OTAA, esta la encontrarás accediendo al dispositivo y, dentro de él, en la pestaña _Keys (OTAA)_
   * Cambia el perfil del dispositivo a OTAA en la pestaña General de la configuración si aún no se encuentra en ese perfil.

Deberás dirigirte al apartado _Device-profiles_ del servidor, una vez en él acceder al perfil que interesa (OTAA en este caso) y modificar las versiones:
* **LoRaWAN MAC version: 1.0.2 o 1.0.3** con Regional Parameters revisión de tipo **A**
* ADR Algorith por defecto y activar _Device Supports OTAA_ en la pestaña _Join (OTAA / ABP)_ si no esta activado.

#### **Gateway**

Para crear un gateway accede a la pestaña del mismo nombre y clica el botón +. Rellena el formulario prestando especial atención al campo **Gateway ID** (64 bits en hexadecimal que identificanal gateway), puedes hacer que Chirpstack las generé por ti pero suele usarse la MAC del dispositivo (Si no la conoces en el apartado que explica el gateway PyCom se detallará como obtenerlo).

Puedes dejar el resto de valores por defecto.

#### **Payload:**

Para poder leer los datos que ha enviado el nodo al servidor se necesita descodificar el payload, en el caso de Chirpstack lo haremos para cada perfil de dispositivo, en el apartado Device Profiles_ accedemos al perfil que nos interesa (en este caso OTAA) y accedemos a la pestaña _Codec_. En este caso como solo mandamos un hola mundo, podemos escoger en el desplegable _Custom javascript codec functions_ e indicar la siguiente funcion:
```
function Decode(fPort, bytes) {

    var tempObj = new Object();
  	 tempObj.data=bytes;
    tempObj.decodedData = String.fromCharCode.apply(null, bytes);
    tempObj.message = "Informacion recibida del nodo";
    return tempObj;

}
```

#### **Nota:**

Fijese en que todas las direcciones hexadecimales de Chirpstack se encuentran en minusculas, es importante a la hora de programar los dispositivos.

_____________________________________

## Lanzar servidor Chirpstack privado en local 💻

Chirpstack proporciona una alternativa opensource para lanzar nuestro propio servidor privado de LoRaWAN, y nos permite hacerlo de forma simple y mediante contenedores.

Es por ello que se ha clonado en el presente repositorio el repositorio propiedad del fundador de Chirpstack ([brocaar](https://github.com/brocaar)) que permite esta operación: [Chirpstack-docker](https://github.com/brocaar/chirpstack-docker). Lo encontramos en la carpeta [_chirpstack-docker_](https://github.com/Javieral95/LoRa_Test/tree/main/chirpstack-docker).

### Arquitectura

Chirpstack tiene diversos componentes en su arquitectura para hacer que el servicio sea capaz de funcionar, son los siguientes:

!["Chirpstack_server_arquitecture.png"](https://github.com/Javieral95/LoRa_Test/blob/main/Chirpstack_server_arquitecture.png?raw=true)

La forma de desplegar el servidor en forma de contenedores nos permite abstraernos de mucho de los componentes de la arquitectura, no obstante se detallan a continuación:

* Dispositivos LoRa y Gateway: Ya definidos.
* Gateway Bridge: Primer componente de Chirpstack, recibe la información de todos los gateway, la procesa y la envía al servidor MQTT de mensajeria. 
* Network server: Siguiente componente, monitoriza la red y los dispositivos conectados a este. Es capaz de eliminar duplicados (los mensajes de un nodo puede ser captados por más de un gateway) y consolidar la información para ponerla a disposición del servidor de aplicaciones. También es el encargado de autenticar los dispositivos y de enviar los mensajes descendientes.
* Application Server: Tercer componente. Permite crear las _aplicaciones_ (grupos de dispositivos finales que envian informacion). Es capaz de relacionar la información con el dispositivo final y de almacenarla.
* Integraciones: Las más importantes (y necesarias para lanzar el servidor) son:
   *  Broker MQTT: Servicio de mensajeria interna para los componentes de Chirpstack y gateways (cola de mensajeria).
   *  Redis: Motor de base de datos en memoria que gestiona la información de los dispositivos y las aplicaciones creadas.
   *  PostgreSQL: Almacena la configuración de ChirpStack (organizaciones, aplicaciones, usuarios y el historico de información).

### Configuración (Docker)

Antes de desplegar, se debe configurar todos los parametros necesarios en los ficheros de configuración almacenados en el directorio [_configuration_](https://github.com/Javieral95/LoRa_Test/tree/main/chirpstack-docker/configuration).

Puedes consultar la siguiente documentación oficial:

* Gateway Bridge: https://www.chirpstack.io/gateway-bridge/install/config/
* Network server: https://www.chirpstack.io/network-server/install/config/
* Application server: https://www.chirpstack.io/application-server/install/config/

**Nota:** Los ficheros de configuración son sensibles a espacios en blanco o lineas vacías, eliminelas para evitar errores.

### Despliegue (Docker)

Como ya se ha comentado antes, el despliegue en contenedores es sencillo y se encuentra en el directorio [_chirpstack-docker_](https://github.com/Javieral95/LoRa_Test/tree/main/chirpstack-docker).

Una vez que ya se configure lo necesario basta con colocarse en el directorio _chirpstack-docker_ y lanzar:
```
docker-compose up
```

### Configurar servidor 

Con la configuración por defecto podrás acceder al servidor en la dirección [_localhost:8080_](http://localhost:8080/). El usuario será **admin** y la contraseña **admin**.

Comenzemos a añadir la configuración básica:
* Network-Server: Precisamos de añadir primero este componente, accedemos al apartado correspondiente, clicamos en el botón + y en la dirección del servidor introducimos _chirpstack-network-server:8000_ o lo que corresponda en nuestro caso (debido a usar Docker, si lo lanzamos todo en local deberemos introducir localhost).
* Service-Profile: De forma analoga al paso anterior, accedemos a la ventana de creación de un perfil de servicio. Introducimos el nombre que queramos y seleccionamos como Networ-server el que creamos en el paso anterior usando el desplegable... Podemos dejar el resto de parametros por defecto.
* Device-Profile: Ahora crearemos un perfil para los dispositivos finales, en este caso solo nos interesa un perfil para usar OTAA.
  *  Indicamos el nombre que queramos (una buena práctica es indicar OTAA en él).
  *  Seleccionamos el Network-server creado previamente y una versión de LoRaWAN MAC igual a 1.0.2 o 1.0.3.
  *  Parametros regionales de tipo A.
  *  Algoritmo ADR por defecto, Max EIRP y Uplink Interval puede mantenerse en 0.
  *  En la pestaña Join (OTAA / ABP) habilitamos el check _Device supports OTAA_

### Añadiendo dispositivos

Una vez que se ha configurado el servidor tendremos que registrar nuestros Gateways y crear aplicaciones para registrar nuestros dispositivos finales. Este proceso se realiza de forma análoga al explicado en el apartado anterior de la presente documentación: [Chirpstack (LoRa Server)](###Chirpstack(LoRaServer)).

También se deberá indicar la función que descodifica y codifica la información recibida, también se explica en el apartado anterior.
  _____________________________________

## Pycom Gateway 🎧

A continuación se detalla el código utilizado para lanzar la Gateway en una PyCom (Fipy con Pytrack). Este código se encuentra en [_LoRaWAN/LoRaPycomGateway_](https://github.com/Javieral95/LoRa_Test/tree/main/LoRaWAN/LoRaPycomGateway).
* [_Config_](https://github.com/Javieral95/LoRa_Test/blob/main/LoRaWAN/LoRaPycomGateway/config.py): En este archivo es el archivo configurable para hacer funcioanr tu gateway.
* [_Main_](https://github.com/Javieral95/LoRa_Test/blob/main/LoRaWAN/LoRaPycomGateway/main.py): Archivo principal que lanza el Gateway.

### Libreria

Se ha hecho uso de la librería [_NanoGateway py_](https://pycom.io/lopy-lorawan-nano-gateway-using-micropython-and-ttn/) que permite lanzar el gateway en cuestión de minutos.

### Configuración

En el archivo Config se encuentra todo lo necesario para personalizar el gateway:

* En las primeras lineas deberás descomentar las correspondientes al servicio que usarás (The Things network o Chirpstack), a continuación las lines de código de la segunda opción (fijese como para cumplir el formato de Gateway EUI de los servidores se debe rellenar el ID de forma que los seis primeros y los seis ultimos digitos pertenezcan a la MAC del dispositivo, pero los restantes se rellena con Fs).
  * Si has configurado un servidor privado de ChirpStack, deberás indicar la IP (sin puerto) Network-Server en la variable _SERVER_.

```
WIFI_MAC = ubinascii.hexlify(machine.unique_id()) #Debería pasarse a mayusculas para TTS
SERVER = 'loraserver.pycom.io' #(or url of your server)
GATEWAY_ID = WIFI_MAC[:6] + "ffff" + WIFI_MAC[6:12] #Minusculas: Por ser Chirpstack
```
* El puerto puede mantenerse en _1700_, es el que usan ambos servicios.
* Tras ello, se configura el servidor para el reloj, la red WiFi (junto con el Timeout para determinar el error) como la frecuencia de trabajo (en este caso la europea: 865Mhz).

```
NTP = "pool.ntp.org"
NTP_PERIOD_S = 3600

#WiFi settings (change it)
WLAN_SSID = "foo" #"pycom-wifi"
WLAN_PASS = "123abc123" #"securepassword"
WLAN_TIMEOUT_MS = 180000

# for EU868
LORA_FREQUENCY = 868500000
LORA_GW_DR = "SF7BW125" # DR_5,Can change in range: SF7 to SF15
LORA_NODE_DR = 5
```
* La función que se encuentra al final del archivo config imprimira por pantalla la Gateway EUI que puedes copiar y pegar en la configuración del servidor.


```
def get_gateway_id():
    print("Your gateway_id is: {}".format(GATEWAY_ID)) #The gateway is b'THIS_STRING'
    return GATEWAY_ID
```

### Despliegue

Varias funciones del archivo main no son utilizadas, tan solo es necesario lanzar el gateway de la siguiente manera y ya estará funcionando.
```
def init_loraWAN_gateway():
    print("Initializing LoRaWAN nano Gateway")
    nanogw = NanoGateway(
        id=config.GATEWAY_ID,
        frequency=config.LORA_FREQUENCY,
        datarate=config.LORA_GW_DR,
        ssid=config.WLAN_SSID,
        password=config.WLAN_PASS,
        server=config.SERVER,
        port=config.PORT,
        ntp_server=config.NTP,
        ntp_period=config.NTP_PERIOD_S
        )
    print("Ok! Now you have a LoRaWAN Gateway! Lets start it, wait . . .")
    pycom.rgbled(0xAA0000)
    nanogw.start()
    nanogw._log('. . . Yeah! Nano gateway is connected and running, enjoy the log:')
    pycom.rgbled(0x000000)
```
La Pycom mantendra la luz roja hasta que consiga conectarse, una vez escuche peticiones de dispositivos parpadeará su led en color verde.

_____________________________________
## Arduino End-device 📡

### Libreria

Se ha hecho uso de la librería [_MCCI Arduino LoRaWAN_](https://github.com/mcci-catena/arduino-lorawan) que permite abstraerse de muchos aspectos de comunicación LoRa. Ha sido instalada mediante el gestor de librerias de PlatformIO.

Básicamente el código utilizado para el cliente arduino es el que se encuentra en el ejemplo _ttn-otaa.ino_ de la libreria, salvo alguna modificación.

### Configuracion

La configuración se realiza en dos ficheros diferentes:
* Los pines fisicos se indican en el archivo [_LoRaWAN/LoRaArduinoClient/include/hal.h_](https://github.com/Javieral95/LoRa_Test/blob/main/LoRaWAN/LoRaArduinoClient/include/hal.h).
* La configuracion LoRaWAN se realiza en el archivo [_LoRaWAN/LoRaArduinoClient/src/loraWAN.cpp_](https://github.com/Javieral95/LoRa_Test/blob/main/LoRaWAN/LoRaArduinoClient/src/loraWan.cpp).

Toda la configuración relacionada con LoRaWAN, como se ha indicado antes esta indicada en el archivo _loraWAN.cpp_. Al principio del documento se encuentra detallada que datos deben indicarse: **APP_EUI**, **DEV_EUI** y **APP_KEY** (ojo al formato que se indica a continuación).
* **APP_EUI**: Los 64 bits que identifican a la aplicación (indicado en formato LSB), en TTS puede generarse desde su consola o rellenarlos con 0s. Si usas Chirpstack, dejalo relleno con 0s.
* **DEV_EUI**: Indicador del dispositivo, usaremos la MAC o lo copiaremos de la consola de Chirpstack o TTS como en este ejemplo (ojo: En formato LSB, puedes modificar la vista de MSB a LSB en la consola o simplemente darle la vuelta a los bytes).
* **APP_KEY**: Clave para la autenticación mediante OTAA, copiar de la consola de TTS o Chirpstack (En formato MSB, es decir, tal y como está).

A continuación el ejemplo en Chirpstack:
```
static const u1_t PROGMEM APPEUI[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const u1_t PROGMEM DEVEUI[8] = {0x7b, 0x6b, 0xff, 0x2c, 0x7b, 0x2c, 0x19, 0x5a};
static const u1_t PROGMEM APPKEY[16] = {0xbd, 0x21, 0x5a, 0x82, 0xb2, 0xf7, 0x92, 0xf3, 0xc7, 0xcb, 0xb2, 0x88, 0xc7, 0x55, 0x33, 0xe7};
//Observar que esta en minusculas, en TTS se usarian mayusculas.
```

### Despliegue

Tan solo copia el proyecto a tu placa Arduino.

La librería funciona mediante eventos, en este caso los más importantes serán el de autenticación (cuando se complete verás las claves en consola) y el de envío de datos.

El evento en el que se envia datos será _EV_TXCOMPLETE_ en la funcion _void onEvent(ev_t ev)_ del archivo [_loraWAN.cpp_](https://github.com/Javieral95/LoRa_Test/blob/main/LoRaWAN/LoRaArduinoClient/src/loraWan.cpp), observar que el evento incluye la "Ventana RX", momento en el que el dispositivo escucha.
```
    case EV_TXCOMPLETE:
        Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
        if (LMIC.txrxFlags & TXRX_ACK)
            Serial.println(F("Received ack"));
        if (LMIC.dataLen)
        {
            Serial.print(F("Received "));
            Serial.print(LMIC.dataLen);
            Serial.println(F(" bytes of payload"));
        }
        // Schedule next transmission
        os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);
        break;
```
La función, en el mismo fichero, donde se detallará que datos se envían es:
```
void do_send(osjob_t *j)
{
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND)
    {
        Serial.println(F("OP_TXRXPEND, not sending"));
    }
    else
    {
        LMIC_setTxData2(1, mydata, sizeof(mydata) - 1, 0);
        // Prepare upstream data transmission at the next possible time.
        Serial.println(F("Packet queued"));
    }
    // Next TX is scheduled after TX_COMPLETE event.
}
```

**Nota**: Se sufría un error que impedia al nodo recibir paquetes de vuelta, por lo que era imposible autenticar el dispositivo frente al servidor. Se ha tenido que añadir en el _setup()_ del cliente (más concretamente en la función _LoraWan_startJob()_ del archivo _loraWan.cpp_) la siguiente linea de código que aumenta en un 10% el error del reloj:
```
LMIC_setClockError(MAX_CLOCK_ERROR * 10 / 100);
```
_____________________________________
# Problemática 😥

Como bien se sabe, la tasa de transferencia de LoRA es muy baja, lo que provoca una gran perdida de paquetes y una enorme latencia cuando se envía información (en estos ejemplo se envia cada minuto y se visualiza esta perdida), lo que junto a la escasa documentación por ser una nueva técnologia hace que sea algo tediosa trabajar con ella.

Algunos expertos indican que es necesario cierta distancia entre los dispositivos (30m y preferiblemente algún obstaculo entre ellos) para que la comunicación sea más fluida. No ha sido probado y solo se ha lanzado con las dos tarjetas una cerca de la otra en la misma mesa.

Por otro lado se hace uso de versiones antiguas de LoRaWAN (1.0.2 y 1.0.3) que tienen problemas de seguridad que se solventan en parte en la última versión (1.1.0), pero no se dispone de librerias para trabajar con ellas.

Esto no quita que esta técnología pueda _pegar fuerte_ debido a no depender de proveedores externos (de comunicaciones y electricidad), siendo una opción ecónomica y muy llamativa para utilizar en proyectos IoT de grandes ciudades o entornos rurales.

# Fin
## Autores ✒️

* **Javier Álvarez de la Fuente** - *Investigación inicial* para Fundación CTIC - [JavierAl95](https://github.com/javieral95)


## Licencia 📄

Este proyecto ha sido realizado para la Fundación CTIC, su uso es libre y no es necesarío ningún crédito en su uso (Revisar las licencia de las librerias utilizadas).
