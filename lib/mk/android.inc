ifneq ($(ANDROID_NAME),)

check_android_vars := $(call _assert,$(ANDROID_PORT), ANDROID_PORT not set for $(ANDROID_NAME))
check_android_vars := $(call _assert,$(ANDROID_SECTIONS), ANDROID_SECTIONS not set for $(ANDROID_NAME))
check_android_vars := $(call _assert,$(ANDROID_BUILDTYPE), ANDROID_BUILDTYPE not set for $(ANDROID_NAME))

ANDROID_INCLUDES   ?= . include
ANDROID_SOURCES    ?= .
ANDROID_BUILDFILES ?= Android.bp
ANDROID_UNDEFINED  ?=

ANDROID_PORTDIR = $(call _port_dir,gart_$(ANDROID_PORT))

ifeq ($(ANDROID_PORTDIR),)
DUMMY := $(shell echo GART: Port $(ANDROID_PORT) not found for $(ANDROID_NAME), preparing... 1>&2)
DUMMY := $(shell env -i $(BASE_DIR)/../../tool/ports/prepare_port gart_$(ANDROID_PORT) 1>&2)
endif

ANDROID_PORTDIR = $(call _port_dir,gart_$(ANDROID_PORT))
ifeq ($(ANDROID_PORTDIR),)
$(error Error preparing $(ANDROID_PORT) for $(ANDROID_NAME).)
endif

ANDROID_BASEDIR	= $(ANDROID_PORTDIR)/$(ANDROID_DIR)

internal_android_arch = \
   $(if $(filter $(SPECS), x86_32),x86, \
      $(if $(filter $(SPECS), x86_64),x86_64, \
         $(if $(filter $(SPECS), arm_v7a),arm, \
            $(if $(filter $(SPECS), arm_v8a),arm64, \
               $(if $(filter $(SPECS), linux),x86_64, \
                  $(if $(filter $(SPECS), arm_v6), $(error arm_v6 unsupported on Android), \
                     $(if $(filter $(SPECS), riscv), $(error RISCV unsupported on Android), \
                        $(error Unsupported architecture))))))))

android_arch = $(strip $(call internal_android_arch))

# Set Android-compatible architecture
ANDROID_ARCH = $(call android_arch)

# Set options for operator sources
ifneq ($(GENSRCS_SECTION),)
ANDROID_INCLUDES += $(GENSRCS_SUBDIR)
ANDROID_SECTIONS +=	/gensrcs[@name=$(GENSRCS_SECTION)]
endif

ANDROID_OUTPUT_DIR := $(LIB_CACHE_DIR)/$(ANDROID_BUILDTYPE)/$(ANDROID_NAME)

define GNOOS
$(shell mkdir -p $(ANDROID_OUTPUT_DIR) && \
    $(call select_from_repositories,tool/gnoos) \
		--basedir $(ANDROID_BASEDIR) \
		--prefix $(strip $(1))_ \
		--output $(ANDROID_OUTPUT_DIR)/$(strip $(4)) \
		--variable "BASEDIR:$(ANDROID_BASEDIR)" \
		--variable "EXPORT_INCLUDE_DIRS:$(ANDROID_INCLUDES)" \
        $(addprefix --undefined ,$(ANDROID_UNDEFINED)) \
		$(addprefix --input ,$(2)) \
		$(3))
$(eval include $(ANDROID_OUTPUT_DIR)/$(strip $(4)))
endef

$(call GNOOS, $(ANDROID_NAME), $(ANDROID_BUILDFILES), $(ANDROID_SECTIONS), android.mk)

# Generate operator sources
ifneq ($(GENSRCS_SECTION),)
include $(call select_from_repositories,lib/mk/art-operators.inc)
endif

SRC_C  += $(filter-out $(ANDROID_EXCLUDE_C),$(filter %.c,$($(ANDROID_NAME)_SRCS)))
SRC_CC += $(filter-out $(ANDROID_EXCLUDE_CC),$(filter %.cpp %.cc,$($(ANDROID_NAME)_SRCS)))
SRC_S  += $(filter-out $(ANDROID_EXCLUDE_S),$(filter %.S %.s,$($(ANDROID_NAME)_SRCS)))
SRC_JAVA += $(filter-out $(ANDROID_EXCLUDE_JAVA),$(filter %.java,$($(ANDROID_NAME)_SRCS)))

CC_OPT += $(filter-out $(ANDROID_EXCLUDE_OPT),$($(ANDROID_NAME)_CFLAGS))
CC_OPT += $(filter-out $(ANDROID_EXCLUDE_OPT),$($(ANDROID_NAME)_CPPFLAGS))
CC_OPT += -Werror -D__GENODE__ -D__ANDROID__
CC_OPT += -Wno-unused-function -Wno-unused-variable -Wno-unused-parameter
CC_CXX_WARN_STRICT =

INCLUDE_DIRS = \
	$($(ANDROID_NAME)_EXPORT_INCLUDE_DIRS) \
	$($(ANDROID_NAME)_LOCAL_INCLUDE_DIRS) \
	$($(ANDROID_NAME)_INCLUDE_DIRS)

INC_DIR := $(REP_DIR)/include/gart $(INC_DIR) $(addprefix $(ANDROID_BASEDIR)/,$(sort $(INCLUDE_DIRS)))

vpath % $(addprefix $(ANDROID_BASEDIR)/,$(ANDROID_SOURCES))

# Include these libraries by default
ANDROID_LIBS += \
   $($(ANDROID_NAME)_LIBS) \
   $($(ANDROID_NAME)_SHARED_LIBS) \
   $($(ANDROID_NAME)_STATIC_LIBS) \
   $($(ANDROID_NAME)_WHOLE_STATIC_LIBS)

# Filter out debug libraries
NON_DEBUG_LIBS = $(sort $(ANDROID_LIBS:_debug=))

ifeq ($(ANDROID_BUILDTYPE),jar)
JAVA_JAR_NAME = $($(ANDROID_NAME)_NAME)
JAVA_BASE_DIR = $($(ANDROID_NAME)_BASEDIR)
JAVA_OPTS += $($(ANDROID_NAME)_JAVACFLAGS)
DEX_OPTS += $($(ANDROID_NAME)_DXFLAGS)
include $(call select_from_repositories,lib/mk/gart_java.inc)
else
LIBS += libc cxx stdcxx
endif

LIBS += $(addprefix gart_,$(filter-out $(ANDROID_EXCLUDE_LIBS),$(NON_DEBUG_LIBS)))

else
$(error ANDROID_NAME not set)
endif
