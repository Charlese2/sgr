#include "math.h"

class Math {
  public:
    static float tanf(float x) { return tan(x); }
};

inline double fl_abs(double x) {
    return __fabs(x);
}
