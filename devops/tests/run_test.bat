@echo off

call "%~dp0..\config.bat"

set Before=public bool UnoptimizedCode = false;
set After=public bool UnoptimizedCode = true;
set File=%SourceCodePath%\%ProjectPureName%Editor.Target.cs
powershell -Command "(gc %File%) -replace '%Before%', '%After%' | Out-File %File%"

rem Build sources
call "%RunUATPath%" ^
BuildCookRun ^
-project="%ProjectPath%" ^
-platform=%Platform% ^
-clientconfig=%Configuration% ^
-archivedirectory=%ArchivePath% ^
-build -cook -ubtargs="-UnoptimizedCode"

rem Run tests
set TestRunner="%EditorPath%" "%ProjectPath%" -ExecCmds="Automation; RunTests %TestNames%;Quit" ^
-log -abslog="%TestOutputLogPath%" -nosplash -ReportOutputPath="%ReportOutputPath%"

rem Run code corerage
set ExportType=html:%ReportOutputPath%\Coverage\CodeCoverageReport
::set ExportType=cobertura:%ReportOutputPath%\Coverage\CodeCoverageReport.xml

"%OpenCPPCoveragePath%" --modules="%ProjectRoot%" --sources="%SourceCodePath%" ^
--excluded_sources="%SourceCodePath%\TPS\Tests" --export_type="%ExportType%" -v -- %TestRunner% 

rem Clean obsolete artifacts
del /q LastCoverageResults.log
powershell -Command "(gc %File%) -replace '%After%', '%Before%' | Out-File %File%"


rem Copy test artifacts
set TestDir=%~dp0
set TestDataDir=%~dp0data
robocopy "%TestDataDir%" "%ReportOutputPath%" /E

rem Start local server and show report
set Port=8081
set LocalHost=http://localhost:%Port%

pushd "%ReportOutputPath%"
start "" "%LocalHost%"
start "" "%LocalHost%\Coverage\CodeCoverageReport\index.html"
call http-server -p="%Port%"
popd