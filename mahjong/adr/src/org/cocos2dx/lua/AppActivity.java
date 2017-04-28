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

import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;
import java.util.ArrayList;
import java.util.List;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import com.anysdk.framework.PluginWrapper;

import android.R.bool;
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
import com.tencent.mm.opensdk.openapi.WXAPIFactory;




public class AppActivity extends Cocos2dxActivity{
	private static Context mContext;
	private static final int THUMB_SIZE = 150;
	private static int mTargetScene;
	private CheckBox isTimelineCb;
	private static String mStrRoomNum;
	
    static String hostIPAdress = "0.0.0.0";
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
//		String path = "/sdcard/headshot_example.png";
//		weChatShare(path);
	}
	
	
	private static String buildTransaction(final String type) {
		return (type == null) ? String.valueOf(System.currentTimeMillis()) : type + System.currentTimeMillis();
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
		

		
		//msg.title = "房间号: " + String.valueOf(roomNum) ;
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
	
//    private void shareText(String text) {
//        // 初始化一个WXTextObject对象
//        WXTextObject textObj = new WXTextObject();
//        textObj.text = text;
//
//        // 用WXTextObject对象初始化一个WXMediaMessage对象
//        WXMediaMessage msg = new WXMediaMessage();
//        msg.mediaObject = textObj;
//        // 发送文本类型的消息时，title字段不起作用
//        // msg.title = "Will be ignored";
//        msg.description = text;
//
//        // 构造一个Req
//        SendMessageToWX.Req req = new SendMessageToWX.Req();
//        req.transaction = buildTransaction("text"); // transaction字段用于唯一标识一个请求
//        req.message = msg;
//        // 是否分享到朋友圈
//        req.scene = isTimelineCb.isChecked() ? SendMessageToWX.Req.WXSceneTimeline : SendMessageToWX.Req.WXSceneSession;
//
//        // 调用api接口发送数据到微信
//        api.sendReq(req);
//    }
	
	
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
