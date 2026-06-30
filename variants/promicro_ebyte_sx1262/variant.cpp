#include "variant.h"
#include "wiring_constants.h"
#include "wiring_digital.h"
#include "ProMicroEbyteSx1262Board.h"

const uint32_t g_ADigitalPinMap[] = {
  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
  16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
  32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47
};

extern ProMicroEbyteSx1262Board board; 

namespace mesh {
  struct NodePrefs;
}
extern mesh::NodePrefs* _node_prefs; 

void initVariant() {
  // If your early I2C bus scan in ProMicroEbyteSx1262Board.cpp flagged the node as headless
  if (board.is_headless) {
    if (_node_prefs != nullptr) {
      // Force your fixed passcode straight into the memory register structure 
      // the absolute millisecond the storage subsystem mounts!
      // Using a standard 6-digit integer (no leading zero macro bugs)
      ((uint32_t*)_node_prefs)[4] = 864200; // Directly overwrites the ble_pin memory offset slot
    }
  }
}

