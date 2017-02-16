
local LoginScene = class("LoginScene", cc.load("mvc").ViewBase)

LoginScene.RESOURCE_FILENAME = "LoginScene.csb"

function LoginScene:onCreate()
    printf("resource node = %s", tostring(self:getResourceNode()))
    ContentManager:getInstance():test()
end

function LoginScene:startNetWork()
    TTSocketClient:getInstance():startSocketLogin("139.196.237.203",5050)
    printf("startNetWork  ")


    local listener = cc.EventListenerCustom:create("rcvDataLogin", handler(self, self.handleEvent))
    local eventDispatcher = self:getEventDispatcher()
    eventDispatcher:addEventListenerWithFixedPriority(listener, 1)

    local snd = DataSnd:create(1, 2)
    local uid = "1711514028"
    snd:wrDWORD(65536)
    snd:wrString(uid, 66)
    snd:wrString(uid, 64)
    snd:wrString(uid, 66)
    snd:wrString(uid, 32)
    snd:wrDWORD(3)
    snd:sendData(0)
  printf("sendData  ")

end

function LoginScene:loginComplete( rcv )


   local wFaceID            = rcv:readWORD()    
   local dwUserID           = rcv:readDWORD()   
   local dwGameID           = rcv:readDWORD()   
   local dwGroupID          = rcv:readDWORD()   
   local dwCustomID         = rcv:readDWORD()   
   local dwUserMedal        = rcv:readDWORD()   
   local dwExperience       = rcv:readDWORD()   
   local dwLoveLiness       = rcv:readDWORD()   
   local lUserScore         = rcv:readUInt64()  
   local lUserInsure        = rcv:readUInt64()  
   local cbGender           = rcv:readByte()    
   local cbMoorMachine      = rcv:readByte()    
   local szAccounts         = rcv:readString(64)
   local szNickName         =  rcv:readString(64)
   local szGroupName        = rcv:readString(64)
   local cbShowServerStatus = rcv:readByte()    
   local isFirstLogin       = rcv:readDWORD()   
   local rmb                = rcv:readDWORD()  
   printf(wFaceID             )
   printf(dwUserID          )
   printf(dwGameID          )
   printf(dwGroupID         )
   printf(dwCustomID        )
   printf(dwUserMedal       )
   printf(dwExperience      )
   printf(dwLoveLiness      )
   printf(lUserScore        )
   printf(lUserInsure       )
   printf(cbGender          )
   printf(cbMoorMachine     )
   printf(szAccounts        )
   printf(szNickName        )
   printf(szGroupName       )
   printf(cbShowServerStatus)
   printf(isFirstLogin      )
   printf(rmb               )

end



function LoginScene:handleEvent( event)
  local rcv = DataRcv:create(event)
  local wMainCmd = rcv:readWORD()
  local wSubCmd = rcv:readWORD()
  printf("\n\nMain  %d  Sub %d ",wMainCmd,wSubCmd)
  if wMainCmd == 1 then
     if wSubCmd == 100 then
        self:loginComplete(rcv)
     end
   end
end

function LoginScene:onEnter()
   local rootNode = self:getResourceNode()
   local btn = rootNode:getChildByName("Button_changeScene")
   -- btn:setVisible(false)
    btn:onClicked(
    function ()
    -- print("密码中不能使用中文和表情以及其他字符")
      self:getApp():enterScene("LobbyScene")
        end
    )

    self:startNetWork()

end
return LoginScene
