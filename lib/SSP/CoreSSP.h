#pragma once

#include <SafeSimpleSerialProtocol.h>
#include <SoftwareSerial.h>

class CoreSSP {
 public:
  CoreSSP(HardwareSerial& serial, Stream* logSerial,
              byte reDePin = 2, long baudrate = 115200, int timeout = 100);
  void onError(void (*callback)(uint8_t));
  virtual void begin() = 0;
 protected:
  static CoreSSP* _thisInstance;
  Stream* _logSerial;
  SafeSimpleSerialProtocol* _ssp;
  HardwareSerial& _serial;
  long _baudrate;
  int _timeout;
  byte _reDePin;
  bool _reDeTransmiting;
 // do nothing as default
  void _setupToTransmit();
  void _setupToReceive();
  void _clearSerialBuffer();
  void _defaultErrorCallback(uint8_t errorNum);
  void (*_errorCallback)(uint8_t);
  void _logPrintln(String msg);
  static SafeSimpleSerialProtocol* getSSP();

};
