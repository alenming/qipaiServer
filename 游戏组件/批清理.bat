@echo off
echo 万博棋牌开发科技(www.webqp.net) 清除所有obj pch idb pdb ncb opt plg res sbr ilk suo文件，请稍等......
pause
del /f /s /q .\*.obj
del /f /s /q .\*.pch
del /f /s /q .\*.idb
del /f /s /q .\*.pdb
del /f /s /q .\*.ncb 
del /f /s /q .\*.opt 
del /f /s /q .\*.plg
del /f /s /q .\*.res
del /f /s /q .\*.sbr
del /f /s /q .\*.ilk
del /f /s /q .\*.aps

del /f /s /q .\*.tlg
del /f /s /q .\*.exp
del /f /s /q .\*.lib
del /f /s /q .\*.log
del /f /s /q .\*.tlh
del /f /s /q .\*.tli

echo 清除文件完成！
echo. & pause