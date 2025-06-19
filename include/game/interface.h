typedef void (*DialogueCallback)(int state);

extern int Num_interfaces_showing;

class interface_sound {
  public:
    int m_index;
};

class new_interface {
  public:
    static int toggle_debug_animator_mode(void);
    static int toggle_debug_frame_dump_protection(void);
    static void Unknown(void);
    static void give_party_levels_and_empowered(int levels);
    static void DebugButtonsCheck(void);
};

void ShowDialogue(char *buffer, int unk_2, int num_options, int unk_4, DialogueCallback dialogueCallback, int unk_6);
