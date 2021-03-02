import collections

models = {
    "pixel-shuffling": [38.4],
    "TWSC": [37.93, 37.94],
    "CBDNet": [37.57, 38.06],
    "MCWNNM": [37.38],
    "FFDNet": [34.40, 27.96, 31.21, 29.58],
    "TNRD": [33.65],
    "CDnCNN-B": [32.43],
    "NLRN": [30.8],
    "rd+": [28.34],
    "RIDNet": [28.14, 39.23, 31.37, 34.01],
    "DnCNN": [28.01],
    "CSCNet": [28.00, 31.18],
    "IRCNN": [27.86],
    "Spatial-CNN": [27.63, 30.99],
    "CBUIFD75": [26.60, 30.76],
    "MemNet": [26.33],
    "google": [40.35],
    "PRIDNet": [39.4],
}


est_models = collections.OrderedDict()
for key, value in models.items():
    s = 0
    for i in value:
        s += i
    est_models[key] = s / len(value)

sorted_dict = dict(sorted(est_models.items(), key=lambda item: item[1]))
with open("models", "w") as f:
    for key, value in sorted_dict.items():
        print(key, value)
        f.write(str(key) + " ")
        f.write(str(value))
        f.write('\n')
