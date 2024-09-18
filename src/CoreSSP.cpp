
#include <CoreSSP.h>
#define NO_RE_DE_PIN 255

CoreSSP* CoreSSP::_thisInstance = nullptr;

CoreSSP::CoreSSP(HardwareSerial& serial, Stream* logSerial, errCallBackFnc errorCallback,
              byte reDePin, long baudrate, int timeout):_serial(serial), _errorCallback(errorCallback){
  this->_thisInstance = this;
  if (!errorCallback) {
    _errorCallback = this->_defaultErrorCallback;
  }
}
 
void CoreSSP::_setupToTransmit() {
#ifndef HUBRELAY_ONLY_RECEIVE
  if (!_reDeTransmiting && _reDePin != NO_RE_DE_PIN) {
    digitalWrite(_reDePin, HIGH);
    _reDeTransmiting = true;
  }
#endif
}

void CoreSSP::_setupToReceive() {
#ifndef HUBRELAY_ONLY_TRANSMIT
  if (_reDeTransmiting && _reDePin != NO_RE_DE_PIN) {
    digitalWrite(_reDePin, LOW);
    _reDeTransmiting = false;
  }
#endif
}

void CoreSSP::_clearSerialBuffer() {
  while (_serial.available() > 0) {
    _serial.read();
  }
}

void CoreSSP::_defaultErrorCallback(uint8_t errorNum) {
  auto logSerial = _thisInstance->_logSerial;
  logSerial->println("Error in SSP:" + String(errorNum));
  switch (errorNum) {
    case ERROR_IS_DEAD:
      logSerial->println(F("Flushing cache e recriando SSP"));
    case ERROR_COMMAND_IS_NOT_REGISTERED:
      logSerial->println(F("Comando nao registrado. Ignorando"));
      break;
    case ERROR_COMMAND_IS_NOT_IN_RESERVED_RANGE:
      logSerial->println(F("Comando fora do intervalo permitido"));
      break;
    case ERROR_EOT_WAS_NOT_READ:
      logSerial->println(F("ERR: EOT nao foi lido"));
      break;
    case ERROR_IS_NOT_WAITING_FOR_READ_EOT:
      logSerial->println(F("ERR: EOT nao esperado"));
      break;
  }
  // never die
  if (_thisInstance->_ssp->isDead()) {
    delete _thisInstance->_ssp;
    _thisInstance->_ssp = nullptr;
    _thisInstance->_clearSerialBuffer();
    _thisInstance->begin();
  }
}

void CoreSSP::_logPrintln(String msg) {
  if (_logSerial) {
    _logSerial->println(msg);
  }
}

SafeSimpleSerialProtocol* CoreSSP::getSSP() { 
  return _thisInstance->_ssp; 
}
