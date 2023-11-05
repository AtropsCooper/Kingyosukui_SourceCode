import glob

for f in glob.glob("*.png"):
    print("Resource(resources/Bird/{})".format(f))