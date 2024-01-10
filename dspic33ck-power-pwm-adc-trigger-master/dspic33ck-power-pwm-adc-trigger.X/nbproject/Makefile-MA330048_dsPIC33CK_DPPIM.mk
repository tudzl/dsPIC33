#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-MA330048_dsPIC33CK_DPPIM.mk)" "nbproject/Makefile-local-MA330048_dsPIC33CK_DPPIM.mk"
include nbproject/Makefile-local-MA330048_dsPIC33CK_DPPIM.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=MA330048_dsPIC33CK_DPPIM
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/dspic33ck-power-pwm-adc-trigger.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/dspic33ck-power-pwm-adc-trigger.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=sources/common/p33c_pwm.c sources/common/p33c_adc.c mcc_generated_files/reset.c mcc_generated_files/tmr1.c mcc_generated_files/system.c mcc_generated_files/clock.c mcc_generated_files/traps.c mcc_generated_files/interrupt_manager.c mcc_generated_files/mcc.c mcc_generated_files/pin_manager.c main.c sources/pwm.c sources/adc.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/sources/common/p33c_pwm.o ${OBJECTDIR}/sources/common/p33c_adc.o ${OBJECTDIR}/mcc_generated_files/reset.o ${OBJECTDIR}/mcc_generated_files/tmr1.o ${OBJECTDIR}/mcc_generated_files/system.o ${OBJECTDIR}/mcc_generated_files/clock.o ${OBJECTDIR}/mcc_generated_files/traps.o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o ${OBJECTDIR}/mcc_generated_files/mcc.o ${OBJECTDIR}/mcc_generated_files/pin_manager.o ${OBJECTDIR}/main.o ${OBJECTDIR}/sources/pwm.o ${OBJECTDIR}/sources/adc.o
POSSIBLE_DEPFILES=${OBJECTDIR}/sources/common/p33c_pwm.o.d ${OBJECTDIR}/sources/common/p33c_adc.o.d ${OBJECTDIR}/mcc_generated_files/reset.o.d ${OBJECTDIR}/mcc_generated_files/tmr1.o.d ${OBJECTDIR}/mcc_generated_files/system.o.d ${OBJECTDIR}/mcc_generated_files/clock.o.d ${OBJECTDIR}/mcc_generated_files/traps.o.d ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d ${OBJECTDIR}/mcc_generated_files/mcc.o.d ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/sources/pwm.o.d ${OBJECTDIR}/sources/adc.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/sources/common/p33c_pwm.o ${OBJECTDIR}/sources/common/p33c_adc.o ${OBJECTDIR}/mcc_generated_files/reset.o ${OBJECTDIR}/mcc_generated_files/tmr1.o ${OBJECTDIR}/mcc_generated_files/system.o ${OBJECTDIR}/mcc_generated_files/clock.o ${OBJECTDIR}/mcc_generated_files/traps.o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o ${OBJECTDIR}/mcc_generated_files/mcc.o ${OBJECTDIR}/mcc_generated_files/pin_manager.o ${OBJECTDIR}/main.o ${OBJECTDIR}/sources/pwm.o ${OBJECTDIR}/sources/adc.o

# Source Files
SOURCEFILES=sources/common/p33c_pwm.c sources/common/p33c_adc.c mcc_generated_files/reset.c mcc_generated_files/tmr1.c mcc_generated_files/system.c mcc_generated_files/clock.c mcc_generated_files/traps.c mcc_generated_files/interrupt_manager.c mcc_generated_files/mcc.c mcc_generated_files/pin_manager.c main.c sources/pwm.c sources/adc.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-MA330048_dsPIC33CK_DPPIM.mk ${DISTDIR}/dspic33ck-power-pwm-adc-trigger.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33CK256MP506
MP_LINKER_FILE_OPTION=,--script=p33CK256MP506.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/sources/common/p33c_pwm.o: sources/common/p33c_pwm.c  .generated_files/flags/MA330048_dsPIC33CK_DPPIM/8187c0d3800150b48eda6faaa0d271bc90af36cc .generated_files/flags/MA330048_dsPIC33CK_DPPIM/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/sources/common" 
	@${RM} ${OBJECTDIR}/sources/common/p33c_pwm.o.d 
	@${RM} ${OBJECTDIR}/sources/common/p33c_pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  sources/common/p33c_pwm.c  -o ${OBJECTDIR}/sources/common/p33c_pwm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/sources/common/p33c_pwm.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -D__DM330029_R20__ -D__MA330048_dsPIC33CK_DPPIM__ -D__P33SMPS_CK506__ -DXPRJ_MA330048_dsPIC33CK_DPPIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"sources" -I"mcc_generated_files" -O0 -I"C:/Program Files/Microchip/xc16/v1.50/support/dsPIC33C/h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/sources/common/p33c_adc.o: sources/common/p33c_adc.c  .generated_files/flags/MA330048_dsPIC33CK_DPPIM/64b96063fa226258491e75a7594923f8c9ca3a3d .generated_files/flags/MA330048_dsPIC33CK_DPPIM/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/sources/common" 
	@${RM} ${OBJECTDIR}/sources/common/p33c_adc.o.d 
	@${RM} ${OBJECTDIR}/sources/common/p33c_adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  sources/common/p33c_adc.c  -o ${OBJECTDIR}/sources/common/p33c_adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/sources/common/p33c_adc.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -D__DM330029_R20__ -D__MA330048_dsPIC33CK_DPPIM__ -D__P33SMPS_CK506__ -DXPRJ_MA330048_dsPIC33CK_DPPIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"sources" -I"mcc_generated_files" -O0 -I"C:/Program Files/Microchip/xc16/v1.50/support/dsPIC33C/h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/mcc_generated_files/reset.o: mcc_generated_files/reset.c  .generated_files/flags/MA330048_dsPIC33CK_DPPIM/96e94ecaf3d32e597db6b407d196135cecd71a2 .generated_files/flags/MA330048_dsPIC33CK_DPPIM/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/reset.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/reset.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/reset.c  -o ${OBJECTDIR}/mcc_generated_files/reset.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/reset.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -D__DM330029_R20__ -D__MA330048_dsPIC33CK_DPPIM__ -D__P33SMPS_CK506__ -DXPRJ_MA330048_dsPIC33CK_DPPIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"sources" -I"mcc_generated_files" -O0 -I"C:/Program Files/Microchip/xc16/v1.50/support/dsPIC33C/h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/mcc_generated_files/tmr1.o: mcc_generated_files/tmr1.c  .generated_files/flags/MA330048_dsPIC33CK_DPPIM/e0ca8bf9e6dd0c27df2ef8587c4063d38d6da4f8 .generated_files/flags/MA330048_dsPIC33CK_DPPIM/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr1.c  -o ${OBJECTDIR}/mcc_generated_files/tmr1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -D__DM330029_R20__ -D__MA330048_dsPIC33CK_DPPIM__ -D__P33SMPS_CK506__ -DXPRJ_MA330048_dsPIC33CK_DPPIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"sources" -I"mcc_generated_files" -O0 -I"C:/Program Files/Microchip/xc16/v1.50/support/dsPIC33C/h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/mcc_generated_files/system.o: mcc_generated_files/system.c  .generated_files/flags/MA330048_dsPIC33CK_DPPIM/f449dd13830105fc0a06a65669c988d0aff9235 .generated_files/flags/MA330048_dsPIC33CK_DPPIM/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/system.c  -o ${OBJECTDIR}/mcc_generated_files/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/system.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -D__DM330029_R20__ -D__MA330048_dsPIC33CK_DPPIM__ -D__P33SMPS_CK506__ -DXPRJ_MA330048_dsPIC33CK_DPPIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"sources" -I"mcc_generated_files" -O0 -I"C:/Program Files/Microchip/xc16/v1.50/support/dsPIC33C/h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/mcc_generated_files/clock.o: mcc_generated_files/clock.c  .generated_files/flags/MA330048_dsPIC33CK_DPPIM/d2460f334755180fa421f0fedc279cf04d0ce635 .generated_files/flags/MA330048_dsPIC33CK_DPPIM/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/clock.c  -o ${OBJECTDIR}/mcc_generated_files/clock.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/clock.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -D__DM330029_R20__ -D__MA330048_dsPIC33CK_DPPIM__ -D__P33SMPS_CK506__ -DXPRJ_MA330048_dsPIC33CK_DPPIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"sources" -I"mcc_generated_files" -O0 -I"C:/Program Files/Microchip/xc16/v1.50/support/dsPIC33C/h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/mcc_generated_files/traps.o: mcc_generated_files/traps.c  .generated_files/flags/MA330048_dsPIC33CK_DPPIM/890cf1fae17afb1103a24c0209895bd944bbf4d9 .generated_files/flags/MA330048_dsPIC33CK_DPPIM/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/traps.c  -o ${OBJECTDIR}/mcc_generated_files/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/traps.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -D__DM330029_R20__ -D__MA330048_dsPIC33CK_DPPIM__ -D__P33SMPS_CK506__ -DXPRJ_MA330048_dsPIC33CK_DPPIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"sources" -I"mcc_generated_files" -O0 -I"C:/Program Files/Microchip/xc16/v1.50/support/dsPIC33C/h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/mcc_generated_files/interrupt_manager.o: mcc_generated_files/interrupt_manager.c  .generated_files/flags/MA330048_dsPIC33CK_DPPIM/ca7c95db3350f992e327909ddb204a813d621967 .generated_files/flags/MA330048_dsPIC33CK_DPPIM/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/interrupt_manager.c  -o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -D__DM330029_R20__ -D__MA330048_dsPIC33CK_DPPIM__ -D__P33SMPS_CK506__ -DXPRJ_MA330048_dsPIC33CK_DPPIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"sources" -I"mcc_generated_files" -O0 -I"C:/Program Files/Microchip/xc16/v1.50/support/dsPIC33C/h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/mcc_generated_files/mcc.o: mcc_generated_files/mcc.c  .generated_files/flags/MA330048_dsPIC33CK_DPPIM/8bcf2c7878eafd334d89d568f4030fa898baf6ad .generated_files/flags/MA330048_dsPIC33CK_DPPIM/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/mcc.c  -o ${OBJECTDIR}/mcc_generated_files/mcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/mcc.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -D__DM330029_R20__ -D__MA330048_dsPIC33CK_DPPIM__ -D__P33SMPS_CK506__ -DXPRJ_MA330048_dsPIC33CK_DPPIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"sources" -I"mcc_generated_files" -O0 -I"C:/Program Files/Microchip/xc16/v1.50/support/dsPIC33C/h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/mcc_generated_files/pin_manager.o: mcc_generated_files/pin_manager.c  .generated_files/flags/MA330048_dsPIC33CK_DPPIM/f6deb7fc1058beaaa1091d35f41483309137257f .generated_files/flags/MA330048_dsPIC33CK_DPPIM/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/pin_manager.c  -o ${OBJECTDIR}/mcc_generated_files/pin_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/pin_manager.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -D__DM330029_R20__ -D__MA330048_dsPIC33CK_DPPIM__ -D__P33SMPS_CK506__ -DXPRJ_MA330048_dsPIC33CK_DPPIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"sources" -I"mcc_generated_files" -O0 -I"C:/Program Files/Microchip/xc16/v1.50/support/dsPIC33C/h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/MA330048_dsPIC33CK_DPPIM/f0d787be9bcc974fef4a5ad6c368f5f5c1a93506 .generated_files/flags/MA330048_dsPIC33CK_DPPIM/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -D__DM330029_R20__ -D__MA330048_dsPIC33CK_DPPIM__ -D__P33SMPS_CK506__ -DXPRJ_MA330048_dsPIC33CK_DPPIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"sources" -I"mcc_generated_files" -O0 -I"C:/Program Files/Microchip/xc16/v1.50/support/dsPIC33C/h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/sources/pwm.o: sources/pwm.c  .generated_files/flags/MA330048_dsPIC33CK_DPPIM/6700764fb3e0c95190f8cbc2a754762c22fd588c .generated_files/flags/MA330048_dsPIC33CK_DPPIM/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/pwm.o.d 
	@${RM} ${OBJECTDIR}/sources/pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  sources/pwm.c  -o ${OBJECTDIR}/sources/pwm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/sources/pwm.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -D__DM330029_R20__ -D__MA330048_dsPIC33CK_DPPIM__ -D__P33SMPS_CK506__ -DXPRJ_MA330048_dsPIC33CK_DPPIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"sources" -I"mcc_generated_files" -O0 -I"C:/Program Files/Microchip/xc16/v1.50/support/dsPIC33C/h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/sources/adc.o: sources/adc.c  .generated_files/flags/MA330048_dsPIC33CK_DPPIM/6c2bc509c600536fab986a4450b80666372d9535 .generated_files/flags/MA330048_dsPIC33CK_DPPIM/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/adc.o.d 
	@${RM} ${OBJECTDIR}/sources/adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  sources/adc.c  -o ${OBJECTDIR}/sources/adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/sources/adc.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -D__DM330029_R20__ -D__MA330048_dsPIC33CK_DPPIM__ -D__P33SMPS_CK506__ -DXPRJ_MA330048_dsPIC33CK_DPPIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"sources" -I"mcc_generated_files" -O0 -I"C:/Program Files/Microchip/xc16/v1.50/support/dsPIC33C/h" -msmart-io=1 -Wall -msfr-warn=off   
	
else
${OBJECTDIR}/sources/common/p33c_pwm.o: sources/common/p33c_pwm.c  .generated_files/flags/MA330048_dsPIC33CK_DPPIM/6bdf6c154513f1e5cad83c62e616e744b7cede9d .generated_files/flags/MA330048_dsPIC33CK_DPPIM/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/sources/common" 
	@${RM} ${OBJECTDIR}/sources/common/p33c_pwm.o.d 
	@${RM} ${OBJECTDIR}/sources/common/p33c_pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  sources/common/p33c_pwm.c  -o ${OBJECTDIR}/sources/common/p33c_pwm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/sources/common/p33c_pwm.o.d"      -mno-eds-warn  -g -omf=elf -D__DM330029_R20__ -D__MA330048_dsPIC33CK_DPPIM__ -D__P33SMPS_CK506__ -DXPRJ_MA330048_dsPIC33CK_DPPIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"sources" -I"mcc_generated_files" -O0 -I"C:/Program Files/Microchip/xc16/v1.50/support/dsPIC33C/h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/sources/common/p33c_adc.o: sources/common/p33c_adc.c  .generated_files/flags/MA330048_dsPIC33CK_DPPIM/117c6e6b2e7ae86c234c27a9ef47ce3d681e0a9e .generated_files/flags/MA330048_dsPIC33CK_DPPIM/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/sources/common" 
	@${RM} ${OBJECTDIR}/sources/common/p33c_adc.o.d 
	@${RM} ${OBJECTDIR}/sources/common/p33c_adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  sources/common/p33c_adc.c  -o ${OBJECTDIR}/sources/common/p33c_adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/sources/common/p33c_adc.o.d"      -mno-eds-warn  -g -omf=elf -D__DM330029_R20__ -D__MA330048_dsPIC33CK_DPPIM__ -D__P33SMPS_CK506__ -DXPRJ_MA330048_dsPIC33CK_DPPIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"sources" -I"mcc_generated_files" -O0 -I"C:/Program Files/Microchip/xc16/v1.50/support/dsPIC33C/h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/mcc_generated_files/reset.o: mcc_generated_files/reset.c  .generated_files/flags/MA330048_dsPIC33CK_DPPIM/d910d895b59d444cf1c2ce42d6cf1fe341f98687 .generated_files/flags/MA330048_dsPIC33CK_DPPIM/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/reset.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/reset.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/reset.c  -o ${OBJECTDIR}/mcc_generated_files/reset.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/reset.o.d"      -mno-eds-warn  -g -omf=elf -D__DM330029_R20__ -D__MA330048_dsPIC33CK_DPPIM__ -D__P33SMPS_CK506__ -DXPRJ_MA330048_dsPIC33CK_DPPIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"sources" -I"mcc_generated_files" -O0 -I"C:/Program Files/Microchip/xc16/v1.50/support/dsPIC33C/h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/mcc_generated_files/tmr1.o: mcc_generated_files/tmr1.c  .generated_files/flags/MA330048_dsPIC33CK_DPPIM/b8301a530f9d8e574926679725730e77fc8437b9 .generated_files/flags/MA330048_dsPIC33CK_DPPIM/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr1.c  -o ${OBJECTDIR}/mcc_generated_files/tmr1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr1.o.d"      -mno-eds-warn  -g -omf=elf -D__DM330029_R20__ -D__MA330048_dsPIC33CK_DPPIM__ -D__P33SMPS_CK506__ -DXPRJ_MA330048_dsPIC33CK_DPPIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"sources" -I"mcc_generated_files" -O0 -I"C:/Program Files/Microchip/xc16/v1.50/support/dsPIC33C/h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/mcc_generated_files/system.o: mcc_generated_files/system.c  .generated_files/flags/MA330048_dsPIC33CK_DPPIM/571ca8f4103ef8ba39bbd8f7e1670603bc2051d4 .generated_files/flags/MA330048_dsPIC33CK_DPPIM/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/system.c  -o ${OBJECTDIR}/mcc_generated_files/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/system.o.d"      -mno-eds-warn  -g -omf=elf -D__DM330029_R20__ -D__MA330048_dsPIC33CK_DPPIM__ -D__P33SMPS_CK506__ -DXPRJ_MA330048_dsPIC33CK_DPPIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"sources" -I"mcc_generated_files" -O0 -I"C:/Program Files/Microchip/xc16/v1.50/support/dsPIC33C/h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/mcc_generated_files/clock.o: mcc_generated_files/clock.c  .generated_files/flags/MA330048_dsPIC33CK_DPPIM/c3aa5938ec7d11691b556ec0a369603d437ee8dc .generated_files/flags/MA330048_dsPIC33CK_DPPIM/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/clock.c  -o ${OBJECTDIR}/mcc_generated_files/clock.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/clock.o.d"      -mno-eds-warn  -g -omf=elf -D__DM330029_R20__ -D__MA330048_dsPIC33CK_DPPIM__ -D__P33SMPS_CK506__ -DXPRJ_MA330048_dsPIC33CK_DPPIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"sources" -I"mcc_generated_files" -O0 -I"C:/Program Files/Microchip/xc16/v1.50/support/dsPIC33C/h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/mcc_generated_files/traps.o: mcc_generated_files/traps.c  .generated_files/flags/MA330048_dsPIC33CK_DPPIM/8c758b838df1c4276c2d5af37ad9b154949e83ac .generated_files/flags/MA330048_dsPIC33CK_DPPIM/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/traps.c  -o ${OBJECTDIR}/mcc_generated_files/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/traps.o.d"      -mno-eds-warn  -g -omf=elf -D__DM330029_R20__ -D__MA330048_dsPIC33CK_DPPIM__ -D__P33SMPS_CK506__ -DXPRJ_MA330048_dsPIC33CK_DPPIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"sources" -I"mcc_generated_files" -O0 -I"C:/Program Files/Microchip/xc16/v1.50/support/dsPIC33C/h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/mcc_generated_files/interrupt_manager.o: mcc_generated_files/interrupt_manager.c  .generated_files/flags/MA330048_dsPIC33CK_DPPIM/46e5bcf0f8cc85528367e56a0626d8655b3bd49c .generated_files/flags/MA330048_dsPIC33CK_DPPIM/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/interrupt_manager.c  -o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d"      -mno-eds-warn  -g -omf=elf -D__DM330029_R20__ -D__MA330048_dsPIC33CK_DPPIM__ -D__P33SMPS_CK506__ -DXPRJ_MA330048_dsPIC33CK_DPPIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"sources" -I"mcc_generated_files" -O0 -I"C:/Program Files/Microchip/xc16/v1.50/support/dsPIC33C/h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/mcc_generated_files/mcc.o: mcc_generated_files/mcc.c  .generated_files/flags/MA330048_dsPIC33CK_DPPIM/c532897d4d91fad177432b782d328b52138c79f0 .generated_files/flags/MA330048_dsPIC33CK_DPPIM/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/mcc.c  -o ${OBJECTDIR}/mcc_generated_files/mcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/mcc.o.d"      -mno-eds-warn  -g -omf=elf -D__DM330029_R20__ -D__MA330048_dsPIC33CK_DPPIM__ -D__P33SMPS_CK506__ -DXPRJ_MA330048_dsPIC33CK_DPPIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"sources" -I"mcc_generated_files" -O0 -I"C:/Program Files/Microchip/xc16/v1.50/support/dsPIC33C/h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/mcc_generated_files/pin_manager.o: mcc_generated_files/pin_manager.c  .generated_files/flags/MA330048_dsPIC33CK_DPPIM/4554e2a2f9f32397966aa740fd7b1342f55ccaa2 .generated_files/flags/MA330048_dsPIC33CK_DPPIM/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/pin_manager.c  -o ${OBJECTDIR}/mcc_generated_files/pin_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/pin_manager.o.d"      -mno-eds-warn  -g -omf=elf -D__DM330029_R20__ -D__MA330048_dsPIC33CK_DPPIM__ -D__P33SMPS_CK506__ -DXPRJ_MA330048_dsPIC33CK_DPPIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"sources" -I"mcc_generated_files" -O0 -I"C:/Program Files/Microchip/xc16/v1.50/support/dsPIC33C/h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/MA330048_dsPIC33CK_DPPIM/4eb61839c7cfe83ce7ab8748425ccbfa9220f6ec .generated_files/flags/MA330048_dsPIC33CK_DPPIM/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -mno-eds-warn  -g -omf=elf -D__DM330029_R20__ -D__MA330048_dsPIC33CK_DPPIM__ -D__P33SMPS_CK506__ -DXPRJ_MA330048_dsPIC33CK_DPPIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"sources" -I"mcc_generated_files" -O0 -I"C:/Program Files/Microchip/xc16/v1.50/support/dsPIC33C/h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/sources/pwm.o: sources/pwm.c  .generated_files/flags/MA330048_dsPIC33CK_DPPIM/5c4c2a6c1adc076ff0180097a6118275c468d0ae .generated_files/flags/MA330048_dsPIC33CK_DPPIM/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/pwm.o.d 
	@${RM} ${OBJECTDIR}/sources/pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  sources/pwm.c  -o ${OBJECTDIR}/sources/pwm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/sources/pwm.o.d"      -mno-eds-warn  -g -omf=elf -D__DM330029_R20__ -D__MA330048_dsPIC33CK_DPPIM__ -D__P33SMPS_CK506__ -DXPRJ_MA330048_dsPIC33CK_DPPIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"sources" -I"mcc_generated_files" -O0 -I"C:/Program Files/Microchip/xc16/v1.50/support/dsPIC33C/h" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/sources/adc.o: sources/adc.c  .generated_files/flags/MA330048_dsPIC33CK_DPPIM/1facb715991233c0143121c42cf4fe41bff62e9c .generated_files/flags/MA330048_dsPIC33CK_DPPIM/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/adc.o.d 
	@${RM} ${OBJECTDIR}/sources/adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  sources/adc.c  -o ${OBJECTDIR}/sources/adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/sources/adc.o.d"      -mno-eds-warn  -g -omf=elf -D__DM330029_R20__ -D__MA330048_dsPIC33CK_DPPIM__ -D__P33SMPS_CK506__ -DXPRJ_MA330048_dsPIC33CK_DPPIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"sources" -I"mcc_generated_files" -O0 -I"C:/Program Files/Microchip/xc16/v1.50/support/dsPIC33C/h" -msmart-io=1 -Wall -msfr-warn=off   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/dspic33ck-power-pwm-adc-trigger.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/dspic33ck-power-pwm-adc-trigger.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -omf=elf -D__DM330029_R20__ -D__MA330048_dsPIC33CK_DPPIM__ -D__P33SMPS_CK506__ -DXPRJ_MA330048_dsPIC33CK_DPPIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"sources" -I"mcc_generated_files"  -mreserve=data@0x1000:0x101B -mreserve=data@0x101C:0x101D -mreserve=data@0x101E:0x101F -mreserve=data@0x1020:0x1021 -mreserve=data@0x1022:0x1023 -mreserve=data@0x1024:0x1027 -mreserve=data@0x1028:0x104F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_REAL_ICE=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  
	
else
${DISTDIR}/dspic33ck-power-pwm-adc-trigger.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/dspic33ck-power-pwm-adc-trigger.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -D__DM330029_R20__ -D__MA330048_dsPIC33CK_DPPIM__ -D__P33SMPS_CK506__ -DXPRJ_MA330048_dsPIC33CK_DPPIM=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -I"sources" -I"mcc_generated_files" -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  
	${MP_CC_DIR}\\xc16-bin2hex ${DISTDIR}/dspic33ck-power-pwm-adc-trigger.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
