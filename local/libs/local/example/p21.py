
import itertools

def AT(n, t): return "A" + str(n) + "T" + str(t)

arity = 3
types = 2

l = []
for n in range(0, arity):
    for t in range(0, types):
        l.append(AT(n, t))

print l
for s in itertools.combinations(l, 3):
    print s

for t in range(types):
    for u in range(types):
        for v in range(types):
            for w in range(types):
                print t, u, v, w

print

for i in range(arity):
    print i

