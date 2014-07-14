#include "Transfer.h"

Transfer::Transfer() {
	syncSequence[0] = 0xFF;
	syncSequence[1] = 0xFA;
}

void Transfer::setStream(Stream *_stream) {
	stream = _stream;
}

bool Transfer::compareArrays(uint8_t* a, uint8_t* b, uint8_t length) {
  for ( uint8_t i = 0 ; i < length ; i++ ) {
    if ( a[i] != b[i] ) {
      return false;
    }
  }
  return true;
}

bool Transfer::synchronize() {
  uint8_t data[2];
  uint8_t i;
  uint8_t time = 64;
  
  for ( i = 0 ; i < 2 ; i++ ) {
    data[i] = stream->read();
  }
    
  while ( !compareArrays(syncSequence,data,2) ) {
    if ( stream->available() > 0 ) {
			data[0] = data[1];
      data[1] = stream->read();
    }
    if ( time == 0 ) {
      return false;
    }
    time--;
  }
  
  return true;
}