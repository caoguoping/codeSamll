package org.cocos2dx.lua;

public class SDKPlugin{
	
	public static native void LoginCallback(String openid, String nickname, String sex, String headimgurl, String city);
	public static native void paySecessCallback(String orderNum, String save);   //充值成功回调
	public static native void sendDeepLink(String RoomNum, String save);   //获取参数后回调
}
