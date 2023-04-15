@echo off
chcp 1252 >nul
setlocal enabledelayedexpansion
set nomPoste=%2
set cheminSauvegarde=%1\!nomPoste!
set logs=%1\logs\sauvegarde_!nomPoste!.log
mkdir %1\logs\ 2>nul
mkdir !cheminSauvegarde!  2>nul

echo reduction copie on
taskkill /F /IM chrome* >> !logs! 2>&1
timeout /T 3
echo Ouverture de Chrome : Exporter les mots de passe
start /wait /max chrome >> !logs! 2>&1

taskkill /F /IM msedge* >> !logs! 2>&1
timeout /T 3
echo Ouverture de Edge : Exporter les mots de passe
start /wait /max msedge >> !logs! 2>&1
echo reduction copie off

echo sauvegarde emplacement des icones
%1\icon\ReIcon.exe /S /Name !nomPoste!
REM FIN

echo Kill Proccessus...
for /f "delims=" %%p in ('type autoTaches.ini') do (
	taskkill /F /IM %%p* >> !logs!
)

echo Nettoyage du caches des navigateurs internet
echo SUPPRESSION du cache des navigateurs >> !logs!
del /S /F /Q "%appdata%\..\Local\Google\Chrome\User Data\Default\Cache\*" >> !logs! 2>&1
del /S /F /Q "%appdata%\..\Local\Google\Chrome\User Data\Default\GPUCache\*" >> !logs! 2>&1
del /S /F /Q "%appdata%\..\Local\Google\Chrome\User Data\Default\Storage\ext\*" >> !logs! 2>&1
del /S /F /Q "%appdata%\..\Local\Google\Chrome\User Data\GrShaderCache\GPUCache\*" >> !logs! 2>&1
rmdir /S /Q "%appdata%\..\Local\Google\Chrome\User Data\Default\Application Cache\." >> !logs! 2>&1
rmdir /S /Q "%appdata%\..\Local\Google\Chrome\User Data\Default\Service Worker" >> !logs! 2>&1
del /S /F /Q "%appdata%\..\Local\Google\Chrome\User Data\Profile 1\Cache\*" >> !logs! 2>&1
del /S /F /Q "%appdata%\..\Local\Google\Chrome\User Data\Profile 1\GPUCache\*" >> !logs! 2>&1
del /S /F /Q "%appdata%\..\Local\Google\Chrome\User Data\Profile 1\Storage\ext\*" >> !logs! 2>&1
rmdir /S /Q "%appdata%\..\Local\Google\Chrome\User Data\Profile 1\Application Cache\." >> !logs! 2>&1
rmdir /S /Q "%appdata%\..\Local\Google\Chrome\User Data\Profile 1\Service Worker" >> !logs! 2>&1
del /S /F /Q "%appdata%\..\Local\Google\Chrome\User Data\Profile 2\Cache\*" >> !logs! 2>&1
del /S /F /Q "%appdata%\..\Local\Google\Chrome\User Data\Profile 2\GPUCache\*" >> !logs! 2>&1
del /S /F /Q "%appdata%\..\Local\Google\Chrome\User Data\Profile 2\Storage\ext\*" >> !logs! 2>&1
rmdir /S /Q "%appdata%\..\Local\Google\Chrome\User Data\Profile 2\Application Cache\." >> !logs! 2>&1
rmdir /S /Q "%appdata%\..\Local\Google\Chrome\User Data\Profile 2\Service Worker" >> !logs! 2>&1
del /S /F /Q "%appdata%\..\Local\Google\Chrome\User Data\Profile 3\Cache\*" >> !logs! 2>&1
del /S /F /Q "%appdata%\..\Local\Google\Chrome\User Data\Profile 3\GPUCache\*" >> !logs! 2>&1
del /S /F /Q "%appdata%\..\Local\Google\Chrome\User Data\Profile 3\Storage\ext\*" >> !logs! 2>&1
rmdir /S /Q "%appdata%\..\Local\Google\Chrome\User Data\Profile 3\Application Cache\." >> !logs! 2>&1
rmdir /S /Q "%appdata%\..\Local\Google\Chrome\User Data\Profile 3\Service Worker" >> !logs! 2>&1

del /S /F /Q "%appdata%\..\Local\Microsoft\Edge\User Data\Default\Cache\*" >> !logs! 2>&1
del /S /F /Q "%appdata%\..\Local\Microsoft\Edge\User Data\GrShaderCache\GPUCache\*" >> !logs! 2>&1
del /S /F /Q "%appdata%\..\Local\Google\Chrome\User Data\GrShaderCache\GPUCache\*" >> !logs! 2>&1
del /S /F /Q "%appdata%\..\Local\Microsoft\Edge\User Data\Default\*-journal" >> !logs! 2>&1
del /S /F /Q "%appdata%\..\Local\Microsoft\Edge\User Data\Default\Safe Browsing Network\*-journal"
del /S /F /Q "%appdata%\..\Local\Microsoft\Edge\User Data\Nurturing\*-journal" >> !logs! 2>&1
del /S /F /Q "%appdata%\..\Local\Microsoft\Edge\User Data\Default\IndexedDB\*" >> !logs! 2>&1
rmdir /S /Q "%appdata%\..\Local\Microsoft\Edge\User Data\Default\Service Worker" >> !logs! 2>&1

REM ne pas changer, déclenche la progression
echo Sauvegarde registre Outlook
reg export "HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Windows Messaging Subsystem\Profiles\Outlook" !cheminSauvegarde!\outlook.reg /y >>!logs! 2>&1


REM "C:\Program Files\CCleaner\CCleaner64.exe" /auto	
echo ========================================================================================== >> !logs!
echo %USERNAME% : SAUVEGARDE DOSSIERS... >> !logs!
for /f "delims=" %%j in ('type user.ini') do (
	if not exist "%HOMEPATH%\%%j" (
		echo :^(  %%j n'existe pas >> !logs!
	)
	if exist "%HOMEPATH%\%%j" (
		echo ========================================================================================== >> !logs!
		echo COPIE :  %%j >> !logs!
		echo Sauvegarde :  %%j
		robocopy /MIR /NP /R:1 /W:1 /V "%HOMEPATH%\%%j" "!cheminSauvegarde!\%%j" >> !logs!
		)
)
echo ========================================================================================== >> !logs!
echo SAUVEGARDE DOSSIERS HORS UTILISATEUR... ================================================== >> !logs!
for /f "delims=" %%i in ('type noUser.ini') do (
	if not exist %%i (
		echo :( :   %%i n'existe pas >> !logs!
	)
	if exist %%i (
		echo ========================================================================================== >> !logs!
		echo COPIE :   %%i >> !logs!
		echo Sauvegarde :   %%i
		robocopy /MIR /NP /R:1 /W:1 /V "%%i" "!cheminSauvegarde!\noUser\%%i" >> !logs!
	)
)

echo ========================================================================================== >> !logs!
echo %USERNAME% : SAUVEGARDE APPDATA\LOCAL... ================================================= >> !logs!
for /f "delims=" %%l in ('type userLocal.ini') do (
	if not exist "%HOMEPATH%\AppData\Local\%%l" (
		echo :(   %%l n'existe pas >> !logs!
	)
	if exist "%HOMEPATH%\AppData\Local\%%l" (
		echo ========================================================================================== >> !logs!
		echo COPIE :   %%l >> !logs!
		echo Sauvegarde :   %%l
		robocopy /MIR /NP /R:1 /W:1 /V "%HOMEPATH%\AppData\Local\%%l" "!cheminSauvegarde!\Local\%%l" /XD "%HOMEPATH%\AppData\Local\Local\Microsoft\Internet Explorer\CacheStorage" >> !logs!
	)
)
echo ========================================================================================== >> !logs!
echo %USERNAME% : SAUVEGARDE APPDATA\ROAMING... =============================================== >> !logs!
for /f "delims=" %%n in ('type userRoaming.ini') do (
	if not exist "%HOMEPATH%\AppData\Roaming\%%n" (
		echo :(  %%n n'existe pas >> !logs!
	)
	if exist "%HOMEPATH%\AppData\Roaming\%%n" (
		echo ========================================================================================== >> !logs!
		echo COPIE :   %%n >> !logs!
		echo Sauvegarde :   %%n
		robocopy /MIR /NP /R:1 /W:1 /V "%HOMEPATH%\AppData\Roaming\%%n" "!cheminSauvegarde!\Roaming\%%n" >> !logs!		
	)
)
echo Sauvegarde des spoolers d'impression
REM Sauvegarde des spoolers
del !cheminSauvegarde!\imp-!nomPoste!.printerexport >>!logs!
del c:\imp-!nomPoste!.printerexport >>!logs!

c:\windows\system32\spool\tools\printbrm.exe -s %COMPUTERNAME% -b -f c:\imp-!nomPoste!.printerexport >>!logs!
copy /y c:\imp-!nomPoste!.printerexport !cheminSauvegarde! >>!logs!
REM FIN

echo Sauvegarde !nomPoste! OK >>!logs!
echo Sauvegarde !nomPoste! terminée