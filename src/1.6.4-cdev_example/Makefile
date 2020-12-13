KMOD= cdev
SRCS= cdev.c

.include <bsd.kmod.mk>

cdevload:
	kldload ./$(KMOD).ko

cdevunload:
	kldunload $(KMOD)

