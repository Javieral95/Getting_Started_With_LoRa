# LoRaWAN mode
El modo de funcionamiento LoRaWAN mas complejo de LoRa y el más recomendado para aplicaciones IoT.

!["LoRaWAN_Topology.png"](https://github.com/Javieral95/Getting_Started_With_LoRa/blob/main/LoRaWAN/LoRaWAN_Topology.png?raw=true)

Como se puede observar en la imagen anterior se tienen varios tipos de dispositivos:
* **EndNodes** (Nodos, clientes o dispositivos finales): Captan información del entorno y la emiten a traves de LoRa.
* **Gateways** (Antenas): Captan la información que proviene de los nodos finales, la recogen y la envian a través de la red (4G, WiFi...) al servidor central.
  * En el servidor se decide que hacer con la información recibida.
  * La información de un mismo nodo puede ser recogida por más de un Gateway, será el servidor quien elimine los duplicados.

La información se envía encriptada con el metodo AES, por lo que los Gateway reciben información a través de LoRa de cualquier dispositivo pero es el servidor quien descarta o no los paquetes recibidos.

Las claves son hardcodeadas o negociadas entre el nodo y el servidor dependiendo del tipo de autenticación utilizado:
* **ABP**: "Activation by Personalization", se hardcodea tanto la dirección del dispositivo como las claves de seguridad de este. Es más simple por prescindir del paso previo de autenticación, pero no es seguro.
* **OTAA**: "Over-The-Air Activation" (el utilizado en los ejemplos), es el metodo seguro de conectarse con el servidor. El dispositivo en primer lugar realiza una operación de autenticación en la red (la petición pasa por el gateway), la dirección del dispositivo es dinamica y las claves de seguridad se negocian con el dispositivo.


## Ejemplos

Se tienen los siguientes ejemplos:
* ``LoRaArduinoClient``
* ``LoRaPycomClient`` (No se ha comprobado su funcionalidad).
* ``LoRaPycomGateway`` (NanoGateway)