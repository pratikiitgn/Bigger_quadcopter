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
//  sh2_SensorId_t reportType = SH2_GYRO_INTEGRATED_RV;
//  long reportIntervalUs = 2000;
//#else
  // Top frequency is about 250Hz but this report is more accurate
  sh2_SensorId_t reportType = SH2_ARVR_STABILIZED_RV;
  long reportIntervalUs = 2000;
#else
  sh2_SensorId_t reportType = SH2_LINEAR_ACCELERATION;
  long reportIntervalUs = 5000;
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


void loop() {

  if (bno08x.getSensorEvent(&sensorValue)) {

  setReports(reportType, reportIntervalUs);
    
    switch (sensorValue.sensorId) {
      case SH2_ACCELEROMETER:
        break;
      case SH2_LINEAR_ACCELERATION:
        break;
    }

//    Serial.print(sensorValue.un.linearAcceleration.x);
//    Serial.print(",");
//    Serial.print(sensorValue.un.linearAcceleration.y);
//    Serial.print(",");
//    Serial.println(sensorValue.un.linearAcceleration.z);

    int x_local_acceleration_scaled = 500000 + sensorValue.un.linearAcceleration.x   * 100.0;
    int y_local_acceleration_scaled = 500000 + sensorValue.un.linearAcceleration.y   * 100.0;
    int z_local_acceleration_scaled = 500000 + sensorValue.un.linearAcceleration.z   * 100.0;

    String x_local_acceleration_data    = String(x_local_acceleration_scaled);
    String y_local_acceleration_data    = String(y_local_acceleration_scaled);
    String z_local_acceleration_data    = String(z_local_acceleration_scaled);

    String temp3 = "," + x_local_acceleration_data + "_" + y_local_acceleration_scaled + "_" + z_local_acceleration_data + "/";

    char HHD_data[23];
    temp3.toCharArray(HHD_data, 23);
    Serial.println(HHD_data);
    Serial1.println(HHD_data);
  }

}
