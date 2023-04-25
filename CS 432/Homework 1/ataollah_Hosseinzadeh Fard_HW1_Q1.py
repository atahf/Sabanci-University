def isPrime(n):
    for i in range(2,int(n**0.5)+1):
        if n%i==0:
            return False
    return True

def findNextPrime(n):
    res = n + 1
    while not isPrime(res):
        res += 1
    return res

def isPrimitive(a, count):
    res = []
    q = findNextPrime(a)
    while len(res) < count:
        #a^n mod q = m -> distinct m
        tmp = []
        for i in range(q):
            step = (a ** i) % q
            if step not in tmp:
                tmp.append(step)
            else:
                break
        if len(tmp) == q - 1 and isPrime(q):
            res.append(q)
        q = findNextPrime(q)
    return res

if __name__ == "__main__":
    num = 13
    c = 3 
    ps = isPrimitive(num, c)
    print(f"{num} is primitive root for following {c} smallest possible primes: {ps}")
