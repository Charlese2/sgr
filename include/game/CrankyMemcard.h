class CrankyMemcard {
    public:
    bool m_isMounted; //0x20

    bool IsMounted(void);
};

inline bool CrankyMemcard::IsMounted(void) {
    return m_isMounted;
}
