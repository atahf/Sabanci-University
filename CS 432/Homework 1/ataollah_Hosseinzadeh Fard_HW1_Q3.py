if __name__ == "__main__":
    c1 = 0x586EFEA0879BBE0B
    c2 = 0x2EBEFD6874DCEE2F
    c3 = 0xC284E0DC4F364811

    p1 = 0xFF7AB6C7303DFF4F
    
    GeneralKey = c1 ^ p1
    print(f"General Key is ({hex(GeneralKey)}) that is obtained by 'c1 XOR p1'")
    
    p2 = c2 ^ GeneralKey
    print(f"p2 is ({hex(p2)}) that is obtained by 'c2 XOR General Key'")
    
    p3 = c3 ^ GeneralKey
    print(f"p3 is ({hex(p3)}) that is obtained by 'c3 XOR General Key'")
