# LoRa(MAC) mode
El modo de funcionamiento LoRa (o LoRaMAC) es el modo de funcionamiento más sencillo de LoRa.

!["LoRaMAC_Topology.png"](https://github.com/Javieral95/Getting_Started_With_LoRa/blob/main/LoRaMAC/LoRaMAC_Topology.png?raw=true)

Como se puede observar en la imagen anterior, un dispositivo emite información mientras que otro la recoge y la interpreta (y viceversa).

## Ejemplos

Se tienen los siguientes ejemplos:
* ``LoRaMACArduinoClient``
* ``LoRaMACPycomClient``
* ``LoRaMACPycomGateway``

En este modo no existe como tal la figura del Gateway que puede existir en el modo _LoRaWAN_, no obstante se usa esa denominación para hacer más graficos los ejemplos: El cliente envia la información y el nodo la intercepta.

Cualquier dispositivo LoRa puede captar los mensajes enviados y leerlos, por lo que los ejemplos de este repositorio no tienen seguridad alguna. No obstante, puede hacerse uso de la "Palabra de seguridad" (que no se encuentra funcionando en este repositorio), esta palabra es conocida por ambos dispositivos y sirve para encriptar y desencriptar la información.