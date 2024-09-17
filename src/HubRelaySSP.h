#pragma once

#include <CoreSSP.h>
#include <SafeSimpleSerialProtocol.h>
#include <SoftwareSerial.h>

class HubrelaySSP : CoreSSP {
 public:
  virtual ~HubrelaySSP() {}
  HubrelaySSP(HardwareSerial& serial, Stream* logSerial, errCallBackFnc errorCallback, byte reDePin = 2,
              long baudrate = 115200, int timeout = 100);
  void loop();
  void begin() override;
  typedef void (*handleSwitchPressedFnc)(byte deviceId, byte switchNum);  
  void switchPressedCallbackFnc(handleSwitchPressedFnc callback);
 private:
  HardwareSerial* _logSerial;
  void (*_onCmdSwitchStatusCallback)(void);
  static handleSwitchPressedFnc _handleSwitchPressed;

 protected:
  friend class CoreSSP;
  static void _onCmdSwitchPressed();
};

class DistributorSSP : CoreSSP {
 public:
  virtual ~DistributorSSP() {}
  DistributorSSP(HardwareSerial& serial, Stream* logSerial, errCallBackFnc errorCallback, byte reDePin = 2,
                 long baudrate = 115200, int timeout = 100);
  void begin() override;
  bool sendSwitchPressed(byte deviceId, bool switchStatus[], byte switches);
};
