#pragma once

# include <SimpleSerialProtocol.h>

class SafeSimpleSerialProtocol : public SimpleSerialProtocol {
public:
    virtual ~SafeSimpleSerialProtocol() {
        // Aqui você pode colocar qualquer limpeza adicional, se necessário.
    }
    SafeSimpleSerialProtocol(HardwareSerial& serial, long baudrate, int timeout, void (&callback)(uint8_t), char startChar, char endChar)
        : SimpleSerialProtocol(serial, baudrate, timeout, callback, startChar, endChar) {
        // Código adicional, se necessário
    }
    boolean isDead() {
        return _isDead;
    }
};