import numpy as np
import matplotlib.pyplot as plt

if __name__ == '__main__':
    n = 8
    weight = []
    for _ in range(n):
        weight.append([float(i) for i in input().split()])

    weight = np.array(weight)

    plt.imshow(weight)
    plt.colorbar()
    plt.savefig("visualize.png")
