#----------------------------- User configuration -----------------------------

# Common settings
#------------------------

# How you're loading your module. Used to determine how to find the target module. (AsRtld/Module/Kip)
LOAD_KIND := Module

# Program you're targetting. Used to determine where to deploy your files.
PROGRAM_ID := 0100F2C0115B6000

# Optional path to copy the final ELF to, for convenience.
ELF_EXTRACT :=

# Python command to use. Must be Python 3.4+.
PYTHON := python3

# JSON to use to make .npdm
NPDM_JSON := qlaunch.json

# Additional C/C++ flags to use.
C_FLAGS := 
CXX_FLAGS := -Oz -fno-strict-aliasing -fwrapv -fno-rtti -std=gnu++20 -Wno-sign-compare -Wno-error

# AsRtld settings
#------------------------

# Path to the SD card. Used to mount and deploy files on SD, likely with hekate UMS.
MOUNT_PATH := /mnt/k

# Module settings
#------------------------

# Settings for deploying over FTP. Used by the deploy-ftp.py script.
FTP_IP := 192.168.86.23
FTP_PORT := 5000
FTP_USERNAME := Anon
FTP_PASSWORD := Anon

# Settings for deploying to Ryu. Used by the deploy-ryu.sh script.
RYU_PATH := /mnt/c/Users/shado/AppData/Roaming/Ryujinx
