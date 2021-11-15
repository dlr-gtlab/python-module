@echo off

if %COMMIT_BRANCH%=="" (
    echo "COMMIT_BRANCH is empty"
	exit -1
) else (
	echo "COMMIT_BRANCH is not empty, start copy"
)
