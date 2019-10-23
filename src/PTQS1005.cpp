#include "Arduino.h"
#include "PTQS1005.h"

//Based on the library for plantower sensors found here: https://github.com/fu-hsi/pms 
 
PTQS1005::PTQS1005(Stream& stream)
{
  this->_stream = &stream;
}

// Request read
void PTQS1005::requestRead()
{
  uint8_t command[] = { 0x42, 0x4D, 0xAB, 0x00, 0x00, 0x01, 0x3A };
  _stream->write(command, sizeof(command));
}

// Non-blocking function for parse response.
bool PTQS1005::read(DATA& data)
{
  _data = &data;
  loop();

  return _status == STATUS_OK;
}

void PTQS1005::loop()
{
  _status = STATUS_WAITING;
  if (_stream->available())
  {
    uint8_t ch = _stream->read();

    switch (_index)
    {
      case 0:
        if (ch != 0x42)
        {
          return;
        }
        _calculatedChecksum = ch;
        break;

      case 1:
        if (ch != 0x4D)
        {
          _index = 0;
          return;
        }
        _calculatedChecksum += ch;
        break;

      case 2:
        _calculatedChecksum += ch;
        _frameLen = ch << 8;
        break;

      case 3:
        _frameLen |= ch;
        // Unsupported sensor, different frame length, transmission error e.t.c.
        if (_frameLen != 2 * 9 + 2 && _frameLen != 2 * 13 + 2)
        {
          _index = 0;
          return;
        }
        _calculatedChecksum += ch;
        break;

      default:
        if (_index == _frameLen + 2)
        {
          _checksum = ch << 8;
        }
        else if (_index == _frameLen + 2 + 1)
        {
          _checksum |= ch;

          if (_calculatedChecksum == _checksum)
          {
            _status = STATUS_OK;

            _data->PM_AE_UG_2_5 = makeWord(_payload[0], _payload[1]);
            _data->TVOC_PPM = (makeWord(_payload[2], _payload[3]))/100.0;
            _data->HCHO_MGM3 = (makeWord(_payload[5], _payload[6]))/100.0;
            _data->CO2_PPM = makeWord(_payload[8], _payload[9]);
            _data->TEMP = (makeWord(_payload[10], _payload[11]))/10.0;
            _data->HUMIDITY = (makeWord(_payload[12], _payload[13]))/10.0;
          }

          _index = 0;
          return;
        }
        else
        {
          _calculatedChecksum += ch;
          uint8_t payloadIndex = _index - 4;

          if (payloadIndex < sizeof(_payload))
          {
            _payload[payloadIndex] = ch;
          }
        }

        break;
    }

    _index++;
  }
}
