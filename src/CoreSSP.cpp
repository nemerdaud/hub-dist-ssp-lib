
#include <CoreSSP.h>
#define NO_RE_DE_PIN 255

CoreSSP* CoreSSP::_thisInstance = nullptr;

CoreSSP::CoreSSP(HardwareSerial& serial, Stream* logSerial,
              byte reDePin, long baudrate, int timeout):_serial(serial){
  this->_thisInstance = this;
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
    digitalWrite(_reDePin, HIGH);
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
  _logPrintln("Error in SSP:" + String(errorNum));
  switch (errorNum) {
    case ERROR_IS_DEAD:
      _logPrintln(F("Flushing cache e recriando SSP"));
    case ERROR_COMMAND_IS_NOT_REGISTERED:
      _logPrintln(F("Comando nao registrado. Ignorando"));
      break;
    case ERROR_COMMAND_IS_NOT_IN_RESERVED_RANGE:
      _logPrintln(F("Comando fora do intervalo permitido"));
      break;
    case ERROR_EOT_WAS_NOT_READ:
      _logPrintln(F("ERR: EOT nao foi lido"));
      break;
    case ERROR_IS_NOT_WAITING_FOR_READ_EOT:
      _logPrintln(F("ERR: EOT nao esperado"));
      break;
  }
  // never dead
  if (_ssp->isDead()) {
    delete _ssp;
    _ssp = nullptr;
    _clearSerialBuffer();
    begin();
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

void CoreSSP::onError(void (*callback)(uint8_t)) {
  _errorCallback = callback;
}
