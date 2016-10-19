LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)



LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AccountMediator.cpp \
                   ../../Classes/AccountView.cpp \
                   ../../Classes/ActivityMediator.cpp \
                   ../../Classes/ActivityView.cpp \
                   ../../Classes/AlertCommand.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/BlueSkyCommand.cpp \
                   ../../Classes/BlueSkyContext.cpp \
                   ../../Classes/BlueSkyEvent.cpp \
                   ../../Classes/BlueSkyMediator.cpp \
                   ../../Classes/BlueSkyModel.cpp \
                   ../../Classes/BlueSkyRegister.cpp \
                   ../../Classes/BlueSkyService.cpp \
                   ../../Classes/BlueSkyView.cpp \
                   ../../Classes/CallCppHelper.cpp \
                   ../../Classes/CancelAutoMediator.cpp \
                   ../../Classes/CancelAutoView.cpp \
                   ../../Classes/ChatMediator.cpp \
                   ../../Classes/ChatView.cpp \
                   ../../Classes/ConnectGameServiceCommand.cpp \
                   ../../Classes/CreateRoleMediator.cpp \
                   ../../Classes/CreateRoleView.cpp \
                   ../../Classes/DailyMissionMediator.cpp \
                   ../../Classes/DailyMissionView.cpp \
                   ../../Classes/DaoJuActionMediator.cpp \
                   ../../Classes/DaoJuActionView.cpp \
                   ../../Classes/DataManager.cpp \
                   ../../Classes/GameDataModel.cpp \
                   ../../Classes/GameOverCommand.cpp \
                   ../../Classes/GameStartCommand.cpp \
                   ../../Classes/GetJingGongCommand.cpp \
                   ../../Classes/GongPaiActionMediator.cpp \
                   ../../Classes/GongPaiActionView.cpp \
                   ../../Classes/HelpMediator.cpp \
                   ../../Classes/HelpView.cpp \
                   ../../Classes/InDeskService.cpp \
                   ../../Classes/JiPaiMediator.cpp \
                   ../../Classes/JiPaiView.cpp \
                   ../../Classes/LobbyMediator.cpp \
                   ../../Classes/LobbyView.cpp \
                   ../../Classes/LoginCompleteCommand.cpp \
                   ../../Classes/LoginMediator.cpp \
                   ../../Classes/LoginView.cpp \
                   ../../Classes/MallMediator.cpp \
                   ../../Classes/MallView.cpp \
                   ../../Classes/MatchMediator.cpp \
                   ../../Classes/MatchView.cpp \
                   ../../Classes/md5.cpp \
                   ../../Classes/MessageShowMediator.cpp \
                   ../../Classes/MessageShowView.cpp \
                   ../../Classes/MusicService.cpp \
                   ../../Classes/MyInfoMediator.cpp \
                   ../../Classes/MyInfoView.cpp \
                   ../../Classes/MTNotificationQueue.cpp \
                   ../../Classes/NetDataCommand.cpp \
                   ../../Classes/OutPokerCommand.cpp \
                   ../../Classes/OutPokerLogicRule.cpp \
                   ../../Classes/PackageMediator.cpp \
                   ../../Classes/PackageView.cpp \
                   ../../Classes/PlayerInDeskModel.cpp \
                   ../../Classes/PlayerInfoMediator.cpp \
                   ../../Classes/PlayerInfoView.cpp \
                   ../../Classes/PlayPokerMediator.cpp \
                   ../../Classes/PlayPokerView.cpp \
                   ../../Classes/PokerActionMediator.cpp \
                   ../../Classes/PokerActionView.cpp \
                   ../../Classes/PokerGameModel.cpp \
                   ../../Classes/PokerLogic.cpp \
                   ../../Classes/PokerMediator.cpp \
                   ../../Classes/PokerView.cpp \
                   ../../Classes/RegistCharacterCommand.cpp \
                   ../../Classes/ReShowPokerCommand.cpp \
                   ../../Classes/ReSortPokerCommand.cpp \
                   ../../Classes/RoomListModel.cpp \
                   ../../Classes/SendDataService.cpp \
                   ../../Classes/SendPokerCommand.cpp \
                   ../../Classes/SetMediator.cpp \
                   ../../Classes/SetView.cpp \
                   ../../Classes/SevenDayGiftMediator.cpp \
                   ../../Classes/SevenDayGiftView.cpp \
                   ../../Classes/SGTools.cpp \
                   ../../Classes/ShopMediator.cpp \
                   ../../Classes/ShopView.cpp \
                   ../../Classes/ShowChatCommand.cpp \
                   ../../Classes/ShowChatMediator.cpp \
                   ../../Classes/ShowChatView.cpp \
                   ../../Classes/ShowDaoJuActionCommand.cpp \
                   ../../Classes/SocketInitCommand.cpp \
                   ../../Classes/TCPSocketService.cpp \
                   ../../Classes/UILayerService.cpp \
                   ../../Classes/UIUtil.cpp \
                   ../../Classes/UTF8.cpp \
                   ../../Classes/ViewMatchRanking.cpp \
                   ../../Classes/ViewManager.cpp \
                   ../../Classes/WarningMediator.cpp \
                   ../../Classes/WarningView.cpp \
                   ../../Classes/FriendView.cpp \
                   ../../Classes/FriendMediator.cpp \
                   ../../Classes/PlayGoldView.cpp \
                   ../../Classes/PlayGoldMediator.cpp 


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

LOCAL_CFLAGS += -DLIBUTILS_NATIVE=1 $(TOOL_CFLAGS) -fpermissive  #add "-fpermissiv"

LOCAL_CFLAGS +=  -fshort-wchar

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END

