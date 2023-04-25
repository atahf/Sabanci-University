def fast_mod_exp(b, exp, m):
    res = 1
    while exp > 1:
        if exp & 1:
            res = (res * b) % m
        b = b ** 2 % m
        exp >>= 1
    return (b * res) % m

def factor(n):
    res = []
    i = 2
    while n > 1:
        if n % i == 0:
            res.append(i)
            n = n // i
        else:
            i += 1
    return res

def decrypt(c, d, n):
    return fast_mod_exp(c, d, n)

def egcd(a, b):
    if a == 0:
        return (b, 0, 1)
    g, y, x = egcd(b%a,a)
    return (g, x - (b//a) * y, y)

def modinv(a, m):
    g, x, y = egcd(a, m)
    if g == 1:
        return x%m
    return -1

if __name__ == "__main__":
    N = 1746786788707
    e = 263
    c = 1661993860336
    
    pNq = factor(N)
    print(f"p is {pNq[0]} and q is {pNq[1]}")
    
    phi = (pNq[0] - 1) * (pNq[1] - 1)
    
    d = modinv(e, phi)
    print(f"private key(d) is {d}")
    
    m = decrypt(c, d, N)
    print(f"decrypted message(m) is {m}")
    
