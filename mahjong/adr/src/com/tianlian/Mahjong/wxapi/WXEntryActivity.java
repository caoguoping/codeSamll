package com.tianlian.Mahjong.wxapi;

import org.cocos2dx.lua.SDKPlugin;
import com.tianlian.Mahjong.Constants;
import java.net.URLEncoder;

import org.json.JSONException;
import org.json.JSONObject;

import com.tencent.mm.opensdk.constants.ConstantsAPI;
import com.tencent.mm.opensdk.modelbase.BaseReq;
import com.tencent.mm.opensdk.modelbase.BaseResp;
import com.tencent.mm.opensdk.modelmsg.SendAuth;
import com.tencent.mm.opensdk.openapi.IWXAPI;
import com.tencent.mm.opensdk.openapi.IWXAPIEventHandler;
import com.tencent.mm.opensdk.openapi.WXAPIFactory;
import com.tencent.mm.opensdk.modelmsg.SendAuth.Resp;  
import com.lidroid.xutils.HttpUtils;
import com.lidroid.xutils.exception.HttpException;
import com.lidroid.xutils.http.ResponseInfo;
import com.lidroid.xutils.http.callback.RequestCallBack;
import com.lidroid.xutils.http.client.HttpRequest;

import android.app.Activity;  
import android.os.Bundle;  
import android.util.Log;

public class WXEntryActivity extends Activity implements IWXAPIEventHandler {
	
	public static final String APP_ID = "wx28da1972ed3f2ee3"; 
    public static final String APP_SECRET = "0788c23a7b0af62542ae7d1b36981c6e";
    
 // private MyApplication myApp;  
    // ����access_token��ַ��ʽ��Ҫ�滻�����APPID��SECRET����CODE  
    public static String GetCodeRequest = "https://api.weixin.qq.com/sns/oauth2/access_token?"  
            + "appid=APPID&secret=SECRET&code=CODE&grant_type=authorization_code";  
    // ����unionid��ַ��ʽ��Ҫ�滻�����ACCESS_TOKEN��OPENID  
    public static String GetUnionIDRequest = "https://api.weixin.qq.com/sns/userinfo?"  
            + "access_token=ACCESS_TOKEN&openid=OPENID";  
  
    private String newGetCodeRequest = "";  
    private String newGetUnionIDRequest = "";  
    private String mOpenId = "";  
    private String mAccess_token = ""; 
    
    @Override  
    protected void onCreate(Bundle savedInstanceState) {  
        // TODO Auto-generated method stub  
    	super.onCreate(savedInstanceState);  
    	  //WechatShareManager.getInstance(this);
    	
//    	if (mWXApi == null) {
//    		mWXApi = WXAPIFactory.createWXAPI(this, APP_ID, false);
//    	}
    	 Constants.wx_api.handleIntent(getIntent(), this);
//    	mWXApi.registerApp(APP_ID);
//    	//WxLogin();
//    	final SendAuth.Req req = new SendAuth.Req();
//    	req.scope = "snsapi_userinfo"; //���������Ϣ
//    	req.state = "null";
//    	mWXApi.sendReq(req);
    	
    	 // finish();
    }  
    
    /** 
     * �ص�΢�ŷ��͵����� 
     */  
    @Override  
    public void onReq(BaseReq arg0) {  
        // TODO Auto-generated method stub 
    	switch (arg0.getType()) {
		case ConstantsAPI.COMMAND_GETMESSAGE_FROM_WX:
			//goToGetMsg();		
			Log.e("WXActivity", "COMMAND_GETMESSAGE_FROM_WX");
			break;
		case ConstantsAPI.COMMAND_SHOWMESSAGE_FROM_WX:
			//goToShowMsg((ShowMessageFromWX.Req) req);
			Log.e("WXActivity", "COMMAND_SHOWMESSAGE_FROM_WX");
			break;
		default:
			break;
		}
  
    }  
  
    /** 
     * ���͵�΢���������Ӧ��� 
     *  
     * ��1���û�ͬ����Ȩ��õ�΢�ŷ��ص�һ��code����code�滻�������ַGetCodeRequest���CODE��ͬ���滻APPID��SECRET 
     * ��2�����µ�ַnewGetCodeRequestͨ��HttpClientȥ���󣬽������ص�JSON���� 
     * ��3��ͨ������JSON�õ������openid �����ڻ�ȡ�û�������Ϣ������ access_token 
     * ��4��ͬ���أ���openid��access_token�滻��GetUnionIDRequest���������Ϣ�ĵ�ַ�� 
     * ��5�����µ�ַnewGetUnionIDRequestͨ��HttpClientȥ���󣬽������ص�JSON���� 
     * ��6��ͨ������JSON�õ����û��ĸ�����Ϣ������unionid 
     */  
    @Override  
    public void onResp(BaseResp arg0) {  
  
        if (arg0.getType() == 2) {  
        	Log.e("WXActivity", "arg0.getType() == 2");
            finish();  
        }  
        if (arg0.getType() == 1) {  
        	Log.e("WXActivity", "arg0.getType() == 1");
            switch (arg0.errCode) 
            {  
            // ͬ����Ȩ  
            case BaseResp.ErrCode.ERR_OK:  
                    
  
            	SendAuth.Resp respLogin = (Resp) arg0;  
            	// ���code  
            	String code = respLogin.code; 
            
            	// ��code��APPID��APPSECRET�滻��Ҫ����ĵ�ַ���Ϊ�µ������ַ  
            	newGetCodeRequest = getCodeRequest(code);  
      
//            // �����µĵ�ַ������������ݣ�����openid��acces_token�� 
            	HttpUtils httpUtils = new HttpUtils();
            	httpUtils.send(HttpRequest.HttpMethod.GET, newGetCodeRequest, new RequestCallBack<String>(){
//                	//�������ɹ�֮��Ļص� 	
        		@Override
            	public void onSuccess(ResponseInfo<String> responseInfo)
            	{
            		Log.e("WXActivity", responseInfo.result.toString());
            		parseAccessTokenJSON(responseInfo.result.toString());
//            		 // �������õ���access_token��openid������unionid��ַ���滻  
            		newGetUnionIDRequest = getUnionID(mAccess_token, mOpenId);  
////                  // �����µ�unionid��ַ�����������ص�unionid������  
            		HttpUtils n_httpUtils = new HttpUtils();
            		n_httpUtils.send(HttpRequest.HttpMethod.GET, newGetUnionIDRequest, new RequestCallBack<String>(){
           			
            			//�������ɹ�֮��Ļص�
            			@Override  	
                   		public void onSuccess(ResponseInfo<String> n_responseInfo)
                    	{
            				parseUnionIdJson(n_responseInfo.result.toString());
                    	}
            			@Override
						public void onFailure(HttpException error, String msg){
                    		
                    	}
            		 });            		
            	}
//            	//������ʧ��         	
            	@Override
            	public void onFailure(HttpException error, String msg){
            		
            	}
            	
            	
            });
            case BaseResp.ErrCode.ERR_AUTH_DENIED:
            	Log.e("WXActivity","1");

            case BaseResp.ErrCode.ERR_BAN:
            	Log.e("WXActivity","1");
            	
            case BaseResp.ErrCode.ERR_COMM:
            	Log.e("WXActivity","1");
            	
            case BaseResp.ErrCode.ERR_SENT_FAILED:
            	Log.e("WXActivity","1");
            	
            case BaseResp.ErrCode.ERR_UNSUPPORT:
            	Log.e("WXActivity","1");
            	
            case BaseResp.ErrCode.ERR_USER_CANCEL:
            	Log.e("WXActivity","1");


//            Timer timer = new Timer();  
//            TimerTask task = new TimerTask()   
//            {  
//                @Override  
//                public void run()   
//                {// TODO Auto-generated method stub  
//                    WXEntryActivity.this.finish();  
//                    }};  
//                    timer.schedule(task, 2000);  
//                    break;  
//                    // �ܾ���Ȩ  
//                    case BaseResp.ErrCode.ERR_AUTH_DENIED:  
//                        finish();  
//                        break;  
//                        // ȡ������  
//                        case BaseResp.ErrCode.ERR_USER_CANCEL:  
//                            finish();  
//                            break;  
//                            default:  
//                                break;  
//                                }  
//            }  
        }  
        }
    }
            
  
    /** 
     * * �滻GetCodeRequest ��APP ID��APP SECRET��code�滻�������� * * @param code * 
     * ��Ȩʱ��΢�Żص����� * @return URL 
     */  
    public static String getCodeRequest(String code) {  
        String result = null;  
        GetCodeRequest = GetCodeRequest.replace("APPID", urlEnodeUTF8(APP_ID));  
        GetCodeRequest = GetCodeRequest.replace("SECRET", urlEnodeUTF8(APP_SECRET));  
        GetCodeRequest = GetCodeRequest.replace("CODE", urlEnodeUTF8(code));  
       result = GetCodeRequest;  
        return result;  
    }  
  
    /** 
     * * �滻GetUnionID * * @param access_token * @param open_id * @return 
     */  
    public static String getUnionID(String access_token, String open_id) {  
        String result = null;  
        GetUnionIDRequest = GetUnionIDRequest.replace("ACCESS_TOKEN", urlEnodeUTF8(access_token));  
        GetUnionIDRequest = GetUnionIDRequest.replace("OPENID", urlEnodeUTF8(open_id));  
        result = GetUnionIDRequest;  
        return result;  
    }  
  
    public static String urlEnodeUTF8(String code) {  
        String result = code;  
        try {  
            result = URLEncoder.encode(code, "UTF-8");  
        	} catch (Exception e) {  
        		e.printStackTrace();  
        		}  
        return result;  
    }  
  
    /** 
     * * ����access_token���ص�JSON���� * * @param response 
     */  
    private void parseAccessTokenJSON(String response) {  
        // TODO Auto-generated method stubtry  
       try {  
            JSONObject jsonObject = new JSONObject(response);  
            mAccess_token = jsonObject.getString("access_token");  
            String expiresIn = jsonObject.getString("expires_in");  
            String refreshToken = jsonObject.getString("refresh_token");  
            mOpenId = jsonObject.getString("openid");  
            String scope = jsonObject.getString("scope");  
            //����ȡ��������д��SharedPreferences��  
//            editor.putString("access_token",mAccess_token);  
//            editor.putString("expires_in",expiresIn);  
//            editor.putString("refresh_token",refreshToken);  
//            editor.putString("openid",mOpenId);  
//            editor.putString("scope", scope);  
//            editor.commit();  
             Log.e("WXActivity", "access_token is " + mAccess_token);  
             Log.e("WXActivity", "expires_in is " + expiresIn);  
             Log.e("WXActivity", "refresh_token is " + refreshToken);  
             Log.e("WXActivity", "openid is " + mOpenId);  
             Log.e("WXActivity", "scope is " + scope);  
             }   
        catch (JSONException e) {  
            // TODO Auto-generated catch block  
            e.printStackTrace();  
            }  
        }  
  

    
    
    /** 
     * * ����unionid���� * @param response 
     */  
    private void parseUnionIdJson(String response) {  
                                                      
        try {  
            JSONObject jsonObject = new JSONObject(response);  
            String openid = jsonObject.getString("openid");  
            String nickname = jsonObject.getString("nickname");  
            String sex = jsonObject.getString("sex");  
            String province = jsonObject.getString("province");  
//            String city = jsonObject.getString("city");  
            String country = jsonObject.getString("country"); 
            String headimgurl = jsonObject.getString("headimgurl");  
           // Log.e("LUA-print WXActivity ", " cityLen is " + city.length());  
            Log.e("LUA-print WXActivity ", " headimgurl is " + headimgurl.length());  
//            if(city.length() <= 0)
//            {
//            	city = "Nanjing";
//            }
            if(headimgurl.length() <= 0)
            {
            	headimgurl = "http://wx.qlogo.cn/mmopen/jLhVg6sHdslhWCyQCWralWY7gXJbvIzZDZzpuZv14icHuuBKwk4nIEzUkgzXibC4iczHKq9buo4C4tWd2A5DlicRsHpLaONibo06D/0";
            }            
            String unionid = jsonObject.getString("unionid");
            String strRoom = Constants.strRoom;     //�����
            Log.e("LUA-print WXActivity ", " openid is " + openid);  
            Log.e("LUA-print WXActivity", "nickname is " + nickname);  
            Log.e("LUA-print WXActivity", "sex is " + sex);  
            Log.e("LUA-print WXActivity", "province is " + province);  
            Log.e("LUA-print WXActivity", "strRoom is " + strRoom);  
            Log.e("LUA-print WXActivity", "country is " + country);  
            Log.e("LUA-print WXActivity", "headimgurl is " + headimgurl);  
            Log.e("LUA-print WXActivity", "unionid is " + unionid);  
            Log.e("LUA-print WXActivity", "ip is " + Constants.hostIp);
            
            SDKPlugin.LoginCallback(openid, nickname, sex, headimgurl,  strRoom, Constants.hostIp);
            finish();
            
            
        } catch (JSONException e) {  
            // TODO Auto-generated catch block  
            e.printStackTrace();  
        }  
        
    }  
    

}
