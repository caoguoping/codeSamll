
local LobbyScene = class("LobbyScene", cc.load("mvc").ViewBase)


function LobbyScene:onCreate()
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

function LobbyScene:onEnter()
   self.rootNode = cc.CSLoader:createNode("NewLobby.csb"):addTo(self)
end

return LobbyScene
