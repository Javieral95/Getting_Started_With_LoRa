/******************************************************************************************************************/
/**************************** Example function in NodeJS (using express). ****************************************/
/* suppose that we have a Mosquitto Broker running in WSL (chirpstack-docker) using anonymous = true (no secure) */
/*****************************************************************************************************************/
var mqtt = require('mqtt') //https://www.npmjs.com/package/mqtt

//Mqtt config
const host = 'WSL_IP'
const port = '1883' //or your port
const clientId = 'mqtt_NodeApp_' + Math.random().toString(16).slice(3)

const connectUrl = 'mqtt://' + host + ':' + port;

const client = mqtt.connect(connectUrl, {
    clientId,
    clean: true,
    //username: "nodeApp", //No comment if you use Acls and password
    //password: "admin", //Use your user and password
    connectTimeout: 4000,
    reconnectPeriod: 1000,
    debug: true
  })

//Topics
const chirpstackApplicationID = 1; //Check url, for example: http://localhost:8080/#/organizations/1/applications. /1/ is the ID
const chirpstackDeviceID = "DEV_EUI";
const chirpstackReadAppTopic = "application/" + chirpstackApplicationID + "/device/#";
const chirpstackWriteAppTopic = "application/" + chirpstackApplicationID + "/device/"+chirpstackDeviceID+"/EXAMPLE";

//Connection and subscripcion
client.on('connect', function () {
    console.log("Connected")
    client.subscribe(chirpstackReadAppTopic, function (err) {
      if (!err) {
        console.log("Subscribed to topic: "+chirpstackReadAppTopic)
        //client.publish(chirpstackWriteAppTopic, 'Hello mqtt') //For debug and testing
      }
      else {
        console.log("Error in connection:")
        console.log(err)
      }
    })
  })
  
  client.on('message', function (topic, message) {
    // Message is Buffer
    var stringMsg = message.toString();
    console.log(topic + " - " + stringMsg)  
    insertSensorEntry_Mqtt(topic, stringMsg);
  })
  
  client.on('error', function () {
    console.log("ERROR connecting to " + connectUrl)
    client.end()
  })
 
  //FUNCTION
  function insertSensorEntry_Mqtt(topic, msg){
    console.log("INSERTAMOS DATO DE SENSOR RECIBIDO POR MQTT EN TOPICO: "+topic);
    //Format (using the chirpstack decoder using as example):
    //{"applicationID":"1","applicationName":"APP_NAME","deviceName":"DEV_NAME","devEUI":"DEV_ADDRESS",
    //"txInfo":{"frequency":868500000,"dr":5},"adr":true,"fCnt":2,
    //"fPort":1,"data":"DATA","object":{"data":"DATA","decodedData":{"humidity":0,"temperature":-327},"message":"Informacion recibida del nodo"}}
    console.log(msg)
    const parseMsg = JSON.parse(msg);
    var deviceName = parseMsg.deviceName;
    var devAddr = parseMsg.devEUI;
    var temperature = parseMsg.object.decodedData.temperature;
    var humidity = parseMsg.object.decodedData.humidity;
    var success = true;
    
    try {
        await mongoClient.connect();
        console.log("Connect to DB :)");

        const db = mongoClient.db("THE DATABASE NAME");
        const sensors = db.collection("THE COLLECTION NAME");

        var tmp = SensorData({
            sensor_id: devAddr,
            sensor_name: deviceName,
            temperature: temperature,
            humidity: humidity,
        });

        const result = await sensors.insertOne(tmp);
        console.log("1 sensor entry inserted using MQTT");
    }
    catch (err) {
        console.log(err)
        success = false;            
    }
    finally {
        await mongoClient.close();
    }
  }