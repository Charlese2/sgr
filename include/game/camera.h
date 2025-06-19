extern bool Camera_slew_mode_enabled;

class camera {
  public:
    void SetMode(int mode);

private:
    int unk0[3];
};

extern camera gCamera;
