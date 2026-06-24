@echo off

call "%~dp0\..\config.bat"

rem Remove previous  data folder
set TestDir=%~dp0
set TestDataDir=%~dp0data
rmdir /s /q "%TestDataDir%"

rem Copy automation content
robocopy %UEAutomationContentPath% "%TestDataDir%" /E

rem Install bower
call npm install -g bower

rem Install bower packages
pushd "%TestDir%"
cd "%TestDataDir%"
call bower install "%TestDataDir%\bower.json"
popd

rem Install simple server
call npm install http-server bower -g