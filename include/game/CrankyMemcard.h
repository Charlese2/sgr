class CrankyMemcard{
    public:
    virtual void unk0();
    int* m_cardWorkArea;
    int unk8;
    int unkC;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
    bool m_isMounted;
    
    CrankyMemcard();
    ~CrankyMemcard();
    void NewMemcard(int* cardWorkArea);
    bool IsMounted(void) { return m_isMounted; };
};
