// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "library_common.h" 

namespace Libraries::UserService{

int PS4_SYSV_ABI sceUserServiceInitializeForShellCore();
int PS4_SYSV_ABI sceUserServiceTerminateForShellCore();
int PS4_SYSV_ABI sceUserServiceDestroyUser();
int PS4_SYSV_ABI sceUserServiceGetAccessibilityKeyremapData();
int PS4_SYSV_ABI sceUserServiceGetAccessibilityKeyremapEnable();
int PS4_SYSV_ABI sceUserServiceGetAccessibilityPressAndHoldDelay();
int PS4_SYSV_ABI sceUserServiceGetAccessibilityVibration();
int PS4_SYSV_ABI sceUserServiceGetAccessibilityZoom();
int PS4_SYSV_ABI sceUserServiceGetAccessibilityZoomEnabled();
int PS4_SYSV_ABI sceUserServiceGetAccountRemarks();
int PS4_SYSV_ABI sceUserServiceGetAgeVerified();
int PS4_SYSV_ABI sceUserServiceGetAppearOfflineSetting();
int PS4_SYSV_ABI sceUserServiceGetAppSortOrder();
int PS4_SYSV_ABI sceUserServiceGetAutoLoginEnabled();
int PS4_SYSV_ABI sceUserServiceGetCreatedVersion();
int PS4_SYSV_ABI sceUserServiceGetCurrentUserGroupIndex();
int PS4_SYSV_ABI sceUserServiceGetDefaultNewUserGroupName();
int PS4_SYSV_ABI sceUserServiceGetDeletedUserInfo();
int PS4_SYSV_ABI sceUserServiceGetDiscPlayerFlag();
int PS4_SYSV_ABI sceUserServiceGetEvent();
int PS4_SYSV_ABI sceUserServiceGetEventCalendarType();
int PS4_SYSV_ABI sceUserServiceGetEventFilterTeamEvent();
int PS4_SYSV_ABI sceUserServiceGetEventSortEvent();
int PS4_SYSV_ABI sceUserServiceGetEventSortTitle();
int PS4_SYSV_ABI sceUserServiceGetEventUiFlag();
int PS4_SYSV_ABI sceUserServiceGetEventVsh();
int PS4_SYSV_ABI sceUserServiceGetFaceRecognitionDeleteCount();
int PS4_SYSV_ABI sceUserServiceGetFaceRecognitionRegisterCount();
int PS4_SYSV_ABI sceUserServiceGetFileBrowserFilter();
int PS4_SYSV_ABI sceUserServiceGetFileBrowserSortContent();
int PS4_SYSV_ABI sceUserServiceGetFileBrowserSortTitle();
int PS4_SYSV_ABI sceUserServiceGetFileSelectorFilter();
int PS4_SYSV_ABI sceUserServiceGetFileSelectorSortContent();
int PS4_SYSV_ABI sceUserServiceGetFileSelectorSortTitle();
int PS4_SYSV_ABI sceUserServiceGetForegroundUser();
int PS4_SYSV_ABI sceUserServiceGetFriendCustomListLastFocus();
int PS4_SYSV_ABI sceUserServiceGetFriendFlag();
int PS4_SYSV_ABI sceUserServiceGetGlsAccessTokenNiconicoLive();
int PS4_SYSV_ABI sceUserServiceGetGlsAccessTokenTwitch();
int PS4_SYSV_ABI sceUserServiceGetGlsAccessTokenUstream();
int PS4_SYSV_ABI sceUserServiceGetGlsAnonymousUserId();
int PS4_SYSV_ABI sceUserServiceGetGlsBcTags();
int PS4_SYSV_ABI sceUserServiceGetGlsBcTitle();
int PS4_SYSV_ABI sceUserServiceGetGlsBroadcastChannel();
int PS4_SYSV_ABI sceUserServiceGetGlsBroadcastersComment();
int PS4_SYSV_ABI sceUserServiceGetGlsBroadcastersCommentColor();
int PS4_SYSV_ABI sceUserServiceGetGlsBroadcastService();
int PS4_SYSV_ABI sceUserServiceGetGlsBroadcastUiLayout();
int PS4_SYSV_ABI sceUserServiceGetGlsCamCrop();
int PS4_SYSV_ABI sceUserServiceGetGlsCameraBgFilter();
int PS4_SYSV_ABI sceUserServiceGetGlsCameraBrightness();
int PS4_SYSV_ABI sceUserServiceGetGlsCameraChromaKeyLevel();
int PS4_SYSV_ABI sceUserServiceGetGlsCameraContrast();
int PS4_SYSV_ABI sceUserServiceGetGlsCameraDepthLevel();
int PS4_SYSV_ABI sceUserServiceGetGlsCameraEdgeLevel();
int PS4_SYSV_ABI sceUserServiceGetGlsCameraEffect();
int PS4_SYSV_ABI sceUserServiceGetGlsCameraEliminationLevel();
int PS4_SYSV_ABI sceUserServiceGetGlsCameraPosition();
int PS4_SYSV_ABI sceUserServiceGetGlsCameraReflection();
int PS4_SYSV_ABI sceUserServiceGetGlsCameraSize();
int PS4_SYSV_ABI sceUserServiceGetGlsCameraTransparency();
int PS4_SYSV_ABI sceUserServiceGetGlsCommunityId();
int PS4_SYSV_ABI sceUserServiceGetGlsFloatingMessage();
int PS4_SYSV_ABI sceUserServiceGetGlsHintFlag();
int PS4_SYSV_ABI sceUserServiceGetGlsInitSpectating();
int PS4_SYSV_ABI sceUserServiceGetGlsIsCameraHidden();
int PS4_SYSV_ABI sceUserServiceGetGlsIsFacebookEnabled();
int PS4_SYSV_ABI sceUserServiceGetGlsIsMuteEnabled();
int PS4_SYSV_ABI sceUserServiceGetGlsIsRecDisabled();
int PS4_SYSV_ABI sceUserServiceGetGlsIsRecievedMessageHidden();
int PS4_SYSV_ABI sceUserServiceGetGlsIsTwitterEnabled();
int PS4_SYSV_ABI sceUserServiceGetGlsLanguageFilter();
int PS4_SYSV_ABI sceUserServiceGetGlsLfpsSortOrder();
int PS4_SYSV_ABI sceUserServiceGetGlsLiveQuality();
int PS4_SYSV_ABI sceUserServiceGetGlsLiveQuality2();
int PS4_SYSV_ABI sceUserServiceGetGlsLiveQuality3();
int PS4_SYSV_ABI sceUserServiceGetGlsLiveQuality4();
int PS4_SYSV_ABI sceUserServiceGetGlsLiveQuality5();
int PS4_SYSV_ABI sceUserServiceGetGlsMessageFilterLevel();
int PS4_SYSV_ABI sceUserServiceGetGlsTtsFlags();
int PS4_SYSV_ABI sceUserServiceGetGlsTtsPitch();
int PS4_SYSV_ABI sceUserServiceGetGlsTtsSpeed();
int PS4_SYSV_ABI sceUserServiceGetGlsTtsVolume();
int PS4_SYSV_ABI sceUserServiceGetHmuBrightness();
int PS4_SYSV_ABI sceUserServiceGetHmuZoom();
int PS4_SYSV_ABI sceUserServiceGetHoldAudioOutDevice();
int PS4_SYSV_ABI sceUserServiceGetHomeDirectory();
int PS4_SYSV_ABI sceUserServiceGetImeAutoCapitalEnabled();
int PS4_SYSV_ABI sceUserServiceGetImeInitFlag();
int PS4_SYSV_ABI sceUserServiceGetImeInputType();
int PS4_SYSV_ABI sceUserServiceGetImeLastUnit();
int PS4_SYSV_ABI sceUserServiceGetImePointerMode();
int PS4_SYSV_ABI sceUserServiceGetImePredictiveTextEnabled();
int PS4_SYSV_ABI sceUserServiceGetImeRunCount();
int PS4_SYSV_ABI sceUserServiceGetInitialUser();
int PS4_SYSV_ABI sceUserServiceGetIPDLeft();
int PS4_SYSV_ABI sceUserServiceGetIPDRight();
int PS4_SYSV_ABI sceUserServiceGetIsFakePlus();
int PS4_SYSV_ABI sceUserServiceGetIsQuickSignup();
int PS4_SYSV_ABI sceUserServiceGetIsRemotePlayAllowed();
int PS4_SYSV_ABI sceUserServiceGetJapaneseInputType();
int PS4_SYSV_ABI sceUserServiceGetKeyboardType();
int PS4_SYSV_ABI sceUserServiceGetKeyRepeatSpeed();
int PS4_SYSV_ABI sceUserServiceGetKeyRepeatStartingTime();
int PS4_SYSV_ABI sceUserServiceGetKratosPrimaryUser();
int PS4_SYSV_ABI sceUserServiceGetLastLoginOrder();
int PS4_SYSV_ABI sceUserServiceGetLightBarBaseBrightness();
int PS4_SYSV_ABI sceUserServiceGetLoginFlag();
int PS4_SYSV_ABI sceUserServiceGetLoginUserIdList();
int PS4_SYSV_ABI sceUserServiceGetMicLevel();
int PS4_SYSV_ABI sceUserServiceGetMouseHandType();
int PS4_SYSV_ABI sceUserServiceGetMousePointerSpeed();
int PS4_SYSV_ABI sceUserServiceGetNotificationBehavior();
int PS4_SYSV_ABI sceUserServiceGetNotificationSettings();
int PS4_SYSV_ABI sceUserServiceGetNpAccountId();
int PS4_SYSV_ABI sceUserServiceGetNpAccountUpgradeFlag();
int PS4_SYSV_ABI sceUserServiceGetNpAge();
int PS4_SYSV_ABI sceUserServiceGetNpAuthErrorFlag();
int PS4_SYSV_ABI sceUserServiceGetNpCountryCode();
int PS4_SYSV_ABI sceUserServiceGetNpDateOfBirth();
int PS4_SYSV_ABI sceUserServiceGetNpEnv();
int PS4_SYSV_ABI sceUserServiceGetNpLanguageCode();
int PS4_SYSV_ABI sceUserServiceGetNpLanguageCode2();
int PS4_SYSV_ABI sceUserServiceGetNpLoginId();
int PS4_SYSV_ABI sceUserServiceGetNpMAccountId();
int PS4_SYSV_ABI sceUserServiceGetNpNpId();
int PS4_SYSV_ABI sceUserServiceGetNpOfflineAccountAdult();
int PS4_SYSV_ABI sceUserServiceGetNpOfflineAccountId();
int PS4_SYSV_ABI sceUserServiceGetNpOnlineId();
int PS4_SYSV_ABI sceUserServiceGetNpSubAccount();
int PS4_SYSV_ABI sceUserServiceGetPadSpeakerVolume();
int PS4_SYSV_ABI sceUserServiceGetParentalBdAge();
int PS4_SYSV_ABI sceUserServiceGetParentalBrowser();
int PS4_SYSV_ABI sceUserServiceGetParentalDvd();
int PS4_SYSV_ABI sceUserServiceGetParentalDvdRegion();
int PS4_SYSV_ABI sceUserServiceGetParentalGame();
int PS4_SYSV_ABI sceUserServiceGetParentalGameAgeLevel();
int PS4_SYSV_ABI sceUserServiceGetParentalMorpheus();
int PS4_SYSV_ABI sceUserServiceGetPartyMuteList();
int PS4_SYSV_ABI sceUserServiceGetPartyMuteListA();
int PS4_SYSV_ABI sceUserServiceGetPartySettingFlags();
int PS4_SYSV_ABI sceUserServiceGetPasscode();
int PS4_SYSV_ABI sceUserServiceGetPbtcAdditionalTime();
int PS4_SYSV_ABI sceUserServiceGetPbtcFlag();
int PS4_SYSV_ABI sceUserServiceGetPbtcFridayDuration();
int PS4_SYSV_ABI sceUserServiceGetPbtcFridayHoursEnd();
int PS4_SYSV_ABI sceUserServiceGetPbtcFridayHoursStart();
int PS4_SYSV_ABI sceUserServiceGetPbtcMode();
int PS4_SYSV_ABI sceUserServiceGetPbtcMondayDuration();
int PS4_SYSV_ABI sceUserServiceGetPbtcMondayHoursEnd();
int PS4_SYSV_ABI sceUserServiceGetPbtcMondayHoursStart();
int PS4_SYSV_ABI sceUserServiceGetPbtcPlayTime();
int PS4_SYSV_ABI sceUserServiceGetPbtcPlayTimeLastUpdated();
int PS4_SYSV_ABI sceUserServiceGetPbtcSaturdayDuration();
int PS4_SYSV_ABI sceUserServiceGetPbtcSaturdayHoursEnd();
int PS4_SYSV_ABI sceUserServiceGetPbtcSaturdayHoursStart();
int PS4_SYSV_ABI sceUserServiceGetPbtcSundayDuration();
int PS4_SYSV_ABI sceUserServiceGetPbtcSundayHoursEnd();
int PS4_SYSV_ABI sceUserServiceGetPbtcSundayHoursStart();
int PS4_SYSV_ABI sceUserServiceGetPbtcThursdayDuration();
int PS4_SYSV_ABI sceUserServiceGetPbtcThursdayHoursEnd();
int PS4_SYSV_ABI sceUserServiceGetPbtcThursdayHoursStart();
int PS4_SYSV_ABI sceUserServiceGetPbtcTuesdayDuration();
int PS4_SYSV_ABI sceUserServiceGetPbtcTuesdayHoursEnd();
int PS4_SYSV_ABI sceUserServiceGetPbtcTuesdayHoursStart();
int PS4_SYSV_ABI sceUserServiceGetPbtcTzOffset();
int PS4_SYSV_ABI sceUserServiceGetPbtcWednesdayDuration();
int PS4_SYSV_ABI sceUserServiceGetPbtcWednesdayHoursEnd();
int PS4_SYSV_ABI sceUserServiceGetPbtcWednesdayHoursStart();
int PS4_SYSV_ABI sceUserServiceGetPlayTogetherFlags();
int PS4_SYSV_ABI sceUserServiceGetPsnPasswordForDebug();
int PS4_SYSV_ABI sceUserServiceGetRegisteredHomeUserIdList();
int PS4_SYSV_ABI sceUserServiceGetRegisteredUserIdList();
int PS4_SYSV_ABI sceUserServiceGetSaveDataAutoUpload();
int PS4_SYSV_ABI sceUserServiceGetSaveDataSort();
int PS4_SYSV_ABI sceUserServiceGetSaveDataTutorialFlag();
int PS4_SYSV_ABI sceUserServiceGetSecureHomeDirectory();
int PS4_SYSV_ABI sceUserServiceGetShareButtonAssign();
int PS4_SYSV_ABI sceUserServiceGetShareDailymotionAccessToken();
int PS4_SYSV_ABI sceUserServiceGetShareDailymotionRefreshToken();
int PS4_SYSV_ABI sceUserServiceGetSharePlayFlags();
int PS4_SYSV_ABI sceUserServiceGetSharePlayFramerateHost();
int PS4_SYSV_ABI sceUserServiceGetSharePlayResolutionHost();
int PS4_SYSV_ABI sceUserServiceGetShareStatus();
int PS4_SYSV_ABI sceUserServiceGetShareStatus2();
int PS4_SYSV_ABI sceUserServiceGetSystemLoggerHashedAccountId();
int PS4_SYSV_ABI sceUserServiceGetSystemLoggerHashedAccountIdClockType();
int PS4_SYSV_ABI sceUserServiceGetSystemLoggerHashedAccountIdParam();
int PS4_SYSV_ABI sceUserServiceGetSystemLoggerHashedAccountIdTtl();
int PS4_SYSV_ABI sceUserServiceGetTeamShowAboutTeam();
int PS4_SYSV_ABI sceUserServiceGetThemeBgImageDimmer();
int PS4_SYSV_ABI sceUserServiceGetThemeBgImageWaveColor();
int PS4_SYSV_ABI sceUserServiceGetThemeBgImageZoom();
int PS4_SYSV_ABI sceUserServiceGetThemeEntitlementId();
int PS4_SYSV_ABI sceUserServiceGetThemeHomeShareOwner();
int PS4_SYSV_ABI sceUserServiceGetThemeTextShadow();
int PS4_SYSV_ABI sceUserServiceGetThemeWaveColor();
int PS4_SYSV_ABI sceUserServiceGetTopMenuLimitItem();
int PS4_SYSV_ABI sceUserServiceGetTopMenuNotificationFlag();
int PS4_SYSV_ABI sceUserServiceGetTopMenuTutorialFlag();
int PS4_SYSV_ABI sceUserServiceGetTraditionalChineseInputType();
int PS4_SYSV_ABI sceUserServiceGetUserColor();
int PS4_SYSV_ABI sceUserServiceGetUserGroupName();
int PS4_SYSV_ABI sceUserServiceGetUserGroupNameList();
int PS4_SYSV_ABI sceUserServiceGetUserGroupNum();
int PS4_SYSV_ABI sceUserServiceGetUserName();
int PS4_SYSV_ABI sceUserServiceGetUserStatus();
int PS4_SYSV_ABI sceUserServiceGetVibrationEnabled();
int PS4_SYSV_ABI sceUserServiceGetVoiceRecognitionLastUsedOsk();
int PS4_SYSV_ABI sceUserServiceGetVoiceRecognitionTutorialState();
int PS4_SYSV_ABI sceUserServiceGetVolumeForController();
int PS4_SYSV_ABI sceUserServiceGetVolumeForGenericUSB();
int PS4_SYSV_ABI sceUserServiceGetVolumeForMorpheusSidetone();
int PS4_SYSV_ABI sceUserServiceGetVolumeForSidetone();
int PS4_SYSV_ABI sceUserServiceInitialize();
int PS4_SYSV_ABI sceUserServiceInitialize2();
int PS4_SYSV_ABI sceUserServiceIsGuestUser();
int PS4_SYSV_ABI sceUserServiceIsKratosPrimaryUser();
int PS4_SYSV_ABI sceUserServiceIsKratosUser();
int PS4_SYSV_ABI sceUserServiceIsLoggedIn();
int PS4_SYSV_ABI sceUserServiceIsLoggedInWithoutLock();
int PS4_SYSV_ABI sceUserServiceIsSharePlayClientUser();
int PS4_SYSV_ABI sceUserServiceIsUserStorageAccountBound();
int PS4_SYSV_ABI sceUserServiceLogin();
int PS4_SYSV_ABI sceUserServiceLogout();
int PS4_SYSV_ABI sceUserServiceRegisterEventCallback();
int PS4_SYSV_ABI sceUserServiceSetAccessibilityKeyremapData();
int PS4_SYSV_ABI sceUserServiceSetAccessibilityKeyremapEnable();
int PS4_SYSV_ABI sceUserServiceSetAccessibilityZoom();
int PS4_SYSV_ABI sceUserServiceSetAccountRemarks();
int PS4_SYSV_ABI sceUserServiceSetAgeVerified();
int PS4_SYSV_ABI sceUserServiceSetAppearOfflineSetting();
int PS4_SYSV_ABI sceUserServiceSetAppSortOrder();
int PS4_SYSV_ABI sceUserServiceSetAutoLoginEnabled();
int PS4_SYSV_ABI sceUserServiceSetCreatedVersion();
int PS4_SYSV_ABI sceUserServiceSetDiscPlayerFlag();
int PS4_SYSV_ABI sceUserServiceSetEventCalendarType();
int PS4_SYSV_ABI sceUserServiceSetEventFilterTeamEvent();
int PS4_SYSV_ABI sceUserServiceSetEventSortEvent();
int PS4_SYSV_ABI sceUserServiceSetEventSortTitle();
int PS4_SYSV_ABI sceUserServiceSetEventUiFlag();
int PS4_SYSV_ABI sceUserServiceSetFaceRecognitionDeleteCount();
int PS4_SYSV_ABI sceUserServiceSetFaceRecognitionRegisterCount();
int PS4_SYSV_ABI sceUserServiceSetFileBrowserFilter();
int PS4_SYSV_ABI sceUserServiceSetFileBrowserSortContent();
int PS4_SYSV_ABI sceUserServiceSetFileBrowserSortTitle();
int PS4_SYSV_ABI sceUserServiceSetFileSelectorFilter();
int PS4_SYSV_ABI sceUserServiceSetFileSelectorSortContent();
int PS4_SYSV_ABI sceUserServiceSetFileSelectorSortTitle();
int PS4_SYSV_ABI sceUserServiceSetForegroundUser();
int PS4_SYSV_ABI sceUserServiceSetFriendCustomListLastFocus();
int PS4_SYSV_ABI sceUserServiceSetFriendFlag();
int PS4_SYSV_ABI sceUserServiceSetGlsAccessTokenNiconicoLive();
int PS4_SYSV_ABI sceUserServiceSetGlsAccessTokenTwitch();
int PS4_SYSV_ABI sceUserServiceSetGlsAccessTokenUstream();
int PS4_SYSV_ABI sceUserServiceSetGlsAnonymousUserId();
int PS4_SYSV_ABI sceUserServiceSetGlsBcTags();
int PS4_SYSV_ABI sceUserServiceSetGlsBcTitle();
int PS4_SYSV_ABI sceUserServiceSetGlsBroadcastChannel();
int PS4_SYSV_ABI sceUserServiceSetGlsBroadcastersComment();
int PS4_SYSV_ABI sceUserServiceSetGlsBroadcastersCommentColor();
int PS4_SYSV_ABI sceUserServiceSetGlsBroadcastService();
int PS4_SYSV_ABI sceUserServiceSetGlsBroadcastUiLayout();
int PS4_SYSV_ABI sceUserServiceSetGlsCamCrop();
int PS4_SYSV_ABI sceUserServiceSetGlsCameraBgFilter();
int PS4_SYSV_ABI sceUserServiceSetGlsCameraBrightness();
int PS4_SYSV_ABI sceUserServiceSetGlsCameraChromaKeyLevel();
int PS4_SYSV_ABI sceUserServiceSetGlsCameraContrast();
int PS4_SYSV_ABI sceUserServiceSetGlsCameraDepthLevel();
int PS4_SYSV_ABI sceUserServiceSetGlsCameraEdgeLevel();
int PS4_SYSV_ABI sceUserServiceSetGlsCameraEffect();
int PS4_SYSV_ABI sceUserServiceSetGlsCameraEliminationLevel();
int PS4_SYSV_ABI sceUserServiceSetGlsCameraPosition();
int PS4_SYSV_ABI sceUserServiceSetGlsCameraReflection();
int PS4_SYSV_ABI sceUserServiceSetGlsCameraSize();
int PS4_SYSV_ABI sceUserServiceSetGlsCameraTransparency();
int PS4_SYSV_ABI sceUserServiceSetGlsCommunityId();
int PS4_SYSV_ABI sceUserServiceSetGlsFloatingMessage();
int PS4_SYSV_ABI sceUserServiceSetGlsHintFlag();
int PS4_SYSV_ABI sceUserServiceSetGlsInitSpectating();
int PS4_SYSV_ABI sceUserServiceSetGlsIsCameraHidden();
int PS4_SYSV_ABI sceUserServiceSetGlsIsFacebookEnabled();
int PS4_SYSV_ABI sceUserServiceSetGlsIsMuteEnabled();
int PS4_SYSV_ABI sceUserServiceSetGlsIsRecDisabled();
int PS4_SYSV_ABI sceUserServiceSetGlsIsRecievedMessageHidden();
int PS4_SYSV_ABI sceUserServiceSetGlsIsTwitterEnabled();
int PS4_SYSV_ABI sceUserServiceSetGlsLanguageFilter();
int PS4_SYSV_ABI sceUserServiceSetGlsLfpsSortOrder();
int PS4_SYSV_ABI sceUserServiceSetGlsLiveQuality();
int PS4_SYSV_ABI sceUserServiceSetGlsLiveQuality2();
int PS4_SYSV_ABI sceUserServiceSetGlsLiveQuality3();
int PS4_SYSV_ABI sceUserServiceSetGlsLiveQuality4();
int PS4_SYSV_ABI sceUserServiceSetGlsLiveQuality5();
int PS4_SYSV_ABI sceUserServiceSetGlsMessageFilterLevel();
int PS4_SYSV_ABI sceUserServiceSetGlsTtsFlags();
int PS4_SYSV_ABI sceUserServiceSetGlsTtsPitch();
int PS4_SYSV_ABI sceUserServiceSetGlsTtsSpeed();
int PS4_SYSV_ABI sceUserServiceSetGlsTtsVolume();
int PS4_SYSV_ABI sceUserServiceSetHmuBrightness();
int PS4_SYSV_ABI sceUserServiceSetHmuZoom();
int PS4_SYSV_ABI sceUserServiceSetHoldAudioOutDevice();
int PS4_SYSV_ABI sceUserServiceSetImeAutoCapitalEnabled();
int PS4_SYSV_ABI sceUserServiceSetImeInitFlag();
int PS4_SYSV_ABI sceUserServiceSetImeInputType();
int PS4_SYSV_ABI sceUserServiceSetImeLastUnit();
int PS4_SYSV_ABI sceUserServiceSetImePointerMode();
int PS4_SYSV_ABI sceUserServiceSetImePredictiveTextEnabled();
int PS4_SYSV_ABI sceUserServiceSetImeRunCount();
int PS4_SYSV_ABI sceUserServiceSetIPDLeft();
int PS4_SYSV_ABI sceUserServiceSetIPDRight();
int PS4_SYSV_ABI sceUserServiceSetIsFakePlus();
int PS4_SYSV_ABI sceUserServiceSetIsQuickSignup();
int PS4_SYSV_ABI sceUserServiceSetIsRemotePlayAllowed();
int PS4_SYSV_ABI sceUserServiceSetJapaneseInputType();
int PS4_SYSV_ABI sceUserServiceSetKeyboardType();
int PS4_SYSV_ABI sceUserServiceSetKeyRepeatSpeed();
int PS4_SYSV_ABI sceUserServiceSetKeyRepeatStartingTime();
int PS4_SYSV_ABI sceUserServiceSetLightBarBaseBrightness();
int PS4_SYSV_ABI sceUserServiceSetLoginFlag();
int PS4_SYSV_ABI sceUserServiceSetMicLevel();
int PS4_SYSV_ABI sceUserServiceSetMouseHandType();
int PS4_SYSV_ABI sceUserServiceSetMousePointerSpeed();
int PS4_SYSV_ABI sceUserServiceSetNotificationBehavior();
int PS4_SYSV_ABI sceUserServiceSetNotificationSettings();
int PS4_SYSV_ABI sceUserServiceSetNpAccountUpgradeFlag();
int PS4_SYSV_ABI sceUserServiceSetNpAge();
int PS4_SYSV_ABI sceUserServiceSetNpAuthErrorFlag();
int PS4_SYSV_ABI sceUserServiceSetNpCountryCode();
int PS4_SYSV_ABI sceUserServiceSetNpDateOfBirth();
int PS4_SYSV_ABI sceUserServiceSetNpEnv();
int PS4_SYSV_ABI sceUserServiceSetNpLanguageCode();
int PS4_SYSV_ABI sceUserServiceSetNpLanguageCode2();
int PS4_SYSV_ABI sceUserServiceSetNpLoginId();
int PS4_SYSV_ABI sceUserServiceSetNpMAccountId();
int PS4_SYSV_ABI sceUserServiceSetNpNpId();
int PS4_SYSV_ABI sceUserServiceSetNpOfflineAccountAdult();
int PS4_SYSV_ABI sceUserServiceSetNpOnlineId();
int PS4_SYSV_ABI sceUserServiceSetNpSubAccount();
int PS4_SYSV_ABI sceUserServiceSetPadSpeakerVolume();
int PS4_SYSV_ABI sceUserServiceSetParentalBdAge();
int PS4_SYSV_ABI sceUserServiceSetParentalBrowser();
int PS4_SYSV_ABI sceUserServiceSetParentalDvd();
int PS4_SYSV_ABI sceUserServiceSetParentalDvdRegion();
int PS4_SYSV_ABI sceUserServiceSetParentalGame();
int PS4_SYSV_ABI sceUserServiceSetParentalGameAgeLevel();
int PS4_SYSV_ABI sceUserServiceSetParentalMorpheus();
int PS4_SYSV_ABI sceUserServiceSetPartyMuteList();
int PS4_SYSV_ABI sceUserServiceSetPartyMuteListA();
int PS4_SYSV_ABI sceUserServiceSetPartySettingFlags();
int PS4_SYSV_ABI sceUserServiceSetPasscode();
int PS4_SYSV_ABI sceUserServiceSetPbtcAdditionalTime();
int PS4_SYSV_ABI sceUserServiceSetPbtcFlag();
int PS4_SYSV_ABI sceUserServiceSetPbtcFridayDuration();
int PS4_SYSV_ABI sceUserServiceSetPbtcFridayHoursEnd();
int PS4_SYSV_ABI sceUserServiceSetPbtcFridayHoursStart();
int PS4_SYSV_ABI sceUserServiceSetPbtcMode();
int PS4_SYSV_ABI sceUserServiceSetPbtcMondayDuration();
int PS4_SYSV_ABI sceUserServiceSetPbtcMondayHoursEnd();
int PS4_SYSV_ABI sceUserServiceSetPbtcMondayHoursStart();
int PS4_SYSV_ABI sceUserServiceSetPbtcPlayTime();
int PS4_SYSV_ABI sceUserServiceSetPbtcPlayTimeLastUpdated();
int PS4_SYSV_ABI sceUserServiceSetPbtcSaturdayDuration();
int PS4_SYSV_ABI sceUserServiceSetPbtcSaturdayHoursEnd();
int PS4_SYSV_ABI sceUserServiceSetPbtcSaturdayHoursStart();
int PS4_SYSV_ABI sceUserServiceSetPbtcSundayDuration();
int PS4_SYSV_ABI sceUserServiceSetPbtcSundayHoursEnd();
int PS4_SYSV_ABI sceUserServiceSetPbtcSundayHoursStart();
int PS4_SYSV_ABI sceUserServiceSetPbtcThursdayDuration();
int PS4_SYSV_ABI sceUserServiceSetPbtcThursdayHoursEnd();
int PS4_SYSV_ABI sceUserServiceSetPbtcThursdayHoursStart();
int PS4_SYSV_ABI sceUserServiceSetPbtcTuesdayDuration();
int PS4_SYSV_ABI sceUserServiceSetPbtcTuesdayHoursEnd();
int PS4_SYSV_ABI sceUserServiceSetPbtcTuesdayHoursStart();
int PS4_SYSV_ABI sceUserServiceSetPbtcTzOffset();
int PS4_SYSV_ABI sceUserServiceSetPbtcWednesdayDuration();
int PS4_SYSV_ABI sceUserServiceSetPbtcWednesdayHoursEnd();
int PS4_SYSV_ABI sceUserServiceSetPbtcWednesdayHoursStart();
int PS4_SYSV_ABI sceUserServiceSetPlayTogetherFlags();
int PS4_SYSV_ABI sceUserServiceSetPsnPasswordForDebug();
int PS4_SYSV_ABI sceUserServiceSetSaveDataAutoUpload();
int PS4_SYSV_ABI sceUserServiceSetSaveDataSort();
int PS4_SYSV_ABI sceUserServiceSetSaveDataTutorialFlag();
int PS4_SYSV_ABI sceUserServiceSetShareButtonAssign();
int PS4_SYSV_ABI sceUserServiceSetShareDailymotionAccessToken();
int PS4_SYSV_ABI sceUserServiceSetShareDailymotionRefreshToken();
int PS4_SYSV_ABI sceUserServiceSetSharePlayFlags();
int PS4_SYSV_ABI sceUserServiceSetSharePlayFramerateHost();
int PS4_SYSV_ABI sceUserServiceSetSharePlayResolutionHost();
int PS4_SYSV_ABI sceUserServiceSetShareStatus();
int PS4_SYSV_ABI sceUserServiceSetShareStatus2();
int PS4_SYSV_ABI sceUserServiceSetSystemLoggerHashedAccountId();
int PS4_SYSV_ABI sceUserServiceSetSystemLoggerHashedAccountIdClockType();
int PS4_SYSV_ABI sceUserServiceSetSystemLoggerHashedAccountIdParam();
int PS4_SYSV_ABI sceUserServiceSetSystemLoggerHashedAccountIdTtl();
int PS4_SYSV_ABI sceUserServiceSetTeamShowAboutTeam();
int PS4_SYSV_ABI sceUserServiceSetThemeBgImageDimmer();
int PS4_SYSV_ABI sceUserServiceSetThemeBgImageWaveColor();
int PS4_SYSV_ABI sceUserServiceSetThemeBgImageZoom();
int PS4_SYSV_ABI sceUserServiceSetThemeEntitlementId();
int PS4_SYSV_ABI sceUserServiceSetThemeHomeShareOwner();
int PS4_SYSV_ABI sceUserServiceSetThemeTextShadow();
int PS4_SYSV_ABI sceUserServiceSetThemeWaveColor();
int PS4_SYSV_ABI sceUserServiceSetTopMenuLimitItem();
int PS4_SYSV_ABI sceUserServiceSetTopMenuNotificationFlag();
int PS4_SYSV_ABI sceUserServiceSetTopMenuTutorialFlag();
int PS4_SYSV_ABI sceUserServiceSetTraditionalChineseInputType();
int PS4_SYSV_ABI sceUserServiceSetUserGroupIndex();
int PS4_SYSV_ABI sceUserServiceSetUserGroupName();
int PS4_SYSV_ABI sceUserServiceSetUserName();
int PS4_SYSV_ABI sceUserServiceSetUserStatus();
int PS4_SYSV_ABI sceUserServiceSetVibrationEnabled();
int PS4_SYSV_ABI sceUserServiceSetVoiceRecognitionLastUsedOsk();
int PS4_SYSV_ABI sceUserServiceSetVoiceRecognitionTutorialState();
int PS4_SYSV_ABI sceUserServiceSetVolumeForController();
int PS4_SYSV_ABI sceUserServiceSetVolumeForGenericUSB();
int PS4_SYSV_ABI sceUserServiceSetVolumeForMorpheusSidetone();
int PS4_SYSV_ABI sceUserServiceSetVolumeForSidetone();
int PS4_SYSV_ABI sceUserServiceTerminate();
int PS4_SYSV_ABI sceUserServiceUnregisterEventCallback();
int PS4_SYSV_ABI Func_isbcQWjV-qU();
int PS4_SYSV_ABI Func_pr3J39r9ArQ();
int PS4_SYSV_ABI Func_u5SR3+a0lTw();
int PS4_SYSV_ABI Func_0rgUYD57RHc();

void RegisterlibSceUserService(Loader::SymbolsResolver * sym);
}