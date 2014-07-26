#ifndef NVM_H
#define NVM_H

#include <EEPROM.h>

/** NVM provides a templated mechanism for writing data to non-volatile memory
  * easily. 
  *
  * \todo Add a specialized template for type double.
  */
namespace NVM {

  /** Provides a templated function to write data to the non-volatile memory. 
    * The type of the data to be written must be provided as the template argument.
    * 
    * @note This function will currently not work for type double on the AVR platform.
    *
    * @param obj Object to be written to NVM.
    * @param address Memory address to store the object.This value is updated by reference to the next available memory address.
    */
  template <class T>
  void write(T obj,uint16_t* address) {
    uint8_t* buffer = reinterpret_cast<uint8_t*>(&obj);
    for (uint8_t i = 0; i < sizeof(T); i++) {
      EEPROM.write(*address + i, buffer[i]);
    }
    *address += sizeof(T);
  }
  
  /** Provides a templated function to read data to the non-volatile memory. 
    * The type of the data to be read must be provided as the template argument.
    * 
    * @note This function will currently not work for type double on the AVR platform.
    *
    * @param obj Object to be read from NVM.
    * @param address Memory address to access the object in NVM. This value is updated by reference to the next available memory address.
    */
  template <class T>
  T read(uint16_t* address) {
    uint8_t buffer[sizeof(T)];
    for (uint8_t i = 0; i < sizeof(T); i++) {
      buffer[i] = EEPROM.read(*address + i);
    }
    *address += sizeof(T);
    return *reinterpret_cast<T*>(buffer);
  }
  
}

#endif
