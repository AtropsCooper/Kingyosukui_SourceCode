import glob

for f in glob.glob("*.*"):
    print("Resource(resources/SFX/{})".format(f))