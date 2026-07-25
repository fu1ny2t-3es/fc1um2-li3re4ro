LOCAL_PATH := $(call my-dir)

CORE_DIR := $(LOCAL_PATH)/../src
LINK_T := $(LOCAL_PATH)/../libretro/link.T

HAVE_NTSC := 1

include $(LOCAL_PATH)/../Makefile.common

COREFLAGS := $(COREDEFINES) $(INCFLAGS)

GIT_VERSION := " $(shell git rev-parse --short HEAD || echo unknown)"
ifneq ($(GIT_VERSION)," unknown")
	COREFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"
endif

include $(CLEAR_VARS)
LOCAL_MODULE    := retro
LOCAL_SRC_FILES := $(SOURCES_C)
LOCAL_CFLAGS    := $(COREFLAGS)
<<<<<<< HEAD
<<<<<<< HEAD
LOCAL_LDFLAGS   := -Wl,-version-script=$(CORE_DIR)/drivers/libretro/link.T,-z,max-page-size=16384
=======
LOCAL_LDFLAGS   := -Wl,-version-script=$(CORE_DIR)/libretro/link.T
>>>>>>> 3752be2b ((Android) Fix build)
=======
LOCAL_LDFLAGS   := -Wl,-version-script=$(LINK_T)
>>>>>>> 0cd1c82b (Update ppu.c)
include $(BUILD_SHARED_LIBRARY)
