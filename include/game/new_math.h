#include "math.h"

class Math {
  public:
    static float tanf(float x) { return tan(x); }
    static float powf(float x, float y) { return pow(x, y); }
};

inline double fl_abs(double x) { return __fabs(x); }
