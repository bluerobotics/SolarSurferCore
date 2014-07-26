#include "GPS_UBX.h"

#define LONG(X)    *(long*)(&data[X])
#define ULONG(X)   *(unsigned long*)(&data[X])
#define INT(X)     *(int*)(&data[X])
#define UINT(X)    *(unsigned int*)(&data[X])

namespace {
	unsigned char  state, lstate, code, id, chk1, chk2, ck1, ck2;
	unsigned int  length, idx, cnt;

	unsigned char data[MAX_LENGTH];
	
	long lastTime = 0;
}

namespace GPS_UBX {

long time;
float longitude;
float latitude;
float altitude;
float groundSpeed;
float course;
uint8_t fix;

void enableMsg (unsigned char id, boolean enable) {
  //               MSG   NAV   < length >  NAV
  byte cmdBuf[] = {0x06, 0x01, 0x03, 0x00, 0x01, id, enable ? 1 : 0};
  sendCmd(sizeof(cmdBuf), cmdBuf);
}

void init() {
  Serial1.begin(38400);

  // Modify these to control which messages are sent from module
  enableMsg(POSLLH_MSG, true);    // Enable position messages
  enableMsg(SBAS_MSG, false);      // Enable SBAS messages
  enableMsg(VELNED_MSG, true);    // Enable velocity messages
  enableMsg(STATUS_MSG, true);    // Enable status messages
  enableMsg(SOL_MSG, false);       // Enable soluton messages
  enableMsg(DOP_MSG, false);       // Enable DOP messages
  enableMsg(DGPS_MSG, false);     // Disable DGPS messages
}

void read() {
  if (Serial1.available()) {
    unsigned char cc = Serial1.read();
    switch (state) {
      case 0:    // wait for sync 1 (0xB5)
        ck1 = ck2 = 0;
        if (cc == 0xB5)
          state++;
        break;
      case 1:    // wait for sync 2 (0x62)
        if (cc == 0x62)
          state++;
        else
          state = 0;
        break;
      case 2:    // wait for class code
        code = cc;
        ck1 += cc;
        ck2 += ck1;
        state++;
        break;
      case 3:    // wait for Id
        id = cc;
        ck1 += cc;
        ck2 += ck1;
        state++;
        break;
      case 4:    // wait for length byte 1
        length = cc;
        ck1 += cc;
        ck2 += ck1;
        state++;
        break;
      case 5:    // wait for length byte 2
        length |= (unsigned int) cc << 8;
        ck1 += cc;
        ck2 += ck1;
        idx = 0;
        state++;
        if (length > MAX_LENGTH)
          state= 0;
        break;
      case 6:    // wait for <length> payload bytes
        data[idx++] = cc;
        ck1 += cc;
        ck2 += ck1;
        if (idx >= length) {
          state++;
        }
        break;
      case 7:    // wait for checksum 1
        chk1 = cc;
        state++;
        break;
      case 8:    // wait for checksum 2
        chk2 = cc;
        boolean checkOk = ck1 == chk1  &&  ck2 == chk2;
        if (checkOk) {
          switch (code) {
            case 0x01:      // NAV-
              // Add blank line between time groups
              time = ULONG(0);
              /*if (lastTime != ULONG(0)) {
                lastTime = ULONG(0);
                Serial.print("\nTime: ");
                Serial.println(ULONG(0), DEC);
              }*/
              //Serial.print("NAV-");
              switch (id) {
                case 0x02:  // NAV-POSLLH
                  longitude = LONG(4)/10000000.0f;
                  latitude = LONG(8)/10000000.0f;
                  altitude = LONG(16)/1000.0f; // meters
                  /*Serial.print("POSLLH: lon = ");
				          Serial.print(longitude,10);
                  Serial.print(", lat = ");
                  Serial.print(latitude,10);
                  Serial.print(", vAcc = ");
                  Serial.print(ULONG(24), DEC);
                  Serial.print(" mm, hAcc = ");
                  Serial.print(ULONG(20), DEC);
                  Serial.print(" mm");*/
                 break;
                case 0x03:  // NAV-STATUS
                  fix = data[4];
                  /*Serial.print("STATUS: gpsFix = ");
                  Serial.print(fix, DEC);
                  if (data[5] & 2) {
                    Serial.print(", dgpsFix");
                  }*/
                  break;
                case 0x04:  // NAV-DOP
                  Serial.print("DOP:    gDOP = ");
                  Serial.print((float) UINT(4) / 100, 2);
                  Serial.print(", tDOP = ");
                  Serial.print((float) UINT(8) / 100, 2);
                  Serial.print(", vDOP = ");
                  Serial.print((float) UINT(10) / 100, 2);
                  Serial.print(", hDOP = ");
                  Serial.print((float) UINT(12) / 100, 2);
                  break;
                case 0x06:  // NAV-SOL
                  Serial.print("SOL:    week = ");
                  Serial.print(UINT(8), DEC);
                  Serial.print(", gpsFix = ");
                  Serial.print(data[10], DEC);
                  Serial.print(", pDOP = ");
                  Serial.print((float) UINT(44) / 100.0, 2);
                  Serial.print(", pAcc = ");
                  Serial.print(ULONG(24), DEC);
                  Serial.print(" cm, numSV = ");
                  Serial.print(data[47], DEC);
                  break;
                case 0x12:  // NAV-VELNED
                  groundSpeed = ULONG(20)*0.01f; // cm/s to m/s
                  course = radians((float) LONG(24)/100000.0); // deg to rad
                  if ( course > PI ) {
                    course -= 2*PI;
                  } else if ( course < -PI ) {
                    course += 2*PI;
                  }
                  /*Serial.print("VELNED: gSpeed = ");
                  Serial.print(groundSpeed, 1);
                  Serial.print(" cm/sec, sAcc = ");
                  Serial.print(ULONG(28), DEC);
                  Serial.print(" cm/sec, heading = ");
                  Serial.print(course, 2);
                  Serial.print(" deg, cAcc = ");
                  Serial.print((float) LONG(32) / 100000, 2);
                  Serial.print(" deg");*/
                  break;
                case 0x31:  // NAV-DGPS
                  Serial.print("DGPS:   age = ");
                  Serial.print(LONG(4), DEC);
                  Serial.print(", baseId = ");
                  Serial.print(INT(8), DEC);
                  Serial.print(", numCh = ");
                  Serial.print(INT(12), DEC);
                  break;
                case 0x32:  // NAV-SBAS
                  Serial.print("SBAS:   geo = ");
                  switch (data[4]) {
                    case 133:
                      Serial.print("Inmarsat 4F3");
                      break;
                    case 135:
                      Serial.print("Galaxy 15");
                      break;
                    case 138:
                      Serial.print("Anik F1R");
                      break;
                    default:
                      Serial.print(data[4], DEC);
                      break;
                  }
                  Serial.print(", mode = ");
                  switch (data[5]) {
                    case 0:
                      Serial.print("disabled");
                      break;
                    case 1:
                      Serial.print("enabled integrity");
                      break;
                    case 2:
                      Serial.print("enabled test mode");
                      break;
                    default:
                      Serial.print(data[5], DEC);
                  }
                  Serial.print(", sys = ");
                   switch (data[6]) {
                    case 0:
                      Serial.print("WAAS");
                      break;
                    case 1:
                      Serial.print("EGNOS");
                      break;
                    case 2:
                      Serial.print("MSAS");
                      break;
                     case 16:
                      Serial.print("GPS");
                      break;
                   default:
                      Serial.print(data[6], DEC);
                  }
                  break;
                default:
                  printHex(id);
              }
              //Serial.println();
              break;
            case 0x05:      // ACK-
              /*Serial.print("ACK-");
              switch (id) {
                case 0x00:  // ACK-NAK
                Serial.print("NAK: ");
                break;
                case 0x01:  // ACK-ACK
                Serial.print("ACK: ");
                break;
              }
              printHex(data[0]);
              Serial.print(" ");
              printHex(data[1]);
              Serial.println();*/
              break;
          }
        }
        state = 0;
        break;
    }
  }
}

void printHex (unsigned char val) {
  if (val < 0x10)
    Serial.print("0");
  Serial.print(val, HEX);
}

void sendCmd (unsigned char len, byte data[]) {
  Serial1.write(0xB5);
  Serial1.write(0x62);
  unsigned char chk1 = 0, chk2 = 0;
  for (unsigned char ii = 0; ii < len; ii++) {
    unsigned char cc = data[ii];
    Serial1.write(cc);
    chk1 += cc;
    chk2 += chk1;
  }
  Serial1.write(chk1);
  Serial1.write(chk2);
}

} 
