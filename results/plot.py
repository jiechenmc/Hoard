import pandas as pd

larson = {
    "jemalloc": [78043455, 161027354, 256153328, 261340054, 335376096, 444991282],
    "mimalloc": [77403648, 185587938, 294374430, 544515478, 1012536743, 1444591556],
    "hoard": [72585417, 160659040, 248808928, 252012132, 379740493, 539213503],
    "506hoard": [118286, 261863, 363959, 0, 0, 0],
}


threadtest = {
    8: {
        "jemalloc": [1.18205, 0.495003, 0.306256, 0.154758, 0.0909655, 0.111515],
        "mimalloc": [0.653144, 0.23028, 0.145947, 0.19479, 0.0524435, 0.0531666],
        "hoard": [1.20897, 0.834406, 0.272041, 0.147261, 0.0854719, 0.156487],
        "506hoard": [0, 0, 0, 0, 0, 0],
    },
    64: {
        "jemalloc": [3.80598, 2.07617, 1.04402, 0.893054, 0.263236, 0.295798],
        "mimalloc": [2.6097, 1.37526, 0.999048, 0.957271, 0.150905, 0.102485],
        "hoard": [3.61751, 1.81369, 0.901592, 0.34245, 0.173642, 0.203126],
        "506hoard": [0, 0, 0, 0, 0, 0],
    },
    256: {
        "jemalloc": [13.1696, 7.24241, 6.64533, 5.33391, 5.78417, 5.22161],
        "mimalloc": [8.95733, 6.74172, 6.25874, 5.55014, 5.81977, 5.38857],
        "hoard": [9.34026, 6.66753, 6.33943, 5.44998, 5.98456, 6.2838],
        "506hoard": [0, 0, 0, 0, 0, 0],
    },
    512: {
        "jemalloc": [26.9086, 15.2833, 14.3463, 12.7494, 13.2595, 12.7971],
        "mimalloc": [16.8295, 13.7966, 13.4992, 12.2846, 12.475, 12.4688],
        "hoard": [17.0399, 14.0398, 13.6236, 12.4845, 12.9478, 11.9465],
        "506hoard": [0, 0, 0, 0, 0, 0],
    },
}

cache_scratch = {
    8: {
        "jemalloc": [0.290681, 0.096756, 0.068229, 0.03642, 0.023722, 0.021224],
        "mimalloc": [0.305374, 0.105132, 0.060217, 0.043775, 0.024273, 0.022121],
        "hoard": [0.290637, 0.099305, 0.067278, 0.039196, 0.024553, 0.021792],
        "506hoard": [0.29977, 0.099848, 0.058611, 0.039623, 0.022535, 0.022228],
    },
    64: {
        "jemalloc": [2.259629, 0.874095, 0.555699, 0.295178, 0.182441, 0.154609],
        "mimalloc": [2.258448, 0.798442, 0.501075, 0.284192, 0.171356, 0.150687],
        "hoard": [2.251934, 0.789964, 0.531023, 0.289862, 0.166306, 0.144942],
        "506hoard": [2.259227, 0.812661, 0.515171, 0.302384, 0.178933, 0.142997],
    },
    256: {
        "jemalloc": [9.570032, 3.434901, 2.181173, 1.22686, 0.707404, 0.580911],
        "mimalloc": [9.585005, 3.628192, 2.175451, 1.212195, 0.696852, 0.605999],
        "hoard": [9.526037, 3.33631, 2.170047, 1.212405, 0.707665, 0.593814],
        "506hoard": [9.590125, 3.328068, 2.183928, 1.225108, 0.707796, 0.594659],
    },
    512: {
        "jemalloc": [19.149371, 6.796528, 4.340655, 2.456093, 1.395538, 1.165851],
        "mimalloc": [19.137213, 6.75115, 4.264144, 2.409924, 1.402517, 1.177913],
        "hoard": [19.000005, 6.772873, 4.36177, 2.395357, 1.404505, 1.204795],
        "506hoard": [18.982501, 6.775581, 4.409142, 2.436484, 1.398497, 1.174315],
    },
}

cache_thrash = {
    8: {
        "jemalloc": [0.295088, 0.104814, 0.061165, 0.037467, 0.025454, 0.022949],
        "mimalloc": [0.298061, 0.09778, 0.066228, 0.04047, 0.024741, 0.022484],
        "hoard": [0.294786, 0.098057, 0.070361, 0.037785, 0.025315, 0.020895],
        "506hoard": [0.304622, 0.103921, 0.074418, 0.048654, 0.027907, 0.024201],
    },
    64: {
        "jemalloc": [2.31061, 0.79414, 0.532559, 0.283034, 0.172821, 0.139803],
        "mimalloc": [2.28791, 0.885636, 0.523899, 0.310864, 0.168544, 0.148975],
        "hoard": [2.24996, 0.777533, 0.496799, 0.306191, 0.173175, 0.141959],
        "506hoard": [2.26402, 0.766142, 0.520837, 0.288642, 0.172871, 0.148948],
    },
    256: {
        "jemalloc": [9.53701, 3.46337, 2.18098, 1.21453, 0.717163, 0.618632],
        "mimalloc": [9.56333, 3.36868, 2.23792, 1.19859, 0.701783, 0.583752],
        "hoard": [9.54425, 3.5057, 2.21189, 1.20864, 0.706175, 0.60283],
        "506hoard": [9.49434, 3.40505, 2.21195, 1.21059, 0.702237, 0.589056],
    },
    512: {
        "jemalloc": [19.1708, 6.76757, 4.34872, 2.45214, 1.41337, 1.1737],
        "mimalloc": [19.0036, 6.69802, 4.34658, 2.43608, 1.4064, 1.17256],
        "hoard": [19.1785, 7.02583, 4.33198, 2.39831, 1.40303, 1.18161],
        "506hoard": [18.9829, 7.00148, 4.39054, 2.43068, 1.38796, 1.18108],
    },
}

# Plotting Larson

larson = pd.DataFrame(larson, index=[1, 3, 5, 10, 20, 32])

ax = larson.plot.bar()
ax.set_xlabel("Thread Count")
ax.set_ylabel("Throughput")
ax.set_title("Larson Allocator Throughput Benchmark")


fig = ax.get_figure()
fig.savefig("larson.png")

# Plotting Individual

larson_506 = pd.DataFrame(larson["506hoard"], index=[1, 3, 5, 10, 20, 32])

ax = larson_506.head(3).plot.bar()
ax.set_xlabel("Thread Count")
ax.set_ylabel("Throughput")
ax.set_title("506 Hoard Larson Benchmark")

fig = ax.get_figure()
fig.savefig("larson_506hoard.png")

# Plotting Threadtest

tt8 = pd.DataFrame(threadtest[8], index=[1, 3, 5, 10, 20, 32])
tt64 = pd.DataFrame(threadtest[64], index=[1, 3, 5, 10, 20, 32])
tt256 = pd.DataFrame(threadtest[256], index=[1, 3, 5, 10, 20, 32])
tt512 = pd.DataFrame(threadtest[512], index=[1, 3, 5, 10, 20, 32])

res = pd.concat([tt8, tt64, tt256, tt512])
ax = res.plot.bar()

ax.set_xlabel("Thread Count")
ax.set_ylabel("Time Elapsed (seconds)")
ax.set_title("Threadtest Allocator Speed Benchmark")

fig = ax.get_figure()
fig.savefig("threadtest.png")

# Plotting cache-scratch

tt8 = pd.DataFrame(cache_scratch[8], index=[1, 3, 5, 10, 20, 32])
tt64 = pd.DataFrame(cache_scratch[64], index=[1, 3, 5, 10, 20, 32])
tt256 = pd.DataFrame(cache_scratch[256], index=[1, 3, 5, 10, 20, 32])
tt512 = pd.DataFrame(cache_scratch[512], index=[1, 3, 5, 10, 20, 32])

res = pd.concat([tt8, tt64, tt256, tt512])
ax = res.plot.bar()

ax.set_xlabel("Thread Count")
ax.set_ylabel("Time Elapsed (seconds)")
ax.set_title("cache_scratch Passive-false sharing Benchmark")

fig = ax.get_figure()
fig.savefig("cache_scratch.png")

# Plotting cache-thrash

tt8 = pd.DataFrame(cache_thrash[8], index=[1, 3, 5, 10, 20, 32])
tt64 = pd.DataFrame(cache_thrash[64], index=[1, 3, 5, 10, 20, 32])
tt256 = pd.DataFrame(cache_thrash[256], index=[1, 3, 5, 10, 20, 32])
tt512 = pd.DataFrame(cache_thrash[512], index=[1, 3, 5, 10, 20, 32])

res = pd.concat([tt8, tt64, tt256, tt512])
ax = res.plot.bar()

ax.set_xlabel("Thread Count")
ax.set_ylabel("Time Elapsed (seconds)")
ax.set_title("cache_thrash Active-false sharing Benchmark")

fig = ax.get_figure()
fig.savefig("cache_thrash.png")
