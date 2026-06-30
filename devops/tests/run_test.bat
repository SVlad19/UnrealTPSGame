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
set TestRunner="%EditorPath%" "%ProjectPath%" -ExecCmds="Automation; RunTests %TestNames%;Quit" ^
-log -abslog="%TestOutputLogPath%" -nosplash -ReportOutputPath="%ReportOutputPath%"

rem Run code corerage
set ExportType=html:%ReportOutputPath%\Coverage\CodeCoverageReport
::set ExportType=cobertura:%ReportOutputPath%\Coverage\CodeCoverageReport.xml

"%OpenCPPCoveragePath%" --modules="%ProjectRoot%" --sources="%SourceCodePath%" ^
--excluded_sources="%SourceCodePath%\TPS\Tests" --export_type="%ExportType%" -- %TestRunner% -v

rem Clean obsolete artifacts
del /q LastCoverageResults.log

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