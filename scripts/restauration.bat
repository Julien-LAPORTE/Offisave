@echo off
chcp 1252 >nul
setlocal enabledelayedexpansion
set nomPoste=%2
set cheminSauvegarde=%1\!nomPoste!
set logs=%1\logs\restauration_!nomPoste!.log

echo ###################################### >>!logs! 2>&1
echo Mise à jour Chrome >>!logs! 2>&1
echo ###################################### >>!logs! 2>&1
"C:\Program Files (x86)\Google\Update\GoogleUpdate.exe" >>!logs! 2>&1

if exist %1\firefox_gdehtdklkdfnjkk.temp (
del %1\firefox_%date:~6,4%_%date:~3,2%_%date:~0,2%.exe
set "tFirefox=fichier corrompu : nouveau téléchargement de firefox"
) else (
set "tFirefox=téléchargement firefox"
)
if exist %1\firefox_%date:~6,4%_%date:~3,2%_%date:~0,2%.exe (
echo firefox déjà téléchargé
) else (
mkdir %1\firefox_gdehtdklkdfnjkk.temp 2>nul
attrib +H +S %1\firefox_gdehtdklkdfnjkk.temp /D
echo !tFirefox!
powershell "& {$url='https://download.mozilla.org/?product=firefox-latest-ssl&os=win64&lang=fr';$clnt=new-object System.Net.WebClient;$clnt.DownloadFile($url,'%1\firefox_%date:~6,4%_%date:~3,2%_%date:~0,2%.exe')}"
rmdir %1\firefox_gdehtdklkdfnjkk.temp
)

echo Installation firefox téléchargé le %date:~0,2%/%date:~3,2%  %date:~6,4%
%1\firefox_%date:~6,4%_%date:~3,2%_%date:~0,2%.exe /S

if exist !cheminSauvegarde!\Local\Thunderbird (
echo Installation de Thunderbird >>!logs! 2>&1
echo Installation de Thunderbird
if exist C:\WPI (
echo Installation via WPI >>!logs! 2>&1
C:\WPI\Install\Internet\thunderbird\ThunderbirdSetupx64.exe /s
) ELSE (
		if exist C:\pharmagest\PCInstall (
		echo Installation via PCInstall >>!logs! 2>&1
		C:\pharmagest\PCInstall\apps\Messagerie\thunderbird\ThunderbirdSetupFRx64.exe /s
			) ELSE (
				echo ^:^( Pas de WPI ou de PCInstall d'installé >>!logs! 2>&1
			)
		)
)

echo Restauration registre Outlook
reg import !cheminSauvegarde!\outlook.reg >>!logs! 2>&1

for /f "delims=" %%p in ('type taches.ini') do (
	taskkill /F /IM %%p* >>!logs!
)
echo ========================================================================================== >> !logs!
echo %USERNAME% : RESTAURATION DOSSIERS... >> !logs!
for /f "delims=" %%j in ('type user.ini') do (
	if not exist "!cheminSauvegarde!\%%j" (
		echo :(  %%j n'existe pas >> !logs!
	)
	if exist "!cheminSauvegarde!\%%j" (
		echo ========================================================================================== >> !logs!
		echo COPIE :  %%j >> !logs!
		echo Restauration :  %%j
		robocopy /MIR /NP /R:1 /W:1 /V  "!cheminSauvegarde!\%%j" "%HOMEPATH%\%%j" >> !logs!
		)
)
echo ========================================================================================== >> !logs!
echo RESTAURATION DOSSIERS HORS UTILISATEUR... ================================================== >> !logs!
for /f "delims=" %%i in ('type noUser.ini') do (
	if not exist "!cheminSauvegarde!\noUser\%%i" (
		echo :( :   %%i n'existe pas >> !logs!
	)
	if exist  "!cheminSauvegarde!\noUser\%%i" (
		echo ========================================================================================== >> !logs!
		echo COPIE :   %%i >> !logs!
		echo Restauration :   %%i
		robocopy /MIR /NP /R:1 /W:1 /V "!cheminSauvegarde!\noUser\%%i" "%%i" >> !logs!
	)
)

echo ========================================================================================== >> !logs!
echo %USERNAME% : RESTAURATION APPDATA\LOCAL... ================================================= >> !logs!
for /f "delims=" %%l in ('type userLocal.ini') do (
	if not exist "!cheminSauvegarde!\Local\%%l" (
		echo :(   %%l n'existe pas >> !logs!
	)
	if exist "!cheminSauvegarde!\Local\%%l" (
		@echo ========================================================================================== >> !logs!
		@echo COPIE :   %%l >> !logs!
		@echo Restauration :   %%l
		robocopy /MIR /NP /R:1 /W:1 /V "!cheminSauvegarde!\Local\%%l"  "%HOMEPATH%\AppData\Local\%%l" >> !logs!
	)
)
echo ========================================================================================== >> !logs!
echo %USERNAME% : RESTAURATION APPDATA\ROAMING... =============================================== >> !logs!
for /f "delims=" %%n in ('type userRoaming.ini') do (
	if not exist "!cheminSauvegarde!\Roaming\%%n" (
		echo :(  %%n n'existe pas >> !logs!
	)
	if exist "!cheminSauvegarde!\Roaming\%%n" (
		echo ========================================================================================== >> !logs!
		echo COPIE :   %%n >> !logs!
		echo Restauration :   %%n
		robocopy /MIR /NP /R:1 /W:1 /V "!cheminSauvegarde!\Roaming\%%n" "%HOMEPATH%\AppData\Roaming\%%n" >> !logs!		
	)
)


echo restauration des spoolers d'impression
del c:\imp-!nomPoste!.printerexport >>!logs!
copy /y  !cheminSauvegarde!\imp-!nomPoste!.printerexport c:\ >>!logs!
c:\windows\system32\spool\tools\printbrm.exe -s %COMPUTERNAME% -r -f c:\imp-!nomPoste!.printerexport >>!logs!
taskkill /F /IM chrome* >>nul
taskkill /F /IM msedge* >>nul
timeout /T 1

echo reduction copie on
echo Ouverture de Chrome : Importer les mots de passe
start /wait /max chrome -enable-features=PasswordImport >> !logs! 2>&1

echo Ouverture de Edge : Importer les mots de passe
start /wait /max msedge >> !logs! 2>&1

echo reduction copie off
echo Mise en place de chrome par défaut >>!logs! 2>&1
if exist C:\WPI (
echo Mise en place via WPI >>!logs! 2>&1
C:\WPI\Install\Internet\chrome\SetDefaultBrowser.exe chrome >> !logs! 2>&1
) ELSE (
		if exist C:\pharmagest\PCInstall (
		echo Mise en place via PCInstall >>!logs! 2>&1
		C:\pharmagest\PCInstall\apps\Navigateur\chrome\SetDefaultBrowser.exe chrome >> !logs! 2>&1
			) ELSE (
				echo ^:^( Pas de WPI ou de PCInstall d'installé >>!logs! 2>&1
			)
		)

del /S /F /Q "%userprofile%\Documents\*Mots de passe Google*.csv" >>!logs! 2>&1

REM Paramétrage de la barre des tâches
REM REG ADD "HKCU\Software\Microsoft\Windows\Shell\Bags\1\Desktop" /v "IconSize" /t REG_DWORD /d 34 /f >>!logs! 2>&1
taskkill /f /im explorer.exe >>!logs! 2>&1
REG ADD "HKCU\Software\Microsoft\Windows\CurrentVersion\Feeds" /V ShellFeedsTaskbarViewMode /T REG_DWORD /D 2 /F
REG ADD "HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\Advanced" /V ShowTaskViewButton /T REG_DWORD /D 0 /F
REG ADD "HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\Advanced" /V ShowTaskViewButton /T REG_DWORD /D 0 /F
REG ADD "HKCU\Software\Microsoft\Windows\CurrentVersion\Search" /V SearchboxTaskbarMode /T REG_DWORD /D 0 /F
REG ADD "HKEY_CURRENT_USER\Control Panel\Desktop\WindowMetrics" /v MinAnimate /t REG_SZ /d 0 /f >nul 2>&1
start %windir%\explorer.exe

echo restauration emplacement des icones
%1\icon\ReIcon.exe /R /Name !nomPoste!

echo installation TOFF et redémarrage auto à 7h >>!logs! 2>&1
echo installation TOFF et redémarrage auto à 7h
if exist C:\WPI (
echo Installation via WPI >>!logs! 2>&1
C:\WPI\Install\Utilitaires\Toff\TOFF.exe /silent
start "" C:\PROGRA~2\TIMERO~1\TOff.exe /run /systray todo=rh type=pt time="07:00" /everyday /eachtime
) ELSE (
		if exist C:\pharmagest\PCInstall (
		echo Installation via PCInstall >>!logs! 2>&1
		C:\pharmagest\PCInstall\apps\Utilitaires\Toff\TOFF.exe /silent
		start "" C:\PROGRA~2\TIMERO~1\TOff.exe /run /systray todo=rh type=pt time="07:00" /everyday /eachtime
			) ELSE (
				echo ^:^( Pas de WPI ou de PCInstall d'installé >>!logs! 2>&1
			)
		)
REM FIN


echo Suppression des raccourcis
for /f "delims=" %%b in ('type autoBureau.ini') do (
	echo ========================================================================================== >> !logs!
	echo SUPPRESSION :   %%b >> !logs!
	del /F /Q "%HOMEPATH%\Desktop\*%%b*.lnk" >> !logs!
	del /F /Q "%PUBLIC%\Desktop\*%%b*.lnk" >> !logs!
	echo ========================================================================================== >> !logs!
)

echo Restauration !nomPoste! OK >>!logs!
echo Restauration !nomPoste! terminée