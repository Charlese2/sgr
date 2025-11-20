#define MAX_POSTED_TEXT_LENGTH        128
#define MAX_POSTED_CORNER_TEXT_LENGTH 128
#define MAX_MBOX_STRING_LENGTH        256
#define MAX_MBOX_OPTIONS              8
#define MAX_MBOX_OPTION_STRING_LENGTH 32

typedef void (*DialogueCallback)(int state);

extern int Num_interfaces_showing;
extern int front_end_peg_loaded;
extern float Time_played;

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
