#ifndef _VECTOR_H_
#define _VECTOR_H_

class vector2 {
  public:
    float x;
    float y;
    vector2(){};

    vector2(const float x, const float y) {
        this->x = x;
        this->y = y;
    };
};

class vector3 {
  public:
    float x;
    float y;
    float z;

    vector3(){};
    vector3(vector3 &rhs) {
        this->x = rhs.x;
        this->y = rhs.y;
        this->z = rhs.z;
    };
    vector3(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    };
    vector3(const vector3 &rhs) {}
    vector3 operator+=(const vector3 &rhs) { return vector3(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z); };
    vector3 operator-=(const vector3 &rhs) { return vector3(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z); };
    vector3 operator*=(const float mag) { return vector3(this->x * mag, this->y * mag, this->z * mag); };
    void operator+(const vector3 rhs) {
        this->x += rhs.x;
        this->y += rhs.y;
        this->z += rhs.z;
    }
    void operator*(const float rhs) {
        this->x *= rhs;
        this->y *= rhs;
        this->z *= rhs;
    }
    static float SumOfSquares(vector3 lhs, const vector3 rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; };
};

class vector3_matrix {
  public:
    float x;
    float y;
    float z;
    vector3_matrix(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

class vector3x3 {
  public:
    vector3 vec1;
    vector3 vec2;
    vector3 vec3;
    vector3x3();
    vector3x3(vector3x3 &rhs) {
        this->vec1 = rhs.vec1;
        this->vec2 = rhs.vec2;
        this->vec3 = rhs.vec3;
    }
};

class pathfinding_transform {
  public:
    vector3 unk0;
    int unkC;
    vector3 unk10;
    int unk1C;
    pathfinding_transform *next;
    pathfinding_transform *prev;
    pathfinding_transform(){};
};

class effect_transform {
  public:
    int unk0;
    int unk4;
    vector3 unk8;
    vector3 unk14;
    int unk20;
    int unk24;
};

#endif // _VECTOR_H_
