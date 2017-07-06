#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=Cygwin_4.x-Windows
CND_DLIB_EXT=dll
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/HUD.o \
	${OBJECTDIR}/backInstructions.o \
	${OBJECTDIR}/backLevel0.o \
	${OBJECTDIR}/backLevel1.o \
	${OBJECTDIR}/backLevel2.o \
	${OBJECTDIR}/backLevel3.o \
	${OBJECTDIR}/backLose.o \
	${OBJECTDIR}/backPause.o \
	${OBJECTDIR}/backScene0.o \
	${OBJECTDIR}/backSplash.o \
	${OBJECTDIR}/backWin.o \
	${OBJECTDIR}/collisionLevel0.o \
	${OBJECTDIR}/collisionLevel1.o \
	${OBJECTDIR}/collisionLevel2.o \
	${OBJECTDIR}/collisionLevel3.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/myLib.o \
	${OBJECTDIR}/soundAdventure.o \
	${OBJECTDIR}/soundHurt.o \
	${OBJECTDIR}/soundJump.o \
	${OBJECTDIR}/soundLose.o \
	${OBJECTDIR}/soundSelect.o \
	${OBJECTDIR}/soundShieldDown.o \
	${OBJECTDIR}/soundShieldUp.o \
	${OBJECTDIR}/soundVictory.o \
	${OBJECTDIR}/spriteSheet.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk cs2261.exe

cs2261.exe: ${OBJECTFILES}
	${LINK.c} -o cs2261 ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/HUD.o: HUD.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/HUD.o HUD.c

${OBJECTDIR}/backInstructions.o: backInstructions.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/backInstructions.o backInstructions.c

${OBJECTDIR}/backLevel0.o: backLevel0.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/backLevel0.o backLevel0.c

${OBJECTDIR}/backLevel1.o: backLevel1.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/backLevel1.o backLevel1.c

${OBJECTDIR}/backLevel2.o: backLevel2.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/backLevel2.o backLevel2.c

${OBJECTDIR}/backLevel3.o: backLevel3.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/backLevel3.o backLevel3.c

${OBJECTDIR}/backLose.o: backLose.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/backLose.o backLose.c

${OBJECTDIR}/backPause.o: backPause.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/backPause.o backPause.c

${OBJECTDIR}/backScene0.o: backScene0.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/backScene0.o backScene0.c

${OBJECTDIR}/backSplash.o: backSplash.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/backSplash.o backSplash.c

${OBJECTDIR}/backWin.o: backWin.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/backWin.o backWin.c

${OBJECTDIR}/collisionLevel0.o: collisionLevel0.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/collisionLevel0.o collisionLevel0.c

${OBJECTDIR}/collisionLevel1.o: collisionLevel1.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/collisionLevel1.o collisionLevel1.c

${OBJECTDIR}/collisionLevel2.o: collisionLevel2.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/collisionLevel2.o collisionLevel2.c

${OBJECTDIR}/collisionLevel3.o: collisionLevel3.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/collisionLevel3.o collisionLevel3.c

${OBJECTDIR}/main.o: main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.c

${OBJECTDIR}/myLib.o: myLib.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/myLib.o myLib.c

${OBJECTDIR}/soundAdventure.o: soundAdventure.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/soundAdventure.o soundAdventure.c

${OBJECTDIR}/soundHurt.o: soundHurt.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/soundHurt.o soundHurt.c

${OBJECTDIR}/soundJump.o: soundJump.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/soundJump.o soundJump.c

${OBJECTDIR}/soundLose.o: soundLose.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/soundLose.o soundLose.c

${OBJECTDIR}/soundSelect.o: soundSelect.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/soundSelect.o soundSelect.c

${OBJECTDIR}/soundShieldDown.o: soundShieldDown.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/soundShieldDown.o soundShieldDown.c

${OBJECTDIR}/soundShieldUp.o: soundShieldUp.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/soundShieldUp.o soundShieldUp.c

${OBJECTDIR}/soundVictory.o: soundVictory.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/soundVictory.o soundVictory.c

${OBJECTDIR}/spriteSheet.o: spriteSheet.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/spriteSheet.o spriteSheet.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} cs2261.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
