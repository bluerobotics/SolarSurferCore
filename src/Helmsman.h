#ifndef HELMSMAN_H
#define HELMSMAN_H

#include <WProgram.h>

namespace Helmsman {
	void setHeading(float heading);
  
  void setPower(float power);
  
  void execute(float heading, float power);

  void executeManual(float steering, float power);

  void resetIntegrals();
}

#endif
