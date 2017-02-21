LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dlua_shared

LOCAL_MODULE_FILENAME := libcocos2dlua



FILE_LIST := hellolua/main.cpp \
../../Classes/ide-support/SimpleConfigParser.cpp \
../../Classes/ide-support/RuntimeLuaImpl.cpp \
../../Classes/ide-support/lua_debugger.c
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/*.cpp)
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)


LOCAL_CPPFLAGS := -DSDKBOX_ENABLED
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_WHOLE_STATIC_LIBRARIES := PluginProtocolStatic

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END

LOCAL_STATIC_LIBRARIES := cocos2d_lua_static
LOCAL_STATIC_LIBRARIES += cocos2d_simulator_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)
$(call import-add-path, $(LOCAL_PATH)/..)

$(call import-module,scripting/lua-bindings/proj.android)
$(call import-module,tools/simulator/libsimulator/proj.android)
$(call import-module,protocols/android)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
