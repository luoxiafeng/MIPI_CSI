include $(SRCTREE)/build/config.mk
ARCHIVE := drv_csi.a

SRCS := csi.c \
	csi_core.c \
	log.c \
	csi_dma.c \

include $(BUILD_LIB)

