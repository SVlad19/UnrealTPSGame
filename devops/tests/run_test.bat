@echo off

call "%~dp0..\config.bat"

rem Build sources
call "%RunUATPath%" ^
BuildCookRun ^
-project="%ProjectPath%" ^
-platform=%Platform% ^
-clientconfig=%Configuration% ^
-archivedirectory=%ArchivePath% ^
-build -cook

rem Run tests
"%EditorPath%" "%ProjectPath%" -ExecCmds="Automation; RunTests %TestNames%;Quit" ^
-log -abslog="%TestOutputLogPath%" -nosplash -ReportOutputPath="%ReportOutputPath%"

rem Copy test artifacts
set TestDir=%~dp0
set TestDataDir=%~dp0data
robocopy "%TestDataDir%" "%ReportOutputPath%" /E

rem Start local server and show report
set Port=8081
set LocalHost=http://localhost:%Port%

pushd "%ReportOutputPath%"
start "" "%LocalHost%"
call http-server -p="%Port%"
popd