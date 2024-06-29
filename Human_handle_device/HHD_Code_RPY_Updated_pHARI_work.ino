//#include <Arduino.h>
// This demo explores two reports (SH2_ARVR_STABILIZED_RV and SH2_GYRO_INTEGRATED_RV) both can be used to give 
// quartenion and euler (yaw, pitch roll) angles.  Toggle the FAST_MODE define to see other report.  
// Note sensorValue.status gives calibration accuracy (which improves over time)
#include <Adafruit_BNO08x.h>

// For SPI mode, we need a CS pin
#define BNO08X_CS 10
#define BNO08X_INT 9

// #define FAST_MODE
//  For SPI mode, we also need a RESET 
//#define BNO08X_RESET 5
// but not for I2C or UART
#define BNO08X_RESET -1

struct euler_t {
  float yaw;
  float pitch;
  float roll;
} ypr;

Adafruit_BNO08x  bno08x(BNO08X_RESET);
sh2_SensorValue_t sensorValue;

#ifdef FAST_MODE
  // Top frequency is reported to be 1000Hz (but freq is somewhat variable)
  sh2_SensorId_t reportType = SH2_GYRO_INTEGRATED_RV;
  long reportIntervalUs = 2000;
#else
  // Top frequency is about 250Hz but this report is more accurate
  sh2_SensorId_t reportType = SH2_ARVR_STABILIZED_RV;
  long reportIntervalUs = 2000;
#endif

void setReports(sh2_SensorId_t reportType, long report_interval) {
//  Serial.println("Setting desired reports");
  if (! bno08x.enableReport(reportType, report_interval)) {
//    Serial.println("Could not enable stabilized remote vector");
  }
}

void setup(void) {

  Serial.begin(230400);
  Serial1.begin(230400);

  delay(2000);
  
//  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit BNO08x test!");

  // Try to initialize!
  if (!bno08x.begin_I2C()) {
  //if (!bno08x.begin_UART(&Serial1)) {  // Requires a device with > 300 byte UART buffer!
  //if (!bno08x.begin_SPI(BNO08X_CS, BNO08X_INT)) {
    Serial.println("Failed to find BNO08x chip");
    while (1) { delay(10); }
  }
  Serial.println("BNO08x Found!");

  setReports(reportType, reportIntervalUs);
  Serial.print(reportType);

  Serial.println("Reading events");
  delay(100);
}

void quaternionToEuler(float qr, float qi, float qj, float qk, euler_t* ypr, bool degrees = false) {

    float sqr = sq(qr);
    float sqi = sq(qi);
    float sqj = sq(qj);
    float sqk = sq(qk);

    ypr->yaw = atan2(2.0 * (qi * qj + qk * qr), (sqi - sqj - sqk + sqr));
    ypr->pitch = asin(-2.0 * (qi * qk - qj * qr) / (sqi + sqj + sqk + sqr));
    ypr->roll = atan2(2.0 * (qj * qk + qi * qr), (-sqi - sqj + sqk + sqr));

    if (degrees) {
      ypr->yaw *= RAD_TO_DEG;
      ypr->pitch *= RAD_TO_DEG;
      ypr->roll *= RAD_TO_DEG;
    }
}

void quaternionToEulerRV(sh2_RotationVectorWAcc_t* rotational_vector, euler_t* ypr, bool degrees = false) {
    quaternionToEuler(rotational_vector->real, rotational_vector->i, rotational_vector->j, rotational_vector->k, ypr, degrees);
}

void quaternionToEulerGI(sh2_GyroIntegratedRV_t* rotational_vector, euler_t* ypr, bool degrees = false) {
    quaternionToEuler(rotational_vector->real, rotational_vector->i, rotational_vector->j, rotational_vector->k, ypr, degrees);
}

void loop() {

//  if (bno08x.wasReset()) {
//    Serial.print("sensor was reset ");
//    setReports(reportType, reportIntervalUs);
//  }

  sh2_SensorId_t reportType = SH2_ARVR_STABILIZED_RV;
  long reportIntervalUs = 2000;
  setReports(reportType, reportIntervalUs);

  if (bno08x.getSensorEvent(&sensorValue)) {
    
    // in this demo only one report type will be received depending on FAST_MODE define (above)
    switch (sensorValue.sensorId) {
      case SH2_ARVR_STABILIZED_RV:
        quaternionToEulerRV(&sensorValue.un.arvrStabilizedRV, &ypr, true);
      case SH2_GYRO_INTEGRATED_RV:
        // faster (more noise?)
        quaternionToEulerGI(&sensorValue.un.gyroIntegratedRV, &ypr, true);
        break;
    }

//    Serial.print(ypr.roll);
//    Serial.print(",");
//    Serial.print(ypr.pitch);
//    Serial.print(",");
//    Serial.print(ypr.yaw);
//    Serial.println(",");

    int roll_data_scaled    = 500000 + ypr.roll   * 100.0;
    int pitch_data_scaled   = 500000 + ypr.pitch  * 100.0;
    int yaw_data_scaled     = 500000 + ypr.yaw    * 100.0;
    String roll_data      = String(roll_data_scaled);
    String pitch_data     = String(pitch_data_scaled);
    String yaw_data       = String(yaw_data_scaled);

    String temp3 = "," + roll_data + "_" + pitch_data + "_" + yaw_data + "/";
//                    1  +     6     +  1  +     6      +  1  +     6    +  1 = 1 + 18 + 3 + 1 = 25

    char HHD_data[25];
    temp3.toCharArray(HHD_data, 25);
    Serial.println(HHD_data);
    Serial1.println(HHD_data);

//  sh2_SensorId_t reportType = SH2_LINEAR_ACCELERATION;
//  long reportIntervalUs = 5000;
//  setReports(reportType, reportIntervalUs);
    
//    switch (sensorValue.sensorId) {
////      case SH2_LINEAR_ACCELERATION:
////        break;
//      case SH2_ACCELEROMETER:
//        break;
//      case SH2_LINEAR_ACCELERATION:
////        Serial.print(sensorValue.un.linearAcceleration.x);
////        Serial.print(",");
////        Serial.print(sensorValue.un.linearAcceleration.y);
////        Serial.print(",");
////        Serial.println(sensorValue.un.linearAcceleration.z);
//        break;
//    }

//      case SH2_ACCELEROMETER:
//        break;
//      case SH2_LINEAR_ACCELERATION:
////        Serial.print(sensorValue.un.linearAcceleration.x);
////        Serial.print(",");
////        Serial.print(sensorValue.un.linearAcceleration.y);
////        Serial.print(",");
////        Serial.println(sensorValue.un.linearAcceleration.z);
//        break;

//    Serial.print(sensorValue.un.linearAcceleration.x);
//    Serial.print(",");
//    Serial.print(sensorValue.un.linearAcceleration.y);
//    Serial.print(",");
//    Serial.println(sensorValue.un.linearAcceleration.z);

//    int x_local_acceleration_scaled = 500000 + sensorValue.un.linearAcceleration.x   * 100.0;
//    int y_local_acceleration_scaled = 500000 + sensorValue.un.linearAcceleration.y   * 100.0;
//    int z_local_acceleration_scaled = 500000 + sensorValue.un.linearAcceleration.z   * 100.0;
//
//    String x_local_acceleration_data    = String(x_local_acceleration_scaled);
//    String y_local_acceleration_data    = String(y_local_acceleration_scaled);
//    String z_local_acceleration_data    = String(z_local_acceleration_scaled);



//    String temp3 = "," + x_local_acceleration_data + "_" + y_local_acceleration_data + "_" + z_local_acceleration_data + "_" + roll_data + "_" + pitch_data + "_" + yaw_data + "/";
//                   1    +             6             +  1  +             6             +  1  +              6            +  1  +     6      + 1  +      6     +  1  +   6      +  1 + extra byte
//     char HHD_data[44];
//    temp3.toCharArray(HHD_data, 44);
//    Serial.println(HHD_data);
//    Serial1.println(HHD_data);
  }

}
