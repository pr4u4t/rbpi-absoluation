http    = require "socket.http"
ltn12   = require "ltn12"

function influxDBWrite(url,name,host,volts,timestamp)
    local reqbody = name .. ",host=" .. host ..  " value=" .. volts .. " " .. timestamp
    local respbody = {} 
    local  body, code, headers, status = http.request {
        method = "POST",
        url = url,
        source = ltn12.source.string(reqbody),
        headers = 
                {
                        ["Accept"] = "*/*",
                        ["Accept-Encoding"] = "gzip, deflate",
                        ["Accept-Language"] = "en-us",
                        ["Content-Type"] = "application/x-www-form-urlencoded",
                        ["content-length"] = string.len(reqbody)
                },
        sink = ltn12.sink.table(response_body)
    }

    if code == 204 then
	    return 1
    else
	    return 0
   end
end

function getAdc(name,channel)
    if adc == nil then 
        adc = Factory.instance(name)
    end

    if adc ~= nil then
        return adc:voltage(channel)
    end
    return 0
end

return influxDBWrite("http://192.168.3.148:8086/write?db=icinga2","compressed_air","alarmpi0",
                        getAdc("ADS1256",0),math.floor(os.time(os.date("!*t"))*10^9))
