@echo off

rem Engine params
set EnginePath_5.4=E:\EpicGames\UE_5.4
set EnginePath_5.7_InstalledBuild=E:\JenkinsJobs\UE_5.7_InstalledBuild\Windows

set UBTRelativePath5=Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe
set VersionSelector=C:\Program Files (x86)\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe

rem Current engine path
set CurrentEnginePath=%EnginePath_5.7_InstalledBuild%

rem Editor path 
set EditorPath=%CurrentEnginePath%\Engine\Binaries\Win64\UnrealEditor.exe

rem !! Engine version for packaging !!
set RunUATPath=%CurrentEnginePath%\Engine\Build\BatchFiles\RunUAT.bat

rem Project params
set ProjectRoot=E:\UE_Projects\TPS
set ProjectPureName=TPS
set ProjectName=%ProjectPureName%.uproject
set ProjectPath=%ProjectRoot%\%ProjectName%

rem Build params
set Platform=Win64
set Configuration=Development
set ArchivePath=%ProjectRoot%\Build

rem Other params
set SourceCodePath=%ProjectRoot%\Source
set dirsToRemove=Intermediate DerivedDataCache Saved Binaries .vs
set filesToRemove=*.sln

rem Target params
set COPYRIGHT_LINE=// Copyright Epic Games, Inc. All Rights Reserved.
set EXTRA_MODULE_NAMES=%ProjectPureName%
set TargetTemplateFilePath=%ProjectRoot%\devops\targets\GameModule.Target.cs.template

rem Run
set ServerExePath=%ProjectRoot%\Build\WindowsServer\%ProjectPureName%Server.exe
set ClientExePath=%ProjectRoot%\Build\WindowsClient\%ProjectPureName%Client.exe
set GameExePath=%ProjectRoot%\Build\Windows\%ProjectPureName%.exe

rem Tests
set TestOutputLogPath=%ProjectRoot%\Build\Tests\Tests.log
set ReportOutputPath=%ProjectRoot%\Build\Tests
set UEAutomationContentPath=%CurrentEnginePath%\Engine\Content\Automation
set TestNames=TPSGame.Science
set OpenCPPCoveragePath=E:\OpenCppCoverage\OpenCppCoverage.exe
