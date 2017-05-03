/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.lua;

import java.math.BigInteger;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.security.MessageDigest;
import java.util.Collection;
import java.util.Collections;
import java.util.Enumeration;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import com.anysdk.framework.PluginWrapper;
import android.R.bool;
import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.ActivityInfo;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.provider.Settings;
import android.text.format.Formatter;
import android.util.Log;
import android.view.WindowManager;
import android.widget.CheckBox;
import android.widget.Toast;
import com.tianlian.Mahjong.Constants;
import com.tianlian.Mahjong.wxapi.Util;
import com.tencent.mm.opensdk.modelmsg.SendAuth;
import com.tencent.mm.opensdk.modelmsg.SendMessageToWX;
import com.tencent.mm.opensdk.modelmsg.WXImageObject;
import com.tencent.mm.opensdk.modelmsg.WXMediaMessage;
import com.tencent.mm.opensdk.modelmsg.WXTextObject;
import com.tencent.mm.opensdk.modelmsg.WXWebpageObject;
import com.tencent.mm.opensdk.modelpay.PayReq;
import com.tencent.mm.opensdk.openapi.WXAPIFactory;






@SuppressLint("DefaultLocale") public class AppActivity extends Cocos2dxActivity{
	private static Context mContext;
	private static final int THUMB_SIZE = 150;
	private static int mTargetScene;
	private CheckBox isTimelineCb;
	private static String mStrRoomNum;
	
    static String hostIPAdress = "0.0.0.0";
    static String strBody = "南京秦淮麻将充值中心";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        mContext = this;
        
        
        Uri data = getIntent().getData();
        Log.e("LUA-print", "LUA-print  " + "Uri data " + data);
        if(data != null)
        {
            List<String> params = data.getPathSegments();
            Log.e("LUA-print", "LUA-print  " + "params " ); 
            
            
            String testId = params.get(0); // "dwRoomId"
            Log.e("LUA-print", "LUA-print  " + "testId " + testId); 
            Constants.strRoom = testId;
        }
        else
        {
        	Constants.strRoom = "0";
        }

        //获取ip
		Constants.hostIp = getHostIpAddress();

        PluginWrapper.init(this);
		PluginWrapper.setGLSurfaceView(Cocos2dxGLSurfaceView.getInstance());
		
        if(nativeIsLandScape()) {
            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
        } else {
            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT);
        }

        // Check the wifi is opened when the native is debug.
        if(nativeIsDebug())
        {
            getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
            if(!isNetworkConnected())
            {
                AlertDialog.Builder builder=new AlertDialog.Builder(this);
                builder.setTitle("Warning");
                builder.setMessage("Please open WIFI for debuging...");
                builder.setPositiveButton("OK",new DialogInterface.OnClickListener() {
                    
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        startActivity(new Intent(Settings.ACTION_WIFI_SETTINGS));
                        finish();
                        System.exit(0);
                    }
                });

                builder.setNegativeButton("Cancel", null);
                builder.setCancelable(true);
                builder.show();
            }
            hostIPAdress = getHostIpAddress();
        }
    }
    
    public static void getParam()
    {
    	int a = Integer.parseInt(mStrRoomNum);
    	   
    }
    
    //微信登录
	public static void login()
	{
        if (Constants.wx_api == null) 
        {
    	    Constants.wx_api = WXAPIFactory.createWXAPI(mContext, Constants.APP_ID, false);
  	    }
	    boolean isSuccess = Constants.wx_api.registerApp(Constants.APP_ID);
	    
	   	final SendAuth.Req req = new SendAuth.Req();
	   	req.scope = "snsapi_userinfo"; //请求个人信息   
		req.state = "null";
		boolean isSuccessSend = Constants.wx_api.sendReq(req);
	

	}
	
	
	private static String buildTransaction(final String type) {
		return (type == null) ? String.valueOf(System.currentTimeMillis()) : type + System.currentTimeMillis();
	}
	
	
	
	
	/**
	 * 对字符串md5加密
	 *
	 * @param str
	 * @return
	 */
	public static String getMD5(String str) {
	    try {
	        // 生成一个MD5加密计算摘要
	        MessageDigest md = MessageDigest.getInstance("MD5");
	        // 计算md5函数
	        md.update(str.getBytes());
	        // digest()最后确定返回md5 hash值，返回值为8位字符串。因为md5 hash值是16位的hex值，实际上就是8位的字符
	        // BigInteger函数则将8位的字符串转换成16位hex值，用字符串来表示；得到字符串形式的hash值
	        return new BigInteger(1, md.digest()).toString(16);
	    } catch (Exception e) {
	        //throw new SpeedException("MD5加密出现错误");
	    	return "";
	    }
	}
	
	
	/**
	 * 微信支付
	 * **/
	@SuppressLint("DefaultLocale") public static void callWeChatPay(String prePayId, String  saves)
	{

		
		
		
        PayReq req = new PayReq();
        req.appId = "wx28da1972ed3f2ee3";
        req.partnerId = "1459102302";
        req.prepayId = prePayId;
        req.packageValue = "Sign=WXPay";
        req.nonceStr = String.valueOf((int)(1+Math.random()*100000000));
        req.timeStamp = saves;
        
		//字典序排序  
		HashMap<String,String> map=new HashMap<String,String>();  
		  
		map.put("appid", req.appId);  
		map.put("partnerid", req.partnerId);  
		map.put("prepayid", prePayId);  
		map.put("package", req.packageValue);  
		map.put("noncestr", req.nonceStr);
		map.put("timestamp", req.timeStamp);
		  
		Collection<String> keyset= map.keySet();   
		  
		List list=new ArrayList<String>(keyset);  
		  
		Collections.sort(list);  
		//这种打印出的字符串顺序和微信官网提供的字典序顺序是一致的  
		String stringA = "appid=wx28da1972ed3f2ee3";
		
		for(int i=1;i<list.size();i++)   //第0个是appid
		{  
			stringA = stringA + "&" + list.get(i) + "=" + map.get(list.get(i));
			//System.out.println();  

		}  
		
		
		//Log.e("LUA-print", "LUA-print stirngA " + stringA); 
		String stringSignTemp = stringA + "&key=T3CXEPObkNPmk1qTZ27VLIvtQu2wdKDc";
		//Log.e("LUA-print", "LUA-print stirngSignTmp " + stringSignTemp); 
		
		String signTmp = getMD5(stringSignTemp) ;   //.toUpperCase();
		String sign = signTmp.toUpperCase();
		//Log.e("LUA-print", "LUA-print getMD5 " + "LUA-print SendOK"); 
		//Log.e("LUA-print", "LUA-print  " + sign); 
        
        
        
        req.sign = sign;

        Constants.wx_api.sendReq(req);
        //Log.e("LUA-print", "LUA-print wx_api sendReq OK"); 
		
	}
	
	
	
	
	//// 邀请微信好友 path 图片路径  url 网址  ，房间号，    isToAll  1:分享到朋友圈， 0  分享给好友 
	public static void WechatShareJoin(String path, String url,  int roomNum,  int isToAll)
	{
		Log.e("LUA-print", "LUA-print  " + path);
//		Bitmap bmp = BitmapFactory.decodeFile(path);
//		WXImageObject imgObj = new WXImageObject(bmp);
//		WXMediaMessage msg = new WXMediaMessage();
//		msg.mediaObject = imgObj;
//		//图片压缩
//		Bitmap thumbBmp = Bitmap.createScaledBitmap(bmp, THUMB_SIZE, THUMB_SIZE, true);
//		bmp.recycle();
//		msg.thumbData = Util.bmpToByteArray(thumbBmp, true);  
//		mTargetScene = (isToAll == 1) ? SendMessageToWX.Req.WXSceneTimeline : SendMessageToWX.Req.WXSceneSession;
//		//邀请好友打牌
		// 封装发送信息
//		SendMessageToWX.Req req = new SendMessageToWX.Req();
//		req.transaction = buildTransaction("img");
//		req.message = msg;
//		req.scene = mTargetScene; 

	    String str_m = String.valueOf(roomNum); 
	    String str ="0000000";
	    str_m=str.substring(0, 7-str_m.length())+str_m;
		
		WXWebpageObject webpage = new WXWebpageObject();
		webpage.webpageUrl = url + "?id=" + str_m;
		Log.e("LUA-print", "LUA-print  " + "webpage.webpageUrl" + webpage.webpageUrl);
		WXMediaMessage msg = new WXMediaMessage(webpage);

		msg.title = "房间号: " + str_m ;
		msg.description = "我正在秦淮南京麻将等你，快来加入吧！ ";
		
		//图片，暂时注释掉
//		Bitmap bmp = BitmapFactory.decodeFile(path);
//		Bitmap thumbBmp = Bitmap.createScaledBitmap(bmp, THUMB_SIZE, THUMB_SIZE, true);
//		bmp.recycle();
//		msg.thumbData = Util.bmpToByteArray(thumbBmp, true);
		
		
		mTargetScene = (isToAll == 1) ? SendMessageToWX.Req.WXSceneTimeline : SendMessageToWX.Req.WXSceneSession;
		
		SendMessageToWX.Req req = new SendMessageToWX.Req();
		req.transaction = buildTransaction("webpage");
		req.message = msg;
		req.scene = mTargetScene;
		Constants.wx_api.sendReq(req);
		Log.e("LUA-print", "LUA-print  " + "LUA-print SendOK");  
	}
	
	
	
	
	
	
	/**
     * 微信支付
     * @param pay_param 支付服务生成的支付参数
     */
	
	
//    private void doWXPay(String pay_param) {
//	private void doPay(String path, String url,  int roomNum,  int isToAll) {
//        PayReq req = new PayReq();
//        req.appId = param.optString("appid");
//        req.partnerId = param.optString("partnerid");
//        req.prepayId = param.optString("prepayid");
//        req.packageValue = param.optString("package");
//        req.nonceStr = param.optString("noncestr");
//        req.timeStamp = param.optString("timestamp");
//        req.sign = param.optString("sign");
//
//        mWXApi.sendReq(req);
//    }
//	
    private boolean isNetworkConnected() {
            ConnectivityManager cm = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);  
            if (cm != null) {  
                NetworkInfo networkInfo = cm.getActiveNetworkInfo();  
            ArrayList networkTypes = new ArrayList();
            networkTypes.add(ConnectivityManager.TYPE_WIFI);
            try {
                networkTypes.add(ConnectivityManager.class.getDeclaredField("TYPE_ETHERNET").getInt(null));
            } catch (NoSuchFieldException nsfe) {
            }
            catch (IllegalAccessException iae) {
                throw new RuntimeException(iae);
            }
            if (networkInfo != null && networkTypes.contains(networkInfo.getType())) {
                    return true;  
                }  
            }  
            return false;  
        } 
     
    public String getHostIpAddress() {
        WifiManager wifiMgr = (WifiManager) getSystemService(WIFI_SERVICE);
        WifiInfo wifiInfo = wifiMgr.getConnectionInfo();
        int ip = wifiInfo.getIpAddress();
        return ((ip & 0xFF) + "." + ((ip >>>= 8) & 0xFF) + "." + ((ip >>>= 8) & 0xFF) + "." + ((ip >>>= 8) & 0xFF));
    }
    
    public static String getLocalIpAddress() {
        return hostIPAdress;
    }
    
	@Override
	protected void onResume() {
		super.onResume();
		PluginWrapper.onResume();
	}

	@Override
	protected void onPause() {
		super.onPause();
		PluginWrapper.onPause();
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		PluginWrapper.onDestroy();
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		PluginWrapper.onActivityResult(requestCode, resultCode, data);
	}

	@Override
	protected void onNewIntent(Intent intent) {
		super.onNewIntent(intent);
		PluginWrapper.onNewIntent(intent);
	}

	@Override
	protected void onRestart() {
		super.onRestart();
		PluginWrapper.onRestart();
	}

	@Override
	protected void onStop() {
		super.onStop();
		PluginWrapper.onStop();
	}
    private static native boolean nativeIsLandScape();
    private static native boolean nativeIsDebug();
    
}
