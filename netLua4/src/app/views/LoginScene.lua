
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

function LoginScene:handleEvent( event)
        local pData = event:getCgpData()
        -- printf("\n\n\n\ngetEventName %s\n\n\n", name)
    local rcv = DataRcv:create(pData)
 local wMainCmd = rcv:getMainCmd()
  local wSubCmd = rcv:getSubCmd()
   printf("\n\nMain  %d  Sub %d ",wMainCmd,wSubCmd)
  if wMainCmd == 1 then
     if wSubCmd == 100 then
       printf("\n\n@@@@@@@@@@@ userData = OKOKOKOKOK\n\n ")
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
