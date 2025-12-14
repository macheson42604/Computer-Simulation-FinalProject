import sys
import numpy as np
from matplotlib import pyplot as plt

# read the first file to get the x and y values
filename = "piecewise-function.dat"
data_filename = "output.txt"
if len(sys.argv) > 2:
    filename = sys.argv[1]
    data_filename = sys.argv[2]
elif len(sys.argv) > 1:
    filename = sys.argv[1]
else:
    print("Filename: " + filename)
    print("Data Filename: " + data_filename)



# read points from disk file
x = []
y = []

try:
    with open(filename, 'r') as f:
        for line in f:
            try:
                values = line.strip().split()
                assert len(values) == 2, f"expected a line to have two values, but it has {len(values)} values: {values}"
                x.append(float(values[0]))
                y.append(float(values[1]))
            except (ValueError, AssertionE rror) as e:
                print(f"Invalid data in file: {line.strip()} - {e}")
except FileNotFoundError:
    print(f"File not found: {filename}")
    sys.exit(1)


# normalizes piecewise curve to 1 by scaling y_i (creating PDF)
# find total area
total_area = np.trapezoid(y, x)
# divide each y by current area
y = [(float)(y_i / total_area) for y_i in y]


# use output data to plot histogram
data = []
try:
    with open(data_filename, 'r') as f:
        for line in f:
            try:
                value = float(line.strip())
                data.append(value)
            except ValueError as e:
                print(f"Invalid data in file: {line.strip()} - {e}")
except FileNotFoundError:
    print(f"File not found: {data_filename}")
    sys.exit(1)


plt.hist(data, density=True, bins=20, color="blue")
plt.plot(x,y, color="black", alpha=0.5)
plt.title("Binned and Expected PDF")
plt.xlabel("x")
plt.ylabel("f(x0)")
plt.show()