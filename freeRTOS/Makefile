#工程名
PROJECT=freertos_cm4f

#要编译的文件
SRCS = port.c croutine.c event_groups.c list.c queue.c tasks.c timers.c heap_3.c

#C编译器
CC=arm-none-eabi-gcc

#链接器
LD=arm-none-eabi-gcc

#打包器
AR=arm-none-eabi-ar

#二进制文件生成
OBJCOPY=arm-none-eabi-objcopy

#SIZE
SIZE=arm-none-eabi-size

#编译选项
CFLAGS  = -nostartfiles -Wl,--gc-sections -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 
CFLAGS += -mfloat-abi=hard -mabi=aapcs
CFLAGS += -I Source/include
CFLAGS += -I Source/portable/GCC/ARM_CM4F
CFLAGS += -I ../../STM32F4xx_DSP_StdPeriph_Lib_V1.5.1/Libraries/CMSIS/Device/ST/STM32F4xx/Include
CFLAGS += -I ../../STM32F4xx_DSP_StdPeriph_Lib_V1.5.1/Libraries/CMSIS/Include
CFLAGS += -I ../../STM32F4xx_DSP_StdPeriph_Lib_V1.5.1/Project/STM32F4xx_StdPeriph_Templates
CFLAGS += -I ../../STM32F4xx_DSP_StdPeriph_Lib_V1.5.1/Libraries/STM32F4xx_StdPeriph_Driver/inc
CFLAGS += -D USE_STDPERIPH_DRIVER -D STM32F40_41xxx

vpath %.c Source
vpath %.c Source/portable/GCC/ARM_CM4F
vpath %.c Source/portable/MemMang

OBJS = $(SRCS:.c=.o)
STDLIB = libfreertos_cm4f.a

all:$(OBJS) $(STDLIB)

$(OBJS):%.o:%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(STDLIB):$(OBJS)
	$(AR) -r $(STDLIB) $(OBJS)

clean:
	-rm *.o
	-rm libfreertos_cm4f.a

