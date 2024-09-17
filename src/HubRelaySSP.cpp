#include "HubRelaySSP.h"
#include "SafeSimpleSerialProtocol.h"

#define CHARACTER_TIMEOUT 1000
#define CMD_SEND_DEVICE_SWITCH_PRESSED 's'

HubrelaySSP::HubrelaySSP(HardwareSerial& serial, Stream* logSerial,
              byte reDePin, long baudrate, int timeout):CoreSSP(serial,logSerial,
              reDePin, baudrate, timeout) {}

void HubrelaySSP::_onCmdSwitchPressed() {
  uint8_t deviceId = getSSP()->readByte();
  uint8_t qtdSwitch = getSSP()->readByte();
  for (int i = 0; i < qtdSwitch; i++) {
    bool isPressed = getSSP()->readBool();
    if (isPressed && _handleSwitchPressed) {
      _handleSwitchPressed(deviceId, i);
    }
  }
  getSSP()->readEot();
}

void HubrelaySSP::begin() {
  _serial.begin(_baudrate);
  _clearSerialBuffer();
  // ============================
  _ssp = new SafeSimpleSerialProtocol(_serial, _baudrate, CHARACTER_TIMEOUT,
                                      *_errorCallback, 'a', 'z');
  _ssp->setDieInstantlyOnNotRegisteredCommand(false);
  _ssp->registerCommand(CMD_SEND_DEVICE_SWITCH_PRESSED, _onCmdSwitchPressed);
  _ssp->init();
  
  #ifdef HUBRELAY_ONLY_RECEIVE
  _setupToReceive();
  #elif HUBRELAY_ONLY_TRANSMIT
  _setupToTransmit();
  #else
  _setupToReceive();
  #endif
  // registra comando de solicitacao de inicio do consumo do arquivo
}

void HubrelaySSP::loop() { 
  _setupToReceive();
  _ssp->loop(); 
  // receive é prioritario, então nunca volta para transmit
}

void HubrelaySSP::switchPressedCallbackFnc(handleSwitchPressedFnc callback) {
  _handleSwitchPressed = callback;
}

DistributorSSP::DistributorSSP(HardwareSerial& serial, Stream* logSerial,
              byte reDePin, long baudrate, int timeout):CoreSSP(serial,logSerial,
              reDePin, baudrate, timeout) {};

void DistributorSSP::begin() {
  _serial.begin(_baudrate);
  _clearSerialBuffer();
  // ============================
  _ssp = new SafeSimpleSerialProtocol(_serial, _baudrate, CHARACTER_TIMEOUT,
                                      *_errorCallback, 'a', 'z');
  _ssp->setDieInstantlyOnNotRegisteredCommand(false);
  _ssp->init();
  #ifdef HUBRELAY_ONLY_RECEIVE
  _setupToReceive();
  #elif HUBRELAY_ONLY_TRANSMIT
  _setupToTransmit();
  #else
  _setupToReceive();
  #endif
}

bool DistributorSSP::sendSwitchPressed(byte deviceId, byte switchStatus[],
                                       byte qtdSwitches) {
  _ssp->writeCommand(CMD_SEND_DEVICE_SWITCH_PRESSED);
  _ssp->writeByte(deviceId);
  _ssp->writeByte(qtdSwitches);
  for (byte i=0; i< qtdSwitches; i++) {
    _ssp->writeBool(switchStatus[i]);
  }
  _ssp->writeEot();
  return true;
}

