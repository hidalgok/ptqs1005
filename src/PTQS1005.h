#ifndef PTQS1005_H
#define PTQS1005_H
#include "Stream.h"

class PTQS1005
{
public:

  static const uint16_t BAUD_RATE = 9600;

  struct DATA {
    uint16_t PM_AE_UG_2_5;
    float TVOC_PPM;
    float HCHO_MGM3;
    uint16_t CO2_PPM;
    float TEMP;
    float HUMIDITY;
    
  };

  PTQS1005(Stream&);

  void requestRead();
  bool read(DATA& data);

private:
  enum STATUS { STATUS_WAITING, STATUS_OK };

  uint8_t _payload[14];
  Stream* _stream;
  DATA* _data;
  STATUS _status;

  uint8_t _index = 0;
  uint16_t _frameLen;
  uint16_t _checksum;
  uint16_t _calculatedChecksum;

  void loop();
};

#endif
