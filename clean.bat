@echo off
set dir=%cd%cd %dir%
@del /f /s /q *.obj
@del /f /s /q *.pdb
@del /f /s /q *.vc.db
@del /f /s /q *.ipch
@del /f /s /q *.log
@del /f /s /q *.tlog
@del /f /s /q *.ilk
@del /f /s /q *.exp
@del /f /s /q *.sdf
@del /f /s /q *.lastcodeanalysissucceeded
@del /f /s /q *.ipdb
@del /f /s /q *.iobj
@del /f /s /q *.idb
@del /f /s /q *.pch
@del /f /s /q *.codeanalysis