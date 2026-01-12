#ifndef SD_LOGGER_H
#define SD_LOGGER_H

#include <SD.h>
#include <SPI.h>

#define SD_CHIP_SELECT 53  // Chip select pin for SD card module
#define LOG_FILENAME "templog.csv"
#define MAX_LOG_SIZE 104857600  // 100 MB in bytes
#define BACKUP_FILENAME "templog_old.csv"

File logFile;
bool sdCardReady = false;

void initializeSDCard() {
  if (!SD.begin(SD_CHIP_SELECT)) {
    sdCardReady = false;
    return;
  }
  sdCardReady = true;
  
  // Create header if file doesn't exist
  if (!SD.exists(LOG_FILENAME)) {
    File file = SD.open(LOG_FILENAME, FILE_WRITE);
    if (file) {
      file.println("Timestamp,Temperature_F,Humidity_%,Servo_Status");
      file.close();
    }
  }
}

void checkAndRotateLogFile() {
  if (!sdCardReady) return;
  
  if (SD.exists(LOG_FILENAME)) {
    File file = SD.open(LOG_FILENAME);
    if (file) {
      unsigned long fileSize = file.size();
      file.close();
      
      // If file exceeds 100MB, rotate logs
      if (fileSize > MAX_LOG_SIZE) {
        // Delete old backup if it exists
        if (SD.exists(BACKUP_FILENAME)) {
          SD.remove(BACKUP_FILENAME);
        }
        
        // Rename current log to backup
        SD.rename(LOG_FILENAME, BACKUP_FILENAME);
        
        // Create new log file with header
        File newFile = SD.open(LOG_FILENAME, FILE_WRITE);
        if (newFile) {
          newFile.println("Timestamp,Temperature_F,Humidity_%,Servo_Status");
          newFile.close();
        }
      }
    }
  }
}

void logTemperatureData(double tempF, int humidity, bool servoActive) {
  if (!sdCardReady) return;
  
  // Get timestamp (using millis for uptime, can be enhanced with RTC)
  unsigned long uptime = millis() / 1000;  // seconds
  unsigned long hours = uptime / 3600;
  unsigned long minutes = (uptime % 3600) / 60;
  unsigned long seconds = uptime % 60;
  
  File file = SD.open(LOG_FILENAME, FILE_WRITE);
  if (file) {
    // Write timestamp, temperature, humidity, servo status
    file.print(hours);
    file.print(":");
    if (minutes < 10) file.print("0");
    file.print(minutes);
    file.print(":");
    if (seconds < 10) file.print("0");
    file.print(seconds);
    file.print(",");
    file.print(tempF, 2);
    file.print(",");
    file.print(humidity);
    file.print(",");
    file.println(servoActive ? "ON" : "OFF");
    
    file.close();
  }
}

#endif
