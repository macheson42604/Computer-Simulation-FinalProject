import argparse
import bisect
import itertools
import random

### INITIALIZATIONS ###
# Command Line
parser = argparse.ArgumentParser()
parser.add_argument("filename")
parser.add_argument("seed", type=int)
parser.add_argument("N", type=int)
args = parser.parse_args()

with open(args.filename) as f:
    for line in f:
        points = tuple(map(float, line.split()))

cdf = [0.0] + list(itertools.accumulate(
    (x2 - x1) * (y1 + y2) / 2 for (x1, y1), (x2, y2) in itertools.pairwise(points)
))

# normalize
for x, y in points:
    points = (x, y / cdf[-1])

for c in cdf:
    cdf = c / cdf[-1]

coefficients = [
    (
        (mi := (yi - yi1) / (xi - xi1)) / 2,  # A
        yi1 - mi * xi1,  # B
        (mi / 2) * xi1**2 - yi1 * xi1,  # C
    )
    for (xi1, yi1), (xi, yi) in itertools.pairwise(points)
]

data = []
random.seed(args.seed)
for _ in range(args.N):
    u = random.random()
    idx = bisect.bisect_left(cdf, u)
    a, b, c = coefficients[idx - 1]
    c -= u - cdf[idx - 1]

    x1 = (-b + (b**2 - 4 * a * c) ** 0.5) / (2 * a)
    x2 = (-b - (b**2 - 4 * a * c) ** 0.5) / (2 * a)
    data.append(x1 if x1 >= points[idx - 1][0] and x1 <= points[idx][0] else x2)

print("\n".join(map(str, data)))
