
function lua_input(pkt, len)
    -- 只处理前 len 个字节（防止越界）
    local data = pkt:sub(1, len)

    -- 直接匹配 Host 的值
    local host = data:match("\r\nHost:%s*([^\r\n]+)")

    return host
end


