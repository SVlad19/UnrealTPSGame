@echo off

call "%~dp0\..\config.bat"

pushd "%ProjectRoot%"
for %%f in (%dirsToRemove%) do (
	rmdir /s /q %%f
)

for %%f in (%filesToRemove%) do (
	del /q %%f
)
popd