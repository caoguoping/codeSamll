package org.cocos2dx.lua;

public class SDKPlugin{
	
	public static native void LoginCallback(String openid, String nickname, String sex, String headimgurl, String city, String ip);
	public static native void paySecessCallback(String orderNum, String save);   //��ֵ�ɹ��ص�
	public static native void sendDeepLink(String RoomNum, String save);   //��ȡ������ص�
}
