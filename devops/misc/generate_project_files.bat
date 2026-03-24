@echo off

set EnginePath=%~1
set UEBTRelativePath=%~2
set VersionSelector=%~3
set ProjectPath=%CD%\%~4

"%VersionSelector%" -switchversionsilent "%ProjectPath%" "%EnginePath%"

rem UBT flags for build from source: -game -engine