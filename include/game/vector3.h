class vector3 {
  public:
    float x;
    float y;
    float z;
    vector3(){};
};

class transform{
  public:
    vector3 position;
    float scale;
    vector3 rotation;
    transform() {};
};
