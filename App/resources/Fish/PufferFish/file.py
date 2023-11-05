import glob

for f in glob.glob("*.png"):
    print("Resource(resources/Fish/PufferFish/{})".format(f))