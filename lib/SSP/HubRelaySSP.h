#pragma once

#include <CoreSSP.h>
#include <SafeSimpleSerialProtocol.h>
#include <SoftwareSerial.h>

class HubrelaySSP : CoreSSP {
 public:
  virtual ~HubrelaySSP() {}
  HubrelaySSP(HardwareSerial& serial, Stream* logSerial, byte reDePin = 2,
              long baudrate = 115200, int timeout = 100);
  void loop();
  typedef void (*handleSwitchPressedFnc)(byte deviceId, byte switchNum);  

 private:
  HardwareSerial* _logSerial;
  void (*_onCmdSwitchStatusCallback)(void);
  static handleSwitchPressedFnc _handleSwitchPressed;
  static handleSwitchPressedFnc getSwitchPressedHandler();

 protected:
  friend class CoreSSP;
  void _switchPressedCallbackFnc(handleSwitchPressedFnc callback);
  static void _onCmdSwitchPressed();
  void begin() override;
};

class DistributorSSP : CoreSSP {
 public:
  virtual ~DistributorSSP() {}
  DistributorSSP(HardwareSerial& serial, Stream* logSerial, byte reDePin = 2,
                 long baudrate = 115200, int timeout = 100);
  void begin() override;
  bool sendSwitchPressed(byte deviceId, byte switchStatus[], byte switches);
};
