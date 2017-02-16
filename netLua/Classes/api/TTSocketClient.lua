
--------------------------------
-- @module TTSocketClient
-- @parent_module 

--------------------------------
-- 
-- @function [parent=#TTSocketClient] MapRecvByte 
-- @param self
-- @param #unsigned char cbData
-- @return unsigned char#unsigned char ret (return value: unsigned char)
        
--------------------------------
-- 
-- @function [parent=#TTSocketClient] ConnectIPv4 
-- @param self
-- @param #char ip
-- @param #unsigned short port
-- @param #unsigned char bSocketType
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#TTSocketClient] Send 
-- @param self
-- @param #char buf
-- @param #int len
-- @param #unsigned char bSocketType
-- @param #int flags
-- @return int#int ret (return value: int)
        
--------------------------------
-- 
-- @function [parent=#TTSocketClient] closeMySocket 
-- @param self
-- @param #unsigned char bSocketType
-- @return int#int ret (return value: int)
        
--------------------------------
-- 
-- @function [parent=#TTSocketClient] recvDateLogin 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#TTSocketClient] startSocketLogin 
-- @param self
-- @param #char ip
-- @param #unsigned short port
-- @return TTSocketClient#TTSocketClient self (return value: TTSocketClient)
        
--------------------------------
-- 
-- @function [parent=#TTSocketClient] getInstance 
-- @param self
-- @return TTSocketClient#TTSocketClient ret (return value: TTSocketClient)
        
--------------------------------
-- 
-- @function [parent=#TTSocketClient] TTSocketClient 
-- @param self
-- @return TTSocketClient#TTSocketClient self (return value: TTSocketClient)
        
return nil
