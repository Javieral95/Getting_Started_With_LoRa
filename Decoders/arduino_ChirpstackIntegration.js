/**********************************************************************************************************/
/**************************** Example function in NodeJS (using express). *********************************/
/* suppose that its called when a user do a POST request to http://MiExampleAppURL:MyPort/api/sensorData  */
/**********************************************************************************************************/
insertSensorEntry: async (req, res) => {
        console.log("Insert new Sensor data entry:");
        const { deviceName, objectJSON, devAddr} = req.body; //ObjectJSON: The Chirpstack decoder return this object (Rest of parameters are from Chirpstack).
        var success = true;
        var sensorParsedData = JSON.parse(objectJSON);

        var temperature = sensorParsedData.decodedData.temperature;
        var humidity = sensorParsedData.decodedData.humidity;

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
            console.log("1 sensor entry inserted");
        }
        catch (err) {
            console.log(err)
            success = false;
            res.json({ success: false, msg: 'Cannot save sensor entry' })
        }
        finally {
            await mongoClient.close();
        }
        if (success)
            res.json({ success: true, msg: 'Sensor entry has been saved', item: tmp })
}