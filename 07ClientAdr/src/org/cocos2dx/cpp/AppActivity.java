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
package org.cocos2dx.cpp;

import org.cocos2dx.lib.Cocos2dxActivity;
import android.view.WindowManager;

//import com.example.tbltestsdk.R;
//import com.tbl.tbltestsdk.CrashThread;
import com.tongbulv.sdk.ITBLCallback;
import com.tongbulv.sdk.TBLUserCenter;
import com.tongbulv.sdk.models.TBLResultMsg;
import com.tongbulv.sdk.models.TBLTransferParams;

import android.content.Context;
import android.os.Bundle;
import android.util.Log;




//import android.os.Bundle;
//import android.app.Activity;
//import android.content.Intent;
//import android.view.View;
//import android.view.View.OnClickListener;
//import android.widget.Button;
//import android.widget.Toast;

//import android.os.Bundle;

public class AppActivity extends Cocos2dxActivity implements ITBLCallback {
	
	private static Context mContext;
	private static String mUid;
	private static String mOrderNum; //璁㈠崟鍙�
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		mContext = this;
	}
	
//	public static String createOrderId(String uid, int goodsId, int payAmount)
//	{
//		int iuid = Integer.parseInt(uid);
//		long start_time = System.currentTimeMillis();
//		
//		return "1234455";
//	}
	
	public static void login()
	{
		//涓夋柟璋冪敤
		TBLTransferParams params = new TBLTransferParams();
		params.frm = "测试";
		params.fdata = "测试";
		TBLUserCenter.initClient(mContext, TBLUserCenter.ACTION_TYPE_LOGIN, (AppActivity) mContext, params);
	}
	
	
	public static void payHandle(int igoodsId, int orderNum, int a, int b)
	{
		//igoodsId = 8401;  //test
		//涓夋柟璋冪敤
		TBLTransferParams params = new TBLTransferParams();
		
		params.uid =  getmUid();              //鐢ㄦ埛id
		params.cp_param = "1";                 //cp鍙傛暟
		
		String tempString = String.valueOf((int)(1+Math.random()*100000000)) + getmUid();
		params.ordernum = tempString.substring(0,16);
		setmOrderNum(params.ordernum);
		
		params.sid = 1;                //鏈嶅姟鍣╥d
		params.gameName = "天天爱掼蛋";   //"澶ц瘽瑗挎父"
		
		
		
		/*
			8403       鐗逛环閽荤煶 12
			8404       100閽荤煶 10
			8405       300閽荤煶 30
			8406       500閽荤煶 50,
			8407       1000閽荤煶 100
			8408       鐗逛环閲戝竵 6
			8409       10涓囬噾甯� 10
			8410       30涓囬噾甯� 30
			8411       50涓囬噾甯� 50
			8412       100涓囬噾甯� 100
		 * */
		params.goodsId = igoodsId;        //鍟嗗搧id
		switch(igoodsId)
		{
			case 8401:
				params.goodsItem = "10钻石";  
				params.pay_amount = 1;       
				params.gold = 10;             
				params.frm = "mall";                
				params.fdata = "mallTestActivity0";    
				break;
				
			case 8402:
				params.goodsItem = "50钻石"; 
				params.pay_amount = 5;        
				params.gold = 50;            
				params.frm = "mall";                
				params.fdata = "mallTestActivity0";    
				break;	
				
			case 8403:
				params.goodsItem = "特价钻石160"; 
				params.pay_amount = 12;        
				params.gold = 160;            
				params.frm = "mall";                 
				params.fdata = "mallTestActivity0";   
				break;
				
			case 8404:
				params.goodsItem = "100钻石"; 
				params.pay_amount = 10;       
				params.gold = 100;            
				params.frm = "mall";          
				params.fdata = "mallzuanshi0";
				break;
				
			case 8405:
				params.goodsItem = "300钻石";  
				params.pay_amount = 30;        
				params.gold = 300;             
				params.frm = "mall";           
				params.fdata = "mallTestActivity0";   
				break;
				
			case 8406:
				params.goodsItem = "500钻石";  
				params.pay_amount = 50;        
				params.gold = 500;             
				params.frm = "mall";           
				params.fdata = "mallTestActivity0";    
				break;
				
			case 8407:
//				params.goodsItem = "1000閽荤煶";  //"20棰楅捇鐭�"
//				params.pay_amount = 100;        //浜烘皯甯侀噾棰�(鍏�)
//				params.gold = 1000;             //娓告垙涓殑閲戝竵銆佺爾鐭炽�佸厓瀹濇暟
//				params.frm = "mall";                 //鏉ユ簮
//				params.fdata = "mallTestActivity0";    //鏉ユ簮鎻忚堪
//				break;
				
				//test
				params.goodsId = 8401;
				params.goodsItem = "10钻石";  
				params.pay_amount = 1;        
				params.gold = 10;             
				params.frm = "mall";          
				params.fdata = "mallTestActivity0";  
				break;
				
			case 8408:
				params.goodsItem = "特价金币10万";  
				params.pay_amount = 6;       
				params.gold = 100000;            
				params.frm = "mall";                
				params.fdata = "mallTestActivity0";    
				break;
				
			case 8409:
				params.goodsItem = "10万金币"; 
				params.pay_amount = 10;        
				params.gold = 100000;          
				params.frm = "mall";           
				params.fdata = "mallTestActivity0";   
				break;
				
			case 8410:
				params.goodsItem = "30万金币";
				params.pay_amount = 30;       
				params.gold = 300000;         
				params.frm = "mall";          
				params.fdata = "mallTestActivity0"; 
				break;
				
			case 8411:
				params.goodsItem = "50万金币";
				params.pay_amount = 50;       
				params.gold = 500000;         
				params.frm = "mall";          
				params.fdata = "mallTestActivity0";  
				break;
				
			case 8412:
				params.goodsItem = "100万金币";
				params.pay_amount = 100;       
				params.gold = 1000000;         
				params.frm = "mall";           
				params.fdata = "mallTestActivity0"; 
				break;
				
			default:
					break;
				
				
		}
		Log.d("callback", "cocos2d-x payhandle before ");
		TBLUserCenter.initClient(mContext, TBLUserCenter.ACTION_TYPE_CHARGE, (AppActivity) mContext, params);
	
		Log.d("callback", "cocos2d-x payhandle ssecess ");
	}
	
	@Override
	public void failLoginCallBack(int arg0) {
		// TODO Auto-generated method stub
		Log.d("callback","##cocos2d-x failLoginCallBack");
		
	}

	@Override
	public void failPayCallBack(int arg0) {
		// TODO Auto-generated method stub
		Log.d("callback","##cocos2d-x failPayCallBack");
	}

	@Override
	public void loginCallBack(TBLResultMsg arg0) {
		// TODO Auto-generated method stub
		Log.d("callback","##cocos2d-x loginCallBack : " + arg0.code );
		Log.d("callback", "cocos2d-x loginCallBack uid = " + arg0.uid );
		Log.d("callback", "cocos2d-x loginCallBack tocken = " + arg0.access_token );
		setmUid(arg0.uid);
		SDKPlugin.LoginCallback(arg0.uid, arg0.access_token );

	}

	@Override
	public void payCallBack(TBLResultMsg arg0) {
		// TODO Auto-generated method stub
		Log.d("callback","##cocos2d-x payCallBack : arg0.code" + arg0.code);
		
		SDKPlugin.paySecessCallback(getmOrderNum(), "123");
		
		Log.d("callback","##cocos2d-x payCallBack secseese");
	}


	public static String getmUid() {
		return mUid;
	}


	public static void setmUid(String mUid) {
		AppActivity.mUid = mUid;
	}
	
	
	public static String getmOrderNum() {
		Log.d("callback","##cocos2d-x morderNum in java" + mOrderNum);
		return mOrderNum;
	}


	public static void setmOrderNum(String mOrderNum) {
		AppActivity.mOrderNum = mOrderNum;
	}
}
