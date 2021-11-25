Import("env")

#get environment specific parameters
environment = env['PIOENV']
exec(open(("./ota/%s_config.py" % (environment))).read())
print(environment)

my_flags = env.ParseFlags(env['BUILD_FLAGS'])
defines = {k: v for (k, v) in my_flags.get("CPPDEFINES")}

deviceName = defines["DEVICE_NAME"].replace('"', '')

env.AddCustomTarget(
name="otaprep",
dependencies= ["$BUILD_DIR/${PROGNAME}.bin"],
actions=[
    "ssh %s@%s 'mkdir -p %s/%s'" % (OTA_USER, OTA_SERVER, OTA_PATH, deviceName),
    "ssh %s@%s 'if [ -f %s/%s/* ]; then rm %s/%s/*; fi'" % (OTA_USER, OTA_SERVER, OTA_PATH, deviceName, OTA_PATH, deviceName),
    "scp '$BUILD_DIR/${PROGNAME}.bin' %s@%s:%s/%s" % (OTA_USER, OTA_SERVER, OTA_PATH, deviceName)
],
title="Prepare OTA Uploads",
    description="Uploads the firmware to the specific folder on the OTA server")