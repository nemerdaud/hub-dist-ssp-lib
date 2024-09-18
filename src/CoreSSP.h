#pragma once

#include <SafeSimpleSerialProtocol.h>
#include <SoftwareSerial.h>

class CoreSSP {
 public:
  typedef void (*errCallBackFnc)(uint8_t); 
  CoreSSP(HardwareSerial& serial, Stream* logSerial, errCallBackFnc errorCallback,
              byte reDePin = 2, long baudrate = 115200, int timeout = 100); 
  virtual void begin() = 0;
 protected:
  static CoreSSP* _thisInstance;
  Stream* _logSerial;
  SafeSimpleSerialProtocol* _ssp;
  HardwareSerial& _serial;
  long _baudrate;
  int _timeout;
  byte _reDePin;
 // do nothing as default
  void _setupToTransmit();
  void _setupToReceive();
  void _clearSerialBuffer();
  static void _defaultErrorCallback(uint8_t errorNum);
  void _logPrintln(String msg);
  static SafeSimpleSerialProtocol* getSSP();
  static Stream* getLogSerial();

  errCallBackFnc _errorCallback;

};
