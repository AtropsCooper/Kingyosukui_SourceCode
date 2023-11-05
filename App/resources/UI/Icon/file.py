import glob

for f in glob.glob("*.png"):
    print("Resource(resources/UI/Icon/{})".format(f))

for f in glob.glob("*.png"):
    print("TextureAsset::Register(U\"icon_\",Resource(U\"resources/UI/Icon/{}\"));".format(f))