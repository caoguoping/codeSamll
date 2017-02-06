
local LoginScene = class("LoginScene", cc.load("mvc").ViewBase)

function LoginScene:onCreate()
    printf("resource node = %s", tostring(self:getResourceNode()))
    
   ContentManager:getInstance():test()
   
    --[[ you can create scene with following comment code instead of using csb file.
    -- add background image
    display.newSprite("HelloWorld.png")
        :move(display.center)
        :addTo(self)

    -- add HelloWorld label
    cc.Label:createWithSystemFont("Hello World", "Arial", 40)
        :move(display.cx, display.cy + 200)
        :addTo(self)
    ]]
end

function LoginScene:onEnter()
   self.rootNode = cc.CSLoader:createNode("LoginScene.csb"):addTo(self)
   -- addTo(self)
   local btn = self.rootNode:getChildByName("Button_changeScene")
   -- btn:setVisible(false)
    btn:onClicked(
    function ()
    print("密码中不能使用中文和表情以及其他字符")
        end
    )


end

return LoginScene
