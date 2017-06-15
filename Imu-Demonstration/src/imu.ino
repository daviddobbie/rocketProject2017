/**
 * Global & environmental variables are declared below for the express purpose
 * of allowing the devleper to tinker.
 */
int BAUD_RATE = 9600;
int SERIAL_OUTPUT_SPEED_IN_MS = 40;
int ENVIRONMENT_IS_DEV = true;

/**
 * Type definition of a struct that shall contain "raw" IMU sensor data with
 * little to no sanitization. It is currently expected that this data will be
 * floats for pitch, roll, and yaw.
 */
typedef struct ImuSensorDataStruct {
    float pitch;
    float roll;
    float yaw;
} ImuSensorDataStruct;

/**
 * A sanitized IMU sensor data struct that contains all values cast into Strings
 * for the express purpose of allowing a data output function to concatenate
 * Strings together to form an output that is acceptable for Serial.println.
 */
typedef struct SanitizedImuDataStruct {
    String pitch;
    String roll;
    String yaw;
} SanitizedImuDataStruct;

/**
 * Arduino setup, including beginning the serial output on 9600 baud.
 */
void setup() {
    Serial.begin(BAUD_RATE);
}

/**
 * Arduino loop, execute this code every SERIAL_OUTPUT_SPEED_IN_MS. The
 * execution pipeline is as follows:
 *
 * 1. Firstly create an ImuSensorDataStruct, and depending on whether the
 * environment we are running in is dev, retrieve data from the IMU into the
 * struct, or retrieve randomized garbage data from a facade representing a mock
 * IMU.
 *
 * 2. Sanitize and transform the raw values from the IMU into data that is
 * tolerable for the serial monitor.
 *
 * 3. Output the data to the serial monitor ready to be read elsewhere.
 */
void loop() {
    // Sample data every 50ms to start with.
    delay(SERIAL_OUTPUT_SPEED_IN_MS);

    // Selection of data from either IMU or mock IMU facade here.
    ImuSensorDataStruct data;
    if (ENVIRONMENT_IS_DEV) {
        data = fetchDataMock();
    } else {
        data = fetchData();
    }

    // Sanitize
    SanitizedImuDataStruct outputData = transformValues(data);

    // Output
    outputToCereal(outputData);
}

/**
 * [fetchDataMock description]
 * @return [description]
 */
ImuSensorDataStruct fetchDataMock() {
    return { random(361), random(361), random(361) };
}

/**
 *
 * @return an IMU data struct containing pitch, roll, and yaw values.
 */
ImuSensorDataStruct fetchData() {
    //
}

/**
 *
 */
SanitizedImuDataStruct transformValues(ImuSensorDataStruct data) {
    return { String(data.pitch), String(data.roll), String(data.yaw) };
}

/**
 * Outputs data over a Serial port as a JSON string, ready to be read by other
 * interested sources.
 *
 * @param outputData A sanitized struct containing data that should be output to
 * the Serial monitor.
 */
void outputToCereal(SanitizedImuDataStruct outputData) {
    String out = "{ \"pitch\": \"" + outputData.pitch + "\", \"roll\": \"" + outputData.roll + "\", \"yaw\": \"" + outputData.yaw + "\"}";
    Serial.println(out);
}
