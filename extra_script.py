Import("env")

import subprocess
from os.path import expanduser, join

home = expanduser("~")
esptool = join(home, ".platformio", "packages", "tool-esptoolpy", "esptool.py")

def on_upload(source, target, env):
    file = str(source[0])
    subprocess.run(["python", esptool, "-b 921600", "write_flash", "-z", "0x10000", file])

env.Replace(UPLOADCMD=on_upload)
