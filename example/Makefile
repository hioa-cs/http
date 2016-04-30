#################################################
#          IncludeOS SERVICE makefile           #
#################################################

# The name of your service
SERVICE = http_IncludeOS_Demo_Service

# Your service parts
FILES = service.cpp

# Your disk image
DISK=

# Your own include-path
LOCAL_INCLUDES=-I../inc

# IncludeOS location
ifndef INCLUDEOS_INSTALL
	INCLUDEOS_INSTALL=$(HOME)/IncludeOS_install
endif

# Include the installed seed makefile
include $(INCLUDEOS_INSTALL)/Makeseed
