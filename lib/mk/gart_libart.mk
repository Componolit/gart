ANDROID_PORT		 = art
ANDROID_DIR			 = art
ANDROID_BUILDFILES = build/Android.bp test/Android.bp runtime/Android.bp
ANDROID_INCLUDES   = runtime runtime/generated cmdline
ANDROID_SOURCES	 = runtime
ANDROID_SECTIONS	= \
	/art_cc_library[@name=libart] \
	/art_cc_library[@name=libart]/shared \
	/art_cc_library[@name=libart]/target/android \
	/art_cc_library[@name=libart]/target/android_$(call android_arch) \
	/art_cc_library[@name=libart]/arch/$(call android_arch) \

GENSRCS_SECTION=art_operator_srcs
GENSRCS_SUBDIR=runtime

# jni.h required
LIBS += gart_libnativehelper

# dlmalloc required
LIBS += gart_dlmalloc

# sigchain lib required
LIBS += gart_libsigchain

# unicode required
LIBS += gart_libicuuc

# Genode compatibility lib
LIBS += gart_genode

# Remove libtombstone (mocked)
ANDROID_EXCLUDE_LIBS += libtombstoned_client

# attributes are not allowed on a function-definition with gcc
CC_OPT_entrypoints/quick/quick_dexcache_entrypoints += -D'__attribute__(x)='

# The fault handler is Android/Linux specific - replace by a Genode implementation
ANDROID_EXCLUDE_CC += arch/x86/fault_handler_x86.cc
ANDROID_EXCLUDE_CC += arch/arm/fault_handler_arm.cc
ANDROID_EXCLUDE_CC += arch/arm64/fault_handler_arm64.cc
SRC_CC += fault_handler_genode.cc

# Replace Android-specific memory mapping
ANDROID_EXCLUDE_CC += mem_map.cc
SRC_CC += mem_map_genode.cc

vpath %.cc $(REP_DIR)/src/libart

include $(call select_from_repositories,lib/mk/android-lib.inc)

# __atomic_load_16/__atomic_store_16 on ARMv8
ifeq ($(call android_arch),arm64)
LIBS += gart_libatomic
endif
